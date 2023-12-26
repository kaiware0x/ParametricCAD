
#include "change_visibility_cmd.h"
#include "TNaming_Builder.hxx"
#include "Defines.h"
#include "document.h"
#include "mainwindow.h"
#include "tree/treemodel.h"

#include <TNaming_NamedShape.hxx>

ChangeVisibilityCmd::ChangeVisibilityCmd(TreeItem *tritem)
    : m_tritem(tritem), m_show(tritem->checkState() == Qt::Checked)
{
    MYLOG;
    auto doc    = MainWindow::instance()->document();
    auto occdoc = doc->ocafDocument();
    occdoc->OpenCommand();

    try {
        Handle(TDataStd_TreeNode) treenode = doc->treeModel()->nodeFromItem(m_tritem);
        if (treenode.IsNull()) {
            throw std::exception("Cannot find tree node.");
        }

        auto            box_label = treenode->Label();
        TNaming_Builder B(box_label);
        Handle(TNaming_NamedShape) named_shape = B.NamedShape();

        TopoDS_Shape shape = named_shape->Get();
        m_shape            = new AIS_Shape(shape);

        if (m_show) {
            doc->showShadedShape(m_shape);
        }
        else {
            doc->hideShape(m_shape);
        }

        //        auto named_shape = B.NamedShape();
        //        named_shape->Get();

        //        if (named_shape->Get().IsNull()) {
        //            CALLOUT << "named shape is null";
        //        }
        //        else {
        //            CALLOUT << "named shape is NOT null";
        //        }
        //        Handle(TPrsStd_AISPresentation) prs = TPrsStd_AISPresentation::Set(box_label,
        //        XCAFPrs_Driver::GetID()); prs->SetDisplayed(false);
    }
    catch (...) {
        MYWARNLOG << "cannot make tree item, label";
        occdoc->AbortCommand();
        return;
    }

    occdoc->CommitCommand();

    m_can_continue = true;
    m_redo_called  = true;
}

void ChangeVisibilityCmd::redo() {}

void ChangeVisibilityCmd::undo() {}
