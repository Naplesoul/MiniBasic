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

    bool parseStatements(const QList<Line> &code);
    bool run(QString &input, QString &output);

    void initialize();

private:
    QList<Statement*> statements;
    EvaluationContext evaluationContext;

    int pc;

signals:
    void printTree(QString output);
};

#endif // PROGRAM_H
