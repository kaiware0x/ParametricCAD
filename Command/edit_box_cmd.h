#ifndef EDITBOXCMD_H
#define EDITBOXCMD_H

#include <QUndoCommand>

#include <TDataStd_TreeNode.hxx>

#include "abstract_cmd.h"
#include "values.h"

class EditBoxCmd : public AbstractCmd {
public:
    EditBoxCmd(const Handle(TDataStd_TreeNode) & box_tree_node, const BoxValue& box_param, Document* document);

    void redo() override;
    void undo() override;

private:
    Handle(TDataStd_TreeNode) m_box_func_node;
};

#endif    // EDITBOXCMD_H
