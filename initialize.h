#ifndef INITIALIZE_H
#define INITIALIZE_H

#include <QString>

enum Status
{
    INPUT, RUN, LOAD, SAVE, WAIT_FOR_INPUT, CLEAR
};

enum ExpressionType
{
    CONSTANT, IDENTIFIER, COMPOUND
};

enum StatementType
{
    REMSTMT, LETSTMT, PRINTSTMT, INPUTSTMT, GOTOSTMT, IFSTMT, ENDSTMT
};



enum Operator
{
    EVAL, ADD, MINUS, MULTIPLY, DIVIDE, EXP, LBRACKET, RBRACKET
};

bool isInt(const QString &s)
{
    int len = s.length();
    for(int i = 0; i < len; ++i)
    {
        if(!(s[i] <= '9' && s[i] >='0'))
            return false;
    }
    return true;
}

//enum CompareOperator
//{
//    EQUAL, LESS, MORE
//};

#endif // INITIALIZE_H
