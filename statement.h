#ifndef STATEMENT_H
#define STATEMENT_H

#include "expression.h"
#include "initialize.h"
#include "code.h"
#include <QString>
#include <QVector>
#include <QList>


struct FormatContent
{
    bool isString;
    QString string;
    CompoundExp *exp;
    FormatContent(bool _isString, const QString &_string): isString(_isString), string(_string), exp(nullptr)
    {
        if (!isString)
            exp = new CompoundExp(_string);
    }
    ~FormatContent()
    {
        if (!isString)
            delete exp;
    }
    QString eval(EvaluationContext &evaluationContext)
    {
        if (isString)
            return string;
        QString result;
        try {
            result = QString::number(exp->eval(evaluationContext));
        }  catch (QString) {
            // the symbol context for int does not contains the var
            if (evaluationContext.isStringDefined(string))
                return evaluationContext.getString(string);
            throw QString("[Undefined variable name in line ");
        }
        return result;
    }

};


class Statement
{
public:
    Statement(){}
    virtual ~Statement(){}

    int lineNum;
    StatementType type;
// return true if parsing is successful, throws a QString describing error infomation.
    virtual bool parse(const QString &code) = 0;
// return true if run successfully, false if the statement needs input, throws a QString describing error infomation when meet error.
    virtual bool run(EvaluationContext &evaluationContext, int &pc, QString &input, QString &output) = 0;
    virtual QString printTree() = 0;

// isValid is false if this statement contains errors when parsing
    bool isValid = true;
// errInfo is not empty when the isValid is false
    QString errInfo = "";

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



class RemStmt : public Statement
{
public:
    RemStmt(int l){lineNum = l; type = REMSTMT;}
    bool parse(const QString &){return true;}
    bool run(EvaluationContext &, int &, QString &, QString &){return true;}
    QString printTree(){return "";}
    ~RemStmt(){}
};


class LetStmt : public Statement
{
public:
    LetStmt(int l):isString(false) {lineNum = l; type = LETSTMT;}
    ~LetStmt(){delete exp;}

    bool parse(const QString &code);
    bool run(EvaluationContext &evaluationContext, int &pc, QString &input, QString &output);
    QString printTree();
private:
    bool isString;
    QString stringVal;
    IdentifierExp *var;
    CompoundExp *exp;
};


class PrintStmt : public Statement
{
public:
    PrintStmt(int l){lineNum = l; type = PRINTSTMT;}
    ~PrintStmt(){delete exp;}

    virtual bool parse(const QString &code);
    virtual bool run(EvaluationContext &evaluationContext, int &pc, QString &input, QString &output);
    QString printTree();
private:
    CompoundExp *exp;
};

class PrintfStmt : public Statement
{
public:
    PrintfStmt(int l){lineNum = l; type = PRINTFSTMT;}
    ~PrintfStmt()
    {
        for (auto it : contents)
            delete it;
    }

    virtual bool parse(const QString &code);
    virtual bool run(EvaluationContext &evaluationContext, int &pc, QString &input, QString &output);
    QString printTree();
    static int findChar(const QString &content, const char &find)
    {
        int length = content.length();
        for (int i = 0; i < length; ++i) {
            if (content[i] == find)
                return i;
        }
        return -1;
    }
private:
    QString text;
    QList<FormatContent*> contents;
    QList<QString> textFragments;
};


class InputStmt : public Statement
{
public:
    InputStmt(int l){lineNum = l; type = INPUTSTMT;}
    ~InputStmt(){delete var;}

    bool parse(const QString &code);
    bool run(EvaluationContext &evaluationContext, int &pc, QString &input, QString &output);
    QString printTree();
private:
    IdentifierExp *var;
};

class InputsStmt : public Statement
{
public:
    InputsStmt(int l){lineNum = l; type = INPUTSSTMT;}
    ~InputsStmt() {delete var;}

    bool parse(const QString &code);
    bool run(EvaluationContext &evaluationContext, int &pc, QString &input, QString &output);
    QString printTree();
private:
    IdentifierExp *var;
};


class GotoStmt : public Statement
{
public:
    GotoStmt(int l){lineNum = l, type = GOTOSTMT;}
    ~GotoStmt(){}

    bool parse(const QString &code);
    bool run(EvaluationContext &evaluationContext, int &pc, QString &input, QString &output);
    QString printTree();
private:
    int targetPC;
};




class IfStmt : public Statement
{
public:
    IfStmt(int l){lineNum = l, type = IFSTMT;}
    ~IfStmt(){delete lExp; delete rExp;}

    bool parse(const QString &code);
    bool run(EvaluationContext &evaluationContext, int &pc, QString &input, QString &output);
    QString printTree();
private:
    CompoundExp *lExp;
    CompoundExp *rExp;
    QString compareOp;
    int targetPC;
};




class EndStmt : public Statement
{
public:
    EndStmt(int l){lineNum = l, type = ENDSTMT;}
    bool parse(const QString &){return true;}
    bool run(EvaluationContext &, int &, QString &, QString &){return true;}
    QString printTree(){return "";}
    ~EndStmt(){}
};

#endif // STATEMENT_H
