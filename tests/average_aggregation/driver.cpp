// Copyright 2019 [BVU CMSC491 class]
#include <deque>
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

class AvgAggOp : public Operator {
 public:
  AvgAggOp(int r, int s) : Operator(r, s) {}

  void processData(Data data) {}

  void processData() {
    float total = 0;
    int count = 0;
    for (Data d : window) {
      total += std::stoi(d.value);
      count++;
    }
    emit(Data(std::to_string(total / count)));
  }
};

// 4) Output (print) tuples
class PrintData : public Operator {
 public:
  void processData(Data data) { printf("%.1f\n", std::stof(data.value)); }
};

int main(int argc, char** argv) {
  FileReader inputSource;
  AvgAggOp op1(5, 2);
  PrintData op2;

  StreamProcessingEngine spe;
  spe.addInputSource(&inputSource, {&op1});
  spe.connectOperators(&op1, {&op2});
  spe.run();

  return 0;
}
