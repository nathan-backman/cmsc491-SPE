// Copyright 2019 [BVU CMSC491 class]
#include <iostream>
#include <string>
#include <vector>
#include "SPE.h"

class NumberGenerator : public InputSource {
  void generateData() {
    for (int i = 1; i <= 1000000; i++) {
      int *a = new int;
      *a = i;
      emit(Data(a));
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
    int *intPtr = static_cast<int*>(data.value);
    int num = *intPtr;
    if (num % 100000 == 0)
      std::cout << num << std::endl;
  }
};

int main(int argc, char** argv) {
  NumberGenerator inputSource;
  std::vector<IdentityOp> iOp(10);
  PrintData finalOp;

  StreamProcessingEngine spe;
  spe.addInputSource(&inputSource, {&(iOp.front())});
  for (int i = 0; i < iOp.size() - 1; i++)
    spe.connectOperators(&(iOp[i]), {&(iOp[i + 1])});
  spe.connectOperators(&(iOp.back()), {&finalOp});
  spe.run();

  return 0;
}
