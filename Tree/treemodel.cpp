
#include <TDataStd_Name.hxx>

#include "Defines.h"
#include "document.h"
#include "mainwindow.h"
#include "Util.h"
#include "treemodel.h"

/**
 * @brief TreeModel::TreeModel
 * @param main_label
 * @param parent
 */
TreeModel::TreeModel(const TDF_Label &root_label, const TDF_Label &main_label, QObject *parent)
    : QStandardItemModel(parent)
{
    beginResetModel();

    ///---------------------------------------------------------------------------------------------------
    /// Entry"0"のRootLabelとRootTreeItemの紐づけ.
    ///---------------------------------------------------------------------------------------------------
    auto root_item = invisibleRootItem();
    auto root_node = util::setTreeNode(root_label);
    registerItemToNode(root_item, root_node);

    ///---------------------------------------------------------------------------------------------------
    /// Entry"0:1"のMainLabelに対応するMainTreeItemを生成.
    ///---------------------------------------------------------------------------------------------------
    util::setName(main_label, "Main");
    m_main_item = newChildTreeItem(root_item, TreeItem::Type::Main, main_label);

    ///---------------------------------------------------------------------------------------------------
    /// Parameters TreeItemを生成.
    ///---------------------------------------------------------------------------------------------------
    auto param_label = main_label.NewChild();
    util::setName(param_label, "Parameters");
    newChildTreeItem(m_main_item, TreeItem::Type::Parameters, param_label);

    ///---------------------------------------------------------------------------------------------------
    /// Shapes TreeItemを生成.
    ///---------------------------------------------------------------------------------------------------
    auto shapes_label = main_label.NewChild();
    util::setName(shapes_label, "Shapes");
    newChildTreeItem(m_main_item, TreeItem::Type::Shapes, shapes_label);

    endResetModel();

    connect(this, &TreeModel::itemChanged, this, &TreeModel::onItemChanged);
}

TreeItem *TreeModel::invisibleRootItem() const
{
    auto std_root = QStandardItemModel::invisibleRootItem();
    return static_cast<TreeItem *>(std_root);
}

TreeItem *TreeModel::itemFromIndex(const QModelIndex &index) const
{
    const auto &std_item = QStandardItemModel::itemFromIndex(index);
    return static_cast<TreeItem *>(std_item);
}

/**
 * @brief TreeItemの生成はTreeModelに任せ、TreeItem生成と同時にTreeNodeと紐づけるようにする.
 * @param label
 * @return
 */
TreeItem *TreeModel::newTreeItem(const TreeItem::Type &type, const TDF_Label &label)
{
    const QString &name = util::getName(label);

    MY_ASSERT(!name.isEmpty(), "Cannot find TDataStd_Name attribute in label");

    auto treeitem = new TreeItem(type, name);

    /// labelがTreeNode属性を持っていなければ付与.
    auto treenode = util::getTreeNode(label);
    if (treenode.IsNull()) {
        treenode = util::setTreeNode(label);
    }

    /// TreeItemとTreeNodeを紐づけ.
    registerItemToNode(treeitem, treenode);

    return treeitem;
}

/**
 * @brief TreeModel::newChildTreeItem
 * @param parent_item
 * @param child_label
 * @return
 */
TreeItem *TreeModel::newChildTreeItem(TreeItem *parent_item, const TreeItem::Type &child_type,
                                      const TDF_Label &child_label)
{
    ///--------------------------------------------------------------------------------------------------------
    /// Qt Tree (OCC Treeより先にやらないと落ちるが原因不明)
    ///--------------------------------------------------------------------------------------------------------
    auto child_item = newTreeItem(child_type, child_label);

    /// 親TreeItemに新しい子TreeItemをSet
    parent_item->appendRow(child_item);

    ///--------------------------------------------------------------------------------------------------------
    /// OCC Tree
    ///--------------------------------------------------------------------------------------------------------
    /// TreeNode from TreeItem
    auto parent_node = nodeFromItem(parent_item);

    /// TreeNode from Label
    auto child_node = util::getTreeNode(child_label);

    /// 親Labelに新しい子LabelをSet
    parent_node->Append(child_node);

    return child_item;
}

/**
 * @brief TreeModel::newChildTreeItem
 * @param parent_item
 * @param child_label
 * @return
 */
void TreeModel::registerItemToNode(TreeItem *item, Handle(TDataStd_TreeNode) node)
{
    m_item_to_node_map.insert(item, node);
}

TreeItem *TreeModel::parametersTreeItem() const
{
    return m_main_item->child(0);
}

TreeItem *TreeModel::shapesTreeItem() const
{
    return m_main_item->child(1);
}

Handle(TDataStd_TreeNode) TreeModel::parametersTreeNode() const
{
    return nodeFromItem(parametersTreeItem());
}

Handle(TDataStd_TreeNode) TreeModel::shapesTreeNode() const
{
    return nodeFromItem(shapesTreeItem());
}

TreeItem *TreeModel::itemFromNode(Handle(TDataStd_TreeNode) node) const
{
    auto treeitem = m_item_to_node_map.key(node, nullptr);
    MY_ASSERT(treeitem != nullptr, "Cannot find TreeItem from m_item_to_node_map.");
    return treeitem;
}

Handle(TDataStd_TreeNode) TreeModel::nodeFromItem(TreeItem *treeitem) const
{
    auto treenode = m_item_to_node_map.value(treeitem, Handle(TDataStd_TreeNode)());
    MY_ASSERT(!treenode.IsNull(), "Cannot find TreeNode from m_item_to_node_map.");
    return treenode;
}

void TreeModel::onItemChanged(QStandardItem *stditem)
{
    auto item = static_cast<TreeItem *>(stditem);
    if (!item->checkStateIsUpdated()) {
        MYLOG << "Check state changed";
        emit checkStateChanged(item);
        item->updateCheckState();
    }
}
