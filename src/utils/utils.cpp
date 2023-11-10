#include "utils.hpp"

#include <bitset>
#include <cstdlib>
#include <ctime>
#include <sstream>

bool
utils::getRandomBool()
{
  return rand() % 2 == 0;
}

unsigned short
utils::parseBinaryStr(const std::string &text)
{
  return std::stoi(text, nullptr, 2);
}

std::string
utils::toBinaryStr(unsigned short n, unsigned short numDigits)
{
  return std::bitset<16>{n}.to_string().substr(16 - numDigits);
}

std::string
utils::toString(const std::vector<unsigned short> &nums)
{
  std::ostringstream outStream{};
  outStream << "[ ";

  for (const auto &num : nums)
  {
    outStream << num << " ";
  }

  outStream << "]";
  return outStream.str();
}

void
utils::initializeUtils()
{
  srand(time(NULL));
}
