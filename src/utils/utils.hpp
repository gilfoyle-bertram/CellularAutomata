#ifndef __UTILS__
#define __UTILS__

#include <string>
#include <vector>

namespace utils
{
  bool
  getRandomBool();

  unsigned short
  parseBinaryStr(const std::string &text);

  std::string
  toBinaryStr(unsigned short n, unsigned short numDigits);

  std::string
  toString(const std::vector<unsigned short> &nums);

  void
  initializeUtils();
}

#endif
