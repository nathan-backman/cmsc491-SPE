// Copyright 2019 [BVU CMSC491 class]
#include <deque>
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

class AvgAggOp : public Operator {
 public:
  AvgAggOp(int r, int s) : Operator(r, s) {}

  void processData(Data data) {}

  void processData() {
    float total = 0;
    int count = 0;
    for (Data d : window) {
      total += std::atoi( (char*) d.value);
      count++;
    }
    float out = total / count;

    emit(Data(&out, sizeof(float)));
  }
};

// 4) Output (print) tuples
class PrintData : public Operator {
 public:
  void processData(Data data) { printf("%.1f\n", *(float*) data.value ); }
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
