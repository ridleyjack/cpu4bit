#include "Test.h"
#include "TestUtils.h"

#include <cassert>

namespace cpu::test {
void addNumbers() {
  WithCPU cpu{};

  // A = 2
  StoreOp(cpu.mem, OpCode::LoadAI, 0);
  StoreArg(cpu.mem, 2, 1);
  // B = A
  StoreOp(cpu.mem, OpCode::Mov, 2);
  Store2Args(cpu.mem, 0x0, 0x1, 3);
  // A = 5
  StoreOp(cpu.mem, OpCode::LoadAI, 4);
  StoreArg(cpu.mem, 5, 5);
  // Store A: mem[0x9] = A
  StoreOp(cpu.mem, OpCode::StoreA, 6);
  StoreArg(cpu.mem, 0xD, 7);
  // Add: A = A + B == 5 + 2 == 7
  StoreOp(cpu.mem, OpCode::Add, 8);
  Store2Args(cpu.mem, 0x0, 0x1, 9);
  // Store A: mem[0x9] = A
  StoreOp(cpu.mem, OpCode::StoreA, 0xA);
  StoreArg(cpu.mem, 0xE, 0xB);
  // Halt program
  StoreOp(cpu.mem, OpCode::Halt, 0xC);

  cpu->Run();
  assert(cpu.mem.Load(cpu::uint4(0xD)) == 5);
  assert(cpu.mem.Load(cpu::uint4(0xE)) == 7);
}

} // namespace cpu::test

void RunAllSamples() {
  cpu::test::addNumbers();
}
