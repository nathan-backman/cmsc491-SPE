// Copyright 2019 [BVU CMSC491 class]
#include <iostream>
#include <string>
#include <vector>
#include "SPE.h"

class NumberGenerator : public InputSource<int> {
  void generateData() {
    for (int i = 1; i <= 1000000; i++) {
      emit(Data<int>(i));
    }
  }
};

//### Operator Workflow
//  1) Receive data and emit the same data (the *identity* operator)
//  2) Output (print) tuples

// 1) Receive data and emit the same data (the *identity* operator)
class IdentityOp : public Operator<int, int> {
 public:
  void processData(Data<int> data) {emit(Data<int>(data));}
};

// 4) Output (print) tuples
class PrintData : public Operator<int, int> {
 public:
  void processData(Data<int> data) {
    if (data.value % 100000 == 0)
      std::cout << data.value << std::endl;
  }
};

int main(int argc, char** argv) {
  NumberGenerator inputSource;
  IdentityOp op1;
  PrintData op2;

  StreamProcessingEngine spe;
  spe.addInputSource<int>(&inputSource, {&(op1.acceptor)});
  spe.connectOperators<int>(&op1, {&(op2.acceptor)});
  spe.run();

  return 0;
}
