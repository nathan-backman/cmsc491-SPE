// Copyright 2019 [BVU CMSC491 class]
#include <iostream>
#include <string>
#include <vector>
#include "SPE.h"

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

class OutputOp : public Operator {
 public:
  void processData(Data data) { std::cout << data.value << std::endl; }
};

int main(int argc, char** argv) {
  std::cout << "Hello World" << std::endl;

  IncrementOp incOp;
  OutputOp outOp;

  incOp.output = &(outOp.input);

  for (int i = 1; i < 10; i += 2) {
    incOp.input.push(Data(std::to_string(i)));
  }

  std::vector<Operator*> ops = {&incOp, &outOp};

  bool stillProcessing;
  do {
    stillProcessing = false;
    for (auto op : ops) {
      if (op->execute() == true) stillProcessing = true;
    }
  } while (stillProcessing == true);

  return 0;
}
