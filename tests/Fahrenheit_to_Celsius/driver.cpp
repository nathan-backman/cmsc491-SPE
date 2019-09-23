// Copyright 2019 [BVU CMSC491 class]
#include <math.h>
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

//### Operator Workflow
//  1) Change integer from Fahrenheit to Celsius.
//  2) Transform the data to be easily readable.
//  3) Output the data.

// 1) Change integer from Fahrenheit to Celsius.
class FarenheightOp : public Operator {
 public:
  void processData(Data data) {
    int num = std::stoi(data.value);
    int celsius = (num - 32) * (5.0 / 9.0);
    data.value = std::to_string(celsius);
    emit(data);
  }
};

// 2) Transform the data to be easily readable.
class AddStringOp : public Operator {
 public:
  void processData(Data data) {
    data.value = "Celsius Temperature: " + data.value;
    emit(data);
  }
};

// 3) Output the data.
class PrintData : public Operator {
 public:
  void processData(Data data) { std::cout << data.value << std::endl; }
};

int main() {
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
