#ifndef STATEMENT_H
#define STATEMENT_H

#include "expression.h"
#include "initialize.h"
#include "code.h"
#include <QString>


bool isValidVarName(const QString &var);

class Statement
{
public:
    Statement(){}
    virtual ~Statement();

    int lineNum;
    StatementType type;

    virtual bool parse(const QString &code);
    virtual QString run(EvaluationContext &evaluationContext, int &pc);

protected:
    void clearTree(CompoundExp *root);
};



class RemStmt : public Statement
{
public:
    RemStmt(int l){lineNum = l, type = REMSTMT;}
    ~RemStmt(){}
};




class LetStmt : public Statement
{
public:
    LetStmt(int l){lineNum = l, type = LETSTMT;}
    ~LetStmt(){clearTree(exp);}

    bool parse(const QString &code);
    QString run(EvaluationContext &evaluationContext, int &pc);
private:
    CompoundExp *exp;
};




class PrintStmt : public Statement
{
public:
    PrintStmt(int l){lineNum = l, type = PRINTSTMT;}
    ~PrintStmt(){clearTree(exp);}

    bool parse(const QString &code);
    QString run(EvaluationContext &evaluationContext, int &pc);
private:
    CompoundExp *exp;
};




class InputStmt : public Statement
{
public:
    InputStmt(int l){lineNum = l, type = INPUTSTMT;}
    ~InputStmt(){delete var;}

    bool parse(const QString &code);
    QString run(EvaluationContext &evaluationContext, int &pc);
private:
    IdentifierExp *var;
};




class GotoStmt : public Statement
{
public:
    GotoStmt(int l){lineNum = l, type = GOTOSTMT;}
    ~GotoStmt(){}

    bool parse(const QString &code);
    QString run(EvaluationContext &evaluationContext, int &pc);
private:
    int targetPC;
};




class IfStmt : public Statement
{
public:
    IfStmt(int l){lineNum = l, type = IFSTMT;}
    ~IfStmt(){clearTree(lExp); clearTree(rExp);}

    bool parse(const QString &code);
    QString run(EvaluationContext &evaluationContext, int &pc);
private:
    CompoundExp *lExp;
    CompoundExp *rExp;
    QString compareOp;
    int targetPC;
};




class EndStmt : public Statement
{
public:
    EndStmt(int l){lineNum = l, type = ENDSTMT;}
    ~EndStmt();

    bool parse(const QString &code);
    QString run(EvaluationContext &evaluationContext, int &pc);
};

#endif // STATEMENT_H
