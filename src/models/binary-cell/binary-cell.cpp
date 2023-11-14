#include "binary-cell.hpp"

#include <cmath>
#include <sstream>

#include "../../utils/utils.hpp"

bool
models::BinaryCell::getState() const
{
  return this->state;
}

void
models::BinaryCell::setState(bool state)
{
  this->state = state;
}

unsigned short
models::BinaryCell::getRule() const
{
  return this->rule;
}

void
models::BinaryCell::setRule(unsigned short rule)
{
  this->rule = rule;
}

bool
models::BinaryCell::applyRule(const std::string &neighborhood) const
{
  unsigned short neighborhoodSize{static_cast<unsigned short>(neighborhood.size())};

  // The function "utils::toBinaryStr()" can only return binary strings of length atmost 16.
  // So the below code breaks, if neighborhoodSize > 4.
  std::string ruleStr{utils::general::toBinaryStr(this->rule, 1U << neighborhoodSize)};
  return ruleStr.at(ruleStr.size() - utils::general::parseBinaryStr(neighborhood) - 1) == '1';
}

void
models::BinaryCell::updateState(const std::string &neighborhood)
{
  this->state = applyRule(neighborhood);
}
