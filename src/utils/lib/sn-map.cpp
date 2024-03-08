#include "../utils.hpp"

bool
utils::sn_map::is_1_1_or_1_n(const types::sn_map &sn_map)
{
  for (const auto &current_pair : sn_map)
  {
    for (const auto &other_pair : sn_map)
    {
      if (current_pair.first == other_pair.first)
      {
        continue;
      }

      for (const auto &value : current_pair.second)
      {
        if (other_pair.second.count(value))
        {
          return false;
        }
      }
    }
  }

  return true;
}
