#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <QString>
#include <QMap>
#include "initialize.h"

using namespace std;

// contains value for variables
class EvaluationContext
{
public:
    void setValue(QString var, int value);
    int getValue(QString var);
    bool isDefined(QString var);
    bool deleteValue(QString var) {return symbolTable.remove(var);}
    void clear(){symbolTable.clear(); stringTable.clear();}

    void setString(QString var, QString val);
    QString getString(QString var);
    bool isStringDefined(QString var);
    bool deleteString(QString var) {return stringTable.remove(var);}

    QString printContext();

private:
    QMap<QString,QString> stringTable;
    QMap<QString,int> symbolTable;

};


// super class for all kinds of expressions
class Expression
{
public:
    Expression(){}
    virtual ~Expression(){}
    virtual int eval(EvaluationContext & context) = 0;
// get syntax tree in a QString
    virtual QString toTree(int layer) = 0;
    virtual QString toString() = 0;
    virtual ExpressionType type() = 0;

    virtual int getConstantValue(){return 0;}
    virtual int precedence(){return 0;}
    virtual QString getIdentifierName(){return "";}
    virtual QString getOperator(){return "";}
    virtual Expression *getLHS(){return nullptr;}
    virtual Expression *getRHS(){return nullptr;}
    virtual void setLHS(Expression *){};
    virtual void setRHS(Expression *){}
    virtual void setOp(const QString &){}

    bool isIntNumber(const QString &s)
    {
        int len = s.length();
        for(int i = 0; i < len; ++i)
        {
            if(!(s[i] <= '9' && s[i] >='0'))
                return false;
        }
        return true;
    }

};

class ConstantExp: public Expression
{

public:

    ConstantExp(int val):value(val){}
    ~ConstantExp(){}
    virtual int eval(EvaluationContext & context);
    virtual QString toTree(int layer);
    virtual QString toString(){return QString::number(value);}
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
    virtual QString toTree(int layer);
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
    virtual QString toTree(int layer);
    virtual QString toString(){return op;}
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
    Expression *lhs = nullptr, *rhs = nullptr;

};



#endif // EXPRESSION_H
