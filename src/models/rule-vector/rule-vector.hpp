#ifndef __RULE_VECTOR__
#define __RULE_VECTOR__

#include <string>

#include "../../types/types.hpp"

namespace models
{
  class rule_vector
  {
    private:
      types::rules rules{};

      bool
      is_additive() const;

    public:
      static bool
      is_complementable(const types::polynomial &coeffs);

      static void
      print_complementable_rule_vectors();

      rule_vector();

      rule_vector(const types::rules &rules);

      bool
      is_complementable(types::boundary boundary) const;

      types::whole_num
      at(std::size_t index) const;

      types::whole_num &
      at(std::size_t index);

      std::size_t
      size() const;

      types::matrix
      get_characteristic_matrix(types::boundary boundary) const;

      types::polynomial
      get_charactersitic_polynomial(types::boundary boundary) const;

      std::string
      to_string() const;
  };
}

#endif
