#include "../utils.hpp"

#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>

static std::string
get_border(const std::vector<std::pair<std::string, types::short_whole_num>> &entries)
{
  std::ostringstream out_stream{};

  for (const auto &entry : entries)
  {
    out_stream << "+-";
    out_stream << std::string(entry.second, '-');
    out_stream << "-";
  }

  out_stream << "+";
  return out_stream.str();
}

static std::string
get_content(const std::vector<std::pair<std::string, types::short_whole_num>> &entries)
{
  std::ostringstream out_stream{};

  for (const auto &entry : entries)
  {
    out_stream << "| ";
    out_stream << std::setw(entry.second) << entry.first;
    out_stream << " ";
  }

  out_stream << "|";
  return out_stream.str();
}

static types::short_whole_num
read_choice(types::short_whole_num low, types::short_whole_num high)
{
  types::short_whole_num value{};
  std::cout << "\n";

  std::cout << "Enter number in range " << low << " - " << high << " : ";
  std::cin >> value;

  if (low <= value && value <= high)
  {
    return value;
  }

  utils::general::print_msg("ERR: Invalid number\n     Try again", colors::red);
  return read_choice(low, high);
}

bool
utils::general::get_random_bool()
{
  return rand() % 2 == 0;
}

types::short_whole_num
utils::general::get_choice(const std::vector<std::string> &choices)
{
  std::cout << decorate::underline;
  std::cout << colors::pink;
  std::cout << "\n";
  std::cout << "Menu:";
  std::cout << "\n";
  std::cout << colors::reset;
  std::cout << decorate::reset;

  for (types::short_whole_num i{}; i < choices.size(); i++)
  {
    std::cout << "  " << (i + 1) << ") " << choices.at(i) << "\n";
  }

  return read_choice(1, choices.size());
}

void
utils::general::initialize()
{
  std::srand(std::time(NULL));
}

void
utils::general::print_header(
  const std::vector<std::pair<std::string, types::short_whole_num>> &headings,
  const std::string &color
)
{
  std::string border{get_border(headings)};
  std::string content{get_content(headings)};

  std::cout << color;
  std::cout << "\n";
  std::cout << border;
  std::cout << "\n";
  std::cout << content;
  std::cout << "\n";
  std::cout << border;
  std::cout << "\n";
  std::cout << colors::reset;
}

void
utils::general::print_row(
  const std::vector<std::pair<std::string, types::short_whole_num>> &entries,
  const std::string &color
)
{
  std::string border{get_border(entries)};
  std::string content{get_content(entries)};

  std::cout << color;
  std::cout << content;
  std::cout << "\n";
  std::cout << border;
  std::cout << "\n";
  std::cout << colors::reset;
}

void
utils::general::print_msg(const std::string &msg, const std::string &color, bool new_line)
{
  std::cout << color;

  if (new_line)
  {
    std::cout << "\n";
  }

  std::cout << msg;
  std::cout << '\n';
  std::cout << colors::reset;
}
