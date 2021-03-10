#ifndef CODE_H
#define CODE_H

#include <QList>
#include <QString>
#include <QObject>
#include "initialize.h"

struct Line
{
    int lineNum;
    QString code;

    Line(int n, QString c): lineNum(n), code(c){}
};

class CodeBuffer
{
private:
    int currentLineNum = 0;
    QList<Line> codeList;

public:
    CodeBuffer();
    ~CodeBuffer();

    int size();

    bool deleteLine(int lineNum);
    bool insertLine(const Line &text);

    QString printCode();
    void clear();

};




class Code : public QObject
{
    Q_OBJECT
private:
    CodeBuffer *codeBuffer;

public:
    explicit Code(QObject *parent = nullptr);
    bool insert(Line &input);
    bool del(int lineNum);
    int load(const QString &filename);
    bool save(const QString &filename);
    void clear();
    QString printCode();

signals:
    void print(QString);
    void quit();

};

#endif // CODE_H
