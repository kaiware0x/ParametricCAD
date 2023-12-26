
#include <QContextMenuEvent>
#include <QHeaderView>
#include <QMenu>

#include "FromSample/OCAFSample_IShape.hxx"
#include "FromSample/OCAFSample_ObjectType.hxx"

#include "Defines.h"
#include "mainwindow.h"
#include "treeitem.h"
#include "treemodel.h"
#include "treeview.h"

TreeView::TreeView(QWidget *parent) : QTreeView(parent)
{
    this->header()->hide();
    setAutoScroll(false);
}

void TreeView::contextMenuEvent(QContextMenuEvent *event)
{
    const auto &sel_idxs = selectionModel()->selectedIndexes();
    if (sel_idxs.isEmpty()) {
        return;
    }
    const auto &sel_idx  = sel_idxs.constFirst();
    const auto &sel_item = model()->itemFromIndex(sel_idx);

    const auto tree_node = model()->nodeFromItem(sel_item);

    const OCAFSample_ObjectType aType = OCAFSample_IShape::GetObjectType(tree_node);

    MYLOG << VAROUT(aType);

    if (aType == OCAFSample_ObjectType::OCAFSample_Box) {
        onBoxFunctionRightClicked(event->globalPos(), tree_node);
    }

    if (sel_item->myType() == TreeItem::Type::Parameter) {
        onParameterRightClicked(event->globalPos(), tree_node);
    }
}

TreeModel *TreeView::model() const
{
    return static_cast<TreeModel *>(QTreeView::model());
}

void TreeView::onParameterRightClicked(const QPoint &screen_pos, const Handle(TDataStd_TreeNode) & param_node)
{
    QMenu menu(this);

    auto edit_act = menu.addAction("Edit");

    auto selected_act = menu.exec(screen_pos);

    if (selected_act == edit_act) {
        MainWindow::instance()->showParameterDlg(param_node);
    }
}

void TreeView::onBoxFunctionRightClicked(const QPoint &screen_pos, const Handle(TDataStd_TreeNode) & box_func_node)
{
    QMenu menu(this);

    auto edit_act = menu.addAction("Edit");

    auto selected_act = menu.exec(screen_pos);

    if (selected_act == edit_act) {
        MainWindow::instance()->showBoxDlg(box_func_node);
    }
}
