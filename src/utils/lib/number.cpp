#include "../utils.hpp"

#include <bitset>
#include <sstream>

types::long_whole_num
utils::number::parse_binary_str(const std::string &str)
{
  return std::stoul(str, nullptr, 2);
}

std::string
utils::number::to_binary_str(types::long_whole_num num, types::short_whole_num num_digits)
{
  return std::bitset<64>{num}.to_string().substr(64 - num_digits);
}

std::string
utils::number::to_string(
  types::long_whole_num num,
  types::short_whole_num base,
  types::short_whole_num num_digits
)
{
  if (base > 10)
  {
    throw std::invalid_argument{"unsupported base for conversion"};
  }

  static std::string symbols{"0123456789"};

  std::string remainders{};
  std::ostringstream out_stream{};

  while (num)
  {
    remainders += symbols.at(num % base);
    num /= base;
  }

  types::short_num padding{static_cast<types::short_num>(num_digits - remainders.size())};

  if (padding < 0)
  {
    throw std::overflow_error{"base conversion failed due to overflow"};
  }

  out_stream << std::string(padding, '0');
  out_stream << std::string(remainders.rbegin(), remainders.rend());

  return out_stream.str();
}
