#pragma once
#include "Nibble.h"

namespace cpu {
using Register = uint4;

// regID refers to the index of a register when stored in an array of registers.
namespace regID {
constexpr size_t A{0};
constexpr size_t B{1};
} // namespace regID

inline constexpr uint8_t WordSizeBits = 4;
inline constexpr uint8_t NumRegisters = 2;
inline constexpr uint8_t MemSizeWords = 16;

enum class OpCode : uint8_t {
  Halt = 0x0,
  LoadA = 0x1,
  LoadAI = 0x2,
  LoadB = 0x3,
  StoreA = 0x4,
  Mov = 0x5,
  Add = 0x6,
  Sub = 0x7,
  Jump = 0x8,
  JumpZ = 0x9,
  JumpNZ = 0xA
};

} // namespace cpu
