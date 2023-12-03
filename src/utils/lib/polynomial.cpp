#include "../utils.hpp"

#include <cstdlib>
#include <sstream>

std::string
utils::polynomial::to_string(const types::polynomial &coeffs)
{
  std::ostringstream out_stream{};
  types::short_num degree{static_cast<types::short_num>(coeffs.size() - 1)};

  if (degree < 0)
  {
    throw std::invalid_argument{"invalid polynomial (-ve degree)"};
  }

  while (degree > 0 && coeffs.at(degree) == 0)
  {
    degree -= 1;
  }

  if (degree == 0 && coeffs.at(degree) == 0)
  {
    return "zero polynomial";
  }

  for (types::short_num n{degree}; n >= 0; n--)
  {
    if (coeffs.at(n) != 0)
    {
      if (n != degree)
      {
        out_stream << " ";
      }

      if (coeffs.at(n) > 0 && n < degree)
      {
        out_stream << "+";
      }

      if (coeffs.at(n) < 0)
      {
        out_stream << "-";
      }

      if (n != degree)
      {
        out_stream << " ";
      }

      if ((coeffs.at(n) != 1 && coeffs.at(n) != -1) || (n == 0))
      {
        out_stream << std::abs(coeffs.at(n));
      }

      if (n != 0)
      {
        out_stream << "x";
        out_stream << (n != 1 ? "^" + std::to_string(n) : "");
      }
    }
  }

  return out_stream.str();
}

void
utils::polynomial::print(const types::polynomial &coeffs, const std::string &color)
{
  utils::general::print_msg(utils::polynomial::to_string(coeffs), color);
}
