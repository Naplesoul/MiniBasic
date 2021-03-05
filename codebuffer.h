#ifndef CODEBUFFER_H
#define CODEBUFFER_H

#include <QList>
#include <QString>
#include "initialize.h"

class CodeBuffer
{
private:
    int currentLineNum = 0;
    QList<QString> codeList;

public:
    CodeBuffer();
    ~CodeBuffer();

    int size();

    const QString &moveToLine(int idx);

    void showLines(int from, int to);

    void deleteLines(int from, int to);
    void insertLine(const QString &text);
    void appendLine(const QString &text);

};

#endif // CODEBUFFER_H
