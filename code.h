#ifndef CODE_H
#define CODE_H

#include <QList>
#include <QString>
#include <QObject>
#include "initialize.h"

struct Line
{
    int rowNum;
    QString code;

    Line(int n, QString c): rowNum(n), code(c){}
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

    const QString &moveToLine(int idx);

    void showLines(int from, int to);
    int writeToFile(const QString &filename) const;
    void deleteLines(int from, int to);
    bool insertLine(const Line &text);
    void appendLine(const QString &text);

};




class Code : public QObject
{
    Q_OBJECT
private:
    CodeBuffer *codeBuffer;

public:
    explicit Code(QObject *parent = nullptr);
    bool insert(Line &input);
    bool load(const QString &filename);
    bool save(const QString &filename) const;
    void clear();
    void printCode();

signals:
    void print(QString);
    void quit();

};

#endif // CODE_H
