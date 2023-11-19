#include "../utils.hpp"

#include <sstream>
#include <stdexcept>

#include "../../types/types.hpp"

bool
utils::matrix::is_square(const types::matrix &A)
{
  types::whole_num rows{static_cast<types::whole_num>(A.size())};
  types::whole_num cols{static_cast<types::whole_num>(A.at(0).size())};

  return rows == cols;
}

bool
utils::matrix::can_add(const types::matrix &A, const types::matrix &B)
{
  types::whole_num a_rows{static_cast<types::whole_num>(A.size())};
  types::whole_num b_rows{static_cast<types::whole_num>(B.size())};

  types::whole_num a_cols{static_cast<types::whole_num>(A.at(0).size())};
  types::whole_num b_cols{static_cast<types::whole_num>(B.at(0).size())};

  return a_rows == b_rows && a_cols == b_cols;
}

bool
utils::matrix::can_multiply(const types::matrix &A, const types::matrix &B)
{
  types::whole_num a_cols{static_cast<types::whole_num>(A.at(0).size())};
  types::whole_num b_rows{static_cast<types::whole_num>(B.size())};

  return a_cols == b_rows;
}

types::num
utils::matrix::get_trace(const types::matrix &A)
{
  types::num result{};

  if (!utils::matrix::is_square(A))
  {
    throw std::domain_error{"cannot compute trace of a non-square matrix"};
  }

  types::whole_num size{static_cast<types::whole_num>(A.size())};

  for (types::whole_num i{}; i < size; i++)
  {
    result += A.at(i).at(i);
  }

  return result;
}

types::matrix
utils::matrix::get_identity(types::whole_num size)
{
  types::matrix I{};
  I.resize(size);

  for (types::whole_num i{}; i < size; i++)
  {
    I.at(i).resize(size);
    I.at(i).at(i) = 1;
  }

  return I;
}

types::matrix
utils::matrix::add(const types::matrix &A, const types::matrix &B)
{
  if (!utils::matrix::can_add(A, B))
  {
    throw std::domain_error{"matrices incompatible for addition"};
  }

  types::whole_num rows{static_cast<types::whole_num>(A.size())};
  types::whole_num cols{static_cast<types::whole_num>(A.at(0).size())};

  types::matrix C{};
  C.resize(rows);

  for (types::whole_num i{}; i < rows; i++)
  {
    C.at(i).resize(cols);

    for (types::whole_num j{}; j < cols; j++)
    {
      C.at(i).at(j) = A.at(i).at(j) + B.at(i).at(j);
    }
  }

  return C;
}

types::matrix
utils::matrix::multiply(const types::matrix &A, const types::matrix &B)
{
  if (!utils::matrix::can_multiply(A, B))
  {
    throw std::domain_error{"matrices incompatible for multiplication"};
  }

  types::whole_num a_rows{static_cast<types::whole_num>(A.size())};
  types::whole_num a_cols{static_cast<types::whole_num>(A.at(0).size())};
  types::whole_num b_cols{static_cast<types::whole_num>(B.at(0).size())};

  types::matrix C{};
  C.resize(a_rows);

  for (types::whole_num i{}; i < a_rows; i++)
  {
    C.at(i).resize(b_cols);

    for (types::whole_num j{}; j < b_cols; j++)
    {
      for (types::whole_num k{}; k < a_cols; k++)
      {
        C.at(i).at(j) += A.at(i).at(k) * B.at(k).at(j);
      }
    }
  }

  return C;
}

types::matrix
utils::matrix::scalar_multiply(const types::matrix &A, types::num x)
{
  types::whole_num rows{static_cast<types::whole_num>(A.size())};
  types::whole_num cols{static_cast<types::whole_num>(A.at(0).size())};

  types::matrix x_A{};
  x_A.resize(rows);

  for (types::whole_num i{}; i < rows; i++)
  {
    x_A.at(i).resize(cols);

    for (types::whole_num j{}; j < cols; j++)
    {
      x_A.at(i).at(j) = x * A.at(i).at(j);
    }
  }

  return x_A;
}

// This function uses Souriau method to compute the characteristic polynomial of a matrix.
// Refer, https://math.stackexchange.com/questions/405822/what-is-the-fastest-way-to-find-the-characteristic-polynomial-of-a-matrix.
types::polynomial
utils::matrix::get_characteristic_polynomial(const types::matrix &A)
{
  if (!utils::matrix::is_square(A))
  {
    throw std::domain_error{"cannot compute characteristic polynoial of a non-square matrix"};
  }

  types::whole_num size{static_cast<types::whole_num>(A.size())};
  types::polynomial coeffs{};

  types::matrix C{A};
  types::matrix I{utils::matrix::get_identity(size)};

  coeffs.resize(size + 1);
  coeffs.at(size) = 1;
  coeffs.at(size - 1) = -get_trace(C);

  for (types::whole_num i{2}; i <= size; i++)
  {
    types::matrix T1{utils::matrix::scalar_multiply(I, coeffs.at(size - i + 1))};
    types::matrix T2{utils::matrix::add(C, T1)};

    C = utils::matrix::multiply(A, T2);
    coeffs.at(size - i) = -get_trace(C) / i;
  }

  return coeffs;
}

std::string
utils::matrix::to_string(const types::matrix &A)
{
  std::ostringstream out_stream{};

  types::whole_num rows{static_cast<types::whole_num>(A.size())};
  types::whole_num cols{static_cast<types::whole_num>(A.at(0).size())};

  for (types::whole_num i{}; i < rows; i++)
  {
    for (types::whole_num j{}; j < cols; j++)
    {
      out_stream << A.at(i).at(j) << " ";
    }

    out_stream << (i == rows - 1 ? "" : "\n");
  }

  return out_stream.str();
}

void
utils::matrix::print(const types::matrix &A, const std::string &color)
{
  utils::general::print_msg(utils::matrix::to_string(A), color);
}
