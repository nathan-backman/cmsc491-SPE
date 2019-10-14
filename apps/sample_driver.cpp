// Copyright 2019 [BVU CMSC491 class]
#include <iostream>
#include <string>
#include <vector>
#include "SPE.h"

class IncrementOp : public Operator {
 public:
  void processData(Data data) {
    int num = *(int*)data.value;
    num++;
    num *= 10;

    Data outputData(&num, sizeof(num), data.timestamp);
    emit(outputData);
  }
};

class OutputOp : public Operator {
 public:
  void processData(Data data) { std::cout << *(int*)data.value << std::endl; }
};

class NumberGenerator : public InputSource {
  void generateData() {
    for (int i = 1; i < 10; i += 2) {
      emit(Data(&i, sizeof(i)));
    }
  }
};

int main(int argc, char** argv) {
  std::cout << "SPE Starting up." << std::endl;

  NumberGenerator inputSource;
  IncrementOp op1;
  OutputOp op2;

  StreamProcessingEngine spe;

  spe.addInputSource(&inputSource, {&op1});
  spe.connectOperators(&op1, {&op2});

  spe.run();

  std::cout << "SPE Finished." << std::endl;
  return 0;
}
