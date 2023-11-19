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
      static types::boundary
      read_boundary();

      void
      read_ca_details();

      void
      access_system();
  };
}

#endif
