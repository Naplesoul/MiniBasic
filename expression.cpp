#include "expression.h"
#include <QStack>

void EvaluationContext::setValue(QString var, int value)
{
    if(isDefined(var))
    {
        symbolTable.remove(var);
        symbolTable.insert(var, value);
    }
    else
        symbolTable.insert(var, value);
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

QString ConstantExp::toString()
{
    return QString::number(value);
}

int IdentifierExp::eval(EvaluationContext & context)
{
   if (!context.isDefined(name))
       throw QString(name + " is undefined");
   return context.getValue(name);
}

int CompoundExp::eval(EvaluationContext & context)
{
   int right = rhs->eval(context);
   if (op == "=") {
      context.setValue(lhs->getIdentifierName(), right);
      return right;
   }
   int left = lhs->eval(context);
   if (op == "+") return left + right;
   if (op == "-") return left - right;
   if (op == "*") return left * right;
   if (op == "/") {
      if (right == 0) throw QString("Division by 0");
      return left / right;
   }
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
    if(!((var[1] >= 'a' && var[1] <= 'z')
         || (var[1] >= 'A' && var[1] <= 'Z')
         || var[1] == '_'))
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
    QList<Expression*> expression;
    QStack<Expression*> operators;
    QStack<Expression*> operands;

    if(content.contains('='))
        throw QString("Syntax error in Line ");
    int opPosition = findNearestOp(content);
    while(opPosition >= 0)
    {
        QString op(content[opPosition]);
        if(opPosition == 0 && op == "-")
        {
            op = "--";
            expression.push_back(new CompoundExp(op, nullptr, nullptr));
            continue;
        }
        QString newExp = content.left(opPosition - 1).trimmed();
        if(isInt(newExp))
            expression.push_back(new ConstantExp(newExp.toInt()));
        else
            expression.push_back(new IdentifierExp(newExp));


        if(op == "*" && content[opPosition + 1] == '*')
        {
            op += "*";
            opPosition++;
        }
        content = content.mid(opPosition + 1).trimmed();
        expression.push_back(new CompoundExp(op, nullptr, nullptr));
        opPosition = findNearestOp(content);
    }
    if(content.length() > 0)
    {
        if(isInt(content))
            expression.push_back(new ConstantExp(content.toInt()));
        else
            expression.push_back(new IdentifierExp(content));
    }

    while(!expression.empty())
    {
        QString op;
        int precedence;
        Expression *exp = expression.first();
        expression.pop_front();
        switch(exp->type())
        {
            case COMPOUND:
                op = exp->getOperator();
                if(op == "(")
                    operators.push(exp);
                if(op == ")")
                {
                    while(operators.top()->getOperator() != "(")
                    {
                        Expression *pop = operators.pop();

                        if(pop->getOperator() == "--")
                        {
                            if(operands.empty())
                                throw QString("Syntax error in Line ");
                            pop->setRHS(operands.pop());
                            pop->setLHS(new ConstantExp(0));
                            pop->setOp("-");
                            operands.push(pop);
                            continue;
                        }
                        if(operands.empty())
                            throw QString("Syntax error in Line ");
                        pop->setRHS(operands.pop());
                        if(operands.empty())
                            throw QString("Syntax error in Line ");
                        pop->setLHS(operands.pop());
                        operands.push(pop);
                    }
                    operators.pop();
                }

                precedence = exp->precedence();
                while(operators.top()->precedence() >= precedence)
                {
                    Expression *pop = operators.pop();

                    if(pop->getOperator() == "--")
                    {
                        if(operands.empty())
                            throw QString("Syntax error in Line ");
                        pop->setRHS(operands.pop());
                        pop->setLHS(new ConstantExp(0));
                        pop->setOp("-");
                        operands.push(pop);
                        continue;
                    }
                    if(operands.empty())
                        throw QString("Syntax error in Line ");
                    pop->setRHS(operands.pop());
                    if(operands.empty())
                        throw QString("Syntax error in Line ");
                    pop->setLHS(operands.pop());
                    operands.push(pop);
                }
                operands.push(exp);

                break;
            default:
                operands.push(exp);
                break;
        }
    }

    while(!operands.empty())
    {
        Expression *pop = operators.pop();

        if(pop->getOperator() == "--")
        {
            if(operands.empty())
                throw QString("Syntax error in Line ");
            pop->setRHS(operands.pop());
            pop->setLHS(new ConstantExp(0));
            pop->setOp("-");
            operands.push(pop);
            continue;
        }

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

QString CompoundExp::toString()
{
    return "";
}
