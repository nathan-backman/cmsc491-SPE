// Copyright 2019 [BVU CMSC491 class]
#include "Operators/Operator.h"

bool Operator::execute() {
  // If the range is greater than 1, then it is an aggregate operator and uses
  // processData(vector<data>) rather than processData(Data)
  if (range > 1) {
    // Load the winodw if this is the first execute of this operator
    if (window.size() == 0) {
      if (input.size() < range) {
        return false;
      }

      for (int i=0; i<range; i++) {
        window.push_back(input.front());
        input.pop();
      }
    }
    // This is not the first running of the window, add to it
    else {
      if (input.size() < slide) {
        return false;
      }

      // Remove data sliding out, add new data sliding in
      for (int i=0; i<slide; i++) {
        window.pop_front();
        window.push_back(input.front());
        input.pop();
      }
    }

    processData();
    return true;
  }
  else {
    if (input.empty() == false) {
      Data data = input.front();
      input.pop();

      processData(data);

      return true;
    } else {
      return false;
    }
  }
}

void Operator::emit(Data data) { output->push(data); }
