
#include <TDocStd_Modified.hxx>

#include "Defines.h"
#include "document.h"
#include "FromSample/OCAFSample_IBox.hxx"
#include "FromSample/OCAFSample_IDisplayer.hxx"
#include "FromSample/OCAFSample_SolverEx.hxx"
#include "FromSample/Tags.h"
#include "Util.h"
#include "set_parameter_cmd.h"
#include "tree/treemodel.h"

SetParameterCmd::SetParameterCmd(const Handle(TDataStd_TreeNode) & shape_function_node,
                                 const Handle(TDataStd_TreeNode) & param_node, const QSet<tag::Type>& target_tags,
                                 Document* document)
    : AbstractCmd("Set Parameter", document)
    , m_shape_function_node(shape_function_node)
    , m_param_node(param_node)
    , m_target_tags(target_tags)

{
    if (target_tags.isEmpty()) {
        MYWARNLOG << "target tags is empty";
        return;
    }

    MYLOG;
    auto occdoc    = document->ocafDocument();
    auto treemodel = document->treeModel();

    occdoc->OpenCommand();

    m_shape_function_item = treemodel->itemFromNode(shape_function_node);
    m_param_item          = treemodel->itemFromNode(param_node);

    m_param_item->addParameterRef(m_shape_function_item, target_tags);

    TDF_Label     arguments_label = shape_function_node->Label().FindChild(tag::Object::Argments);
    const double& parameter       = util::getRealValue(param_node->Label());

    for (auto&& tag : target_tags) {
        TDF_Label arg_label = arguments_label.FindChild(tag);
        TDataStd_Real::Set(arg_label, parameter);
        TDocStd_Modified::Add(arg_label);
    }

    OCAFSample_SolverEx aSolver;
    if (!aSolver.SolveFrom(shape_function_node)) {
        MYWARNLOG << "Cannot edit box.";
        occdoc->AbortCommand();
        return;
    }

    OCAFSample_IDisplayer::Update(shape_function_node->Father());

    occdoc->CommitCommand();

    m_can_continue = true;
    m_redo_called  = true;
}

void SetParameterCmd::redo()
{
    MYLOG;

    if (m_redo_called) return;

    ocafRedo();
    m_param_item->addParameterRef(m_shape_function_item, m_target_tags);

    m_redo_called = true;
}

void SetParameterCmd::undo()
{
    if (!m_redo_called) return;

    ocafUndo();
    m_param_item->removeParameterRef(m_shape_function_item, m_target_tags);

    m_redo_called = false;
}
