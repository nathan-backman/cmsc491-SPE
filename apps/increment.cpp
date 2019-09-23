// Copyright 2019 [BVU CMSC491 class]
#include <iostream>
#include <string>
#include <vector>
#include "SPE.h"

class IncrementOp : public Operator {
 public:
  void processData(Data data) {
    int num = std::stoi(data.value);
    num++;
    data.value = std::to_string(num);
    emit(data);
  }
};

class OutputOp : public Operator {
 public:
  void processData(Data data) { std::cout << data.value << std::endl; }
};

class NumberGenerator : public InputSource {
  void generateData() {
    for (int i = 1; i < 10; i += 2) {
      emit(Data(std::to_string(i)));
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
