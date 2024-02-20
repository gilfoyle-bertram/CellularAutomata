#ifndef __BINARY_1D_CA_MANAGER__
#define __BINARY_1D_CA_MANAGER__

#include "../../types/types.hpp"
#include "../binary-1d-ca/binary-1d-ca.hpp"

namespace models
{
  class binary_1d_ca_manager
  {
    private:
      models::binary_1d_ca current_ca{};

    public:
      static types::short_whole_num
      read_num_cells();

      static types::boundary
      read_boundary();

      static types::rules
      read_rules(types::short_whole_num num_rules);

      static void
      print_reversed_isomorphable_ecas();

      static void
      print_single_cycle_reversible_ecas();

      void
      read_ca_details();

      void
      access_system();
  };
}

#endif
