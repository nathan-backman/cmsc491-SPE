// Copyright 2019 [BVU CMSC491 class]
#include "Operators/Operator.h"

/*
void Operator<A, B>::execute() {
  // If the range is greater than 1, then it is an aggregate operator and uses
  // processData(vector<data>) rather than processData(Data)
  if (range > 1) {
    // Load the window if this is the first execute of this operator
    if (window.size() == 0) {
      acceptor.inputMutex.lock();
      if (acceptor.input.size() < range) {
        acceptor.inputMutex.unlock();
        return;
      }

      for (int i = 0; i < range; i++) {
        window.push_back(acceptor.input.front());
        acceptor.input.pop();
      }
      acceptor.inputMutex.unlock();
    } else {
      // This is not the first running of the window, add to it
      acceptor.inputMutex.lock();
      if (acceptor.input.size() < slide) {
        acceptor.inputMutex.unlock();
        return;
      }

      // Remove data sliding out, add new data sliding in
      for (int i = 0; i < slide; i++) {
        window.pop_front();
        window.push_back(acceptor.input.front());
        acceptor.input.pop();
      }
      acceptor.inputMutex.unlock();
    }

    this->numItems = acceptor.input.size();
    processData();
  } else {
    acceptor.inputMutex.lock();
    if (acceptor.input.empty() == false) {
      Data<A> data = acceptor.input.front();
      acceptor.input.pop();
      acceptor.inputMutex.unlock();

      this->numItems = acceptor.input.size();
      processData(data);
    } else {
      acceptor.inputMutex.unlock();
    }
  }
}
*/
