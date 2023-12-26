#ifndef TREEITEM_H
#define TREEITEM_H

#include <QSet>
#include <QStandardItem>

#include "FromSample/Tags.h"

class TreeItem : public QStandardItem {
public:
    enum class Type {
        Main,
        Parameters,
        Parameter,
        Shapes,
        Shape,
        ShapeFunction,
        Others,
    };
    ~TreeItem() {}
    Type           myType() const { return m_type; }
    TreeItem*      child(const int& row) const;
    Qt::CheckState prevCheckState() const { return m_prev_check_state; }
    void           updateCheckState();
    bool           checkStateIsUpdated();

    void addParameterRef(TreeItem* item, const tag::Type& tag_) { m_parameterized_shape_tritems[item] << tag_; }
    void addParameterRef(TreeItem* item, const QSet<tag::Type>& tags)
    {
        m_parameterized_shape_tritems[item].unite(tags);
    }
    bool removeParameterRef(TreeItem* item, const tag::Type& tag_)
    {
        return m_parameterized_shape_tritems[item].remove(tag_);
    }
    bool removeParameterRef(TreeItem* item, const QSet<tag::Type>& tags)
    {
        m_parameterized_shape_tritems[item].subtract(tags);
        return true;
    }

private:
    /// TreeModelしかTreeItemをnewできないようにする.
    friend class TreeModel;
    TreeItem(const Type& type, const QString& name);

private:
    const Type     m_type;
    Qt::CheckState m_prev_check_state = Qt::Checked;

    /// Parameter TreeItem が保持する Shape TreeItem
    /// パラメータ適用先のShape
    QMap<TreeItem*, QSet<tag::Type>> m_parameterized_shape_tritems;
};

#endif    // TREEITEM_H
