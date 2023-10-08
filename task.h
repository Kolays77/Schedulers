#pragma once

struct Task {
  int id = 0;
  int arrival = 0;
  int period = 0;
  int deadline = 0;
  mutable int execution = 0;
  int const_execution = 0;
  int priority = 0;
  int addedTime = 0;
  void Execute() const { execution--; }
};