#include "expression.h"

Expression::Expression()
{

}

int ConstantExp::eval(EvaluationContext & context)
{
   return value;
}

int IdentifierExp::eval(EvaluationContext & context)
{
   if (!context.isDefined(name)) throw(name + " is undefined");
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
      if (right == 0) throw("Division by 0");
      return left / right;
   }
   throw("Illegal operator in expression");
   return 0;
}
