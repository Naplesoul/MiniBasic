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
