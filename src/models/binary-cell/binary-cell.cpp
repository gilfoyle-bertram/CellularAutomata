#include "binary-cell.hpp"

#include <cmath>
#include <sstream>

#include "../../utils/utils.hpp"

bool
models::binary_cell::get_state() const
{
  return this->state;
}

types::long_whole_num
models::binary_cell::get_rule() const
{
  return this->rule;
}

void
models::binary_cell::set_state(bool state)
{
  this->state = state;
}

void
models::binary_cell::set_rule(types::long_whole_num rule)
{
  this->rule = rule;
}

bool
models::binary_cell::next_state(const std::string &neighborhood) const
{
  types::whole_num num_neighbors{static_cast<types::whole_num>(neighborhood.size())};
  std::string rule_str{utils::number::to_binary_str(this->rule, 1U << num_neighbors)};

  return rule_str.at(rule_str.size() - utils::number::parse_binary_str(neighborhood) - 1) == '1';
}

void
models::binary_cell::update_state(const std::string &neighborhood)
{
  this->state = this->next_state(neighborhood);
}
