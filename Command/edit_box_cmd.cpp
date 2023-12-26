
#include "edit_box_cmd.h"
#include "Defines.h"
#include "document.h"
#include "mainwindow.h"

#include "FromSample/OCAFSample_IBox.hxx"
#include "FromSample/OCAFSample_IDisplayer.hxx"
#include "FromSample/OCAFSample_IObject.hxx"
#include "FromSample/OCAFSample_IShape.hxx"
#include "FromSample/OCAFSample_SolverEx.hxx"

EditBoxCmd::EditBoxCmd(const Handle(TDataStd_TreeNode) & box_func_node, const BoxValue& box_val, Document* document)
    : AbstractCmd("Edit Box", document), m_box_func_node(box_func_node)
{
    if (box_func_node.IsNull()) {
        MYWARNLOG << "box function node is null";
        return;
    }

    MYLOG;
    auto occdoc = document->ocafDocument();
    occdoc->OpenCommand();

    OCAFSample_IBox aIBox(box_func_node);
    aIBox.SetCenter(box_val.center());
    aIBox.GetDX()->Set(box_val.dx());
    aIBox.GetDY()->Set(box_val.dy());
    aIBox.GetDZ()->Set(box_val.dz());

    OCAFSample_SolverEx aSolver;
    if (!aSolver.SolveFrom(box_func_node)) {
        MYWARNLOG << "Cannot edit box.";
        occdoc->AbortCommand();
        return;
    }

    OCAFSample_IDisplayer::Update(box_func_node->Father());

    /// UpdateにLabelを渡しても TDF_ChildIterator で何故かIterateされない.
    //    OCAFSample_IDisplayer::Update(box_func_node->Label());

    occdoc->CommitCommand();

    m_can_continue = true;
    m_redo_called  = true;
}

void EditBoxCmd::redo()
{
    MYLOG;

    if (m_redo_called) {
        return;
    }

    ocafRedo();

    m_redo_called = true;
}

void EditBoxCmd::undo()
{
    MYLOG;

    if (!m_redo_called) {
        return;
    }

    ocafUndo();

    m_redo_called = false;
}
