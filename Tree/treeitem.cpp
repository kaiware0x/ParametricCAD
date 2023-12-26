
#include <TDataStd_Name.hxx>
#include <TDocStd_Modified.hxx>

#include "treeitem.h"

TreeItem::TreeItem(const Type &type, const QString &name) : QStandardItem(name), m_type(type)
{
    setCheckable(true);
    setCheckState(Qt::Checked);
    setEditable(false);
}

TreeItem *TreeItem::child(const int &row) const
{
    auto stditem = QStandardItem::child(row);
    return static_cast<TreeItem *>(stditem);
}

void TreeItem::updateCheckState()
{
    m_prev_check_state = checkState();
}

bool TreeItem::checkStateIsUpdated()
{
    return m_prev_check_state == checkState();
}
