#ifndef TREEMODEL_H
#define TREEMODEL_H

#include <QStandardItemModel>

#include "TDF_Label.hxx"
#include "TDataStd_TreeNode.hxx"

#include "treeitem.h"

class MainWindow;
class Document;
class ViewerWidget;

/**
 * @brief Ensembleの SpdDocument と SpdModel を兼ねたもの.
 */
class TreeModel : public QStandardItemModel {
    Q_OBJECT
    friend class TreeView;

public:
    explicit TreeModel(const TDF_Label& root_label, const TDF_Label& main_label, QObject* parent = nullptr);

    TreeItem* invisibleRootItem() const;
    TreeItem* itemFromIndex(const QModelIndex& index) const;

    TreeItem* newTreeItem(const TreeItem::Type& type, const TDF_Label& label);
    TreeItem* newChildTreeItem(TreeItem* parent_item, const TreeItem::Type& child_type, const TDF_Label& child_label);

    void registerItemToNode(TreeItem* item, Handle(TDataStd_TreeNode) node);

    TreeItem* parametersTreeItem() const;
    TreeItem* shapesTreeItem() const;
    Handle(TDataStd_TreeNode) parametersTreeNode() const;
    Handle(TDataStd_TreeNode) shapesTreeNode() const;

    TreeItem* itemFromNode(Handle(TDataStd_TreeNode) node) const;
    Handle(TDataStd_TreeNode) nodeFromItem(TreeItem* treeitem) const;

Q_SIGNALS:
    void checkStateChanged(TreeItem* item);

private Q_SLOTS:
    void onItemChanged(QStandardItem* stditem);

private:
    TreeItem* m_main_item;

    QHash<TreeItem*, Handle(TDataStd_TreeNode)> m_item_to_node_map;
};

#endif    // TREEMODEL_H
