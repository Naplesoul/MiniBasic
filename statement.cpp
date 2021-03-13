#include "statement.h"

bool isValidVarName(const QString &var)
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
            || var == "REM" || var == "REM" || var == "REM" || var == "REM"
            || var == "GOTO" || var == "IF" || var == "THEN" || var == "END"
            || var == "RUN" || var == "LOAD" || var == "LIST" || var == "CLEAR"
            || var == "HELP")
        return false;
    return true;
}

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
    if(!isValidVarName(var))
        throw QString("Invalid variable name in Line ") + QString::number(lineNum);
    IdentifierExp *lhs = new IdentifierExp(var);




    return true;
}
