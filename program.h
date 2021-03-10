#ifndef PROGRAM_H
#define PROGRAM_H

#include <QObject>
#include <QList>
#include "statement.h"
#include "code.h"

class Program : public QObject
{
    Q_OBJECT
public:
    explicit Program(QObject *parent = nullptr);

    bool praseStatements(QList<Line> code);

private:
    QList<Statement> statements;
signals:
    void print(QString &output);
};

#endif // PROGRAM_H
