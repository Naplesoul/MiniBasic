#ifndef CODEEDIT_H
#define CODEEDIT_H

#include <QObject>
#include "codebuffer.h"
#include "initialize.h"

class CodeEditor : public QObject
{
    Q_OBJECT
private:
    CodeBuffer *codeBuffer;
public:
    explicit CodeEditor(QObject *parent = nullptr);
    void receiveInput(QString &input);

signals:

};

#endif // CODEEDIT_H
