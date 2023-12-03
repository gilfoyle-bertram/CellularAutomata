#include "binary-1d-ca-manager.hpp"

#include <iostream>
#include <unordered_set>
#include <vector>

#include "../../terminal/terminal.hpp"
#include "../../utils/utils.hpp"

static types::rules
read_rules(types::short_whole_num num_rules)
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

void
models::binary_1d_ca_manager::read_ca_details()
{
  types::short_whole_num num_cells{};
  types::short_whole_num l_radius{};
  types::short_whole_num r_radius{};

  std::cout << "\n";

  std::cout << "no. of cells (max: " << (models::binary_1d_ca::max_size) << " cells): ";
  std::cin >> num_cells;

  std::cout << "left radius: ";
  std::cin >> l_radius;

  std::cout << "right radius: ";
  std::cin >> r_radius;

  types::boundary boundary{read_boundary()};
  types::rules rules{read_rules(num_cells)};

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
    "print isomorphisms",
    "check isomorphism",
    "print complemented isomorphisms",
    "print reversed isomorphisms",
    "print characteristic matrix",
    "print characteristic polynomial",
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

      default: {
        run_system = false;
      }
    }
  }
}
