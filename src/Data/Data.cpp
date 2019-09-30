// Copyright 2019 [BVU CMSC491 class]
#include "Data/Data.h"
Data::Data(std::any value) {
  this->value = value;
  gettimeofday(&timestamp, NULL);  // Gets the current time
}

Data::Data(std::any value, timeval timestamp) {
  this->value = value;
  this->timestamp = timestamp;  // Applies the provided timestamp
}
