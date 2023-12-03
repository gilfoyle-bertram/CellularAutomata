#ifndef __BINARY_CELL__
#define __BINARY_CELL__

#include <string>

#include "../../types/types.hpp"

namespace models
{
  class binary_cell
  {
    private:
      bool state{};
      types::long_whole_num rule{};

    public:
      bool
      get_state() const;

      types::long_whole_num
      get_rule() const;

      void
      set_state(bool state);

      void
      set_rule(types::long_whole_num rule);

      bool
      next_state(const std::string &neighborhood) const;

      void
      update_state(const std::string &neighborhood);
  };
}

#endif
