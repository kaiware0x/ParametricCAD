#ifndef ABSTRACTCMD_H
#define ABSTRACTCMD_H

#include <QUndoCommand>

class Document;

class AbstractCmd : public QUndoCommand {
public:
    AbstractCmd(const QString& text, Document* document);

    virtual void redo() override = 0;
    virtual void undo() override = 0;

    bool canContinue() const { return m_can_continue; }

protected:
    void ocafRedo();
    void ocafUndo();

protected:
    Document* m_document;
    bool      m_can_continue = false;
    bool      m_redo_called  = false;
};

#endif    // ABSTRACTCMD_H
