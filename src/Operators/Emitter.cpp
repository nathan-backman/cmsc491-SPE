// Copyright 2019 [BVU CMSC491 class]
#include "Operators/Emitter.h"
#include "Operators/Operator.h"

template<typename T>
void Emitter<T>::emit(const Data<T> &data) {
  // Add the data to the input queues of each of the downstream operators
  for (auto opPtr : downstreamOperators) {
    opPtr->addData(data);
  }
}
