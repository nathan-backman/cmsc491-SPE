// Copyright 2019 [BVU CMSC491 class]
#include <iostream>
#include <string>
#include <vector>
#include "SPE.h"

//### Operator Workflow
//  1) Filter out (remove) tuples from odd sensor sites (Ex. remove "site_3")
//  2) Transform temperatures to show their distance from freezing (32.0F)
//  3) Filter out tuples with temps less than -10.0 or greater than +10.0
//  4) Output (print) tuples

// 1) Filter out (remove) tuples from odd sensor sites (Ex. remove "site_3")
class DropOdds : public Operator {
 public:
  void processData(Data data) {
    int siteID = atoi(data.value.substr(5, data.value.find(" ") - 4).c_str());
    if (siteID % 2 == 0) {
      emit(data);
    }
  }
};

// 2) Transform temperatures to show their distance from freezing (32.0F)
class TransformTemp : public Operator {
 public:
  void processData(Data data) {
    int start = data.value.find(" ") + 1;
    float temp = std::stof(data.value.substr(start, data.value.size() - start));
    temp = temp - 32.0f;

    char tempBuf[8];
    sprintf(tempBuf, "%+.1f", temp);

    std::string newVal = data.value.substr(0, start) + std::string(tempBuf);
    emit(Data(newVal, data.timestamp));
  }
};

// 3) Filter out tuples with temps less than -10.0 or greater than +10.0
class DropTemps : public Operator {
 public:
  void processData(Data data) {
    int start = data.value.find(" ") + 1;
    float temp = std::stof(data.value.substr(start, data.value.size() - start));

    if (temp >= -10.0 && temp <= 10.0) emit(data);
  }
};

// 4) Output (print) tuples
class PrintData : public Operator {
 public:
  void processData(Data data) { std::cout << data.value << std::endl; }
};

int main(int argc, char** argv) {
  DropOdds op1;
  TransformTemp op2;
  DropTemps op3;
  PrintData op4;

  op1.output = &(op2.input);
  op2.output = &(op3.input);
  op3.output = &(op4.input);

  std::vector<Operator*> ops{&op1, &op2, &op3, &op4};

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
