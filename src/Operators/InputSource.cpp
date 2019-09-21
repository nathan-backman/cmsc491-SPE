// Copyright 2019 [BVU CMSC491 class]
#include "Operators/InputSource.h"

void InputSource::startThread() {
  inputThread = std::thread(&InputSource::generateData, this);
}

void InputSource::waitForCompletion() { inputThread.join(); }
