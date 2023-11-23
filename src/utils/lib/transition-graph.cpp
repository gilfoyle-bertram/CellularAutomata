#include "../utils.hpp"

#include <sstream>
#include <unordered_set>

// For a transition graph, a node can be in maximum of one cycle only.
// This is because the out-degree of any node is exactly 1.
std::vector<std::unordered_set<types::whole_num>>
utils::transition_graph::get_cycles(const types::transition_graph &graph)
{
  types::whole_num current_node{};
  std::unordered_set<types::whole_num> visited_nodes{};
  std::unordered_set<types::whole_num> cycled_nodes{};
  std::vector<std::unordered_set<types::whole_num>> cycles{};

  for (types::whole_num i{}; i < graph.size(); i++)
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
        std::unordered_set<types::whole_num> current_cycle{};
        types::whole_num start_node{current_node};

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

std::string
utils::transition_graph::to_string(const types::transition_graph &graph)
{
  bool is_new_component{true};
  bool is_first_component{true};

  types::whole_num current_node{};
  std::unordered_set<types::whole_num> visited_nodes{};
  std::ostringstream out_stream{};

  for (types::whole_num i{}; i < graph.size(); i++)
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
        out_stream << (is_first_component ? "" : "\n");
        out_stream << current_node;

        is_first_component = false;
      }

      out_stream << " --> ";
      out_stream << graph.at(current_node);

      visited_nodes.insert(current_node);
      current_node = graph.at(current_node);
      is_new_component = false;
    }
  }

  return out_stream.str();
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

  std::vector<std::unordered_set<types::whole_num>> cycles{
    utils::transition_graph::get_cycles(graph)
  };

  std::unordered_set<types::whole_num> visited_nodes{};

  for (const auto &cycle : cycles)
  {
    out_stream << cycle_color;
    out_stream << (is_first_cycle ? "" : "\n");

    types::whole_num start_node{*cycle.begin()};
    types::whole_num current_node{start_node};

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
  types::whole_num current_node{};

  for (types::whole_num i{}; i < graph.size(); i++)
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
