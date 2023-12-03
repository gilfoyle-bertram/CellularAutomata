#include "../utils.hpp"

#include <sstream>
#include <stdexcept>

#include "../../types/types.hpp"

bool
utils::matrix::is_square(const types::matrix &A)
{
  types::short_whole_num rows{static_cast<types::short_whole_num>(A.size())};
  types::short_whole_num cols{static_cast<types::short_whole_num>(A.at(0).size())};

  return rows == cols;
}

bool
utils::matrix::can_add(const types::matrix &A, const types::matrix &B)
{
  types::short_whole_num a_rows{static_cast<types::short_whole_num>(A.size())};
  types::short_whole_num b_rows{static_cast<types::short_whole_num>(B.size())};

  types::short_whole_num a_cols{static_cast<types::short_whole_num>(A.at(0).size())};
  types::short_whole_num b_cols{static_cast<types::short_whole_num>(B.at(0).size())};

  return a_rows == b_rows && a_cols == b_cols;
}

bool
utils::matrix::can_multiply(const types::matrix &A, const types::matrix &B)
{
  types::short_whole_num a_cols{static_cast<types::short_whole_num>(A.at(0).size())};
  types::short_whole_num b_rows{static_cast<types::short_whole_num>(B.size())};

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

  types::short_whole_num size{static_cast<types::short_whole_num>(A.size())};

  for (types::short_whole_num i{}; i < size; i++)
  {
    result += A.at(i).at(i);
  }

  return result;
}

types::matrix
utils::matrix::get_identity(types::short_whole_num size)
{
  types::matrix I{};
  I.resize(size);

  for (types::short_whole_num i{}; i < size; i++)
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

  types::short_whole_num rows{static_cast<types::short_whole_num>(A.size())};
  types::short_whole_num cols{static_cast<types::short_whole_num>(A.at(0).size())};

  types::matrix C{};
  C.resize(rows);

  for (types::short_whole_num i{}; i < rows; i++)
  {
    C.at(i).resize(cols);

    for (types::short_whole_num j{}; j < cols; j++)
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

  types::short_whole_num a_rows{static_cast<types::short_whole_num>(A.size())};
  types::short_whole_num a_cols{static_cast<types::short_whole_num>(A.at(0).size())};
  types::short_whole_num b_cols{static_cast<types::short_whole_num>(B.at(0).size())};

  types::matrix C{};
  C.resize(a_rows);

  for (types::short_whole_num i{}; i < a_rows; i++)
  {
    C.at(i).resize(b_cols);

    for (types::short_whole_num j{}; j < b_cols; j++)
    {
      for (types::short_whole_num k{}; k < a_cols; k++)
      {
        C.at(i).at(j) += A.at(i).at(k) * B.at(k).at(j);
      }
    }
  }

  return C;
}

types::matrix
utils::matrix::scalar_multiply(const types::matrix &A, types::short_num x)
{
  types::short_whole_num rows{static_cast<types::short_whole_num>(A.size())};
  types::short_whole_num cols{static_cast<types::short_whole_num>(A.at(0).size())};

  types::matrix x_A{};
  x_A.resize(rows);

  for (types::short_whole_num i{}; i < rows; i++)
  {
    x_A.at(i).resize(cols);

    for (types::short_whole_num j{}; j < cols; j++)
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

  types::short_whole_num size{static_cast<types::short_whole_num>(A.size())};
  types::polynomial coeffs{};

  types::matrix C{A};
  types::matrix I{utils::matrix::get_identity(size)};

  coeffs.resize(size + 1);
  coeffs.at(size) = 1;
  coeffs.at(size - 1) = -get_trace(C);

  for (types::short_whole_num i{2}; i <= size; i++)
  {
    types::matrix T1{utils::matrix::scalar_multiply(I, coeffs.at(size - i + 1))};
    types::matrix T2{utils::matrix::add(C, T1)};

    C = utils::matrix::multiply(A, T2);

    // It is important to make note of data-types here.
    // The value of `-get_trace(C)` is of type `types::num (signed int)`.
    // The value of `i` is of type `types::short_whole_num (unsigned short)`.
    // So to perform the division operation, `unsigned short` gets cast into `signed int`.
    // This is the correct way of operation, so as to get the expected results of Souriau method.
    //
    // If `types::whole_num (unsigned int)` was used for `i`, then strange results occur.
    // This is because, to perform the division operation, `-get_trace(C)` will be cast from `signed int` to `unsigned int`.
    // This results in wrong interpretation of the value of `-get_trace(C)`.
    coeffs.at(size - i) = -get_trace(C) / i;
  }

  return coeffs;
}

std::string
utils::matrix::to_string(const types::matrix &A)
{
  std::ostringstream out_stream{};

  types::short_whole_num rows{static_cast<types::short_whole_num>(A.size())};
  types::short_whole_num cols{static_cast<types::short_whole_num>(A.at(0).size())};

  for (types::short_whole_num i{}; i < rows; i++)
  {
    for (types::short_whole_num j{}; j < cols; j++)
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
