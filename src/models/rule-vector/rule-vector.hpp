#ifndef __RULE_VECTOR__
#define __RULE_VECTOR__

#include "../../types/types.hpp"

namespace models
{
  class RuleVector
  {
    private:
      types::Rules rules{};

      bool
      isAdditive() const;

    public:
      RuleVector(const RuleVector &other) = delete;

      RuleVector();

      RuleVector(const types::Rules &rules);

      unsigned short
      at(std::size_t index) const;

      unsigned short &
      at(std::size_t index);

      std::size_t
      size() const;

      types::Matrix
      getCharacteristicMatrix(const types::BoundaryCondition &BC) const;

      types::Polynomial
      getCharactersiticPolynomial(const types::BoundaryCondition &BC) const;
  };
}

#endif
