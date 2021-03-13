#include "statement.h"

bool LetStmt::parse(const QString &code)
{
    QString content = code.trimmed();
    QString var;
    int len = content.length();
    int end = -1;
    for(int i = 0; i < len; ++i)
    {
        if(content[i] == '=')
        {
            end = i - 1;
            break;
        }
    }

    if(end < 0)
    {
        throw QString("Syntax error in Line ") + QString::number(lineNum);
    }

    var = content.left(end + 1).trimmed();

    IdentifierExp *lhs;
    CompoundExp *rhs;

    try
    {
        lhs = new IdentifierExp(var);
        rhs = new CompoundExp(content.mid(end + 2).trimmed());
    }
    catch(QString err)
    {
        throw err + QString::number(lineNum);
    }

    exp = new CompoundExp("=", lhs, rhs);

    return true;
}

QString LetStmt::run(EvaluationContext &evaluationContext, int &pc)
{
    evaluationContext.setValue(exp->getLHS()->toString(), exp->getRHS()->eval(evaluationContext));
    return "";
}

bool PrintStmt::parse(const QString &code)
{
    exp = new CompoundExp(code);
    return true;
}

QString PrintStmt::run(EvaluationContext &evaluationContext, int &pc)
{
    int val = exp->eval(evaluationContext);
    return QString::number(val) + "\n";
}

bool GotoStmt::parse(const QString &code)
{
    if(!isInt(code))
        throw QString("Target position should be int in Line ");
    int target = code.toInt();
    if(target < 0)
        throw QString("Target position should be positive in Line ");
    targetPC = target;
    return true;
}

QString GotoStmt::run(EvaluationContext &evaluationContext, int &pc)
{
    pc = targetPC;
    return "";
}

bool IfStmt::parse(const QString &code)
{
    QString content = code.trimmed();
    int len = content.length();
    int op = -1;
    for(int i = 0; i < len; ++i)
    {
        if(content[i] == '=' || content[i] == '<' || content[i] == '>')
        {
            op = i;
            break;
        }
    }

    if(op <= 0)
    {
        throw QString("Syntax error in Line ") + QString::number(lineNum);
    }

    compareOp = content[op];
    lExp = new CompoundExp(content.left(op - 1).trimmed());
    content = content.mid(op + 1).trimmed();
    len = content.length();
    op = -1;
    for(int i = 0; i < len - 3; ++i)
    {
        if(content[i] == 'T' || content[i + 1] == 'H'
                || content[i + 2] == 'E' || content[i + 3] == 'N')
        {
            op = i;
            break;
        }
    }
    if(op <= 0)
    {
        throw QString("Syntax error in Line ") + QString::number(lineNum);
    }
    rExp = new CompoundExp(content.left(op - 1).trimmed());
    content = content.mid(op + 4).trimmed();

    if(!isInt(content))
        throw QString("Target position should be int in Line ");
    int target = content.toInt();
    if(target < 0)
        throw QString("Target position should be positive in Line ");
    targetPC = target;
    return true;
}

QString IfStmt::run(EvaluationContext &evaluationContext, int &pc)
{
    int left = lExp->eval(evaluationContext);
    int right = rExp->eval(evaluationContext);
    if(compareOp == ">")
    {
        if(left > right)
            pc = targetPC;
    }
    else if(compareOp == "<")
    {
        if(left < right)
            pc = targetPC;
    }
    else if(compareOp == "=")
    {
        if(left == right)
            pc = targetPC;
    }
    return "";
}
