#include "statement.h"
#include <QDebug>

bool LetStmt::parse(const QString &code)
{
    QString content = code.trimmed();
    QString varName;
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
        throw QString("[Syntax error in line ") + QString::number(lineNum) + "]\n";
    }

    varName = content.left(end + 1).trimmed();


    try
    {
        var = new IdentifierExp(varName);
        QString val = content.mid(end + 2).trimmed();
        int size = val.size();
        if (size > 0 && ((val[0] == '\'' && val[size - 1] == '\'') || (val[0] == '\"' && val[size - 1] == '\"'))) {
            val = val.mid(1, size - 2);
            size -= 2;
            for (int  i = 0; i < size; ++i) {
                if (val[i] == '\'' || val[i] == '\"')
                    throw QString("[Invalid string in line ");
            }
            isString = true;
            stringVal = val;
        } else {
            exp = new CompoundExp(val);
        }
    }
    catch(QString err)
    {
        throw QString(err + QString::number(lineNum) + "]\n");
    }


    return true;
}

bool LetStmt::run(EvaluationContext &evaluationContext, int &, QString &, QString &)
{
    if (isString)
        evaluationContext.setString(var->toString(), stringVal);
    else
        evaluationContext.setValue(var->toString(), exp->eval(evaluationContext));
    return true;
}

bool PrintStmt::parse(const QString &code)
{
    try
    {
        exp = new CompoundExp(code);
    }
    catch(QString err)
    {
        throw QString(err + QString::number(lineNum) + "]\n");
    }
    return true;
}

bool PrintStmt::run(EvaluationContext &evaluationContext, int &, QString &, QString &output)
{
    int val = exp->eval(evaluationContext);
    output += QString::number(val) + "\n";
    return true;
}

bool InputStmt::parse(const QString &code)
{
    var = new IdentifierExp(code);
    return true;
}

bool InputStmt::run(EvaluationContext &evaluationContext, int &, QString &input, QString &)
{
    input = input.trimmed();
    if(input.isEmpty())
        return false;
    int len = input.length();
    int numEnd = -1;
    for(int i = 0; i < len; ++i)
    {
        if(input[i] == ' ')
        {
            numEnd = i - 1;
            break;
        }
        if(i == len - 1)
        {
            numEnd = i;
        }
    }
    if(numEnd >= 0)
    {
        QString numberString = input.left(numEnd + 1);
        input = input.mid(numEnd + 2);
        if(!isIntNumber(numberString))
            throw QString("[Invalid input. Please input an int]\n[Program terminated with faults]");
        evaluationContext.setValue(var->toString(), numberString.toInt());
        return true;
    }
    return false;
}

bool InputsStmt::parse(const QString &code)
{
    var = new IdentifierExp(code);
    return true;
}

bool InputsStmt::run(EvaluationContext &evaluationContext, int &, QString &input, QString &)
{
    QString val = input.trimmed();
    if (val.isEmpty())
        return false;
    int size = val.size();
    if ((val[0] == '\'' && val[size - 1] == '\'') || (val[0] == '\"' && val[size - 1] == '\"')) {
        val = val.mid(1, size - 2);
        size -= 2;
        for (int  i = 0; i < size; ++i) {
            if (val[i] == '\'' || val[i] == '\"')
                throw QString("[Invalid string. Please input a stirng with \" or \' on both side]\n[Program terminated with faults]");
        }
        evaluationContext.setString(var->toString(), val);
        input.clear();
        return true;
    }
    throw QString("[Invalid string. Please input a stirng with \" or \' on both side]\n[Program terminated with faults]");
}

bool GotoStmt::parse(const QString &code)
{
    if(!isIntNumber(code))
        throw QString("[Target position should be an IMMEDIATE in line " + QString::number(lineNum) + "]\n");
    int target = code.toInt();
    if(target < 0)
        throw QString("[Target position should be positive in line " + QString::number(lineNum) + "]\n");
    targetPC = target;
    return true;
}

bool GotoStmt::run(EvaluationContext &, int &pc, QString &, QString &)
{
    pc = targetPC;
    return true;
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
        throw QString("[Syntax error in line " + QString::number(lineNum) + "]\n");
    }

    compareOp = content[op];
    try
    {
        lExp = new CompoundExp(content.left(op).trimmed());
    }
    catch(QString err)
    {
        throw QString(err + QString::number(lineNum) + "]\n");
    }

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
//        throw int(1);
        throw QString("[Syntax error in line " + QString::number(lineNum) + "]\n");
    }
    try
    {
        rExp = new CompoundExp(content.left(op).trimmed());
    }
    catch(QString err)
    {
        throw QString(err + QString::number(lineNum) + "]\n");
    }

    content = content.mid(op + 4).trimmed();

    if(!isIntNumber(content))
        throw QString("[Target position should be int in line " + QString::number(lineNum) + "]\n");
    int target = content.toInt();
    if(target < 0)
        throw QString("[Target position should be positive in line " + QString::number(lineNum) + "]\n");
    targetPC = target;
    return true;
}

bool IfStmt::run(EvaluationContext &evaluationContext, int &pc, QString &, QString &)
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
    return true;
}

QString LetStmt::printTree()
{
    QString tree = "";
    tree += QString::number(lineNum) + " LET =\n";
    tree += var->toTree(1);
    if (isString)
        tree += "\t\"" + stringVal + "\"";
    else
        tree += exp->toTree(1);
    return tree;
}

QString PrintStmt::printTree()
{
    QString tree = "";
    tree += QString::number(lineNum) + " PRINT\n";
    tree += exp->toTree(1);
    return tree;
}

QString InputStmt::printTree()
{
    QString tree = "";
    tree += QString::number(lineNum) + " INPUT\n";
    tree += var->toTree(1);
    return tree;
}

QString InputsStmt::printTree()
{
    QString tree = "";
    tree += QString::number(lineNum) + " INPUTS\n";
    tree += var->toTree(1);
    return tree;
}

QString GotoStmt::printTree()
{
    QString tree = "";
    tree += QString::number(lineNum) + " GOTO\n";
    tree += "\t" + QString::number(targetPC) + "\n";
    return tree;
}

QString IfStmt::printTree()
{
    QString tree = "";
    tree += QString::number(lineNum) + " IF THEN\n";
    tree += lExp->toTree(1);
    tree += "\t" + compareOp + "\n";
    tree += rExp->toTree(1);
    tree += "\t" + QString::number(targetPC) + "\n";
    return tree;
}
