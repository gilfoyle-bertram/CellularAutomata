#include "../utils.hpp"

#include <cstdlib>
#include <iostream>
#include <unordered_map>

static const std::unordered_map<short, std::string> INDICES{
  std::make_pair<short, std::string>(1, ""),
  std::make_pair<short, std::string>(2, "²"),
  std::make_pair<short, std::string>(3, "³"),
  std::make_pair<short, std::string>(4, "⁴"),
  std::make_pair<short, std::string>(5, "⁵"),
  std::make_pair<short, std::string>(6, "⁶"),
  std::make_pair<short, std::string>(7, "⁷"),
  std::make_pair<short, std::string>(8, "⁸"),
  std::make_pair<short, std::string>(9, "⁹"),
};

// Degree of P should be less than or equal to 9.
// Otherwise this utility function breaks.
void
utils::polynomial::print(const types::Polynomial &P)
{
  short degree{static_cast<short>(P.size() - 1)};

  while (degree > 0 && P.at(degree) == 0)
  {
    degree -= 1;
  }

  std::cout << "\n";

  if (degree == 0 && P.at(degree) == 0)
  {
    std::cout
      << "Zero polynomial"
      << "\n";

    return;
  }

  for (short n{degree}; n >= 0; n--)
  {
    if (P.at(n) != 0)
    {
      if (n != degree)
      {
        std::cout << " ";
      }

      if (P.at(n) > 0 && n < degree)
      {
        std::cout << "+";
      }

      if (P.at(n) < 0)
      {
        std::cout << "-";
      }

      if (n != degree)
      {
        std::cout << " ";
      }

      if ((P.at(n) != 1 && P.at(n) != -1) || (n == 0))
      {
        std::cout << std::abs(P.at(n));
      }

      if (n != 0)
      {
        std::cout << "x" << INDICES.at(n);
      }
    }
  }

  std::cout << "\n";
}
