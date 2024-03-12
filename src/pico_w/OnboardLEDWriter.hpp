#include "IWriter.hpp"
namespace PicoW {

class OnboardLEDWriter : public IWriter {
public:
  void write(bool value) override;
};
} // namespace PicoW
