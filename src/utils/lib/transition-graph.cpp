#include "../utils.hpp"

#include <sstream>
#include <unordered_set>

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
  const std::string &color
)
{
  utils::general::print_msg(utils::transition_graph::to_string(graph), color);
}
