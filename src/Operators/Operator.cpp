// Copyright 2019 [BVU CMSC491 class]
#include "Operators/Operator.h"

void Operator::execute() {
  // If the range is greater than 1, then it is an aggregate operator and uses
  // processData(vector<data>) rather than processData(Data)
  if (range > 1) {
    // Load the window if this is the first execute of this operator
    if (window.size() == 0) {
      inputMutex.lock();
      if (input.size() < range) {
        inputMutex.unlock();
        return;
      }

      for (int i = 0; i < range; i++) {
        window.push_back(input.front());
        input.pop();
      }
      inputMutex.unlock();
    } else {
      // This is not the first running of the window, add to it
      inputMutex.lock();
      if (input.size() < slide) {
        inputMutex.unlock();
        return;
      }

      // Remove data sliding out, add new data sliding in
      for (int i = 0; i < slide; i++) {
        window.pop_front();
        window.push_back(input.front());
        input.pop();
      }
      inputMutex.unlock();
    }

    processData();
  } else {
    inputMutex.lock();
    if (input.empty() == false) {
      Data data = input.front();
      input.pop();
      inputMutex.unlock();

      processData(data);
    } else {
      inputMutex.unlock();
    }
  }
}

void Operator::addData(const Data &data) {
  // Basic mutex protection when adding to the queue
  inputMutex.lock();
  input.push(data);
  inputMutex.unlock();
}
