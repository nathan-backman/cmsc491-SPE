// Copyright 2019 [BVU CMSC491 class]
#include <iostream>
#include <string>
#include <vector>
#include "SPE.h"

int main(int argc, char** argv) {
  std::cout << "SPE Started" << std::endl;

  // TODO(hw1): Create Operators

  // TODO(hw1): Supply initial operator with data

  // TODO(hw1): Itemize operators in a vector
  std::vector<Operator*> ops;

  // Main processing loop -- keep processing until there is nothing left
  bool stillProcessing;
  do {
    stillProcessing = false;
    // Execute each operator in a round-robin format
    for (auto op : ops) {
      if (op->execute() == true) stillProcessing = true;
    }
  } while (stillProcessing == true);

  std::cout << "SPE Finished" << std::endl;
  return 0;
}
