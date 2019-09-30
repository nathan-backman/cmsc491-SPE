// Copyright 2019 [BVU CMSC491 class]
#include <iostream>
#include <string>
#include <vector>
#include <any>
#include "SPE.h"

class IncrementOp : public Operator {
 public:
  void processData(Data data) {
    int num=std::any_cast<int>(data.value);
    num++;
    num *= 10;
    data.value = num;
    emit(data);
  }
};

class OutputOp : public Operator {
 public:
  void processData(Data data) { 
    std::cout << std::any_cast<int>(data.value) << std::endl; 
  }
};

class NumberGenerator : public InputSource {
  void generateData() {
    for (int i = 1; i < 10; i += 2) {
      emit(Data(i));
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
