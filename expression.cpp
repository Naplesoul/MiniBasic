#include "expression.h"
#include <QStack>
#include <QDebug>
#include <cmath>


void EvaluationContext::setValue(QString var, int value)
{
//    if(isDefined(var))
//    {
//        symbolTable.remove(var);
//        symbolTable.insert(var, value);
//    }
//    else
//        symbolTable.insert(var, value);

    symbolTable[var] = value;
}

int EvaluationContext::getValue(QString var)
{
    if(isDefined(var))
    {
        return symbolTable.value(var);
    }
    return 0;
}

bool EvaluationContext::isDefined(QString var)
{
    if(symbolTable.contains(var))
        return true;
    return false;
}

int ConstantExp::eval(EvaluationContext & context)
{
   return value;
}

QString ConstantExp::toTree(int layer)
{
    QString out = "";
    for(int i = 0; i < layer; ++i)
        out += "\t";
    out += QString::number(value);
    out += "\n";
    return out;
}
QString IdentifierExp::toTree(int layer)
{
    QString out = "";
    for(int i = 0; i < layer; ++i)
        out += "\t";
    out += name;
    out += "\n";
    return out;
}

int IdentifierExp::eval(EvaluationContext & context)
{
   if (!context.isDefined(name))
       throw QString(name + " is undefined");
   return context.getValue(name);
}

int CompoundExp::eval(EvaluationContext & context)
{
   int right;
   if(rhs)
      right = rhs->eval(context);
   else
       right = 0;
   int left;
   if(lhs)
      left = lhs->eval(context);
   else
       left = 0;
   if(!rhs)
       return left;
   if(!lhs)
       return right;
   if (op == "=") {
      context.setValue(lhs->getIdentifierName(), right);
      return right;
   }


   if (op == "+") return left + right;
   if (op == "-") return left - right;
   if (op == "*") return left * right;
   if (op == "/") {
      if (right == 0) throw QString("Division by 0");
      return left / right;
   }
   if (op == "**") return pow(left, right);
   throw QString("Illegal operator in expression");
   return 0;
}

IdentifierExp::IdentifierExp(const QString &var)
{
    if(!isValidVarName(var))
        throw QString("Invalid variable name in Line ");
    name = var;
}

bool IdentifierExp::isValidVarName(const QString &var)
{
    if(!((var[0] >= 'a' && var[0] <= 'z')
         || (var[0] >= 'A' && var[0] <= 'Z')
         || var[0] == '_'))
        return false;
    if(var.contains('~') || var.contains('!') || var.contains('@')
            || var.contains('#')|| var.contains('$')|| var.contains('%')
            || var.contains('^')|| var.contains('&')|| var.contains('*')
            || var.contains('(')|| var.contains(')')|| var.contains('-')
            || var.contains('+')|| var.contains(' ')|| var.contains(',')
            || var.contains('.')|| var.contains(';')|| var.contains(':'))
        return false;
    if(var == "INPUT" || var == "REM" || var == "LET" || var == "PRINT"
            || var == "GOTO" || var == "IF" || var == "THEN" || var == "END"
            || var == "RUN" || var == "LOAD" || var == "LIST" || var == "CLEAR"
            || var == "HELP")
        return false;
    return true;
}


int CompoundExp::findNearestOp(const QString &content)
{
    int len = content.length();
    for(int i = 0; i < len; ++i)
    {
        if(content[i] == '+' || content[i] == '-' || content[i] == '*'
                || content[i] == '/' || content[i] == '(' || content[i] == ')')
            return i;
    }
    return -1;
}

CompoundExp::CompoundExp(QString content)
{
    qDebug() << content <<'\n';
    QList<Expression*> expression;
    QStack<Expression*> operators;
    QStack<Expression*> operands;

    if(content.contains('='))
        throw QString("Syntax error in Line ");
    // find the first operator
    int opPosition = findNearestOp(content);
    // push all the expressions into QList expression
    while(opPosition >= 0)
    {
        QString op(content[opPosition]);
        // "-X" = "0 - X"
        if(opPosition == 0 && op == "-" && (expression.isEmpty() || (expression.last()->type() == COMPOUND && expression.last()->getOperator() == "(")))
        {
            expression.push_back(new ConstantExp(0));
            expression.push_back(new CompoundExp("-", nullptr, nullptr));

            content = content.mid(opPosition + 1).trimmed();
            opPosition = findNearestOp(content);
            continue;
        }
        QString newExp = content.left(opPosition).trimmed();
        qDebug() << newExp <<'\n';
        if(newExp != "")
        {
            if(isIntNumber(newExp))
                expression.push_back(new ConstantExp(newExp.toInt()));
            else
                expression.push_back(new IdentifierExp(newExp));
        }

        // operator is **
        if(op == "*" && content[opPosition + 1] == '*')
        {
            op += "*";
            opPosition++;
        }
        content = content.mid(opPosition + 1).trimmed();
        expression.push_back(new CompoundExp(op, nullptr, nullptr));
        opPosition = findNearestOp(content);
    }
    qDebug() << content <<'\n';
    // handle the last expression
    if(content.length() > 0)
    {
        if(isIntNumber(content))
            expression.push_back(new ConstantExp(content.toInt()));
        else
            expression.push_back(new IdentifierExp(content));
    }

    while(!expression.empty())
    {
        QString op;
        int precedence;
        Expression *exp = expression.front();
        expression.pop_front();
        switch(exp->type())
        {
            case COMPOUND:
                op = exp->getOperator();
                if(op == "(")
                {
                    operators.push(exp);
                    break;
                }
                if(op == ")")
                {
                    while(operators.top()->getOperator() != "(")
                    {
                        Expression *pop = operators.pop();

//                        if(pop->getOperator() == "--")
//                        {
//                            if(operands.empty())
//                                throw QString("Syntax error in Line ");
//                            pop->setRHS(operands.pop());
//                            pop->setLHS(new ConstantExp(0));
//                            pop->setOp("-");
//                            operands.push(pop);
//                            continue;
//                        }
                        if(operands.empty())
                            throw QString("Syntax error in Line ");
                        pop->setRHS(operands.pop());
                        if(operands.empty())
                            throw QString("Syntax error in Line ");
                        pop->setLHS(operands.pop());
                        operands.push(pop);
                    }
                    operators.pop();
                    break;
                }

                precedence = exp->precedence();
                if (op == "**")
                    precedence += 1;
                while(!operators.empty() && operators.top()->precedence() >= precedence)
                {
                    Expression *pop = operators.pop();

//                    if(pop->getOperator() == "--")
//                    {
//                        if(operands.empty())
//                            throw QString("Syntax error in Line ");
//                        pop->setRHS(operands.pop());
//                        pop->setLHS(new ConstantExp(0));
//                        pop->setOp("-");
//                        operands.push(pop);
//                        continue;
//                    }
                    if(operands.empty())
                        throw QString("Syntax error in Line ");
                    pop->setRHS(operands.pop());
                    if(operands.empty())
                        throw QString("Syntax error in Line ");
                    pop->setLHS(operands.pop());
                    operands.push(pop);
                }
                operators.push(exp);

                break;
            default:
                operands.push(exp);
                break;
        }
    }

    while(!operators.empty())
    {
        Expression *pop = operators.pop();

//        if(pop->getOperator() == "--")
//        {
//            if(operands.empty())
//                throw QString("Syntax error in Line ");
//            pop->setRHS(operands.pop());
//            pop->setLHS(new ConstantExp(0));
//            pop->setOp("-");
//            operands.push(pop);
//            continue;
//        }

        if(operands.empty())
            throw QString("Syntax error in Line ");
        pop->setRHS(operands.pop());
        if(operands.empty())
            throw QString("Syntax error in Line ");
        pop->setLHS(operands.pop());
        operands.push(pop);
    }
    if(operands.empty())
        throw QString("Syntax error in Line ");
    rhs = operands.pop();
    if(!operands.empty())
        throw QString("Syntax error in Line ");
}

int CompoundExp::precedence()
{
    if(op == "**")
        return 3;
    if(op == "*" || op =="/")
        return 2;
    if(op == "+" || op == "-" || op == "--")
        return 1;
    return 0;
}

CompoundExp::~CompoundExp()
{
    if(lhs)
        delete lhs;
    if(rhs)
        delete rhs;
}

QString CompoundExp::toTree(int layer)
{
    QString out = "";
    if (op == "")
    {
        if (lhs)
            out += lhs->toTree(layer);
        if (rhs)
            out += rhs->toTree(layer);
        return out;
    }
    for(int i = 0; i < layer; ++i)
        out += "\t";
    out += op + "\n";
    if (lhs)
        out += lhs->toTree(layer + 1);
    if (rhs)
        out += rhs->toTree(layer + 1);
    return out;
}
