#include "reversible-eca.hpp"

#include <unordered_map>
#include <vector>

#include "../../types/types.hpp"
#include "../../utils/utils.hpp"

static const std::unordered_map<types::boundary, std::vector<types::rules_group>> first_cell_rule_groups{
  {
    types::boundary::null,
    {
      {{3, 12}, 0},
      {{5, 10}, 1},
      {{6, 9}, 2},
    },
  },
  {
    types::boundary::periodic,
    {
      {{51, 204}, 3},
      {{85, 170}, 4},
      {{102, 153}, 5},
      {{23, 53, 83, 113, 142, 172, 202, 232}, 6},
      {{27, 57, 78, 108, 147, 177, 198, 228}, 7},
      {{89, 106, 149, 166}, 8},
      {{86, 101, 154, 169}, 9},
      {{43, 58, 77, 92, 163, 178, 197, 212}, 10},
      {{39, 54, 99, 114, 141, 156, 201, 216}, 11},
    },
  }
};

static const std::unordered_map<types::boundary, std::vector<std::vector<types::rules_group>>> regular_cell_rule_groups{
  {
    types::boundary::null,
    {
      {
        {{51, 60, 195, 204}, 0},
        {{85, 90, 165, 170}, 1},
        {{102, 105, 150, 153}, 2},
        {{53, 58, 83, 92, 163, 172, 197, 202}, 3},
        {{54, 57, 99, 108, 147, 156, 198, 201}, 4},
        {{86, 89, 101, 106, 149, 154, 166, 169}, 5},
      },
      {
        {{15, 30, 45, 60, 75, 90, 105, 120, 135, 150, 165, 180, 195, 210, 225, 240}, 0},
      },
      {
        {{15, 51, 204, 240}, 0},
        {{85, 105, 150, 170}, 1},
        {{90, 102, 153, 165}, 2},
        {{23, 43, 77, 113, 142, 178, 212, 232}, 3},
        {{27, 39, 78, 114, 141, 177, 216, 228}, 4},
        {{86, 89, 101, 106, 149, 154, 166, 169}, 5},
      },
      {
        {{60, 195}, 0},
        {{90, 165}, 3},
        {{105, 150}, 4},
      },
      {
        {{51, 204}, 0},
        {{85, 170}, 1},
        {{102, 153}, 2},
        {{86, 89, 90, 101, 105, 106, 149, 150, 154, 165, 166, 169}, 5},
      },
      {
        {{15, 240}, 0},
        {{105, 150}, 3},
        {{90, 165}, 4},
      },
    },
  },
  {
    types::boundary::periodic,
    {
      {
        {{53, 54, 57, 58, 85, 86, 89, 90, 101, 102, 105, 106, 149, 150, 153, 154, 165, 166, 169, 170, 197, 198, 201, 202}, 2},
      },
      {
        {{83, 85, 86, 89, 90, 92, 99, 101, 102, 105, 106, 108, 147, 149, 150, 153, 154, 156, 163, 165, 166, 169, 170, 172}, 2},
      },
      {
        {{85, 86, 89, 90, 101, 102, 105, 106, 149, 150, 153, 154, 165, 166, 169, 170}, 2},
      },
      {
        {{51, 60, 195, 204}, 3},
        {{85, 90, 165, 170}, 4},
        {{102, 105, 150, 153}, 5},
        {{53, 58, 83, 92, 163, 172, 197, 202}, 12},
        {{54, 57, 99, 108, 147, 156, 198, 201}, 13},
        {{86, 89, 101, 106, 149, 154, 166, 169}, 14},
      },
      {
        {{29, 46, 89, 106, 149, 166, 209, 226}, 0},
        {{71, 86, 101, 116, 139, 154, 169, 184}, 1},
        {{85, 102, 153, 170}, 2},
        {{15, 30, 45, 60, 75, 90, 105, 120, 135, 150, 165, 180, 195, 210, 225, 240}, 3},
      },
      {
        {{15, 51, 204, 240}, 3},
        {{85, 105, 150, 170}, 4},
        {{90, 102, 153, 165}, 5},
        {{23, 43, 77, 113, 142, 178, 212, 232}, 12},
        {{27, 39, 78, 114, 141, 177, 216, 228}, 13},
        {{86, 89, 101, 106, 149, 154, 166, 169}, 14},
      },
      {
        {{77, 78, 85, 86, 89, 101, 102, 106, 113, 114, 141, 142, 149, 153, 154, 166, 169, 170, 177, 178}, 2},
        {{60, 195}, 3},
        {{90, 165}, 12},
        {{105, 150}, 13},
      },
      {
        {{85, 170}, 4},
        {{102, 153}, 5},
        {{29, 30, 45, 46, 51, 89, 90, 105, 106, 149, 150, 165, 166, 204, 209, 210, 225, 226}, 8},
        {{86, 101, 154, 169}, 14},
      },
      {
        {{53, 54, 57, 58, 85, 86, 89, 101, 102, 106, 149, 153, 154, 166, 169, 170, 197, 198, 201, 202}, 2},
        {{15, 240}, 3},
        {{105, 150}, 12},
        {{90, 165}, 13},
      },
      {
        {{83, 85, 89, 92, 99, 101, 102, 106, 108, 147, 149, 153, 154, 156, 163, 166, 170, 172}, 2},
        {{15, 240}, 3},
        {{105, 150}, 12},
        {{86, 90, 165, 169}, 13},
      },
      {
        {{23, 27, 39, 43, 85, 86, 89, 101, 102, 106, 149, 153, 154, 166, 169, 170, 212, 216, 228, 232}, 2},
        {{60, 195}, 3},
        {{90, 165}, 12},
        {{105, 150}, 13},
      },
      {
        {{51, 204}, 3},
        {{85, 170}, 4},
        {{102, 153}, 5},
        {{71, 75, 86, 90, 101, 105, 116, 120, 135, 139, 150, 154, 165, 169, 180, 184}, 9},
        {{89, 106, 149, 166}, 14},
      },
      {
        {{85, 86, 89, 101, 102, 106, 149, 153, 154, 166, 169, 170}, 2},
        {{60, 195}, 3},
        {{90, 165}, 12},
        {{105, 150}, 13},
      },
      {
        {{51, 204}, 3},
        {{85, 170}, 4},
        {{102, 153}, 5},
        {{86, 89, 90, 101, 105, 106, 149, 150, 154, 165, 166, 169}, 14},
      },
      {
        {{85, 86, 89, 101, 102, 106, 149, 153, 154, 166, 169, 170}, 2},
        {{15, 240}, 3},
        {{105, 150}, 12},
        {{90, 165}, 13},
      },
    },
  },
};

static const std::unordered_map<types::boundary, std::vector<types::rules>> last_cell_rule_groups{
  {
    types::boundary::null,
    {
      {17, 20, 65, 68},
      {5, 20, 65, 80},
      {5, 17, 68, 80},
      {20, 65},
      {17, 68},
      {5, 80},
    },
  }
};

static types::long_whole_num
get_first_cell_rule(types::boundary boundary, types::short_whole_num &next_group_index)
{
  types::short_whole_num group_index{
    utils::number::get_random_num(0, first_cell_rule_groups.at(boundary).size() - 1)
  };

  const types::rules &selected_rules{first_cell_rule_groups.at(boundary).at(group_index).first};
  next_group_index = first_cell_rule_groups.at(boundary).at(group_index).second;

  return selected_rules.at(utils::number::get_random_num(0, selected_rules.size() - 1));
}

static types::long_whole_num
get_regular_cell_rule(
  types::boundary boundary,
  types::short_whole_num group_index,
  types::short_whole_num &next_group_index
)
{
  const std::vector<types::rules_group> &sub_groups{
    regular_cell_rule_groups.at(boundary).at(group_index)
  };

  types::short_whole_num sub_group_index{
    utils::number::get_random_num(0, sub_groups.size() - 1)
  };

  const types::rules &selected_rules{sub_groups.at(sub_group_index).first};
  next_group_index = sub_groups.at(sub_group_index).second;

  return selected_rules.at(utils::number::get_random_num(0, selected_rules.size() - 1));
}

static types::long_whole_num
get_last_cell_rule(types::boundary boundary, types::short_whole_num group_index)
{
  const types::rules &selected_rules{last_cell_rule_groups.at(boundary).at(group_index)};
  return selected_rules.at(utils::number::get_random_num(0, selected_rules.size() - 1));
}

models::binary_1d_ca
models::reversible_eca::get_random(types::short_whole_num size, types::boundary boundary)
{
  if (size < 3)
  {
    throw std::invalid_argument{"minimum size of ECA should be 3"};
  }

  if (size > models::binary_1d_ca::max_size)
  {
    throw std::invalid_argument{"unsupported cellular automata size"};
  }

  types::rules random_rules(size, 0);
  types::short_whole_num next_group_index{};

  random_rules.at(0) = get_first_cell_rule(boundary, next_group_index);

  for (types::short_whole_num i{1}; i < size - 1; i++)
  {
    random_rules.at(i) = get_regular_cell_rule(boundary, next_group_index, next_group_index);
  }

  if (boundary == types::boundary::periodic)
  {
    random_rules.at(size - 1) = get_regular_cell_rule(boundary, next_group_index, next_group_index);
  }
  else
  {
    random_rules.at(size - 1) = get_last_cell_rule(boundary, next_group_index);
  }

  return models::binary_1d_ca{size, 1, 1, boundary, random_rules};
}
