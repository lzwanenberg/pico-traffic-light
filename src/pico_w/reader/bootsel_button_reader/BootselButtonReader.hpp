#pragma once
#include "../IReader.hpp"

namespace PicoW {
class BootselButtonReader : public IReader {
public:
  bool read() override;
};
} // namespace PicoW
