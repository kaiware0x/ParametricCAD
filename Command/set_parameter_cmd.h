#ifndef SETPARAMETERCMD_H
#define SETPARAMETERCMD_H

#include <QSet>

#include <TDataStd_TreeNode.hxx>

#include "abstract_cmd.h"
#include "FromSample/Tags.h"

class TreeItem;

class SetParameterCmd : public AbstractCmd {
public:
    void extracted(Document *&document);
    SetParameterCmd(const Handle(TDataStd_TreeNode) & shape_function_node, const Handle(TDataStd_TreeNode) & param_node,
                    const QSet<tag::Type> &target_tags, Document *document);

    void redo() override;
    void undo() override;

private:
    Handle(TDataStd_TreeNode) m_shape_function_node;
    Handle(TDataStd_TreeNode) m_param_node;

    const QSet<tag::Type> m_target_tags;

    TreeItem *m_shape_function_item;
    TreeItem *m_param_item;
};

#endif    // SETPARAMETERCMD_H
