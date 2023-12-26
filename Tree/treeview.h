#ifndef TREEVIEW_H
#define TREEVIEW_H

#include <QTreeView>

#include <TDataStd_TreeNode.hxx>

class TreeModel;

class TreeView : public QTreeView {
    Q_OBJECT
public:
    TreeView(QWidget* parent);

private:
    void       contextMenuEvent(QContextMenuEvent* event) override;
    TreeModel* model() const;

    void onParameterRightClicked(const QPoint& screen_pos, const Handle(TDataStd_TreeNode) & param_node);
    void onBoxFunctionRightClicked(const QPoint& screen_pos, const Handle(TDataStd_TreeNode) & box_func_node);
};

#endif    // TREEVIEW_H
