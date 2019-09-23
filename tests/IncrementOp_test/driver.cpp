// Copyright 2019 [BVU CMSC491 class]
#include <iostream>
#include <string>
#include <vector>
#include "SPE.h"

class FileReader : public InputSource {
  void generateData() {
    std::string line;
    while (getline(std::cin, line)) {
      emit(Data(line));
    }
  }
};

class IncrementOp : public Operator {
 public:
  void processData(Data data) {
    int num = std::stoi(data.value);
    num++;
    num *= 10;
    data.value = std::to_string(num);
    emit(data);
  }
};

class PrintData : public Operator {
 public:
  void processData(Data data) { std::cout << data.value << std::endl; }
};

int main() {
  FileReader inputSource;
  IncrementOp op1;
  PrintData op2;

  StreamProcessingEngine spe;
  spe.addInputSource(&inputSource, {&op1});
  spe.connectOperators(&op1, {&op2});
  spe.run();

  return 0;
}
