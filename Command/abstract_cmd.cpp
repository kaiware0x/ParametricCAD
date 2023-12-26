#include "abstract_cmd.h"
#include "Defines.h"
#include "document.h"

AbstractCmd::AbstractCmd(const QString &text, Document *document) : QUndoCommand(text), m_document(document)
{
    MY_ASSERT(document != nullptr, "Document is nullptr");
}

void AbstractCmd::ocafRedo()
{
    m_document->ocafDocument()->Redo();
    m_document->context()->UpdateCurrentViewer();
}

void AbstractCmd::ocafUndo()
{
    m_document->ocafDocument()->Undo();
    m_document->context()->UpdateCurrentViewer();
}
