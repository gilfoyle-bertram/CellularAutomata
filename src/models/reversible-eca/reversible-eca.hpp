#ifndef __REVERSIBLE_ECA__
#define __REVERSIBLE_ECA__

#include "../binary-1d-ca/binary-1d-ca.hpp"

namespace models
{
  class reversible_eca : public models::binary_1d_ca
  {
    public:
      reversible_eca() = delete;
      reversible_eca(const reversible_eca &other) = delete;

      static models::binary_1d_ca
      get_random(types::short_whole_num size, types::boundary boundary);
  };
}

#endif
