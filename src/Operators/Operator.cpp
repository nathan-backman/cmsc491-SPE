// Copyright 2019 [BVU CMSC491 class]
#include "Operators/Operator.h"

bool Operator::execute() {
  // TODO(parallel): When we do multithreaded scheduling (many consumers) we
  // need to also call inputMutex.lock() when checking input.empty().
  if (input.empty() == false) {
    // Basic mutex protection when reading/popping from the queue
    inputMutex.lock();
    Data data = input.front();
    input.pop();
    inputMutex.unlock();

    processData(data);

    return true;
  } else {
    return false;
  }
}

void Operator::addData(const Data &data) {
  // Basic mutex protection when adding to the queue
  inputMutex.lock();
  input.push(data);
  inputMutex.unlock();
}
