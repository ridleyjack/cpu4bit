#pragma once

void RunAllMemTests();
void RunAllCPUTests();
void RunAllALUTests();
void RunAllSamples();

inline void RunAllTests() {
  RunAllALUTests();
  RunAllMemTests();
  RunAllCPUTests();
  RunAllSamples();
}