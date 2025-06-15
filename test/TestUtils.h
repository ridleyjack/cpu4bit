#pragma once

#include "CPU.h"
#include "CPUDefs.h"
#include "Memory.h"

#include <utility>

namespace cpu::test {
struct WithCPU {
  CPU* cpu{};
  Memory& mem;
  WithCPU() : cpu{new CPU()}, mem{cpu->GetMemory()} {};
  inline ~WithCPU() {
    delete cpu;
  }

  CPU* operator->() {
    return cpu;
  }
  const CPU* operator->() const {
    return cpu;
  }
};

// Helper functions to make instructions.

// Encodes OpCode + 1 Arg
inline void StoreOp(Memory& m, const cpu::OpCode op, const int address) {
  m.Store(cpu::uint4(std::to_underlying(op)), cpu::uint4(address));
}

// Stores raw data into memory
inline void StoreArg(Memory& m, const int val, const int address) {
  m.Store(cpu::uint4(val), cpu::uint4(address));
}

// Encodes 2-register args: 2 bits each = 4-bit arg
inline void Store2Args(Memory& m, const int arg0, const int arg1,
                       const int address) {
  const cpu::uint4 v = (cpu::uint4(arg0) << 2) | cpu::uint4(arg1);
  m.Store(v, cpu::uint4(address));
}

inline void StoreVal(Memory& m, const int val, const int address) {
  StoreArg(m, val, address);
}

inline int ReadVal(const Memory& m, const int address) {
  const cpu::uint4 a{address};
  return static_cast<int8_t>(cpu::int4(m.Load(a)));
}
} // namespace cpu::test
