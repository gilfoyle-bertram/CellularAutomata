#ifndef __BINARY_1D_CA__
#define __BINARY_1D_CA__

#include <string>
#include <unordered_map>
#include <vector>

#include "../../types/types.hpp"
#include "../binary-cell/binary-cell.hpp"
#include "../rule-vector/rule-vector.hpp"

namespace models
{
  class Binary1DCA
  {
    private:
      unsigned short numCells{};
      unsigned short leftRadius{};
      unsigned short rightRadius{};
      unsigned short neighborhoodSize{};
      unsigned short totalConfigs{};

      std::vector<BinaryCell> cells{};
      std::unordered_map<unsigned short, unsigned short> globalConfigMap{};

      types::BoundaryCondition BC{};
      models::RuleVector ruleVector{};

      bool
      isECA() const;

      bool
      hasComplementedIsomorphisms() const;

      bool
      extractRules(const types::TransitionGraph &transitionGraph, types::Rules &rules);

      unsigned short
      getNextConfig(unsigned short currentConfig);

      std::string
      getNeighborhood(unsigned short cellIndex, const std::string &configStr) const;

      void
      randomizeConfig();

    public:
      Binary1DCA(const Binary1DCA &other) = delete;

      static const unsigned short MAX_SIZE{4};

      Binary1DCA();

      Binary1DCA(
        unsigned short numCells,
        unsigned short leftRadius,
        unsigned short rightRadius,
        const types::BoundaryCondition &BC,
        const types::Rules &rules
      );

      unsigned short
      getCurrentConfig() const;

      std::pair<types::BoundaryCondition, std::string>
      getBoundaryCondition() const;

      types::TransitionGraph
      getTransitionGraph();

      void
      printDetails() const;

      void
      updateConfig();

      void
      printTransitionGraph();

      void
      printIsomorphicPermutations();

      void
      printComplementedIsomorphisms() const;

      void
      printCharacterisitcMatrix() const;

      void
      printCharacterisitcPolynomial() const;

      void
      checkIsomorphismWithOtherCA();
  };
}

#endif
