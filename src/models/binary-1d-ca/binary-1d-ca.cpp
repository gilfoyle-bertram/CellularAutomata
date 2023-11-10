#include "binary-1d-ca.hpp"

#include <omp.h>

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <unordered_set>

#include "../../utils/utils.hpp"

models::Binary1DCA::Binary1DCA()
{
}

models::Binary1DCA::Binary1DCA(
  unsigned short numCells,
  unsigned short leftRadius,
  unsigned short rightRadius,
  models::BoundaryCondition boundaryCondition,
  const std::vector<unsigned short> &rules
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
  this->boundaryCondition = boundaryCondition;

  this->cells.resize(numCells);
  this->randomizeConfig();

  for (unsigned short i{}; i < numCells; i++)
  {
    this->cells.at(i).setRule(rules.at(i));
  }
}

bool
models::Binary1DCA::extractRules(
  const std::vector<unsigned short> &transitionGraph,
  std::vector<unsigned short> &rules
)
{
  for (unsigned short i{}; i < this->numCells; i++)
  {
    std::vector<char> currentRuleTable((1U << this->neighborhoodSize), 'X');
    std::ostringstream currentRuleStream{};

    for (unsigned short j{}; j < this->totalConfigs; j++)
    {
      std::string currentConfigStr{utils::toBinaryStr(j, this->numCells)};
      std::string nextConfigStr{utils::toBinaryStr(transitionGraph.at(j), this->numCells)};
      unsigned int neighborhood{utils::parseBinaryStr(this->getNeighborhood(i, currentConfigStr))};
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

    rules.at(i) = utils::parseBinaryStr(currentRuleStream.str());
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
  std::string currentConfigStr{utils::toBinaryStr(currentConfig, this->numCells)};

  for (unsigned short i{}; i < this->numCells; i++)
  {
    nextConfigStream << this->cells.at(i).applyRule(this->getNeighborhood(i, currentConfigStr));
  }

  return (this->globalConfigMap[currentConfig] = utils::parseBinaryStr(nextConfigStream.str()));
}

std::string
models::Binary1DCA::getNeighborhood(
  unsigned short cellIndex,
  const std::string &configStr
) const
{
  std::ostringstream neighborhood{};

  for (unsigned short l{this->leftRadius}; l >= 1; l--)
  {
    short currentNeighborIndex{static_cast<short>((cellIndex - l))};

    if (
      currentNeighborIndex < 0 &&
      this->boundaryCondition == models::BoundaryCondition::Null
    )
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

    if (
      currentNeighborIndex >= this->numCells &&
      this->boundaryCondition == models::BoundaryCondition::Null
    )
    {
      neighborhood << '0';
      continue;
    }

    neighborhood << configStr.at(currentNeighborIndex % this->numCells);
  }

  return neighborhood.str();
}

std::vector<unsigned short>
models::Binary1DCA::readRules()
{
  unsigned short rule{};
  std::vector<unsigned short> rules(this->numCells, 0);

  std::cout << "\n";

  for (unsigned short i{}; i < this->numCells; ++i)
  {
    std::cout << "Rule for cell-" << (i + 1) << ": ";
    std::cin >> rule;
    rules.at(i) = rule;
  }

  return rules;
}

void
models::Binary1DCA::randomizeConfig()
{
  for (auto &cell : this->cells)
  {
    cell.setState(utils::getRandomBool());
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

  return utils::parseBinaryStr(currentConfigStream.str());
}

std::pair<models::BoundaryCondition, std::string>
models::Binary1DCA::getBoundaryCondition() const
{
  std::pair<models::BoundaryCondition, std::string> result{};

  result.first = this->boundaryCondition;
  result.second = this->boundaryCondition == models::BoundaryCondition::Null ? "Null" : "Periodic";

  return result;
}

// For a CA, the out-degree of any node is exactly 1.
// By exploiting this property, we can represent the transition graph as a 1D-Array itself.
// So "transitionGraph.at(i) = j", means there is an edge from configuration i to configuration j.
std::vector<unsigned short>
models::Binary1DCA::getTransitionGraph()
{
  std::vector<unsigned short> adjacencyList{};

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
  std::string currentConfigStr{utils::toBinaryStr(this->getCurrentConfig(), this->numCells)};

  for (unsigned short i{}; i < this->numCells; i++)
  {
    this->cells.at(i).updateState(this->getNeighborhood(i, currentConfigStr));
  }
}

void
models::Binary1DCA::printTransitionGraph()
{
  const std::vector<unsigned short> &transitionGraph{this->getTransitionGraph()};
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
  std::vector<unsigned short> originalTransitionGraph{this->getTransitionGraph()};
  std::vector<unsigned short> originalPermutation{};

  for (unsigned short i{}; i < this->totalConfigs; i++)
  {
    originalPermutation.push_back(i);
  }

  std::cout
    << "\n"
    << "+" << std::string(10, '-')
    << "+" << std::string(50, '-')
    << "+" << std::string(50, '-')
    << "+"
    << "\n";

  std::cout
    << "|" << std::setw(10) << "S.No "
    << "|" << std::setw(50) << "Permutation "
    << "|" << std::setw(50) << "Rules "
    << "|"
    << "\n";

  std::cout
    << "+" << std::string(10, '-')
    << "+" << std::string(50, '-')
    << "+" << std::string(50, '-')
    << "+"
    << "\n";

#pragma omp parallel
  {
    std::vector<unsigned short> localTransitionGraph(this->totalConfigs, 0);
    std::vector<unsigned short> localRules(this->numCells, 0);
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
              << " |" << std::setw(49) << utils::toString(localPermutation)
              << " |" << std::setw(49) << utils::toString(localRules)
              << " |"
              << "\n";

            std::cout
              << "+" << std::string(10, '-')
              << "+" << std::string(50, '-')
              << "+" << std::string(50, '-')
              << "+"
              << "\n";
          }
        }
      } while (std::next_permutation(localPermutation.begin() + 1, localPermutation.end()));
    }
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
    this->boundaryCondition,
    this->readRules()
  };

  std::vector<unsigned short> otherTransitionGraph{other.getTransitionGraph()};
  std::vector<unsigned short> originalTransitionGraph{this->getTransitionGraph()};
  std::vector<unsigned short> originalPermutation{};

  for (unsigned short i{}; i < this->totalConfigs; i++)
  {
    originalPermutation.push_back(i);
  }

#pragma omp parallel
  {
    std::vector<unsigned short> localTransitionGraph(this->totalConfigs, 0);
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
