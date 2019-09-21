// Copyright 2019 [BVU CMSC491 class]
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
//  1) Filter out (remove) tuples that have less than 2 vowels
//  2) Output (print) the word

//  1) Filter out (remove) tuples that have less than 2 vowels
class FilterWords : public Operator {
 public:
  void processData(Data data) {
    int count = 0;
    for (int i = 0; i < data.value.size(); i++) {
      if (data.value[i] == 'i' || data.value[i] == 'o' || data.value[i] == 'u'
          || data.value[i] == 'e' || data.value[i] == 'a') {
        count++;
      }
    }
    if (count >= 2) {
      emit(data);
    }
  }
};

// 2) Output (print) words
class PrintData : public Operator {
 public:
  void processData(Data data) { std::cout << data.value << std::endl; }
};

int main(int argc, char** argv) {
  FileReader inputSource;
  FilterWords op1;
  PrintData op2;

  StreamProcessingEngine spe;
  spe.addInputSource(&inputSource, {&op1});
  spe.connectOperators(&op1, {&op2});
  spe.run();

  return 0;
}
