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
//  1) Filter out (remove) tuples from non-bvu emails
//  2) Increment the number of games played by 1
//  3) Filter out tuples with less than 8 games played
//  4) Transform data by removing the @domain part of the email
//  5) Output (print) tuples

// 1) Filter out (remove) tuples from non-bvu emails
class DropEmail : public Operator {
 public:
  void processData(Data data) {
    int domainInd = data.value.find("@");
    std::string domain = data.value.substr(domainInd);
    if (domain == "@bvu.edu") {
      emit(data);
    }
  }
};

// 2) Increment the number of games played by 1
class IncrementGames : public Operator {
 public:
  void processData(Data data) {
    int gamesInd = data.value.find(" ") + 1;
    int games = std::stoi(data.value.substr(gamesInd, 1));
    games++;

    std::string line = data.value;
    line.replace(gamesInd, 1, std::to_string(games));
    emit(Data(line, data.timestamp));
  }
};

// 3) Filter out tuples with temps less than 8 games played
class DropGames : public Operator {
 public:
  void processData(Data data) {
    int gamesInd = data.value.find(" ") + 1;
    int games = std::stoi(data.value.substr(gamesInd, 1));
    if (games >= 8) {
      emit(data);
    }
  }
};

// 4) Transform data by removing the @domain part of the email
class TransformLine : public Operator {
 public:
  void processData(Data data) {
    int domainInd = data.value.find("@");
    std::string newstr = data.value.substr(0, domainInd);
    emit(Data(newstr, data.timestamp));
  }
};

// 5) Output (print) tuples
class PrintData : public Operator {
 public:
  void processData(Data data) { std::cout << data.value << std::endl; }
};

int main() {
  FileReader inputSource;
  DropEmail op1;
  IncrementGames op2;
  DropGames op3;
  TransformLine op4;
  PrintData op5;

  StreamProcessingEngine spe;
  spe.addInputSource(&inputSource, {&op1});
  spe.connectOperators(&op1, {&op2});
  spe.connectOperators(&op2, {&op3});
  spe.connectOperators(&op3, {&op4});
  spe.connectOperators(&op4, {&op5});
  spe.run();

  return 0;
}
