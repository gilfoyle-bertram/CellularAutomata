#ifndef __TYPES__
#define __TYPES__

#include <vector>

namespace types
{
  enum boundary
  {
    null,
    periodic
  };

  using matrix = std::vector<std::vector<short>>;

  // A polynomial can be represented as a 1D array, by storing its co-efficients.
  // So `coeffs[i] = j`, means the co-efficient of (x^i) is j.
  using polynomial = std::vector<short>;

  // For a cellular automaton, the out-degree of any node is exactly 1.
  // By exploiting this property, we can represent its transition graph as a 1D array.
  // So `graph[i] = j`, means there is an edge from node i to node j.
  using transition_graph = std::vector<unsigned short>;

  using rules = std::vector<unsigned short>;

  using num = signed short;
  using whole_num = unsigned short;
}

#endif
