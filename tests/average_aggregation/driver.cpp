// Copyright 2019 [BVU CMSC491 class]
#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include "SPE.h"

class AvgAggOp : public Operator {
 public:
  AvgAggOp(int r, int s) : Operator(r, s) { }

  void processData(Data data) {  }

  void processData() {
    float total = 0;
    int count = 0;
    for (Data d : window) {
      total += std::stoi(d.value);
      count++;
    }
    emit(Data( std::to_string(total/count) ));
  }
};

// 4) Output (print) tuples
class PrintData : public Operator {
 public:
  void processData(Data data) { printf("%.1f\n", std::stof(data.value));  }
};

int main(int argc, char** argv) {
  AvgAggOp op1(5, 2);
  PrintData op2;

  op1.output = &(op2.input);

  std::vector<Operator*> ops{&op1, &op2};

  std::string line;
  while (getline(std::cin, line)) {
    op1.input.push(Data(line));
  }

  // Main processing loop -- keep processing until there is nothing left
  bool stillProcessing;
  do {
    stillProcessing = false;
    // Execute each operator in a round-robin format
    for (auto op : ops) {
      if (op->execute() == true) stillProcessing = true;
    }
  } while (stillProcessing == true);

  return 0;
}
