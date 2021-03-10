// Copyright 2019 [BVU CMSC491 class]
#include <math.h>
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

//### Operator Workflow
//  1) Change integer from Fahrenheit to Celsius.
//  2) Transform the data to be easily readable.
//  3) Output the data.

// 1) Change integer from Fahrenheit to Celsius.
class FarenheightOp : public Operator {
 public:
  void processData(Data data) {
    std::string dataVal = std::string((char*) data.value);

    int num = std::stoi(dataVal);
    int celsius = (num - 32) * (5.0 / 9.0);

    emit(Data(&celsius, sizeof(int) ));
  }
};

// 2) Transform the data to be easily readable.
class AddStringOp : public Operator {
 public:
  void processData(Data data) {
    std::string dataVal = "Celsius Temperature: " + std::to_string(*(int*) data.value);
    const char* charData = dataVal.c_str();

    emit(Data((void*) charData, dataVal.length() + 1));
  }
};

// 3) Output the data.
class PrintData : public Operator {
 public:
  void processData(Data data) { std::cout << (char*) data.value << std::endl; }
};

int main(int argc, char** argv) {
  FileReader inputSource;
  FarenheightOp op1;
  AddStringOp op2;
  PrintData op3;

  StreamProcessingEngine spe;
  spe.addInputSource(&inputSource, {&op1});
  spe.connectOperators(&op1, {&op2});
  spe.connectOperators(&op2, {&op3});
  spe.run();

  return 0;
}
