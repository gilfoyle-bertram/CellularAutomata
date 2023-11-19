#ifndef __UTILS__
#define __UTILS__

#include <string>
#include <vector>

#include "../terminal/terminal.hpp"
#include "../types/types.hpp"

namespace utils::general
{
  bool
  get_random_bool();

  types::whole_num
  get_choice(const std::vector<std::string> &choices);

  void
  initialize();

  void
  print_header(
    const std::vector<std::pair<std::string, types::whole_num>> &headings,
    const std::string &color = colors::cyan
  );

  void
  print_row(
    const std::vector<std::pair<std::string, types::whole_num>> &entries,
    const std::string &color = colors::cyan
  );

  void
  print_msg(const std::string &msg, const std::string &color = "");
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
  get_identity(types::whole_num size);

  types::matrix
  add(const types::matrix &A, const types::matrix &B);

  types::matrix
  multiply(const types::matrix &A, const types::matrix &B);

  types::matrix
  scalar_multiply(const types::matrix &A, types::num x);

  types::polynomial
  get_characteristic_polynomial(const types::matrix &A);

  std::string
  to_string(const types::matrix &A);

  void
  print(const types::matrix &A, const std::string &color = colors::cyan);
}

namespace utils::number
{
  types::whole_num
  parse_binary_str(const std::string &txt);

  std::string
  to_binary_str(types::whole_num num, types::whole_num num_digits);

  std::string
  to_string(types::whole_num num, types::whole_num base, types::whole_num num_digits);
}

namespace utils::polynomial
{
  std::string
  to_string(const types::polynomial &coeffs);

  void
  print(const types::polynomial &coeffs, const std::string &color = colors::cyan);
}

namespace utils::transition_graph
{
  std::string
  to_string(const types::transition_graph &graph);

  void
  print(const types::transition_graph &graph, const std::string &color = colors::cyan);
}

namespace utils::vector
{
  std::string
  to_string(const std::vector<types::whole_num> &nums);
}

#endif
