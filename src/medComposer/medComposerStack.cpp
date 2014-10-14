#include <medComposerStack.h>
#include <medComposerStackCommand.h>


void medComposerStack::push(medComposerStackCommand* command)
{
    QUndoStack::push(command);
}
