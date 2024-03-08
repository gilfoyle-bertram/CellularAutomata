#ifndef __TYPES__
#define __TYPES__

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace types
{
  enum boundary
  {
    null,
    periodic
  };

  using short_num = signed short;
  using short_whole_num = unsigned short;

  using num = signed int;
  using whole_num = unsigned int;

  using long_num = signed long;
  using long_whole_num = unsigned long;

  using rules = std::vector<long_whole_num>;
  using matrix = std::vector<std::vector<short_num>>;
  using cycles = std::vector<std::unordered_set<types::short_whole_num>>;

  // A polynomial can be represented as a 1D array, by storing its co-efficients.
  // So `coeffs[i] = j`, means the co-efficient of (x^i) is j.
  using polynomial = std::vector<short_num>;

  // For a cellular automaton, the out-degree of any node is exactly 1.
  // By exploiting this property, we can represent its transition graph as a 1D array.
  // So `graph[i] = j`, means there is an edge from node i to node j.
  using transition_graph = std::vector<short_whole_num>;

  using rules_group = std::pair<types::rules, types::short_whole_num>;
  using sn_map = std::unordered_map<char, std::unordered_set<std::string>>;
}

#endif
