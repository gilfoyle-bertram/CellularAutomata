#ifndef __TERMINAL__
#define __TERMINAL__

#include <string>

namespace colors
{
  static const std::string red{"\033[38:5:203m"};
  static const std::string green{"\033[38:5:120m"};
  static const std::string yellow{"\033[38:5:226m"};
  static const std::string blue{"\033[38:5:75m"};
  static const std::string magenta{"\033[38:5:200m"};
  static const std::string cyan{"\033[38:5:50m"};
  static const std::string pink{"\033[38:5:213m"};
  static const std::string salmon{"\033[38:5:173m"};
  static const std::string reset{"\033[0m"};
}

namespace decorate
{
  static const std::string underline{"\033[4m"};
  static const std::string reset{"\033[0m"};
}

#endif
