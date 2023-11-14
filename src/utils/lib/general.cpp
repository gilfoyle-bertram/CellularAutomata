#include "../utils.hpp"

#include <bitset>
#include <ctime>
#include <sstream>

bool
utils::general::getRandomBool()
{
  return rand() % 2 == 0;
}

unsigned short
utils::general::parseBinaryStr(const std::string &text)
{
  return std::stoi(text, nullptr, 2);
}

std::string
utils::general::toBinaryStr(unsigned short n, unsigned short numDigits)
{
  return std::bitset<16>{n}.to_string().substr(16 - numDigits);
}

std::string
utils::general::toString(const std::vector<unsigned short> &nums)
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
utils::general::initialize()
{
  std::srand(std::time(NULL));
}
