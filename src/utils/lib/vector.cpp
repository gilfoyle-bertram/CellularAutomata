#include "../utils.hpp"

#include <sstream>

std::string
utils::vector::to_string(const std::vector<types::whole_num> &nums)
{
  std::ostringstream out_stream{};
  out_stream << "[ ";

  for (const auto &num : nums)
  {
    out_stream << num << " ";
  }

  out_stream << "]";
  return out_stream.str();
}
