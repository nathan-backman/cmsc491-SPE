// Copyright 2019 [BVU CMSC491 class]
#include <iostream>
#include <string.h>
#include <vector>
#include "SPE.h"

class FileReader : public InputSource {
  void generateData() {
    std::string line;
    while (getline(std::cin, line)) {
      const char* charData = line.c_str();
      emit(Data((void*) charData, line.length() + 1));
    }
  }
};

class IncrementOp : public Operator {
 public:
  void processData(Data data) {
    int num = std::atoi((char*) data.value);
    num++;
    num *= 10;

    emit(Data(&num, sizeof(int)));
  }
};

class PrintData : public Operator {
 public:
  void processData(Data data) { std::cout << *(int*) data.value << std::endl; }
};

int main(int argc, char** argv) {
  FileReader inputSource;
  IncrementOp op1;
  PrintData op2;

  StreamProcessingEngine spe;
  spe.addInputSource(&inputSource, {&op1});
  spe.connectOperators(&op1, {&op2});
  spe.run();

  return 0;
}
