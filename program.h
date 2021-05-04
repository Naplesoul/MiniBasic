#ifndef PROGRAM_H
#define PROGRAM_H

#include <QObject>
#include <QList>
#include "statement.h"
#include "code.h"


// control the parse and run of code, interacts with MainWindow
class Program : public QObject
{
    Q_OBJECT
public:
    explicit Program(QObject *parent = nullptr);

// return true if parsing is successful, store error infomation in statements.
    bool parseStatements(const QList<Line> &code);
// return true if run successfully, false if the statement needs input, throws a QString describing error infomation when meet error.
    bool run(QString &input, QString &output);

    void initialize();
    void clearContext();

    QList<bool> getValidity() const;
private:
    QList<Statement*> statements;
    EvaluationContext evaluationContext;

    int pc;

signals:
    void printTree(QString output);
};

#endif // PROGRAM_H
