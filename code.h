#ifndef CODE_H
#define CODE_H

#include <QList>
#include <QString>
#include <QObject>
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
    int writeToFile(const QString &filename) const;
    void deleteLines(int from, int to);
    void insertLine(const QString &text);
    void appendLine(const QString &text);

};




class Code : public QObject
{
    Q_OBJECT
private:
    CodeBuffer *codeBuffer;

public:
    explicit Code(QObject *parent = nullptr);
    void receiveInput(QString &input);

signals:

};

#endif // CODE_H
