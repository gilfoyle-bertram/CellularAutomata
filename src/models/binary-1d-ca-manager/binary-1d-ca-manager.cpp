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

  std::cout << "no. of cells (max: " << (models::binary_1d_ca::max_size) << " cells): ";
  std::cin >> num_cells;

  if (num_cells > models::binary_1d_ca::max_size)
  {
    throw std::invalid_argument{"unsupported cellular automata size"};
  }

  return num_cells;
}

types::boundary
models::binary_1d_ca_manager::read_boundary()
{
  char boundary_char{};

  std::cout << "\n";

  std::cout << "boundary condition [(n)ull / (p)eriodic]: ";
  std::cin >> boundary_char;

  if (boundary_char == 'n' || boundary_char == 'N')
  {
    return types::boundary::null;
  }

  if (boundary_char == 'p' || boundary_char == 'P')
  {
    return types::boundary::periodic;
  }

  utils::general::print_msg("err: invalid boundary condition\n     try again", colors::red);

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
    std::cout << "rule for cell-" << (i + 1) << ": ";
    std::cin >> rule;

    rules.at(i) = rule;
  }

  return rules;
}

void
models::binary_1d_ca_manager::print_reversed_isomorphable_ecas()
{
  types::short_whole_num counter{};
  types::short_whole_num num_cells{models::binary_1d_ca_manager::read_num_cells()};
  types::boundary boundary{models::binary_1d_ca_manager::read_boundary()};

  bool header_printed{};
  models::binary_1d_ca current_ca{};

  std::vector<std::pair<std::string, types::short_whole_num>> headings{
    std::make_pair<std::string, types::short_whole_num>(
      "s. no", 7
    ),
    std::make_pair<std::string, types::short_whole_num>(
      "rules", std::max(num_cells * 6, 24)
    )
  };

  for (types::short_whole_num i{}; i < 1000; i++)
  {
    current_ca = models::reversible_eca::get_random(num_cells, boundary);

    if (current_ca.has_non_trivial_reversed_isomorphisms())
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
        )
      };

      utils::general::print_row(entries);
    }
  }

  if (!header_printed)
  {
    utils::general::print_msg("couldn't generate any required ECAs", colors::blue);
  }
}

void
models::binary_1d_ca_manager::read_ca_details()
{
  types::short_whole_num num_cells{models::binary_1d_ca_manager::read_num_cells()};

  types::short_whole_num l_radius{};
  types::short_whole_num r_radius{};

  std::cout << "left radius: ";
  std::cin >> l_radius;

  std::cout << "right radius: ";
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
    "print transition graph",
    "print CA details",
    "print isomorphisms (exhaustive)",
    "check isomorphism (exhaustive)",
    "print complemented isomorphisms",
    "print reversed isomorphisms",
    "print characteristic matrix",
    "print characteristic polynomial",
    "print CAs by complementing rules based on equivalent RMTs",
    "go back"
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
          utils::general::print_msg("isomorphic", colors::cyan);
        }
        else
        {
          utils::general::print_msg("not isomorphic", colors::blue);
        }

        break;
      }

      case 5: {
        this->current_ca.print_complemented_isomorphisms();
        break;
      }

      case 6: {
        this->current_ca.print_reversed_isomorphisms();
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

      default: {
        run_system = false;
      }
    }
  }
}
