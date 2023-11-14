#include "rule-vector.hpp"

#include <stdexcept>
#include <unordered_map>
#include <unordered_set>

#include "../../utils/utils.hpp"

// These rules work only on ECAs.
// So they assume that a cell depends on its immediate left and right cells, along with itself.
// Left neighbor is encoded as -1.
// The cell itself is encoded as 0.
// Right neighbor is encoded as 1.
static const std::unordered_map<unsigned short, std::unordered_set<signed short>> ADDITIVE_RULES{
  std::make_pair<unsigned short, std::unordered_set<signed short>>(60, {-1, 0}),
  std::make_pair<unsigned short, std::unordered_set<signed short>>(90, {-1, 1}),
  std::make_pair<unsigned short, std::unordered_set<signed short>>(102, {0, 1}),
  std::make_pair<unsigned short, std::unordered_set<signed short>>(150, {-1, 0, 1}),
  std::make_pair<unsigned short, std::unordered_set<signed short>>(170, {1}),
  std::make_pair<unsigned short, std::unordered_set<signed short>>(204, {0}),
  std::make_pair<unsigned short, std::unordered_set<signed short>>(240, {-1}),

  std::make_pair<unsigned short, std::unordered_set<signed short>>(195, {-1, 0}),
  std::make_pair<unsigned short, std::unordered_set<signed short>>(165, {-1, 1}),
  std::make_pair<unsigned short, std::unordered_set<signed short>>(153, {0, 1}),
  std::make_pair<unsigned short, std::unordered_set<signed short>>(105, {-1, 0, 1}),
  std::make_pair<unsigned short, std::unordered_set<signed short>>(85, {1}),
  std::make_pair<unsigned short, std::unordered_set<signed short>>(51, {0}),
  std::make_pair<unsigned short, std::unordered_set<signed short>>(15, {-1})
};

models::RuleVector::RuleVector()
{
}

models::RuleVector::RuleVector(const types::Rules &rules)
{
  this->rules.reserve(rules.size());

  for (const auto &rule : rules)
  {
    this->rules.push_back(rule);
  }
}

bool
models::RuleVector::isAdditive() const
{
  for (const auto &rule : this->rules)
  {
    if (ADDITIVE_RULES.find(rule) == ADDITIVE_RULES.end())
    {
      return false;
    }
  }

  return true;
}

unsigned short
models::RuleVector::at(std::size_t index) const
{
  return this->rules.at(index);
}

unsigned short &
models::RuleVector::at(std::size_t index)
{
  return this->rules.at(index);
}

std::size_t
models::RuleVector::size() const
{
  return this->rules.size();
}

// This method works only for ECAs with additive rules.
// The elements of the matrix returned by this method belong to GF(2).
types::Matrix
models::RuleVector::getCharacteristicMatrix(const types::BoundaryCondition &BC) const
{
  if (!this->isAdditive())
  {
    throw std::domain_error{"Cannot represent non-additive rule vector as characteristic matrix"};
  }

  types::Matrix M{};
  unsigned short size{static_cast<unsigned short>(this->size())};
  M.resize(size);

  for (unsigned short i{}; i < size; i++)
  {
    M.at(i).resize(size);
    std::unordered_set<signed short> currentRuleDependencies{ADDITIVE_RULES.at(this->rules.at(i))};

    if (currentRuleDependencies.count(-1))
    {
      if (i != 0 || BC != types::BoundaryCondition::Null)
      {
        M.at(i).at((i - 1 + size) % size) = 1;
      }
    }

    if (currentRuleDependencies.count(0))
    {
      M.at(i).at(i) = 1;
    }

    if (currentRuleDependencies.count(1))
    {
      if (i != size - 1 || BC != types::BoundaryCondition::Null)
      {
        M.at(i).at((i + 1 + size) % size) = 1;
      }
    }
  }

  return M;
}

// This method works only for ECAs with additive rules.
// The co-efficients of the polynomial returned by this method belong to GF(2).
types::Polynomial
models::RuleVector::getCharactersiticPolynomial(const types::BoundaryCondition &BC) const
{
  types::Matrix M{this->getCharacteristicMatrix(BC)};
  types::Polynomial P{utils::matrix::getCharacteristicPolynomial(M)};

  for (auto &coefficient : P)
  {
    coefficient = utils::math::isEven(coefficient) ? 0 : 1;
  }

  return P;
}
