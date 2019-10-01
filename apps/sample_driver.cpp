// Copyright 2019 [BVU CMSC491 class]
#include <iostream>
#include <string>
#include <vector>
#include "SPE.h"

class IncrementOp : public Operator {
 public:
  void processData(Data data) {
    int *intPtr = static_cast<int*>(data.value);
    int num = *intPtr;
    num++;
    num *= 10;
    delete(intPtr);
    *(int*)(data.value) = num;
    emit(data);
  }
};

class OutputOp : public Operator {
 public:
  void processData(Data data) { 
    int *intPtr = static_cast<int*>(data.value);
    int num = *intPtr;
    //std::cout << data.value << std::endl;
    std::cout << num << std::endl; 
  }
};

class NumberGenerator : public InputSource {
  void generateData() {
    for (int i = 1; i < 10; i += 2) {
      //int a = i;
      //emit(Data(&a));
      int *a = new int;
      *a = i;
      emit(Data(a));
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
