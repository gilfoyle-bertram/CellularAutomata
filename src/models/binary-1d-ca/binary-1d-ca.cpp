#include "binary-1d-ca.hpp"

#include <omp.h>

#include <algorithm>
#include <climits>
#include <sstream>
#include <stdexcept>
#include <unordered_set>

#include "../../terminal/terminal.hpp"
#include "../../utils/utils.hpp"

static std::string
get_boundary_str(types::boundary boundary)
{
  if (boundary == types::boundary::null)
  {
    return "null";
  }

  return "periodic";
}

static void
validate_radii(
  types::whole_num num_cells,
  types::whole_num l_radius,
  types::whole_num r_radius
)
{
  if (num_cells < l_radius + r_radius + 1)
  {
    throw std::invalid_argument{"neighborhood size can't be greater than CA size"};
  }
}

static void
validate_rules_size(types::whole_num num_cells, const types::rules &rules)
{
  if (num_cells != rules.size())
  {
    throw std::invalid_argument{"number of rules must be equal to number of cells"};
  }
}

static void
validate_ca_size(types::whole_num num_cells)
{
  if (num_cells > models::binary_1d_ca::max_size)
  {
    throw std::invalid_argument{"unsupported cellular automata size"};
  }
}

bool
models::binary_1d_ca::is_elementary() const
{
  return this->l_radius == 1 && this->r_radius == 1;
}

bool
models::binary_1d_ca::has_complemented_isomorphisms() const
{
  if (!this->is_elementary())
  {
    throw std::domain_error{"complemented isomorphisms are only supported for ECAs"};
  }

  return this->rule_vector.is_complementable(this->boundary);
}

bool
models::binary_1d_ca::extract_rules(
  const types::transition_graph &graph,
  types::rules &rules
) const
{
  for (types::whole_num i{}; i < this->num_cells; i++)
  {
    std::vector<char> current_rule_table((1U << this->num_neighbors), 'X');
    std::ostringstream current_rule_stream{};

    for (types::whole_num j{}; j < this->num_configs; j++)
    {
      std::string current_config_str{utils::number::to_binary_str(j, this->num_cells)};
      std::string next_config_str{utils::number::to_binary_str(graph.at(j), this->num_cells)};
      std::string neighborhood_str{this->get_neighborhood_str(i, current_config_str)};
      types::whole_num neighborhood{utils::number::parse_binary_str(neighborhood_str)};

      char next_state{next_config_str.at(i)};

      if (current_rule_table.at(neighborhood) == 'X')
      {
        current_rule_table.at(neighborhood) = next_state;
      }
      else if (current_rule_table.at(neighborhood) != next_state)
      {
        return false;
      }
    }

    for (types::whole_num j{}; j < current_rule_table.size(); j++)
    {
      char current_char{current_rule_table.at(current_rule_table.size() - 1 - j)};
      current_rule_stream << (current_char == 'X' ? '0' : current_char);
    }

    rules.at(i) = utils::number::parse_binary_str(current_rule_stream.str());
  }

  return true;
}

types::whole_num
models::binary_1d_ca::get_next_config(types::whole_num current_config)
{
  std::unordered_map<types::whole_num, types::whole_num> global_config_map{};

  if (global_config_map.count(current_config))
  {
    return global_config_map.at(current_config);
  }

  std::ostringstream next_config_stream{};

  std::string current_config_str{
    utils::number::to_binary_str(current_config, this->num_cells)
  };

  for (types::whole_num i{}; i < this->num_cells; i++)
  {
    models::binary_cell &current_cell{this->cells.at(i)};

    bool next_state{
      current_cell.next_state(this->get_neighborhood_str(i, current_config_str))
    };

    next_config_stream << static_cast<types::whole_num>(next_state);
  }

  types::whole_num next_config{utils::number::parse_binary_str(next_config_stream.str())};

  return global_config_map[current_config] = next_config;
}

std::string
models::binary_1d_ca::get_neighborhood_str(
  types::whole_num cell_num,
  const std::string &config_str
) const
{
  std::ostringstream neighborhood_stream{};
  bool has_null_boundary{this->boundary == types::boundary::null};

  for (types::whole_num l{this->l_radius}; l >= 1; l--)
  {
    short current_neighbor_index{static_cast<short>((cell_num - l))};

    if (current_neighbor_index < 0 && has_null_boundary)
    {
      neighborhood_stream << '0';
      continue;
    }

    current_neighbor_index = (this->num_cells + current_neighbor_index) % this->num_cells;
    neighborhood_stream << config_str.at(current_neighbor_index);
  }

  neighborhood_stream << config_str.at(cell_num);

  for (types::whole_num r{1}; r <= this->r_radius; r++)
  {
    short current_neighbor_index{static_cast<short>((cell_num + r))};

    if (current_neighbor_index >= this->num_cells && has_null_boundary)
    {
      neighborhood_stream << '0';
      continue;
    }

    current_neighbor_index = current_neighbor_index % this->num_cells;
    neighborhood_stream << config_str.at(current_neighbor_index);
  }

  return neighborhood_stream.str();
}

void
models::binary_1d_ca::set_rules(const types::rules &rules)
{
  this->cells.resize(this->num_cells);

  for (types::whole_num i{}; i < this->num_cells; i++)
  {
    this->cells.at(i).set_rule(rules.at(i));
  }
}

void
models::binary_1d_ca::randomize_config()
{
  for (auto &cell : this->cells)
  {
    cell.set_state(utils::general::get_random_bool());
  }
}

void
models::binary_1d_ca::fill_transition_graph()
{
  this->graph.resize(this->num_configs);

  for (types::whole_num i{}; i < this->num_configs; i++)
  {
    this->graph.at(i) = this->get_next_config(i);
  }
}

models::binary_1d_ca::binary_1d_ca()
{
}

models::binary_1d_ca::binary_1d_ca(
  types::whole_num num_cells,
  types::whole_num l_radius,
  types::whole_num r_radius,
  types::boundary boundary,
  const types::rules &rules
)
{
  validate_radii(num_cells, l_radius, r_radius);
  validate_rules_size(num_cells, rules);
  validate_ca_size(num_cells);

  this->num_cells = num_cells;
  this->l_radius = l_radius;
  this->r_radius = r_radius;
  this->num_neighbors = l_radius + r_radius + 1;
  this->num_configs = static_cast<types::whole_num>(1U << num_cells);
  this->boundary = boundary;
  this->rule_vector = models::rule_vector{rules};

  this->set_rules(rules);
  this->randomize_config();
  this->fill_transition_graph();
}

types::whole_num
models::binary_1d_ca::get_num_cells() const
{
  return this->num_cells;
}

types::whole_num
models::binary_1d_ca::get_l_radius() const
{
  return this->l_radius;
}

types::whole_num
models::binary_1d_ca::get_r_radius() const
{
  return this->r_radius;
}

types::whole_num
models::binary_1d_ca::get_num_neighbors() const
{
  return this->num_neighbors;
}

types::whole_num
models::binary_1d_ca::get_num_configs() const
{
  return this->num_configs;
}

types::boundary
models::binary_1d_ca::get_boundary() const
{
  return this->boundary;
}

const types::transition_graph &
models::binary_1d_ca::get_graph() const
{
  return this->graph;
}

const models::rule_vector &
models::binary_1d_ca::get_rule_vector() const
{
  return this->rule_vector;
}

types::whole_num
models::binary_1d_ca::get_current_config() const
{
  std::ostringstream current_config_stream{};

  for (const auto &cell : this->cells)
  {
    current_config_stream << static_cast<types::whole_num>(cell.get_state());
  }

  return utils::number::parse_binary_str(current_config_stream.str());
}

// This implementation is sub-optimal.
// The execution of threads continue, even after determining the CA to be isomorphic.
// This is because there is no way (as per my knowledge) to break out of OMP loops.
bool
models::binary_1d_ca::is_isomorphic(models::binary_1d_ca &other) const
{
  bool is_isomorphic{};

  const types::transition_graph &other_graph{other.get_graph()};
  const types::transition_graph &this_graph{this->get_graph()};
  std::vector<types::whole_num> this_permutation{};

  for (types::whole_num i{}; i < this->num_configs; i++)
  {
    this_permutation.push_back(i);
  }

#pragma omp parallel
  {
    types::transition_graph local_graph(this->num_configs, 0);
    std::vector<types::whole_num> local_permutation{this_permutation};

#pragma omp for
    for (types::whole_num i = 0; i < this->num_configs; i++)
    {
      if (is_isomorphic)
      {
        continue;
      }

      std::rotate(
        local_permutation.begin(),
        local_permutation.begin() + i,
        local_permutation.begin() + i + 1
      );

      do
      {
        if (is_isomorphic)
        {
          continue;
        }

        for (types::whole_num j{}; j < local_permutation.size(); j++)
        {
          types::whole_num current_config{local_permutation.at(j)};
          local_graph.at(current_config) = local_permutation.at(this_graph.at(j));
        }

#pragma omp critical
        {
          if (local_graph == other_graph)
          {
            is_isomorphic = true;
          }
        }
      } while (
        std::next_permutation(local_permutation.begin() + 1, local_permutation.end())
      );
    }
  }

  return is_isomorphic;
}

void
models::binary_1d_ca::print_isomorphisms() const
{
  unsigned int counter{};
  const types::transition_graph &this_graph{this->get_graph()};
  std::vector<types::whole_num> this_permutation{};

  for (types::whole_num i{}; i < this->num_configs; i++)
  {
    this_permutation.push_back(i);
  }

  static std::vector<std::pair<std::string, types::whole_num>> headings{
    std::make_pair<std::string, types::whole_num>("s. no", 10),
    std::make_pair<std::string, types::whole_num>("permutation", 45),
    std::make_pair<std::string, types::whole_num>("rules", 25)
  };

  utils::general::print_header(headings);

#pragma omp parallel
  {
    types::transition_graph local_graph(this->num_configs, 0);
    types::rules local_rules(this->num_cells, 0);
    std::vector<types::whole_num> local_permutation{this_permutation};

#pragma omp for
    for (types::whole_num i = 0; i < this->num_configs; i++)
    {
      std::rotate(
        local_permutation.begin(),
        local_permutation.begin() + i,
        local_permutation.begin() + i + 1
      );

      do
      {
        for (types::whole_num j{}; j < local_permutation.size(); j++)
        {
          types::whole_num current_config{local_permutation.at(j)};
          local_graph.at(current_config) = local_permutation.at(this_graph.at(j));
        }

        bool is_valid_ca{this->extract_rules(local_graph, local_rules)};

#pragma omp critical
        {
          if (is_valid_ca)
          {
            std::string permutation_str{utils::vector::to_string(local_permutation)};
            std::string rules_str{utils::vector::to_string(local_rules)};

            std::vector<std::pair<std::string, types::whole_num>> entries{
              std::make_pair<std::string, types::whole_num>(std::to_string(++counter), 10),
              std::make_pair<std::string &, types::whole_num>(permutation_str, 45),
              std::make_pair<std::string &, types::whole_num>(rules_str, 25)
            };

            utils::general::print_row(entries);
          }
        }
      } while (
        std::next_permutation(local_permutation.begin() + 1, local_permutation.end())
      );
    }
  }
}

void
models::binary_1d_ca::print_details() const
{
  static std::vector<std::pair<std::string, types::whole_num>> headings{
    std::make_pair<std::string, types::whole_num>("num_cells", 9),
    std::make_pair<std::string, types::whole_num>("l_radius", 8),
    std::make_pair<std::string, types::whole_num>("r_radius", 8),
    std::make_pair<std::string, types::whole_num>("boundary", 9),
    std::make_pair<std::string, const types::whole_num &>("rules", 25)
  };

  utils::general::print_header(headings);

  std::string rules_str{this->rule_vector.to_string()};

  std::vector<std::pair<std::string, types::whole_num>> entries{
    std::make_pair<std::string, types::whole_num>(std::to_string(this->num_cells), 9),
    std::make_pair<std::string, types::whole_num>(std::to_string(this->l_radius), 8),
    std::make_pair<std::string, types::whole_num>(std::to_string(this->r_radius), 8),
    std::make_pair<std::string, types::whole_num>(get_boundary_str(this->boundary), 9),
    std::make_pair<const std::string &, types::whole_num>(rules_str, 25)
  };

  utils::general::print_row(entries);
}

void
models::binary_1d_ca::print_transition_graph() const
{
  utils::transition_graph::print(this->get_graph());
}

void
models::binary_1d_ca::print_complemented_isomorphisms() const
{
  try
  {
    if (!this->has_complemented_isomorphisms())
    {
      utils::general::print_msg("no complemented isomorphisms", colors::cyan);
      return;
    }

    static std::vector<std::pair<std::string, types::whole_num>> headings{
      std::make_pair<std::string, types::whole_num>("s. no", 6),
      std::make_pair<std::string, types::whole_num>("rules", 25)
    };

    utils::general::print_header(headings);

    for (types::whole_num i{}; i < (1U << this->num_cells); i++)
    {
      types::whole_num current_cell{};
      types::whole_num current_index{i};
      types::whole_num index_mask{static_cast<types::whole_num>((1U << this->num_cells) - 1)};
      types::rules current_rules{};

      while (index_mask)
      {
        if (current_index & 1)
        {
          current_rules.push_back(255 - this->rule_vector.at(current_cell));
        }
        else
        {
          current_rules.push_back(this->rule_vector.at(current_cell));
        }

        current_index >>= 1;
        index_mask >>= 1;
        current_cell += 1;
      }

      std::vector<std::pair<std::string, types::whole_num>> entries{
        std::make_pair<std::string, types::whole_num>(std::to_string(i + 1), 6),
        std::make_pair<std::string, types::whole_num>(utils::vector::to_string(current_rules), 25)
      };

      utils::general::print_row(entries);
    }
  }
  catch (const std::exception &err)
  {
    utils::general::print_msg(err.what(), colors::red);
  }
}

void
models::binary_1d_ca::print_reversed_isomorphisms() const
{
  std::vector<std::unordered_set<types::whole_num>> cycles{
    utils::transition_graph::get_cycles(this->graph)
  };

  if (cycles.size() == 0)
  {
    utils::general::print_msg("no cycles to reverse", colors::yellow);
    return;
  }

  static std::vector<std::pair<std::string, types::whole_num>> headings{
    std::make_pair<std::string, types::whole_num>("s. no", 6),
    std::make_pair<std::string, types::whole_num>("rules", 25)
  };

  utils::general::print_header(headings);

  for (types::whole_num i{}; i < (1U << cycles.size()); i++)
  {
    types::transition_graph current_graph(this->num_configs, USHRT_MAX);
    types::rules current_rules(this->num_cells, 0);
    types::whole_num current_index{i};
    types::whole_num current_cycle{};
    types::whole_num index_mask{static_cast<types::whole_num>((1U << cycles.size()) - 1)};

    while (index_mask)
    {
      if (current_index & 1)
      {
        for (const auto &config : cycles.at(current_cycle))
        {
          current_graph.at(this->graph.at(config)) = config;
        }
      }
      else
      {
        for (const auto &config : cycles.at(current_cycle))
        {
          current_graph.at(config) = this->graph.at(config);
        }
      }

      current_index >>= 1;
      index_mask >>= 1;
      current_cycle += 1;
    }

    for (types::whole_num i{}; i < current_graph.size(); i++)
    {
      if (current_graph.at(i) == USHRT_MAX)
      {
        current_graph.at(i) = this->graph.at(i);
      }
    }

    std::vector<std::pair<std::string, types::whole_num>> entries{
      std::make_pair<std::string, types::whole_num>(std::to_string(i + 1), 6),
      std::make_pair<std::string, types::whole_num>("X", 25)
    };

    if (this->extract_rules(current_graph, current_rules))
    {
      entries.at(1).first = utils::vector::to_string(current_rules);
      utils::general::print_row(entries);
    }
    else
    {
      entries.at(1).first = "invalid CA";
      utils::general::print_row(entries);
    }
  }
}

void
models::binary_1d_ca::print_characterisitc_matrix() const
{
  try
  {
    if (!this->is_elementary())
    {
      throw std::domain_error{"charactersitic matrix is only supported for ECAs"};
    }

    utils::matrix::print(this->rule_vector.get_characteristic_matrix(this->boundary));
  }
  catch (const std::exception &err)
  {
    utils::general::print_msg(err.what(), colors::red);
  }
}

void
models::binary_1d_ca::print_characterisitc_polynomial() const
{
  try
  {
    if (!this->is_elementary())
    {
      throw std::domain_error{"characteristic polynomial is only supported for ECAs"};
    }

    utils::polynomial::print(this->rule_vector.get_charactersitic_polynomial(this->boundary));
  }
  catch (const std::exception &err)
  {
    utils::general::print_msg(err.what(), colors ::red);
  }
}

void
models::binary_1d_ca::update_config()
{
  std::string current_config_str{
    utils::number::to_binary_str(this->get_current_config(), this->num_cells)
  };

  for (types::whole_num i{}; i < this->num_cells; i++)
  {
    this->cells.at(i).update_state(this->get_neighborhood_str(i, current_config_str));
  }
}
