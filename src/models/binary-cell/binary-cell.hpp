#ifndef __BINARY_CELL__
#define __BINARY_CELL__

#include <string>

namespace models
{
  class BinaryCell
  {
    private:
      bool state{};
      unsigned short rule{};

    public:
      bool
      getState() const;

      void
      setState(bool state);

      unsigned short
      getRule() const;

      void
      setRule(unsigned short rule);

      bool
      applyRule(const std::string &neighborhood) const;

      void
      updateState(const std::string &neighborhood);
  };
}

#endif
