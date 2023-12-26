#ifndef MAKEPARAMETERCMD_H
#define MAKEPARAMETERCMD_H

#include "abstract_cmd.h"
#include "tree/treeitem.h"
#include "values.h"

class MakeParameterCmd : public AbstractCmd {
public:
    MakeParameterCmd(const Parameter& param, Document* document);

    void redo() override;
    void undo() override;

private:
    int       m_row;
    TreeItem* m_param_tritem;
};

#endif    // MAKEPARAMETERCMD_H
