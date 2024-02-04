#include "rule-vector.hpp"

#include <omp.h>

#include <cmath>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <unordered_map>
#include <unordered_set>

#include "../../utils/utils.hpp"
#include "../binary-1d-ca-manager/binary-1d-ca-manager.hpp"
#include "../binary-1d-ca/binary-1d-ca.hpp"

// These rules work only on ECAs.
static types::rules linear_rules{60, 90, 102, 150, 170, 204, 240};

// These rule dependencies work only on ECAs.
// So they assume that a cell depends on its immediate left and right cells, along with itself.
// Left neighbor is encoded as -1.
// The cell itself is encoded as 0.
// Right neighbor is encoded as 1.
static std::unordered_map<types::long_whole_num, std::unordered_set<types::short_num>> additive_rule_deps{
  std::make_pair<types::long_whole_num, std::unordered_set<types::short_num>>(60, {-1, 0}),
  std::make_pair<types::long_whole_num, std::unordered_set<types::short_num>>(90, {-1, 1}),
  std::make_pair<types::long_whole_num, std::unordered_set<types::short_num>>(102, {0, 1}),
  std::make_pair<types::long_whole_num, std::unordered_set<types::short_num>>(150, {-1, 0, 1}),
  std::make_pair<types::long_whole_num, std::unordered_set<types::short_num>>(170, {1}),
  std::make_pair<types::long_whole_num, std::unordered_set<types::short_num>>(204, {0}),
  std::make_pair<types::long_whole_num, std::unordered_set<types::short_num>>(240, {-1}),

  std::make_pair<types::long_whole_num, std::unordered_set<types::short_num>>(195, {-1, 0}),
  std::make_pair<types::long_whole_num, std::unordered_set<types::short_num>>(165, {-1, 1}),
  std::make_pair<types::long_whole_num, std::unordered_set<types::short_num>>(153, {0, 1}),
  std::make_pair<types::long_whole_num, std::unordered_set<types::short_num>>(105, {-1, 0, 1}),
  std::make_pair<types::long_whole_num, std::unordered_set<types::short_num>>(85, {1}),
  std::make_pair<types::long_whole_num, std::unordered_set<types::short_num>>(51, {0}),
  std::make_pair<types::long_whole_num, std::unordered_set<types::short_num>>(15, {-1})
};

static bool
is_complementable_polynomial(const types::polynomial &coeffs)
{
  types::short_whole_num value_at_1{};

  for (types::short_whole_num i{}; i < coeffs.size(); i++)
  {
    value_at_1 += coeffs.at(i);
  }

  if (utils::math::is_even(value_at_1))
  {
    value_at_1 = 0;
  }
  else
  {
    value_at_1 = 1;
  }

  return value_at_1 == 0 ? false : true;
}

static std::vector<std::pair<models::rule_vector, types::polynomial>>
get_complementable_rule_vectors(types::short_whole_num size, types::boundary boundary)
{
  std::vector<std::pair<models::rule_vector, types::polynomial>> result{};

  types::whole_num max_rule_vectors{
    static_cast<types::whole_num>(std::pow(linear_rules.size(), size))
  };

  #pragma omp parallel
  {
    types::rules current_rules(size, 0);
    models::rule_vector current_rule_vector{};
    types::polynomial current_coeffs{};

    #pragma omp for
    for (types::whole_num i = 0; i < max_rule_vectors; i++)
    {
      std::string current_mask{utils::number::to_string(i, linear_rules.size(), size)};

      for (types::short_whole_num j{}; j < size; j++)
      {
        current_rules.at(j) = linear_rules.at(current_mask.at(j) - '0');
      }

      current_rule_vector = models::rule_vector{current_rules};
      current_coeffs = current_rule_vector.get_charactersitic_polynomial(boundary);

      if (is_complementable_polynomial(current_coeffs))
      {
        #pragma omp critical
        {
          result.push_back(std::make_pair<models::rule_vector &, types::polynomial &>(
            current_rule_vector, current_coeffs
          ));
        }
      }
    }
  }

  return result;
}

bool
models::rule_vector::is_additive() const
{
  for (const auto &rule : this->rules)
  {
    if (additive_rule_deps.find(rule) == additive_rule_deps.end())
    {
      return false;
    }
  }

  return true;
}

void
models::rule_vector::print_complementable_rule_vectors()
{
  types::whole_num counter{};

  types::short_whole_num num_cells{models::binary_1d_ca_manager::read_num_cells()};
  types::boundary boundary{models::binary_1d_ca_manager::read_boundary()};

  std::vector<std::pair<models::rule_vector, types::polynomial>> result{
    get_complementable_rule_vectors(num_cells, boundary)
  };

  if (!result.empty())
  {
    std::vector<std::pair<std::string, types::short_whole_num>> headings{
      std::make_pair<std::string, types::short_whole_num>(
        "s. no", 7
      ),
      std::make_pair<std::string, types::short_whole_num>(
        "rules", std::max(num_cells * 6, 24)
      ),
      std::make_pair<std::string, types::short_whole_num>(
        "polynomial", std::max(num_cells * 6, 24)
      ),
    };

    utils::general::print_header(headings);
  }
  else
  {
    utils::general::print_msg("no complementable linear ECAs", colors::blue);
    return;
  }

  for (const auto &pair : result)
  {
    std::vector<std::pair<std::string, types::short_whole_num>> entries{
      std::make_pair<std::string, types::short_whole_num>(
        std::to_string(++counter), 7
      ),
      std::make_pair<std::string, types::short_whole_num>(
        pair.first.to_string(), std::max(num_cells * 6, 24)
      ),
      std::make_pair<std::string, types::short_whole_num>(
        utils::polynomial::to_string(pair.second), std::max(num_cells * 6, 24)
      ),
    };

    utils::general::print_row(entries);
  }
}

models::rule_vector::rule_vector()
{
}

models::rule_vector::rule_vector(const types::rules &rules)
{
  this->rules.reserve(rules.size());

  for (const auto &rule : rules)
  {
    this->rules.push_back(rule);
  }
}

// This method works only for ECAs with additive rules.
// Let P(x) be the characteristic polynomial of the ECA under consideration.
// If (x + 1) is not a factor of P(x), then the ECA has complemented isomorphisms.
// Refer, https://www.researchgate.net/publication/358303083_Maximal_Length_Cellular_Automata.
//
// The values of x, P(x) and co-efficients of P(x) belong to GF(2).
// So to check if (x + 1) is a factor of P(x), we need to compute P(1), since -1 == 1, in GF(2).
// If P(1) == 0, then (x + 1) is a factor of P(x).
// If P(1) == 1, then (x + 1) is not a factor of P(x).
bool
models::rule_vector::is_complementable(types::boundary boundary) const
{
  types::polynomial coeffs{this->get_charactersitic_polynomial(boundary)};
  return is_complementable_polynomial(coeffs);
}

const types::rules &
models::rule_vector::get_rules() const
{
  return this->rules;
}

types::long_whole_num
models::rule_vector::at(std::size_t index) const
{
  return this->rules.at(index);
}

types::long_whole_num &
models::rule_vector::at(std::size_t index)
{
  return this->rules.at(index);
}

std::size_t
models::rule_vector::size() const
{
  return this->rules.size();
}

// This method works only for ECAs with additive rules.
// The elements of the matrix returned by this method belong to GF(2).
types::matrix
models::rule_vector::get_characteristic_matrix(types::boundary boundary) const
{
  if (!this->is_additive())
  {
    throw std::domain_error{"cannot represent non-additive rule vector as characteristic matrix"};
  }

  bool is_null_boundary{boundary == types::boundary::null};
  types::matrix M{};
  types::short_whole_num size{static_cast<types::short_whole_num>(this->size())};
  M.resize(size);

  for (types::short_whole_num i{}; i < size; i++)
  {
    M.at(i).resize(size);

    types::long_whole_num current_rule{this->rules.at(i)};
    std::unordered_set<types::short_num> current_rule_deps{additive_rule_deps.at(current_rule)};

    if (current_rule_deps.count(-1))
    {
      if (i != 0 || !is_null_boundary)
      {
        M.at(i).at((i - 1 + size) % size) = 1;
      }
    }

    if (current_rule_deps.count(0))
    {
      M.at(i).at(i) = 1;
    }

    if (current_rule_deps.count(1))
    {
      if (i != size - 1 || !is_null_boundary)
      {
        M.at(i).at((i + 1 + size) % size) = 1;
      }
    }
  }

  return M;
}

// This method works only for ECAs with additive rules.
// The co-efficients of the polynomial returned by this method belong to GF(2).
types::polynomial
models::rule_vector::get_charactersitic_polynomial(types::boundary boundary) const
{
  types::matrix M{this->get_characteristic_matrix(boundary)};
  types::polynomial coeffs{utils::matrix::get_characteristic_polynomial(M)};

  for (auto &coeff : coeffs)
  {
    coeff = utils::math::is_even(coeff) ? 0 : 1;
  }

  return coeffs;
}

std::string
models::rule_vector::to_string() const
{
  return utils::vector::to_string<types::long_whole_num>(this->rules);
}
