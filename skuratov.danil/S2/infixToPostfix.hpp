#ifndef INFIX_TO_POSTFIX_HPP
#define INFIX_TO_POSTFIX_HPP

#include <string>
#include <sstream>
#include <cctype>
#include "stack.hpp"

namespace skuratov
{
  bool priority(const std::string op1, const std::string op2);
  std::string infixToPostfix(const std::string& exp);
}

#endif
