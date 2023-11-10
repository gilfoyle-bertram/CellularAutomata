#ifndef __BINARY_1D_CA__
#define __BINARY_1D_CA__

#include <string>
#include <unordered_map>
#include <vector>

#include "../binary-cell/binary-cell.hpp"

namespace models
{
  enum BoundaryCondition
  {
    Null,
    Periodic
  };

  class Binary1DCA
  {
    private:
      unsigned short numCells{};
      unsigned short leftRadius{};
      unsigned short rightRadius{};
      unsigned short neighborhoodSize{};
      unsigned short totalConfigs{};
      BoundaryCondition boundaryCondition{};
      std::vector<BinaryCell> cells{};
      std::unordered_map<unsigned short, unsigned short> globalConfigMap{};

      bool
      extractRules(
        const std::vector<unsigned short> &transitionGraph,
        std::vector<unsigned short> &rules
      );

      unsigned short
      getNextConfig(unsigned short currentConfig);

      std::string
      getNeighborhood(unsigned short cellIndex, const std::string &configStr) const;

      std::vector<unsigned short>
      readRules();

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
        BoundaryCondition boundaryCondition,
        const std::vector<unsigned short> &rules
      );

      unsigned short
      getCurrentConfig() const;

      std::pair<BoundaryCondition, std::string>
      getBoundaryCondition() const;

      std::vector<unsigned short>
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
      checkIsomorphismWithOtherCA();
  };
};

#endif
