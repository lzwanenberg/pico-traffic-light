#pragma once

namespace PicoW {
class IReader {
public:
  virtual bool read() = 0;
};
} // namespace PicoW
