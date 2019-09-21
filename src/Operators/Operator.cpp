// Copyright 2019 [BVU CMSC491 class]
#include "Operators/Operator.h"

bool Operator::execute() {
  if (input.empty() == false) {
    Data data = input.front();
    input.pop();

    processData(data);

    return true;
  } else {
    return false;
  }
}
