#include "../utils.hpp"

#include <iostream>
#include <stdexcept>

#include "../../types/types.hpp"

bool
utils::matrix::isSquare(const types::Matrix &A)
{
  unsigned short numRows{static_cast<unsigned short>(A.size())};
  unsigned short numCols{static_cast<unsigned short>(A.at(0).size())};
  return numRows == numCols;
}

bool
utils::matrix::canAdd(const types::Matrix &A, const types::Matrix &B)
{
  unsigned short numRowsInA{static_cast<unsigned short>(A.size())};
  unsigned short numRowsInB{static_cast<unsigned short>(B.size())};
  unsigned short numColsInA{static_cast<unsigned short>(A.at(0).size())};
  unsigned short numColsInB{static_cast<unsigned short>(B.at(0).size())};
  return numRowsInA == numRowsInB && numColsInA == numColsInB;
}

bool
utils::matrix::canMultiply(const types::Matrix &A, const types::Matrix &B)
{
  unsigned short numColsInA{static_cast<unsigned short>(A.at(0).size())};
  unsigned short numRowsInB{static_cast<unsigned short>(B.size())};
  return numColsInA == numRowsInB;
}

short
utils::matrix::getTrace(const types::Matrix &A)
{
  short result{};

  if (!utils::matrix::isSquare(A))
  {
    throw std::domain_error{"Cannot compute trace of a non-square matrix"};
  }

  unsigned short size{static_cast<unsigned short>(A.size())};

  for (unsigned short i{}; i < size; i++)
  {
    result += A.at(i).at(i);
  }

  return result;
}

types::Matrix
utils::matrix::getIdentity(unsigned short n)
{
  types::Matrix I{};
  I.resize(n);

  for (unsigned short i{}; i < n; i++)
  {
    I.at(i).resize(n);
    I.at(i).at(i) = 1;
  }

  return I;
}

types::Matrix
utils::matrix::add(const types::Matrix &A, const types::Matrix &B)
{
  if (!utils::matrix::canAdd(A, B))
  {
    throw std::domain_error{"Matrices incompatible for addition"};
  }

  unsigned short numRows{static_cast<unsigned short>(A.size())};
  unsigned short numCols{static_cast<unsigned short>(A.at(0).size())};

  types::Matrix C{};
  C.resize(numRows);

  for (unsigned short i{}; i < numRows; i++)
  {
    C.at(i).resize(numCols);

    for (unsigned short j{}; j < numCols; j++)
    {
      C.at(i).at(j) = A.at(i).at(j) + B.at(i).at(j);
    }
  }

  return C;
}

types::Matrix
utils::matrix::multiply(const types::Matrix &A, const types::Matrix &B)
{
  if (!utils::matrix::canMultiply(A, B))
  {
    throw std::domain_error{"Matrices incompatible for multiplication"};
  }

  unsigned short numRowsInA{static_cast<unsigned short>(A.size())};
  unsigned short numColsInA{static_cast<unsigned short>(A.at(0).size())};
  unsigned short numColsInB{static_cast<unsigned short>(B.at(0).size())};

  types::Matrix C{};
  C.resize(numRowsInA);

  for (unsigned short i{}; i < numRowsInA; i++)
  {
    C.at(i).resize(numColsInB);

    for (unsigned short j{}; j < numColsInB; j++)
    {
      for (unsigned short k{}; k < numColsInA; k++)
      {
        C.at(i).at(j) += A.at(i).at(k) * B.at(k).at(j);
      }
    }
  }

  return C;
}

types::Matrix
utils::matrix::scalarMultiply(const types::Matrix &A, short x)
{
  unsigned short numRows{static_cast<unsigned short>(A.size())};
  unsigned short numCols{static_cast<unsigned short>(A.at(0).size())};

  types::Matrix xA{};
  xA.resize(numRows);

  for (unsigned short i{}; i < numRows; i++)
  {
    xA.at(i).resize(numCols);

    for (unsigned short j{}; j < numCols; j++)
    {
      xA.at(i).at(j) = x * A.at(i).at(j);
    }
  }

  return xA;
}

// This function uses Souriau method to compute the characteristic polynomial of a matrix.
// Refer - https://math.stackexchange.com/questions/405822/what-is-the-fastest-way-to-find-the-characteristic-polynomial-of-a-matrix.
types::Polynomial
utils::matrix::getCharacteristicPolynomial(const types::Matrix &A)
{
  if (!utils::matrix::isSquare(A))
  {
    throw std::domain_error{"Cannot compute characteristic polynoial of a non-square matrix"};
  }

  unsigned short n{static_cast<unsigned short>(A.size())};
  types::Polynomial P{};
  types::Matrix C{A};
  types::Matrix I{utils::matrix::getIdentity(n)};

  P.resize(n + 1);
  P.at(n) = 1;
  P.at(n - 1) = -getTrace(C);

  for (unsigned short i{2}; i <= n; i++)
  {
    types::Matrix T1{utils::matrix::scalarMultiply(I, P.at(n - i + 1))};
    types::Matrix T2{utils::matrix::add(C, T1)};

    C = utils::matrix::multiply(A, T2);
    P.at(n - i) = -getTrace(C) / i;
  }

  return P;
}

void
utils::matrix::print(const types::Matrix &A)
{
  unsigned short numRows{static_cast<unsigned short>(A.size())};
  unsigned short numCols{static_cast<unsigned short>(A.at(0).size())};

  std::cout << "\n";

  for (unsigned short i{}; i < numRows; i++)
  {
    for (unsigned short j{}; j < numCols; j++)
    {
      std::cout << A.at(i).at(j) << " ";
    }

    std::cout << "\n";
  }
}
