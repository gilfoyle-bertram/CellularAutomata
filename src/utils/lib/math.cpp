#include "../utils.hpp"

bool
utils::math::isOdd(short num)
{
  return static_cast<bool>(num & 1);
}

bool
utils::math::isEven(short num)
{
  return !utils::math::isOdd(num);
}
