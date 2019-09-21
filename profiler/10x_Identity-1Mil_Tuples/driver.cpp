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
  IdentityOp op1, op2, op3, op4, op5, op6, op7, op8, op9, op10;
  PrintData op11;

  op1.output = &(op2.input);
  op2.output = &(op3.input);
  op3.output = &(op4.input);
  op4.output = &(op5.input);
  op5.output = &(op6.input);
  op6.output = &(op7.input);
  op7.output = &(op8.input);
  op8.output = &(op9.input);
  op9.output = &(op10.input);
  op10.output = &(op11.input);

  std::vector<Operator*> ops{&op11, &op10, &op9, &op8, &op7,
                             &op6, &op5, &op4, &op3, &op2, &op1};

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
