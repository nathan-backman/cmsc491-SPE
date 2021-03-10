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
//  1) Filter out (remove) tuples from odd sensor sites (Ex. remove "site_3")
//  2) Transform temperatures to show their distance from freezing (32.0F)
//  3) Filter out tuples with temps less than -10.0 or greater than +10.0
//  4) Output (print) tuples

// 1) Filter out (remove) tuples from odd sensor sites (Ex. remove "site_3")
class DropOdds : public Operator {
 public:
  void processData(Data data) {
    std::string dataVal = std::string((char*) data.value);

    int siteID = atoi(dataVal.substr(5, dataVal.find(" ") - 4).c_str());
    if (siteID % 2 == 0) {
      emit(data);
    }
  }
};

// 2) Transform temperatures to show their distance from freezing (32.0F)
class TransformTemp : public Operator {
 public:
  void processData(Data data) {
    std::string dataVal = std::string((char*) data.value);

    int start = dataVal.find(" ") + 1;
    float temp = std::stof(dataVal.substr(start, dataVal.size() - start));
    temp = temp - 32.0f;

    char tempBuf[8];
    snprintf(tempBuf, sizeof(tempBuf), "%+.1f", temp);

    std::string newVal = dataVal.substr(0, start) + std::string(tempBuf);

    const char* charVal = newVal.c_str();
    emit(Data((void*) charVal, newVal.length() + 1, data.timestamp));
  }
};

// 3) Filter out tuples with temps less than -10.0 or greater than +10.0
class DropTemps : public Operator {
 public:
  void processData(Data data) {
    std::string dataVal = std::string((char*) data.value);

    int start = dataVal.find(" ") + 1;
    float temp = std::stof(dataVal.substr(start, dataVal.size() - start));

    if (temp >= -10.0 && temp <= 10.0) emit(data);
  }
};

// 4) Output (print) tuples
class PrintData : public Operator {
 public:
  void processData(Data data) { std::cout << (char*) data.value << std::endl; }
};

int main(int argc, char** argv) {
  FileReader inputSource;
  DropOdds op1;
  TransformTemp op2;
  DropTemps op3;
  PrintData op4;

  StreamProcessingEngine spe;
  spe.addInputSource(&inputSource, {&op1});
  spe.connectOperators(&op1, {&op2});
  spe.connectOperators(&op2, {&op3});
  spe.connectOperators(&op3, {&op4});
  spe.run();

  return 0;
}
