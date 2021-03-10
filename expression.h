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

private:

   QMap<QString,int> symbolTable;

};


class Expression
{
public:
    Expression();
    virtual ~Expression();
    virtual int eval(EvaluationContext & context) = 0;
    virtual QString toString() = 0;
    virtual ExpressionType type() = 0;

    virtual int getConstantValue();
    virtual QString getIdentifierName();
    virtual QString getOperator();
    virtual Expression *getLHS();
    virtual Expression *getRHS();

};

class ConstantExp: public Expression
{

public:

    ConstantExp(int val);
    ~ConstantExp();
    virtual int eval(EvaluationContext & context);
    virtual QString toString();
    virtual ExpressionType type();

    virtual int getConstantValue();

private:

   int value;

};

class IdentifierExp: public Expression
{

public:

   IdentifierExp(string name);

    virtual int eval(EvaluationContext & context);
    virtual QString toString();
    virtual ExpressionType type();

    virtual QString getIdentifierName();

private:

   QString name;

};

class CompoundExp: public Expression
{

public:

   CompoundExp(string op, Expression *lhs, Expression *rhs);
   virtual ~CompoundExp();

   virtual int eval(EvaluationContext & context);
   virtual QString toString();
   virtual ExpressionType type();

   virtual QString getOperator();
   virtual Expression *getLHS();
   virtual Expression *getRHS();

private:

   QString op;
   Expression *lhs, *rhs;

};



#endif // EXPRESSION_H
