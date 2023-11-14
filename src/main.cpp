// కర్మణ్యే వాధికారస్తే మా ఫలేషు కదాచన |
// మా కర్మ ఫలహేతుర్భూర్మాతే సంగోస్త్వ కర్మణీ ||

// మయి సర్వాణి కర్మాణి సంన్యస్యాధ్యాత్మచేతసా |
// నిరాశీర్నిర్మమో భూత్వా యుధ్యస్వ విగతజ్వరః ||

#include <iostream>

#include "models/binary-1d-ca/binary-1d-ca.hpp"
#include "types/types.hpp"
#include "utils/utils.hpp"

class Controller
{
  private:
    models::Binary1DCA currentCA{};

    int
    getChoice(const std::vector<std::string> &choices) const
    {
      std::cout
        << "\nMenu:"
        << "\n";

      for (unsigned short i{}; i < choices.size(); i++)
      {
        std::cout
          << "  " << (i + 1) << ") " << choices.at(i)
          << "\n";
      }

      int choice{};
      std::cout << "\nEnter your choice: ";
      std::cin >> choice;

      if (choice <= 0 || choice > choices.size())
      {
        std::cout
          << "\nERR: Invalid Choice"
          << "\n     Please Try Again";

        return getChoice(choices);
      }

      return choice;
    }

    void
    run()
    {
      bool runSystem{true};

      std::vector<std::string> choices{
        "Show transition graph",
        "Show CA details",
        "Show isomorphic permutations",
        "Check isomorphism with another CA",
        "Show complemented isomorphisms",
        "Show characteristic matrix",
        "Show characteristic polynomial",
        "Exit"
      };

      while (runSystem)
      {
        int choice{this->getChoice(choices)};

        switch (choice)
        {
          case 1:
            this->currentCA.printTransitionGraph();
            break;

          case 2:
            this->currentCA.printDetails();
            break;

          case 3:
            this->currentCA.printIsomorphicPermutations();
            break;

          case 4:
            this->currentCA.checkIsomorphismWithOtherCA();
            break;

          case 5:
            this->currentCA.printComplementedIsomorphisms();
            break;

          case 6:
            this->currentCA.printCharacterisitcMatrix();
            break;

          case 7:
            this->currentCA.printCharacterisitcPolynomial();
            break;

          default:
            runSystem = false;
            std::cout << "\n";
        }
      }
    }

  public:
    Controller()
    {
      utils::general::initialize();
    }

    void
    initialize()
    {
      unsigned short numCells{};
      unsigned short leftRadius{};
      unsigned short rightRadius{};
      unsigned short rule{};
      char symbolForBC{};
      types::BoundaryCondition BC{};
      types::Rules rules{};

      std::cout << "\nNo. of cells (Max: " << (models::Binary1DCA::MAX_SIZE) << " cells): ";
      std::cin >> numCells;
      rules.reserve(numCells);

      std::cout << "Left radius: ";
      std::cin >> leftRadius;

      std::cout << "Right radius: ";
      std::cin >> rightRadius;

      std::cout << "Boundary condition [(n)ull / (p)eriodic]: ";
      std::cin >> symbolForBC;

      BC = symbolForBC == 'n' ? types::BoundaryCondition::Null : types::BoundaryCondition::Periodic;

      std::cout << "\n";

      for (unsigned short i{}; i < numCells; ++i)
      {
        std::cout << "Rule for cell-" << (i + 1) << ": ";
        std::cin >> rule;
        rules.push_back(rule);
      }

      this->currentCA = models::Binary1DCA{numCells, leftRadius, rightRadius, BC, rules};
      this->run();
    }
};

int
main()
{
  Controller{}.initialize();
  return 0;
}
