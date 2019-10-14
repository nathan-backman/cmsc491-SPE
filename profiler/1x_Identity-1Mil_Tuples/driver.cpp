// Copyright 2019 [BVU CMSC491 class]
#include <iostream>
#include <string>
#include <vector>
#include "SPE.h"

class NumberGenerator : public InputSource {
  void generateData() {
    for (int i = 1; i <= 1000000; i++) {
      emit(Data(&i, sizeof(i)));
    }
  }
};

//### Operator Workflow
//  1) Receive data and emit the same data (the *identity* operator)
//  2) Output (print) tuples

// 1) Receive data and emit the same data (the *identity* operator)
class IdentityOp : public Operator {
 public:
  void processData(Data data) {
    Data outputData(data);
    emit(outputData);
  }
};

// 4) Output (print) tuples
class PrintData : public Operator {
 public:
  void processData(Data data) {
    int &num = *(int*)(data.value);
    if (num % 100000 == 0)
      std::cout << num << std::endl;
  }
};

int main(int argc, char** argv) {
  NumberGenerator inputSource;
  IdentityOp op1;
  PrintData op2;

  StreamProcessingEngine spe;
  spe.addInputSource(&inputSource, {&op1});
  spe.connectOperators(&op1, {&op2});
  spe.run(4);

  return 0;
}
