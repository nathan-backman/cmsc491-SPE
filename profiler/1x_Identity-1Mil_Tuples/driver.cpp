// Copyright 2019 [BVU CMSC491 class]
#include <iostream>
#include <string>
#include <vector>
#include "SPE.h"

//### Operator Workflow
//  1) Receive data and emit the same data (the *identity* operator)
//  2) Output (print) tuples

// 1) Receive data and emit the same data (the *identity* operator)
class IdentityOp : public Operator {
 public:
  void processData(Data data) { emit(data); }
};

// 4) Output (print) tuples
class PrintData : public Operator {
 public:
  void processData(Data data) {
    if (std::stoi(data.value) % 100000 == 0)
      std::cout << data.value << std::endl;
  }
};

int main(int argc, char** argv) {
  IdentityOp op1;
  PrintData op2;

  op1.output = &(op2.input);

  std::vector<Operator*> ops{&op1, &op2};

  int tupleCount = 0;

  // Main processing loop -- keep processing until there is nothing left
  bool stillProcessing;
  do {
    while (++tupleCount <= 1000000)
      op1.input.push(Data(std::to_string(tupleCount)));

    stillProcessing = false;
    // Execute each operator in a round-robin format
    for (auto op : ops) {
      if (op->execute() == true) stillProcessing = true;
    }
  } while (stillProcessing == true);

  return 0;
}
