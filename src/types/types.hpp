#ifndef __TYPES__
#define __TYPES__

#include <vector>

namespace types
{
  enum BoundaryCondition
  {
    Null,
    Periodic
  };

  using Matrix = std::vector<std::vector<short>>;
  using Polynomial = std::vector<short>;
  using TransitionGraph = std::vector<unsigned short>;
  using Rules = std::vector<unsigned short>;
}

#endif
