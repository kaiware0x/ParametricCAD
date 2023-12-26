
#include <TDataStd_Real.hxx>

#include "TDataStd_Name.hxx"
#include "Defines.h"
#include "document.h"
#include "edit_parameter_cmd.h"
#include "Util.h"
#include "tree/treemodel.h"

EditParameterCmd::EditParameterCmd(const Handle(TDataStd_TreeNode) & param_node, const Parameter& param,
                                   Document* document)
    : AbstractCmd("Edit Parameter", document), m_param_node(param_node)
{
    if (param_node.IsNull()) {
        MYWARNLOG << "box function node is null";
        return;
    }

    MYLOG;
    auto model  = document->treeModel();
    auto occdoc = document->ocafDocument();

    try {
        occdoc->OpenCommand();

        auto param_label = param_node->Label();
        m_old_name       = util::getName(param_label);

        ///---------------------------------------------------------------------------------------------------
        /// 値の更新
        ///---------------------------------------------------------------------------------------------------
        Handle(TDataStd_Real) aReal;
        param_label.FindAttribute(TDataStd_Real::GetID(), aReal);
        MY_ASSERT(!aReal.IsNull(), "cannot find the attribute <TDataStd_Real>.");
        aReal->Set(param.value());

        ///---------------------------------------------------------------------------------------------------
        /// Label名の更新
        ///---------------------------------------------------------------------------------------------------
        Handle(TDataStd_Name) name_attr = util::getNameAttribute(param_label);
        MY_ASSERT(!name_attr.IsNull(), "cannot find the attribute <TDataStd_Name>.");
        name_attr->Set(param.occName());

        ///---------------------------------------------------------------------------------------------------
        /// TreeItem名の更新
        ///---------------------------------------------------------------------------------------------------
        m_new_name     = param.qtName();
        m_param_tritem = model->itemFromNode(param_node);
        m_param_tritem->setText(m_new_name);
    }
    catch (...) {
        MYWARNLOG << "Cannot edit parameter.";
        occdoc->AbortCommand();
        return;
    }

    occdoc->CommitCommand();

    m_can_continue = true;
    m_redo_called  = true;
}

void EditParameterCmd::redo()
{
    MYLOG;

    if (m_redo_called) {
        return;
    }

    ocafRedo();
    m_param_tritem->setText(m_new_name);

    m_redo_called = true;
}

void EditParameterCmd::undo()
{
    MYLOG;

    if (!m_redo_called) {
        return;
    }

    ocafUndo();
    m_param_tritem->setText(m_old_name);

    m_redo_called = false;
}
