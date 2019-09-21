// Copyright 2019 [BVU CMSC491 class]
#include <iostream>
#include <string>
#include <vector>
#include "SPE.h"

class NumberGenerator : public InputSource {
  void generateData() {
    for (int i = 1; i <= 1000000; i++) {
      emit(Data(std::to_string(i)));
    }
  }
};

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
  NumberGenerator inputSource;
  IdentityOp op1, op2, op3, op4, op5, op6, op7, op8, op9, op10;
  PrintData op11;

  StreamProcessingEngine spe;
  spe.addInputSource(&inputSource, {&op1});
  spe.connectOperators(&op1, {&op2});
  spe.connectOperators(&op2, {&op3});
  spe.connectOperators(&op3, {&op4});
  spe.connectOperators(&op4, {&op5});
  spe.connectOperators(&op5, {&op6});
  spe.connectOperators(&op6, {&op7});
  spe.connectOperators(&op7, {&op8});
  spe.connectOperators(&op8, {&op9});
  spe.connectOperators(&op9, {&op10});
  spe.connectOperators(&op10, {&op11});
  spe.run();

  return 0;
}
