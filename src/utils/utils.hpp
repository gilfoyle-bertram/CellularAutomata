#ifndef __UTILS__
#define __UTILS__

#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

#include "../terminal/terminal.hpp"
#include "../types/types.hpp"

namespace utils::general
{
  bool
  get_random_bool();

  types::short_whole_num
  get_choice(const std::vector<std::string> &choices);

  void
  initialize();

  void
  print_header(
    const std::vector<std::pair<std::string, types::short_whole_num>> &headings,
    const std::string &color = colors::cyan
  );

  void
  print_row(
    const std::vector<std::pair<std::string, types::short_whole_num>> &entries,
    const std::string &color = colors::cyan
  );

  void
  print_msg(const std::string &msg, const std::string &color = "", bool new_line = true);
}

namespace utils::math
{
  bool
  is_odd(types::num num);

  bool
  is_even(types::num num);
}

namespace utils::matrix
{
  bool
  is_square(const types::matrix &A);

  bool
  can_add(const types::matrix &A, const types::matrix &B);

  bool
  can_multiply(const types::matrix &A, const types::matrix &B);

  types::num
  get_trace(const types::matrix &A);

  types::matrix
  get_identity(types::short_whole_num size);

  types::matrix
  add(const types::matrix &A, const types::matrix &B);

  types::matrix
  multiply(const types::matrix &A, const types::matrix &B);

  types::matrix
  scalar_multiply(const types::matrix &A, types::short_num x);

  types::polynomial
  get_characteristic_polynomial(const types::matrix &A);

  std::string
  to_string(const types::matrix &A);

  void
  print(const types::matrix &A, const std::string &color = colors::cyan);
}

namespace utils::number
{
  types::short_whole_num
  get_random_num(types::short_whole_num min, types::short_whole_num max);

  types::long_whole_num
  parse_binary_str(const std::string &str);

  std::string
  to_binary_str(types::long_whole_num num, types::short_whole_num num_digits);

  std::string
  to_string(
    types::long_whole_num num,
    types::short_whole_num base,
    types::short_whole_num num_digits
  );
}

namespace utils::polynomial
{
  std::string
  to_string(const types::polynomial &coeffs);

  void
  print(const types::polynomial &coeffs, const std::string &color = colors::cyan);
}

namespace utils::sn_map
{
  bool
  is_1_1_or_1_n(const types::sn_map &sn_map);
}

namespace utils::transition_graph
{
  types::cycles
  get_cycles(const types::transition_graph &graph);

  void
  print(
    const types::transition_graph &graph,
    const std::string &cycle_color = colors::salmon,
    const std::string &regular_color = colors::cyan
  );
}

namespace utils::vector
{
  template <typename T>
  inline std::string
  to_string(const std::vector<T> &nums)
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
}

#endif
