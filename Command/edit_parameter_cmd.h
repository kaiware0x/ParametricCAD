#ifndef EDITPARAMETERCMD_H
#define EDITPARAMETERCMD_H

#include <QUndoCommand>

#include <TDataStd_TreeNode.hxx>

#include "abstract_cmd.h"
#include "values.h"

class TreeItem;

class EditParameterCmd : public AbstractCmd {
public:
    void extracted(Document *&document);
    EditParameterCmd(const Handle(TDataStd_TreeNode) & param_node, const Parameter &param, Document *document);

    void redo() override;
    void undo() override;

private:
    Handle(TDataStd_TreeNode) m_param_node;
    TreeItem *m_param_tritem;

    QString m_old_name;
    QString m_new_name;
};

#endif    // EDITPARAMETERCMD_H
