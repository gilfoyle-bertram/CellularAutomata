#ifndef __UTILS__
#define __UTILS__

#include <string>
#include <vector>

#include "../types/types.hpp"

namespace utils::general
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
  initialize();
}

namespace utils::math
{
  bool
  isOdd(short num);

  bool
  isEven(short num);
}

namespace utils::matrix
{
  bool
  isSquare(const types::Matrix &A);

  bool
  canAdd(const types::Matrix &A, const types::Matrix &B);

  bool
  canMultiply(const types::Matrix &A, const types::Matrix &B);

  short
  getTrace(const types::Matrix &A);

  types::Matrix
  getIdentity(unsigned short size);

  types::Matrix
  add(const types::Matrix &A, const types::Matrix &B);

  types::Matrix
  multiply(const types::Matrix &A, const types::Matrix &B);

  types::Matrix
  scalarMultiply(const types::Matrix &A, short x);

  types::Polynomial
  getCharacteristicPolynomial(const types::Matrix &A);

  void
  print(const types::Matrix &A);
}

namespace utils::polynomial
{
  void
  print(const types::Polynomial &P);
}

#endif
