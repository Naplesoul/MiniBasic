#ifndef CODE_H
#define CODE_H

#include <QList>
#include <QString>
#include <QObject>
#include "initialize.h"

// a line of code, with a line number and a QString contains code
struct Line
{
    int lineNum;
    QString code;

    Line(int n, QString c): lineNum(n), code(c){}
};

// store lines of code in codeList
class CodeBuffer
{
private:
    QList<Line> codeList;

public:
    CodeBuffer();
    ~CodeBuffer();

// get the number of lines stored in the buffer
    int size();

// delete a line, returns whether the operation is successful
    bool deleteLine(int lineNum);

// insert a line, returns whether the operation is successful
    bool insertLine(const Line &text);

// get all code in a QString
    QString printCode();
    void clear();

// get all code in a QList of Line
    const QList<Line>& getCode() const;
};


// QObject class interacts with MainWindow, contains CodeBuffer
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
    const QList<Line>& getCode() const;
};

#endif // CODE_H
