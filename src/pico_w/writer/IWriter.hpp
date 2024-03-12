#pragma once

namespace PicoW {
class IWriter {
public:
  virtual void write(bool value) = 0;
};
} // namespace PicoW
