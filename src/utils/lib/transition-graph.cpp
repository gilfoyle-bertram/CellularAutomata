#include "../utils.hpp"

#include <algorithm>
#include <sstream>
#include <unordered_set>

bool
utils::transition_graph::are_isomorphic(
  const types::transition_graph &G,
  const types::transition_graph &H
)
{
  if (G.size() != H.size())
  {
    return false;
  }

  std::vector<types::short_whole_num> permutation{};

  for (types::short_whole_num i{}; i < G.size(); i++)
  {
    permutation.push_back(i);
  }

  types::transition_graph F(G.size(), 0);

  do
  {
    for (types::short_whole_num j{}; j < permutation.size(); j++)
    {
      F.at(permutation.at(j)) = permutation.at(G.at(j));
    }

    if (F == H)
    {
      {
        return true;
      }
    }
  } while (std::next_permutation(permutation.begin(), permutation.end()));

  return false;
}

// For a transition graph, a node can be in maximum of one cycle only.
// This is because the out-degree of any node is exactly 1.
types::cycles
utils::transition_graph::get_cycles(const types::transition_graph &graph)
{
  types::cycles cycles{};
  types::short_whole_num current_node{};
  std::unordered_set<types::short_whole_num> visited_nodes{};
  std::unordered_set<types::short_whole_num> cycled_nodes{};

  for (types::short_whole_num i{}; i < graph.size(); i++)
  {
    while (true)
    {
      if (cycled_nodes.find(current_node) != cycled_nodes.end())
      {
        current_node = i + 1;
        break;
      }

      if (visited_nodes.find(current_node) != visited_nodes.end())
      {
        std::unordered_set<types::short_whole_num> current_cycle{};
        types::short_whole_num start_node{current_node};
        bool is_cycle{true};
        current_cycle.insert(start_node);

        while ((current_node = graph.at(current_node)) != start_node)
        {
          if (cycled_nodes.find(current_node) != cycled_nodes.end())
          {
            is_cycle = false;
            break;
          }

          current_cycle.insert(current_node);
        }

        if (!is_cycle)
        {
          current_node = i + 1;
          break;
        }

        cycles.push_back(current_cycle);

        for (const auto &node : current_cycle)
        {
          cycled_nodes.insert(node);
        }
      }

      visited_nodes.insert(current_node);
      current_node = graph.at(current_node);
    }
  }

  return cycles;
}

void
utils::transition_graph::print(
  const types::transition_graph &graph,
  const std::string &cycle_color,
  const std::string &regular_color
)
{
  std::ostringstream out_stream{};
  bool is_first_cycle{true};
  types::cycles cycles{utils::transition_graph::get_cycles(graph)};
  std::unordered_set<types::short_whole_num> visited_nodes{};

  for (const auto &cycle : cycles)
  {
    out_stream << cycle_color;
    out_stream << (is_first_cycle ? "" : "\n");

    types::short_whole_num start_node{*cycle.begin()};
    types::short_whole_num current_node{start_node};

    visited_nodes.insert(current_node);
    out_stream << current_node;
    out_stream << " --> ";

    while ((current_node = graph.at(current_node)) != start_node)
    {
      out_stream << current_node;
      out_stream << " --> ";
      visited_nodes.insert(current_node);
    }

    out_stream << current_node;
    out_stream << colors::reset;
    is_first_cycle = false;
  }

  bool is_new_component{true};
  types::short_whole_num current_node{};

  for (types::short_whole_num i{}; i < graph.size(); i++)
  {
    while (true)
    {
      if (visited_nodes.find(current_node) != visited_nodes.end())
      {
        current_node = i + 1;
        is_new_component = true;
        break;
      }

      if (is_new_component)
      {
        out_stream << regular_color;
        out_stream << "\n";
        out_stream << current_node;
        out_stream << colors::reset;
      }

      out_stream << regular_color;
      out_stream << " --> ";
      out_stream << graph.at(current_node);
      out_stream << colors::reset;

      visited_nodes.insert(current_node);
      current_node = graph.at(current_node);
      is_new_component = false;
    }
  }

  utils::general::print_msg(out_stream.str());
}
