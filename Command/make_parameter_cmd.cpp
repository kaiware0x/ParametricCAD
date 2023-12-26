
#include <TDataStd_Name.hxx>
#include <TDataStd_Real.hxx>
#include <TDocStd_Modified.hxx>

#include "Defines.h"
#include "document.h"
#include "make_parameter_cmd.h"
#include "Util.h"
#include "tree/treemodel.h"

MakeParameterCmd::MakeParameterCmd(const Parameter &param, Document *document) : AbstractCmd("Add Parameter", document)
{
    MYLOG;
    auto occdoc = document->ocafDocument();
    auto model  = document->treeModel();
    occdoc->OpenCommand();

    try {
        ///---------------------------------------------------------------------------------------------------
        /// Parameter Tree Nodeの生成.
        ///---------------------------------------------------------------------------------------------------
        auto params_node = model->parametersTreeNode();
        auto param_label = params_node->Label().NewChild();

        TDataStd_Real::Set(param_label, param.value());

        auto param_node = util::setTreeNode(param_label);
        params_node->Append(param_node);

        TDataStd_Name::Set(param_label, param.occName());
        TDocStd_Modified::Add(param_label);

        ///---------------------------------------------------------------------------------------------------
        /// Parameter Tree Itemの生成.
        ///---------------------------------------------------------------------------------------------------
        auto params_treeitem = model->parametersTreeItem();
        m_param_tritem       = model->newChildTreeItem(params_treeitem, TreeItem::Type::Parameter, param_label);
        m_row                = m_param_tritem->row();
    }
    catch (...) {
        MYWARNLOG << "Cannot add parameter.";
        occdoc->AbortCommand();
        return;
    }

    occdoc->CommitCommand();

    m_can_continue = true;
    m_redo_called  = true;
}

void MakeParameterCmd::redo()
{
    MYLOG;

    if (m_redo_called) {
        return;
    }

    m_document->treeModel()->parametersTreeItem()->insertRow(m_row, m_param_tritem);
    m_param_tritem->setCheckState(Qt::Checked);

    ocafRedo();

    m_redo_called = true;
}

void MakeParameterCmd::undo()
{
    MYLOG;

    if (!m_redo_called) {
        return;
    }

    m_document->treeModel()->parametersTreeItem()->takeRow(m_row);

    ocafUndo();

    m_redo_called = false;
}
