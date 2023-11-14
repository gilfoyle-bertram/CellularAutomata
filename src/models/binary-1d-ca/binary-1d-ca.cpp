#include "binary-1d-ca.hpp"

#include <omp.h>

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <unordered_set>

#include "../../utils/utils.hpp"

static types::Rules
readRules(unsigned short size)
{
  unsigned short rule{};
  types::Rules rules(size, 0);

  std::cout << "\n";

  for (unsigned short i{}; i < size; ++i)
  {
    std::cout << "Rule for cell-" << (i + 1) << ": ";
    std::cin >> rule;
    rules.at(i) = rule;
  }

  return rules;
}

models::Binary1DCA::Binary1DCA()
{
}

models::Binary1DCA::Binary1DCA(
  unsigned short numCells,
  unsigned short leftRadius,
  unsigned short rightRadius,
  const types::BoundaryCondition &BC,
  const types::Rules &rules
)
{
  if (leftRadius + rightRadius + 1 > numCells)
  {
    throw std::invalid_argument{"Neighborhood size can't be greater than number of cells"};
  }

  if (numCells != rules.size())
  {
    throw std::invalid_argument{"Number of rules must be equal to number of cells"};
  }

  if (numCells > models::Binary1DCA::MAX_SIZE)
  {
    throw std::invalid_argument{"Unsupported CA size"};
  }

  this->numCells = numCells;
  this->leftRadius = leftRadius;
  this->rightRadius = rightRadius;
  this->neighborhoodSize = leftRadius + rightRadius + 1;
  this->totalConfigs = static_cast<unsigned short>(1U << numCells);
  this->BC = BC;
  this->ruleVector = models::RuleVector{rules};

  this->cells.resize(numCells);
  this->randomizeConfig();

  for (unsigned short i{}; i < numCells; i++)
  {
    this->cells.at(i).setRule(rules.at(i));
  }
}

bool
models::Binary1DCA::isECA() const
{
  return this->leftRadius == 1 && this->rightRadius == 1;
}

// This method works only for ECAs with additive rules.
// Let P(x) be the characteristic polynomial of the ECA under consideration.
// If (x + 1) is not a factor of P(x), then the ECA has complemented isomorphisms.
// Refer - https://www.researchgate.net/publication/358303083_Maximal_Length_Cellular_Automata.
//
// The values of x, P(x) and co-efficients of P(x) belong to GF(2).
// So to check if (x + 1) is a factor of P(x), we need to compute P(1), since -1 == 1, in GF(2).
// If P(1) == 0, then (x + 1) is a factor of P(x).
// If P(1) == 1, then (x + 1) is not a factor of P(x).
bool
models::Binary1DCA::hasComplementedIsomorphisms() const
{
  if (!this->isECA())
  {
    throw std::domain_error{"Complemented isomorphisms are only supported for ECAs"};
  }

  types::Polynomial P{this->ruleVector.getCharactersiticPolynomial(this->BC)};
  short valueAt1{};

  for (unsigned short i{}; i < P.size(); i++)
  {
    valueAt1 += P.at(i);
  }

  valueAt1 = utils::math::isEven(valueAt1) ? 0 : 1;

  if (valueAt1 == 0)
  {
    return false;
  }

  return true;
}

bool
models::Binary1DCA::extractRules(const types::TransitionGraph &transitionGraph, types::Rules &rules)
{
  for (unsigned short i{}; i < this->numCells; i++)
  {
    std::vector<char> currentRuleTable((1U << this->neighborhoodSize), 'X');
    std::ostringstream currentRuleStream{};

    for (unsigned short j{}; j < this->totalConfigs; j++)
    {
      std::string currentConfigStr{utils::general::toBinaryStr(j, this->numCells)};
      std::string nextConfigStr{utils::general::toBinaryStr(transitionGraph.at(j), this->numCells)};

      unsigned short neighborhood{utils::general::parseBinaryStr(this->getNeighborhood(i, currentConfigStr))};
      char nextState{nextConfigStr.at(i)};

      if (currentRuleTable.at(neighborhood) == 'X')
      {
        currentRuleTable.at(neighborhood) = nextState;
      }
      else if (currentRuleTable.at(neighborhood) != nextState)
      {
        return false;
      }
    }

    for (unsigned short j{}; j < currentRuleTable.size(); j++)
    {
      char currentChar{currentRuleTable.at(currentRuleTable.size() - 1 - j)};
      currentRuleStream << (currentChar == 'X' ? '0' : currentChar);
    }

    rules.at(i) = utils::general::parseBinaryStr(currentRuleStream.str());
  }

  return true;
}

unsigned short
models::Binary1DCA::getNextConfig(unsigned short currentConfig)
{
  if (this->globalConfigMap.count(currentConfig))
  {
    return this->globalConfigMap.at(currentConfig);
  }

  std::ostringstream nextConfigStream{};
  std::string currentConfigStr{utils::general::toBinaryStr(currentConfig, this->numCells)};

  for (unsigned short i{}; i < this->numCells; i++)
  {
    nextConfigStream << this->cells.at(i).applyRule(this->getNeighborhood(i, currentConfigStr));
  }

  return (this->globalConfigMap[currentConfig] = utils::general::parseBinaryStr(nextConfigStream.str()));
}

std::string
models::Binary1DCA::getNeighborhood(unsigned short cellIndex, const std::string &configStr) const
{
  std::ostringstream neighborhood{};

  for (unsigned short l{this->leftRadius}; l >= 1; l--)
  {
    short currentNeighborIndex{static_cast<short>((cellIndex - l))};

    if (currentNeighborIndex < 0 && this->BC == types::BoundaryCondition::Null)
    {
      neighborhood << '0';
      continue;
    }

    neighborhood << configStr.at((this->numCells + currentNeighborIndex) % this->numCells);
  }

  neighborhood << configStr.at(cellIndex);

  for (unsigned short r{1}; r <= this->rightRadius; r++)
  {
    short currentNeighborIndex{static_cast<short>((cellIndex + r))};

    if (currentNeighborIndex >= this->numCells && this->BC == types::BoundaryCondition::Null)
    {
      neighborhood << '0';
      continue;
    }

    neighborhood << configStr.at(currentNeighborIndex % this->numCells);
  }

  return neighborhood.str();
}

void
models::Binary1DCA::randomizeConfig()
{
  for (auto &cell : this->cells)
  {
    cell.setState(utils::general::getRandomBool());
  }
}

unsigned short
models::Binary1DCA::getCurrentConfig() const
{
  std::ostringstream currentConfigStream{};

  for (const auto &cell : this->cells)
  {
    currentConfigStream << static_cast<unsigned short>(cell.getState());
  }

  return utils::general::parseBinaryStr(currentConfigStream.str());
}

std::pair<types::BoundaryCondition, std::string>
models::Binary1DCA::getBoundaryCondition() const
{
  std::pair<types::BoundaryCondition, std::string> result{};

  result.first = this->BC;
  result.second = this->BC == types::BoundaryCondition::Null ? "Null" : "Periodic";

  return result;
}

// For a CA, the out-degree of any node is exactly 1.
// By exploiting this property, we can represent the transition graph as a 1D-Array itself.
// So "transitionGraph.at(i) = j", means there is an edge from configuration i to configuration j.
types::TransitionGraph
models::Binary1DCA::getTransitionGraph()
{
  types::TransitionGraph adjacencyList{};

  for (unsigned short i{}; i < this->totalConfigs; i++)
  {
    adjacencyList.push_back(this->getNextConfig(i));
  }

  return adjacencyList;
}

void
models::Binary1DCA::printDetails() const
{
  std::ostringstream rulesStream{};
  rulesStream << "[ ";

  for (const auto &cell : this->cells)
  {
    rulesStream << cell.getRule() << " ";
  }

  rulesStream << "]";
  std::string rulesStr{rulesStream.str()};

  std::cout
    << "\n"
    << "+-" << std::string(9, '-')
    << "-+-" << std::string(11, '-')
    << "-+-" << std::string(12, '-')
    << "-+-" << std::string(18, '-')
    << "-+-" << std::setfill('-') << std::setw(std::max(static_cast<int>(rulesStr.size()), 5) + 4)
    << "-+" << std::setfill(' ')
    << "\n";

  std::cout
    << "| Num Cells "
    << "| Left Radius "
    << "| Right Radius "
    << "| Boundary Condition "
    << "| " << std::setw(std::max(static_cast<int>(rulesStr.size()), 5) + 3) << "Rules "
    << "|"
    << "\n";

  std::cout
    << "+-" << std::string(9, '-')
    << "-+-" << std::string(11, '-')
    << "-+-" << std::string(12, '-')
    << "-+-" << std::string(18, '-')
    << "-+-" << std::setfill('-') << std::setw(std::max(static_cast<int>(rulesStr.size()), 5) + 4)
    << "-+" << std::setfill(' ')
    << "\n";

  std::cout
    << "| " << std::setw(9) << this->numCells
    << " | " << std::setw(11) << this->leftRadius
    << " | " << std::setw(12) << this->rightRadius
    << " | " << std::setw(18) << this->getBoundaryCondition().second
    << " | " << std::setw(std::max(static_cast<int>(rulesStr.size()), 5) + 2) << rulesStr
    << " |"
    << "\n";

  std::cout
    << "+-" << std::string(9, '-')
    << "-+-" << std::string(11, '-')
    << "-+-" << std::string(12, '-')
    << "-+-" << std::string(18, '-')
    << "-+-" << std::setfill('-') << std::setw(std::max(static_cast<int>(rulesStr.size()), 5) + 4)
    << "-+" << std::setfill(' ')
    << "\n";
}

void
models::Binary1DCA::updateConfig()
{
  std::string currentConfigStr{utils::general::toBinaryStr(this->getCurrentConfig(), this->numCells)};

  for (unsigned short i{}; i < this->numCells; i++)
  {
    this->cells.at(i).updateState(this->getNeighborhood(i, currentConfigStr));
  }
}

void
models::Binary1DCA::printTransitionGraph()
{
  const types::TransitionGraph &transitionGraph{this->getTransitionGraph()};
  std::unordered_set<unsigned short> visitedConfigs{};

  unsigned short currentConfig{};
  bool isNewComponent{true};

  for (unsigned short i{}; i < this->totalConfigs; i++)
  {
    while (true)
    {
      if (visitedConfigs.find(currentConfig) != visitedConfigs.end())
      {
        currentConfig = i + 1;
        isNewComponent = true;
        break;
      }

      if (isNewComponent)
      {
        std::cout
          << "\n"
          << currentConfig;
      }

      std::cout
        << " --> "
        << transitionGraph.at(currentConfig);

      visitedConfigs.insert(currentConfig);
      currentConfig = transitionGraph.at(currentConfig);
      isNewComponent = false;
    }
  }

  std::cout << "\n";
}

void
models::Binary1DCA::printIsomorphicPermutations()
{
  unsigned int counter{};
  types::TransitionGraph originalTransitionGraph{this->getTransitionGraph()};
  std::vector<unsigned short> originalPermutation{};

  for (unsigned short i{}; i < this->totalConfigs; i++)
  {
    originalPermutation.push_back(i);
  }

  std::cout
    << "\n"
    << "+" << std::string(10, '-')
    << "+" << std::string(50, '-')
    << "+" << std::string(30, '-')
    << "+"
    << "\n";

  std::cout
    << "|" << std::setw(10) << "S.No "
    << "|" << std::setw(50) << "Permutation "
    << "|" << std::setw(30) << "Rules "
    << "|"
    << "\n";

  std::cout
    << "+" << std::string(10, '-')
    << "+" << std::string(50, '-')
    << "+" << std::string(30, '-')
    << "+"
    << "\n";

#pragma omp parallel
  {
    types::TransitionGraph localTransitionGraph(this->totalConfigs, 0);
    types::Rules localRules(this->numCells, 0);
    std::vector<unsigned short> localPermutation{originalPermutation};

#pragma omp for
    for (unsigned short i = 0; i < this->totalConfigs; i++)
    {
      std::rotate(
        localPermutation.begin(),
        localPermutation.begin() + i,
        localPermutation.begin() + i + 1
      );

      do
      {
        for (unsigned short j{}; j < localPermutation.size(); j++)
        {
          unsigned short currentConfig{localPermutation.at(j)};
          localTransitionGraph.at(currentConfig) = localPermutation.at(originalTransitionGraph.at(j));
        }

        bool isValidCA{this->extractRules(localTransitionGraph, localRules)};

#pragma omp critical
        {
          if (isValidCA)
          {
            std::cout
              << "|" << std::setw(9) << ++counter
              << " |" << std::setw(49) << utils::general::toString(localPermutation)
              << " |" << std::setw(29) << utils::general::toString(localRules)
              << " |"
              << "\n";

            std::cout
              << "+" << std::string(10, '-')
              << "+" << std::string(50, '-')
              << "+" << std::string(30, '-')
              << "+"
              << "\n";
          }
        }
      } while (std::next_permutation(localPermutation.begin() + 1, localPermutation.end()));
    }
  }
}

void
models::Binary1DCA::printComplementedIsomorphisms() const
{
  try
  {
    if (!this->hasComplementedIsomorphisms())
    {
      std::cout
        << "\nNo complemented isomorphisms"
        << "\n";

      return;
    }

    std::cout
      << "\n"
      << "+" << std::string(10, '-')
      << "+" << std::string(30, '-')
      << "+"
      << "\n";

    std::cout
      << "|" << std::setw(10) << "S.No "
      << "|" << std::setw(30) << "Rules "
      << "|"
      << "\n";

    std::cout
      << "+" << std::string(10, '-')
      << "+" << std::string(30, '-')
      << "+"
      << "\n";

    for (unsigned short i{}; i < (1 << this->numCells); i++)
    {
      unsigned short currentCell{};
      unsigned short currentIndex{i};
      unsigned short indexMask{static_cast<unsigned short>((1 << this->numCells) - 1)};

      std::ostringstream currentRuleVector{};
      currentRuleVector << "[ ";

      while (indexMask)
      {
        if (currentIndex & 1)
        {
          currentRuleVector << (255 - this->ruleVector.at(currentCell)) << " ";
        }
        else
        {
          currentRuleVector << this->ruleVector.at(currentCell) << " ";
        }

        currentIndex >>= 1;
        indexMask >>= 1;
        currentCell += 1;
      }

      currentRuleVector << "]";

      std::cout
        << "|" << std::setw(9) << (i + 1)
        << " |" << std::setw(29) << currentRuleVector.str()
        << " |"
        << "\n";

      std::cout
        << "+" << std::string(10, '-')
        << "+" << std::string(30, '-')
        << "+"
        << "\n";
    }
  }
  catch (const std::exception &error)
  {
    std::cerr
      << "\n"
      << error.what()
      << '\n';
  }
}

void
models::Binary1DCA::printCharacterisitcMatrix() const
{
  try
  {
    if (!this->isECA())
    {
      throw std::domain_error{"Charactersitic matrix is only supported for ECAs"};
    }

    types::Matrix M{this->ruleVector.getCharacteristicMatrix(this->BC)};
    utils::matrix::print(M);
  }
  catch (const std::exception &error)
  {
    std::cerr
      << "\n"
      << error.what()
      << '\n';
  }
}

void
models::Binary1DCA::printCharacterisitcPolynomial() const
{
  try
  {
    if (!this->isECA())
    {
      throw std::domain_error{"Characteristic polynomial is only supported for ECAs"};
    }

    types::Polynomial P{this->ruleVector.getCharactersiticPolynomial(this->BC)};
    utils::polynomial::print(P);
  }
  catch (const std::exception &error)
  {
    std::cerr
      << "\n"
      << error.what()
      << '\n';
  }
}

void
models::Binary1DCA::checkIsomorphismWithOtherCA()
{
  bool isIsomorphic{};

  models::Binary1DCA other{
    this->numCells,
    this->leftRadius,
    this->rightRadius,
    this->BC,
    readRules(this->numCells)
  };

  types::TransitionGraph otherTransitionGraph{other.getTransitionGraph()};
  types::TransitionGraph originalTransitionGraph{this->getTransitionGraph()};
  std::vector<unsigned short> originalPermutation{};

  for (unsigned short i{}; i < this->totalConfigs; i++)
  {
    originalPermutation.push_back(i);
  }

#pragma omp parallel
  {
    types::TransitionGraph localTransitionGraph(this->totalConfigs, 0);
    std::vector<unsigned short> localPermutation{originalPermutation};

#pragma omp for
    for (unsigned short i = 0; i < this->totalConfigs; i++)
    {
      if (isIsomorphic)
      {
        continue;
      }

      std::rotate(
        localPermutation.begin(),
        localPermutation.begin() + i,
        localPermutation.begin() + i + 1
      );

      do
      {
        if (isIsomorphic)
        {
          continue;
        }

        for (unsigned short j{}; j < localPermutation.size(); j++)
        {
          unsigned short currentConfig{localPermutation.at(j)};
          localTransitionGraph.at(currentConfig) = localPermutation.at(originalTransitionGraph.at(j));
        }

#pragma omp critical
        {
          if (localTransitionGraph == otherTransitionGraph)
          {
            isIsomorphic = true;
          }
        }
      } while (std::next_permutation(localPermutation.begin() + 1, localPermutation.end()));
    }
  }

  if (isIsomorphic)
  {
    std::cout
      << "\nISOMORPHIC"
      << "\n";
  }
  else
  {
    std::cout
      << "\nNOT ISOMORPHIC"
      << "\n";
  }
}
