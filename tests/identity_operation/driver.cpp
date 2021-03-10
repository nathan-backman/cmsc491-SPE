// Copyright 2019 [BVU CMSC491 class]
#include <iostream>
#include <string>
#include <vector>
#include "SPE.h"

class FileReader : public InputSource {
  void generateData() {
    std::string line;
    while (getline(std::cin, line)) {
      const char* charVal = line.c_str();
      emit(Data((void*) charVal, line.length() + 1));
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

// 2) Output (print) tuples
class PrintData : public Operator {
 public:
  void processData(Data data) { std::cout << (char*) data.value << std::endl; }
};

int main(int argc, char** argv) {
  FileReader inputSource;
  IdentityOp op1;
  PrintData op2;

  StreamProcessingEngine spe;
  spe.addInputSource(&inputSource, {&op1});
  spe.connectOperators(&op1, {&op2});
  spe.run();

  return 0;
}
