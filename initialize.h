#ifndef INITIALIZE_H
#define INITIALIZE_H

#include <QString>

enum OPTYPE
{
    INPUT, RUN, LOAD, SAVE, WAIT_FOR_INPUT, CLEAR
};

enum ExpressionType
{
    CONSTANT, IDENTIFIER, COMPOUND
};

#endif // INITIALIZE_H
