// Copyright 2019 [BVU CMSC491 class]
#include <iostream>
#include <string>
#include <vector>
#include "SPE.h"

class IncrementOp : public Operator<std::string, std::string> {
 public:
  IncrementOp() : Operator<std::string, std::string>() {}

  void processData(Data<std::string> data) {
    int num = std::stoi(data.value);
    num++;
    num *= 10;
    data.value = std::to_string(num);
    emit(data);
  }
};

class OutputOp : public Operator <std::string, std::string>{
 public:
  OutputOp() : Operator<std::string, std::string>() {}

  void processData(Data<std::string> data) { std::cout << data.value << std::endl; }
};

class NumberGenerator : public InputSource<std::string> {
 public: 
  NumberGenerator() : InputSource<std::string>() {} 

  void generateData() {
    for (int i = 1; i < 10; i += 2) {
      emit(Data<std::string>(std::to_string(i)));
    }
  }
};

int main(int argc, char** argv) {
  std::cout << "SPE Starting up." << std::endl;

  NumberGenerator inputSource;
  IncrementOp op1;
  OutputOp op2;

  StreamProcessingEngine spe;

  spe.addInputSource<std::string>(&inputSource, {&op1.acceptor});
  spe.connectOperators<std::string, std::string>(&op1, {&op2.acceptor});

  spe.run();

  std::cout << "SPE Finished." << std::endl;
  return 0;
}
