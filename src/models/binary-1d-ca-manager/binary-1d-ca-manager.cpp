#include "binary-1d-ca-manager.hpp"

#include <iostream>
#include <unordered_set>
#include <vector>

#include "../../terminal/terminal.hpp"
#include "../../utils/utils.hpp"
#include "../reversible-eca/reversible-eca.hpp"

types::short_whole_num
models::binary_1d_ca_manager::read_num_cells()
{
  types::short_whole_num num_cells{};
  std::cout << "\n";

  std::cout << "No. of cells (max: " << (models::binary_1d_ca::max_size) << " cells): ";
  std::cin >> num_cells;

  if (num_cells > models::binary_1d_ca::max_size)
  {
    throw std::invalid_argument{"Unsupported cellular automata size"};
  }

  return num_cells;
}

types::boundary
models::binary_1d_ca_manager::read_boundary()
{
  char boundary_char{};
  std::cout << "\n";

  std::cout << "Boundary condition [(n)ull / (p)eriodic]: ";
  std::cin >> boundary_char;

  if (boundary_char == 'n' || boundary_char == 'N')
  {
    return types::boundary::null;
  }

  if (boundary_char == 'p' || boundary_char == 'P')
  {
    return types::boundary::periodic;
  }

  utils::general::print_msg("ERR: Invalid boundary condition\n     Try again", colors::red);
  return read_boundary();
}

types::rules
models::binary_1d_ca_manager::read_rules(types::short_whole_num num_rules)
{
  types::long_whole_num rule{};
  types::rules rules(num_rules, 0);
  std::cout << "\n";

  for (types::short_whole_num i{0}; i < num_rules; ++i)
  {
    std::cout << "Rule for cell-" << (i + 1) << ": ";
    std::cin >> rule;
    rules.at(i) = rule;
  }

  return rules;
}

void
models::binary_1d_ca_manager::print_reversed_pseudo_isomorphable_ecas(bool is_uniform)
{
  types::short_whole_num counter{};
  types::short_whole_num num_cells{models::binary_1d_ca_manager::read_num_cells()};
  types::boundary boundary{models::binary_1d_ca_manager::read_boundary()};

  bool header_printed{};
  bool has_trivial_partition{};
  bool has_non_trivial_partitions{};
  models::binary_1d_ca current_ca{};

  std::vector<std::pair<std::string, types::short_whole_num>> headings{
    std::make_pair<std::string, types::short_whole_num>("S. No", 7),
    std::make_pair<std::string, types::short_whole_num>("Rules", std::max(num_cells * 6, 24)),
    std::make_pair<std::string, types::short_whole_num>("Trivial Partition", 17),
    std::make_pair<std::string, types::short_whole_num>("Non Trivial Partitions", 22),
    std::make_pair<std::string, types::short_whole_num>("1-1/1-N SN Maps", 15)
  };

  types::short_whole_num n = is_uniform ? 256 : 1000;

  for (types::short_whole_num i{}; i < n; i++)
  {
    has_trivial_partition = false;
    has_non_trivial_partitions = false;

    if (is_uniform)
    {
      types::rules rules(num_cells, i);
      current_ca = models::binary_1d_ca{num_cells, 1, 1, boundary, rules};
    }
    else
    {
      current_ca = models::reversible_eca::get_random(num_cells, boundary);
    }

    if (
      current_ca.has_non_trivial_reversed_pseudo_isomorphisms(
        has_trivial_partition, has_non_trivial_partitions
      )
    )
    {
      if (!header_printed)
      {
        utils::general::print_header(headings);
        header_printed = true;
      }

      std::vector<std::pair<std::string, types::short_whole_num>> entries{
        std::make_pair<std::string, types::short_whole_num>(
          std::to_string(++counter), 7
        ),
        std::make_pair<std::string, types::short_whole_num>(
          current_ca.get_rule_vector().to_string(),
          std::max(num_cells * 6, 24)
        ),
        std::make_pair<std::string, types::short_whole_num>(
          has_trivial_partition ? "True" : "False", 17
        ),
        std::make_pair<std::string, types::short_whole_num>(
          has_non_trivial_partitions ? "True" : "False", 22
        ),
        std::make_pair<std::string, types::short_whole_num>(
          current_ca.has_1_1_or_1_n_sn_maps() ? "True" : "False", 15
        )
      };

      utils::general::print_row(entries);
    }
  }

  if (!header_printed)
  {
    utils::general::print_msg("Couldn't generate any required ECAs", colors::blue);
  }
}

void
models::binary_1d_ca_manager::print_single_cycle_reversible_ecas()
{
  types::short_whole_num counter{};
  types::short_whole_num num_cells{models::binary_1d_ca_manager::read_num_cells()};
  types::boundary boundary{models::binary_1d_ca_manager::read_boundary()};

  bool header_printed{};
  models::binary_1d_ca current_ca{};

  std::vector<std::pair<std::string, types::short_whole_num>> headings{
    std::make_pair<std::string, types::short_whole_num>("S. No", 7),
    std::make_pair<std::string, types::short_whole_num>("Rules", std::max(num_cells * 6, 24))
  };

  for (types::short_whole_num i{}; i < 1000; i++)
  {
    current_ca = models::reversible_eca::get_random(num_cells, boundary);

    if (utils::transition_graph::get_cycles(current_ca.get_graph()).size() <= 1)
    {
      if (!header_printed)
      {
        utils::general::print_header(headings);
        header_printed = true;
      }

      std::vector<std::pair<std::string, types::short_whole_num>> entries{
        std::make_pair<std::string, types::short_whole_num>(
          std::to_string(++counter), 7
        ),
        std::make_pair<std::string, types::short_whole_num>(
          current_ca.get_rule_vector().to_string(), std::max(num_cells * 6, 24)
        )
      };

      utils::general::print_row(entries);
    }
  }

  if (!header_printed)
  {
    utils::general::print_msg("Couldn't generate any required ECAs", colors::blue);
  }
}

void
models::binary_1d_ca_manager::print_reversible_ecas()
{
  types::short_whole_num counter{};
  types::short_whole_num num_cells{models::binary_1d_ca_manager::read_num_cells()};
  types::boundary boundary{models::binary_1d_ca_manager::read_boundary()};
  models::binary_1d_ca current_ca{};

  std::vector<std::pair<std::string, types::short_whole_num>> headings{
    std::make_pair<std::string, types::short_whole_num>("S. No", 7),
    std::make_pair<std::string, types::short_whole_num>("Rules", std::max(num_cells * 6, 24))
  };

  utils::general::print_header(headings);

  for (types::short_whole_num i{}; i < 100; i++)
  {
    current_ca = models::reversible_eca::get_random(num_cells, boundary);

    std::vector<std::pair<std::string, types::short_whole_num>> entries{
      std::make_pair<std::string, types::short_whole_num>(
        std::to_string(++counter), 7
      ),
      std::make_pair<std::string, types::short_whole_num>(
        current_ca.get_rule_vector().to_string(),
        std::max(num_cells * 6, 24)
      )
    };

    utils::general::print_row(entries);
  }
}

void
models::binary_1d_ca_manager::read_ca_details()
{
  types::short_whole_num num_cells{models::binary_1d_ca_manager::read_num_cells()};
  types::short_whole_num l_radius{};
  types::short_whole_num r_radius{};

  std::cout << "Left radius: ";
  std::cin >> l_radius;

  std::cout << "Right radius: ";
  std::cin >> r_radius;

  types::boundary boundary{models::binary_1d_ca_manager::read_boundary()};
  types::rules rules{models::binary_1d_ca_manager::read_rules(num_cells)};
  this->current_ca = models::binary_1d_ca{num_cells, l_radius, r_radius, boundary, rules};
}

void
models::binary_1d_ca_manager::access_system()
{
  this->read_ca_details();
  bool run_system{true};

  std::vector<std::string> choices{
    "Print transition graph",
    "Print CA details",
    "Print isomorphisms (*)",
    "Check isomorphism (*)",
    "Print complemented isomorphisms",
    "Print reversed pseudo-isomorphisms",
    "Print characteristic matrix",
    "Print characteristic polynomial",
    "Print CAs by complementing rules based on equivalent RMTs",
    "Print affected configurations by tweaking rules",
    "Print state-neighborhood maps",
    "Go back"
  };

  while (run_system)
  {
    types::short_whole_num choice{utils::general::get_choice(choices)};

    switch (choice)
    {
      case 1: {
        this->current_ca.print_transition_graph();
        break;
      }

      case 2: {
        this->current_ca.print_details();
        break;
      }

      case 3: {
        this->current_ca.print_isomorphisms();
        break;
      }

      case 4: {
        types::short_whole_num num_cells{this->current_ca.get_num_cells()};
        types::short_whole_num l_radius{this->current_ca.get_l_radius()};
        types::short_whole_num r_radius{this->current_ca.get_r_radius()};
        types::boundary boundary{this->current_ca.get_boundary()};
        types::rules rules{read_rules(num_cells)};
        models::binary_1d_ca other{num_cells, l_radius, r_radius, boundary, rules};

        if (this->current_ca.is_isomorphic(other))
        {
          utils::general::print_msg("Isomorphic", colors::cyan);
        }
        else
        {
          utils::general::print_msg("Not isomorphic", colors::blue);
        }

        break;
      }

      case 5: {
        this->current_ca.print_complemented_isomorphisms();
        break;
      }

      case 6: {
        this->current_ca.print_reversed_pseudo_isomorphisms();
        break;
      }

      case 7: {
        this->current_ca.print_characterisitc_matrix();
        break;
      }

      case 8: {
        this->current_ca.print_characterisitc_polynomial();
        break;
      }

      case 9: {
        this->current_ca.print_rmts_complemented_rules();
        break;
      }

      case 10: {
        this->current_ca.tweak_rules();
        break;
      }

      case 11: {
        this->current_ca.print_sn_maps();
        break;
      }

      default: {
        run_system = false;
      }
    }
  }
}
