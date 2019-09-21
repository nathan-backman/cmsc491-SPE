// Copyright 2019 [BVU CMSC491 class]
#include "Operators/Emitter.h"
#include "Operators/Operator.h"

void Emitter::emit(const Data &data) {
  // Add the data to the input queues of each of the downstream operators
  for (auto opPtr : downstreamOperators) {
    opPtr->input.push(data);
  }
}
