#include "IWriter.hpp"

namespace PicoW {
enum class Pin {
  GP0 = 0,
  GP1 = 1,
  GP2 = 2,
  GP3 = 3,
  GP4 = 4,
  GP5 = 5,
  GP6 = 6,
  GP7 = 7,
  GP8 = 8,
  GP9 = 9,
  GP10 = 10,
  GP11 = 11,
  GP12 = 12,
  GP13 = 13,
  GP14 = 14,
  GP15 = 15,
  GP16 = 16,
  GP17 = 17,
  GP18 = 18,
  GP19 = 19,
  GP20 = 20,
  GP21 = 21,
  GP22 = 22,
  GP23 = 23,
  GP24 = 24,
  GP25 = 25,
  GP26 = 26,
  GP27 = 27,
  GP28 = 28
};

class GPIOWriter : public IWriter {
public:
  GPIOWriter(Pin pin);
  void write(bool value) override;

private:
  int pin_id;
};
} // namespace PicoW
