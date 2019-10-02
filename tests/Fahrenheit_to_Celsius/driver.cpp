// Copyright 2019 [BVU CMSC491 class]
#include <math.h>
#include <iostream>
#include <string>
#include <vector>
#include "SPE.h"

class FileReader : public InputSource<int> {
  void generateData() {
    std::string line;
    while (getline(std::cin, line)) {
      emit(Data<int>(std::stoi(line)));
    }
  }
};

//### Operator Workflow
//  1) Change integer from Fahrenheit to Celsius.
//  2) Transform the data to be easily readable.
//  3) Output the data.

// 1) Change integer from Fahrenheit to Celsius.
class FarenheightOp : public Operator<int, int> {
 public:
  void processData(Data<int> data) {
    int num = data.value;
    int celsius = (num - 32) * (5.0 / 9.0);
    emit(Data<int>(celsius));
  }
};

// 2) Transform the data to be easily readable.
class AddStringOp : public Operator<int, std::string> {
 public:
  void processData(Data<int> data) {
    std::string val = "Celsius Temperature: " + std::to_string(data.value);
    emit(Data<std::string>(val));
  }
};

// 3) Output the data.
class PrintData : public Operator<std::string, std::string> {
 public:
  void processData(Data<std::string> data) { std::cout << data.value << std::endl; }
};

int main(int argc, char** argv) {
  FileReader inputSource;
  FarenheightOp op1;
  AddStringOp op2;
  PrintData op3;

  StreamProcessingEngine spe;
  spe.addInputSource<int>(&inputSource, {&(op1.acceptor)});
  spe.connectOperators<int, int>(&op1, {&(op2.acceptor)});
  spe.connectOperators<int, std::string>(&op2, {&(op3.acceptor)});
  spe.run();

  return 0;
}
