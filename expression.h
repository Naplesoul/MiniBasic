#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <QString>
#include <QMap>
#include "initialize.h"

using namespace std;

class EvaluationContext
{

public:

    void setValue(QString var, int value);
    int getValue(QString var);
    bool isDefined(QString var);
    void clear(){symbolTable.clear();}

private:

    QMap<QString,int> symbolTable;

};


class Expression
{
public:
    Expression(){}
    virtual ~Expression();
    virtual int eval(EvaluationContext & context) = 0;
    virtual QString toString() = 0;
    virtual ExpressionType type() = 0;

    virtual int getConstantValue();
    virtual int precedence();
    virtual QString getIdentifierName();
    virtual QString getOperator();
    virtual Expression *getLHS();
    virtual Expression *getRHS();
    virtual void setLHS(Expression *lhs);
    virtual void setRHS(Expression *rhs);
    virtual void setOp(const QString &o);

};

class ConstantExp: public Expression
{

public:

    ConstantExp(int val):value(val){}
    ~ConstantExp(){}
    virtual int eval(EvaluationContext & context);
    virtual QString toString();
    virtual ExpressionType type(){return CONSTANT;}

    virtual int getConstantValue(){return value;}

private:

   int value;

};

class IdentifierExp: public Expression
{

public:
    IdentifierExp(const QString &var);
    ~IdentifierExp(){}
    bool isValidVarName(const QString &var);

    virtual int eval(EvaluationContext & context);
    virtual QString toString(){return name;}
    virtual ExpressionType type(){return IDENTIFIER;}

    virtual QString getIdentifierName(){return name;}

private:

    QString name;

};

class CompoundExp: public Expression
{

public:
    CompoundExp(QString content);
    CompoundExp(QString op, Expression *lhs, Expression *rhs):op(op),lhs(lhs),rhs(rhs){}
    int findNearestOp(const QString &content);
    virtual ~CompoundExp();

    virtual int eval(EvaluationContext & context);
    virtual QString toString();
    virtual ExpressionType type(){return COMPOUND;}
    virtual int precedence();

    virtual QString getOperator(){return op;}
    virtual Expression *getLHS(){return lhs;}
    virtual Expression *getRHS(){return rhs;}

    virtual void setLHS(Expression *l){lhs = l;}
    virtual void setRHS(Expression *r){rhs = r;}
    virtual void setOp(const QString &o){op = o;}

private:
    QString op;
    Expression *lhs, *rhs;

};



#endif // EXPRESSION_H
