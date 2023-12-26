#ifndef CHANGEVISIBILITYCMD_H
#define CHANGEVISIBILITYCMD_H

#include <QUndoCommand>
#include "AIS_Shape.hxx"
#include "tree/treeitem.h"

class ChangeVisibilityCmd : public QUndoCommand {
public:
    ChangeVisibilityCmd(TreeItem* tritem);

    void redo() override;
    void undo() override;

private:
    bool m_can_continue = false;
    bool m_redo_called  = false;

    bool       m_show = false;
    TreeItem*  m_tritem;
    AIS_Shape* m_shape;
};

#endif    // CHANGEVISIBILITYCMD_H
