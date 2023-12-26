
#include <TDataStd_Name.hxx>
#include <TPrsStd_AISPresentation.hxx>
#include <XCAFPrs_Driver.hxx>
#include "TNaming_Builder.hxx"
#include "TNaming_NamedShape.hxx"

#include "FromSample/OCAFSample_IBox.hxx"
#include "FromSample/OCAFSample_IDisplayer.hxx"

#include "Defines.h"
#include "document.h"
#include "mainwindow.h"
#include "make_box_cmd.h"
#include "Util.h"
#include "tree/treemodel.h"

MakeBoxCmd::MakeBoxCmd(const BoxValue &value, Document *document) : AbstractCmd("Make Box", document)
{
    MYLOG;
    auto occdoc = document->ocafDocument();
    auto model  = document->treeModel();
    occdoc->OpenCommand();

    TCollection_ExtendedString anError;
    try {
        auto shapes_node = model->shapesTreeNode();
        auto obj_node    = OCAFSample_IBox::MakeBox(value, shapes_node, anError);
        OCAFSample_IDisplayer::Display(obj_node);

        ///---------------------------------------------------------------------------------------------------
        /// 「Box」TreeItem生成.
        ///---------------------------------------------------------------------------------------------------
        auto shapes_treeitem = model->shapesTreeItem();
        auto obj_label       = obj_node->Label();
        m_box_tritem         = model->newChildTreeItem(shapes_treeitem, TreeItem::Type::Shape, obj_label);
        m_row                = m_box_tritem->row();

        ///---------------------------------------------------------------------------------------------------
        /// 「Box function」TreeItem生成.
        ///---------------------------------------------------------------------------------------------------
        const auto func_label  = obj_label.FindChild(1, false);
        const auto func_tritem = model->newTreeItem(TreeItem::Type::ShapeFunction, func_label);
        m_box_tritem->appendRow(func_tritem);

        //        CALLOUT << VAROUT(util::getEntry(obj_label));
        //        CALLOUT << VAROUT(util::getEntry(occdoc->Main()));
        //        CALLOUT << VAROUT(util::getEntry(occdoc->GetData()->Root()));

        ///
        ///
        ///
        ///
        //        auto model   = doc->treeModel();
        //        m_box_tritem = model->shapesTreeItem()->newChild(maker.name);

        //        m_box_tritem->treeNode();

        //        m_row = m_box_tritem->row();

        //        auto            box_label = m_box_tritem->label();
        //        TNaming_Builder B(*box_label);
        //        B.Generated(box);

        //        auto aisbox = new AIS_Shape(box);
        //        aisbox->SetMaterial(Graphic3d_NameOfMaterial::Graphic3d_NameOfMaterial_Pewter);
        //        doc->showShadedShape(aisbox);

        ///
        ///
        ///
        ///

        //        auto named_shape = B.NamedShape();
        //        named_shape->Get();

        //        if (named_shape->Get().IsNull()) {
        //            CALLOUT << "named shape is null";
        //        }
        //        else {
        //            CALLOUT << "named shape is NOT null";
        //        }

        ///
        ///
        ///
        //        Handle(TPrsStd_AISPresentation) prs = TPrsStd_AISPresentation::Set(*box_label,
        //        XCAFPrs_Driver::GetID());
        ///
        ///
        ///

        //        prs->SetMaterial(Graphic3d_NOM_DEFAULT);    // これを行わないとOCCデフォのGOLDで塗りつぶされてしまう.
        //        prs->Display(true);
    }
    catch (...) {
        MYWARNLOG << "Cannot make tree item or label." << util::toQString(anError);
        occdoc->AbortCommand();
        return;
    }

    occdoc->CommitCommand();

    m_can_continue = true;
    m_redo_called  = true;
}

void MakeBoxCmd::redo()
{
    MYLOG;

    if (m_redo_called) return;

    auto treemodel = m_document->treeModel();

    treemodel->shapesTreeItem()->insertRow(m_row, m_box_tritem);
    m_box_tritem->setCheckState(Qt::Checked);

    ocafRedo();

    m_redo_called = true;
}

void MakeBoxCmd::undo()
{
    if (!m_redo_called) return;

    auto treemodel = m_document->treeModel();

    treemodel->shapesTreeItem()->takeRow(m_row);

    ocafUndo();

    m_redo_called = false;
}
