#include "codeeditor.h"

CodeEditor::CodeEditor(QObject *parent) : QObject(parent)
{
    setParent(parent);
    codeBuffer = new CodeBuffer;
}

void CodeEditor::receiveInput(QString &input)
{

}
