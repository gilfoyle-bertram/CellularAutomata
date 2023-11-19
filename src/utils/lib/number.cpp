#include "../utils.hpp"

#include <bitset>
#include <sstream>

types::whole_num
utils::number::parse_binary_str(const std::string &txt)
{
  return std::stoi(txt, nullptr, 2);
}

std::string
utils::number::to_binary_str(types::whole_num num, types::whole_num num_digits)
{
  return std::bitset<16>{num}.to_string().substr(16 - num_digits);
}

std::string
utils::number::to_string(
  types::whole_num num,
  types::whole_num base,
  types::whole_num num_digits
)
{
  if (base > 10)
  {
    throw std::invalid_argument("unsupported base for conversion");
  }

  static std::string symbols{"0123456789"};

  std::string remainders{};
  std::ostringstream out_stream{};

  while (num)
  {
    remainders += symbols.at(num % base);
    num /= base;
  }

  types::num padding{static_cast<types::num>(num_digits - remainders.size())};

  if (padding < 0)
  {
    throw std::overflow_error{("base conversion failed due to overflow")};
  }

  out_stream << std::string(padding, '0');
  out_stream << std::string(remainders.rbegin(), remainders.rend());

  return out_stream.str();
}
