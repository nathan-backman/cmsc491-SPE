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
//  1) Filter out (remove) tuples that have less than 2 vowels
//  2) Output (print) the word

//  1) Filter out (remove) tuples that have less than 2 vowels
class FilterWords : public Operator {
 public:
  void processData(Data data) {
    std::string dataVal = std::string((char*) data.value);

    int count = 0;
    for (int i = 0; i < dataVal.size(); i++) {
      if (dataVal[i] == 'i' || dataVal[i] == 'o' ||
          dataVal[i] == 'u' || dataVal[i] == 'e' ||
          dataVal[i] == 'a') {
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
  void processData(Data data) { std::cout << (char*) data.value << std::endl; }
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
