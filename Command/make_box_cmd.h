#ifndef MAKEBOXCMD_H
#define MAKEBOXCMD_H

#include <QUndoCommand>

#include "abstract_cmd.h"
#include "values.h"

#include "tree/treeitem.h"

class MakeBoxCmd : public AbstractCmd {
public:
    MakeBoxCmd(const BoxValue& value, Document* document);

    void redo() override;
    void undo() override;

private:
    int       m_row;
    TreeItem* m_box_tritem;
};

#endif    // MAKEBOXCMD_H
