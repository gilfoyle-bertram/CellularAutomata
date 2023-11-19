#include "../utils.hpp"

bool
utils::math::is_odd(types::num num)
{
  return static_cast<bool>(num & 1);
}

bool
utils::math::is_even(types::num num)
{
  return !utils::math::is_odd(num);
}
