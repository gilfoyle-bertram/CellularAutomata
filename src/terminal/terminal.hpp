#ifndef __TERMINAL__
#define __TERMINAL__

#include <string>

namespace colors
{
  static const std::string red{"\033[1;31m"};
  static const std::string green{"\033[1;32m"};
  static const std::string yellow{"\033[1;33m"};
  static const std::string blue{"\033[1;34m"};
  static const std::string magenta{"\033[1;35m"};
  static const std::string cyan{"\033[1;36m"};
  static const std::string reset{"\033[1;0m"};
}

namespace decorate
{
  static const std::string underline{"\033[4m"};
  static const std::string reset{"\033[1;0m"};
}

#endif
