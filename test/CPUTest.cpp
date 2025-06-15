#include "CPU.h"
#include "CPUDefs.h"
#include "Memory.h"

#include "TestUtils.h"

#include <cassert>
#include <iostream>
#include <utility>

namespace cpu::test {
// Test various functionality.

void testLoadI() {
  auto* cpu = new cpu::CPU();
  Memory& mem{cpu->GetMemory()};
  int addr{0};

  // A = 4;
  StoreOp(mem, OpCode::LoadAI, addr++);
  StoreArg(mem, 5, addr++);
  // B = -1;
  StoreOp(mem, OpCode::LoadAI, addr++);
  StoreArg(mem, -1, addr);

  cpu->Cycle();
  assert(cpu->GetRegisterA() == 5);
  cpu->Cycle();
  assert(static_cast<int4>(cpu->GetRegisterA()) == int4{-1});
}

void testStore() {
  auto* cpu = new cpu::CPU();
  Memory& mem{cpu->GetMemory()};
  int addr{0};

  StoreVal(mem, 4, 14);

  // A = 4;
  StoreOp(mem, OpCode::LoadA, addr++);
  StoreArg(mem, 14, addr++);
  // mem[15] = A;
  StoreOp(mem, OpCode::StoreA, addr++);
  StoreArg(mem, 15, addr);

  assert(ReadVal(mem, 15) == 0);
  cpu->Run();
  assert(ReadVal(mem, 15) == 4);

  delete cpu;
}

void testMove() {
  auto* cpu = new cpu::CPU();
  Memory& mem{cpu->GetMemory()};
  int addr{0};

  StoreVal(mem, 15, 15);

  // B = 15;
  StoreOp(mem, OpCode::LoadB, addr++);
  StoreArg(mem, 15, addr++);
  // A = B;
  StoreOp(mem, OpCode::Mov, addr++);
  Store2Args(mem, 1, 0, addr);

  assert(cpu->GetRegisterA() == 0);
  cpu->Run();
  assert(cpu->GetRegisterA() == 15);
  assert(cpu->GetRegisterB() == 15);

  delete cpu;
}

void testMove2() {
  auto* cpu = new cpu::CPU();
  Memory& mem{cpu->GetMemory()};
  int addr{0};

  StoreVal(mem, 15, 15);

  // A = 15;
  StoreOp(mem, OpCode::LoadA, addr++);
  StoreArg(mem, 15, addr++);
  // B = A;
  StoreOp(mem, OpCode::Mov, addr++);
  Store2Args(mem, 0, 1, addr);

  assert(cpu->GetRegisterB() == 0);
  cpu->Run();
  assert(cpu->GetRegisterB() == 15);
  delete cpu;
}

void testAdd() {
  auto* cpu = new cpu::CPU();
  Memory& mem{cpu->GetMemory()};
  int addr{0};

  StoreVal(mem, 2, 14);
  StoreVal(mem, 4, 15);

  // A = 2
  StoreOp(mem, OpCode::LoadA, addr++);
  StoreArg(mem, 14, addr++);
  // B = 4
  StoreOp(mem, OpCode::LoadB, addr++);
  StoreArg(mem, 15, addr++);
  // A = A + B = 6
  StoreOp(mem, OpCode::Add, addr++);
  Store2Args(mem, 0, 1, addr);

  cpu->Run();
  assert(cpu->GetRegisterA() == 6);
  delete cpu;
}

void testSub() {
  auto* cpu = new cpu::CPU();
  Memory& mem{cpu->GetMemory()};
  int addr{0};

  StoreVal(mem, 15, 14);
  StoreVal(mem, 5, 15);

  // A = 15
  StoreOp(mem, OpCode::LoadA, addr++);
  StoreArg(mem, 14, addr++);
  // B = 5
  StoreOp(mem, OpCode::LoadB, addr++);
  StoreArg(mem, 15, addr++);
  // A = A - B = 10
  StoreOp(mem, OpCode::Sub, addr++);
  Store2Args(mem, 0, 1, addr);

  cpu->Run();
  assert(cpu->GetRegisterA() == 10);
  delete cpu;
}

void testSub2() {
  auto* cpu = new cpu::CPU();
  Memory& mem{cpu->GetMemory()};
  int addr{0};

  StoreVal(mem, 5, 14);
  StoreVal(mem, 2, 15);

  // A = 5
  StoreOp(mem, OpCode::LoadA, addr++);
  StoreArg(mem, 14, addr++);
  // B = 2
  StoreOp(mem, OpCode::LoadB, addr++);
  StoreArg(mem, 15, addr++);
  // A = B - A = -3
  StoreOp(mem, OpCode::Sub, addr++);
  Store2Args(mem, 1, 0, addr);

  cpu->Run();
  assert(int4(cpu->GetRegisterA()) == int4(-3));
  delete cpu;
}

void testHalt() {
  auto* cpu = new cpu::CPU();
  Memory& mem{cpu->GetMemory()};
  int addr{0};

  StoreVal(mem, 8, 14);
  StoreVal(mem, 5, 15);

  // A = 8;
  StoreOp(mem, OpCode::LoadA, addr++);
  StoreArg(mem, 14, addr++);
  // Halt.
  StoreOp(mem, OpCode::Halt, addr++);
  // A = 5; (Skipped)
  StoreOp(mem, OpCode::LoadA, addr++);
  StoreArg(mem, 15, addr);

  cpu->Run();
  assert(cpu->GetRegisterA() == 8);
  delete cpu;
}

void testJump() {
  auto* cpu = new cpu::CPU();
  Memory& mem{cpu->GetMemory()};

  StoreVal(mem, 1, 14);
  StoreVal(mem, 10, 15);

  // A = 1
  StoreOp(mem, OpCode::LoadA, 0);
  StoreArg(mem, 14, 1);
  // B = 10
  StoreOp(mem, OpCode::LoadB, 2);
  StoreArg(mem, 15, 3);
  // jump to addr:7
  StoreOp(mem, OpCode::Jump, 4);
  StoreArg(mem, 7, 5);
  // Halt (skipped)
  StoreOp(mem, OpCode::Halt, 6);
  // A = A + B = 11
  StoreOp(mem, OpCode::Add, 7);
  Store2Args(mem, 0, 1, 8);

  cpu->Run();
  assert(cpu->GetRegisterA() == 11);
  delete cpu;
}

void testJumpZero() {
  auto* cpu = new cpu::CPU();
  Memory& mem{cpu->GetMemory()};

  StoreVal(mem, 5, 14);
  StoreVal(mem, 11, 15);

  // A = 5
  StoreOp(mem, OpCode::LoadA, 0);
  StoreArg(mem, 14, 1);
  // B = 5
  StoreOp(mem, OpCode::LoadB, 2);
  StoreArg(mem, 14, 3);
  // A = A-B = 0
  StoreOp(mem, OpCode::Sub, 4);
  Store2Args(mem, 0, 1, 5);
  // Jump over Halt if ALU result == 0
  StoreOp(mem, OpCode::JumpZ, 6);
  StoreArg(mem, 9, 7);
  // Halt (skipped)
  StoreOp(mem, OpCode::Halt, 8);
  // A = 11
  StoreOp(mem, OpCode::LoadA, 9);
  StoreArg(mem, 15, 10);

  cpu->Run();
  assert(cpu->GetRegisterA() == 11);
  delete cpu;
}

void testJumpNotZero() {
  auto* cpu = new cpu::CPU();
  Memory& mem{cpu->GetMemory()};

  StoreVal(mem, 3, 14);
  StoreVal(mem, 1, 15);

  // A = 3
  StoreOp(mem, OpCode::LoadA, 0);
  StoreArg(mem, 14, 1);
  // B = 1
  StoreOp(mem, OpCode::LoadB, 2);
  StoreArg(mem, 15, 3);
  // A = A - B = 2
  StoreOp(mem, OpCode::Sub, 4);
  Store2Args(mem, 0, 1, 5);
  // Jump to previous if A > 0;
  StoreOp(mem, OpCode::JumpNZ, 6);
  StoreArg(mem, 4, 7);

  cpu->Run();

  cpu->Run();
  assert(cpu->GetRegisterA() == 0);
  delete cpu;
}

} // namespace cpu::test

void RunAllCPUTests() {
  cpu::test::testLoadI();
  cpu::test::testStore();
  cpu::test::testMove();
  cpu::test::testMove2();

  cpu::test::testAdd();
  cpu::test::testSub();
  cpu::test::testSub2();

  cpu::test::testHalt();

  cpu::test::testJump();
  cpu::test::testJumpZero();
  cpu::test::testJumpNotZero();
}
