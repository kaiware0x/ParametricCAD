
#include <QDir>

#include <BinXCAFDrivers.hxx>
#include <TNaming_Builder.hxx>
#include <TNaming_NamedShape.hxx>
#include <TPrsStd_AISPresentation.hxx>
#include <TPrsStd_AISViewer.hxx>

#include "FromSample/OCAFSample_IDisplayer.hxx"

#include "Defines.h"
#include "document.h"
#include "graphics/ViewerWidget.h"
#include "Util.h"
#include "tree/treemodel.h"

Document::Document(QObject *parent)
    : QObject(parent), m_ocaf_app(XCAFApp_Application::GetApplication()), m_viewer_widget(new ViewerWidget(this))
{
    BinXCAFDrivers::DefineFormat(m_ocaf_app);

    newDocument(QString());

    /// treemodelはm_std_docの初期化が終わってから初期化する.
    m_treemodel = new TreeModel(m_std_doc->GetData()->Root(), m_std_doc->Main(), this);

    connect(m_treemodel, &TreeModel::checkStateChanged, this, &Document::changeVisibility);
}

Handle(TDataStd_TreeNode) Document::rootNode() const
{
    TDF_Label aRootLabel = m_std_doc->GetData()->Root();
    MY_ASSERT(!aRootLabel.IsNull(), "Root label is null.");

    Handle(TDataStd_TreeNode) aRootNode = util::getTreeNode(aRootLabel);
    MY_ASSERT(!aRootNode.IsNull(), "Root node is null.");

    return aRootNode;
}

void Document::deleteAISObjects(const QList<Handle(AIS_InteractiveObject)> &objects)
{
    if (m_context.IsNull()) return;

    for (auto &&obj : objects) {
        if (obj.IsNull()) continue;

        m_context->UnhilightSelected(Standard_False);
        m_context->Remove(obj, Standard_False);
    }
}

void Document::showWireFrameShape(AIS_Shape *shape)
{
    m_context->Display(shape, AIS_WireFrame, AIS_Shape::SelectionMode(TopAbs_WIRE), true);
    m_context->SetMaterial(shape, Graphic3d_NameOfMaterial_DEFAULT, false);
    m_context->SetColor(shape, Quantity_NOC_VIOLET, false);
    //    m_context->SetTransparency(shape, 0.0, false);
}

void Document::showShadedShape(AIS_Shape *shape)
{
    MYLOG;

    m_context->Display(shape, AIS_Shaded, AIS_Shape::SelectionMode(TopAbs_SOLID), true);
    m_context->SetMaterial(shape, Graphic3d_NameOfMaterial_Brass, false);
    //    m_context->SetColor(shape, Quantity_NOC_HONEYDEW, false);
    //    m_context->SetTransparency(shape, 0.0, false);
}

void Document::showShadedShape(Handle(TDataStd_TreeNode) treenode)
{
    if (treenode.IsNull()) return;
    OCAFSample_IDisplayer::Display(treenode);
}

void Document::hideShape(Handle(TDataStd_TreeNode) treenode)
{
    if (treenode.IsNull()) return;
    OCAFSample_IDisplayer::Erase(treenode);
}

void Document::hideShape(AIS_Shape *shape)
{
    MYLOG;

    m_context->Remove(shape, true);
}

void Document::save() const
{
    QDir directory("D:/tmp");

    if (!directory.exists()) {
        directory.mkpath(".");
    }

    const auto datetime_str = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");
    const auto filename     = QString("proto_%1.xbf").arg(datetime_str);

    const auto filepath = directory.filePath(filename);
    m_ocaf_app->SaveAs(m_std_doc, filepath.toStdString().c_str());
}

void Document::changeVisibility(TreeItem *tritem)
{
    MYLOG;
    //    m_ocaf_app->SaveAs(m_std_doc, "D:/tmp/proto.xbf");

    Handle(TDataStd_TreeNode) treenode = m_treemodel->nodeFromItem(tritem);
    if (treenode.IsNull()) {
        return;
    }

    if (tritem->checkState() == Qt::Checked) {
        showShadedShape(treenode);
    }
    else {
        hideShape(treenode);
    }

    //    auto label = treenode->Label();
    //    Handle(TPrsStd_AISPresentation) prs;
    //    if (label.FindAttribute(TPrsStd_AISPresentation::GetID(), prs)) {
    //        prs->Erase(true);
    //        TPrsStd_AISViewer::Update(label);
    //        return;
    //    }

    //    if (prs.IsNull()) {
    //        MYWARNLOG << "Cannot get TPrsStd_AISPresentation";
    //        return;
    //    }

    //    if (tritem->checkState() == Qt::Checked) {
    //        m_context->Erase(prs->GetAIS(), true);
    //        //        m_context->Remove(prs->GetAIS(), true);
    //        //        prs->Display();
    //    }
    //    else {
    //        m_context->Display(prs->GetAIS(), true);
    //        //        prs->Erase();
    //    }
}

bool Document::newDocument(const QString &filename)
{
    //!< DocumentのNew
    if (filename.isEmpty()) {
        m_ocaf_app->NewDocument("BinXCAF", m_std_doc);
        m_context = new AIS_InteractiveContext(m_viewer_widget->occViewer());

        /// DocumentのRootとContextの紐づけ.
        TDF_Label root_label = m_std_doc->GetData()->Root();
        if (!root_label.FindAttribute(TPrsStd_AISViewer::GetID(), m_viewer_attr)) {
            /// RootのTPrsStd_AISViewer属性にContextがセットされる.
            TPrsStd_AISViewer::New(root_label, m_context);
        }
        util::setTreeNode(root_label);

        TNaming_Builder aBuilder(m_std_doc->Main());
        m_std_doc->Main().ForgetAttribute(TNaming_NamedShape::GetID());
    }
    else {
        //!< DocumentのLoad
        TCollection_ExtendedString OCCDoc_fileName(filename.toLocal8Bit().constData(), Standard_True);
        PCDM_ReaderStatus          status = m_ocaf_app->Open(OCCDoc_fileName, m_std_doc);
        if (status != PCDM_RS_OK) {
            return false;
        }
        TPrsStd_AISViewer::New(m_std_doc->Main(), m_viewer_widget->occViewer());
        TPrsStd_AISViewer::Find(m_std_doc->Main(), m_context);
    }

    m_std_doc->SetUndoLimit(10);

    //    m_context->HighlightStyle()->SetDisplayMode(AIS_Shaded);
    //    m_context->SelectionStyle()->SetDisplayMode(AIS_Shaded);
    m_context->SetDisplayMode(AIS_Shaded, true);

    //    // Configure some global props.
    //    const Handle(Prs3d_Drawer)& contextDrawer = m_context->DefaultDrawer();
    //    //
    //    if (!contextDrawer.IsNull()) {
    //        const Handle(Prs3d_ShadingAspect)& SA        = contextDrawer->ShadingAspect();
    //        const Handle(Graphic3d_AspectFillArea3d)& FA = SA->Aspect();
    //        contextDrawer->SetFaceBoundaryDraw(true);    // Draw edges.
    //        FA->SetEdgeOff();

    //        // Fix for inifinite lines has been reduced to 1000 from its default value 500000.
    //        contextDrawer->SetMaximalParameterValue(1000);
    //    }

    return true;
}
