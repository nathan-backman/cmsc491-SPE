// Copyright 2019 [BVU CMSC491 class]
#include "Data/Data.h"

Data::Data(void* value) {
  this->value = value;
  gettimeofday(&timestamp, NULL);  // Gets the current time
}

Data::Data(void* value, timeval timestamp) {
  this->value = value;
  this->timestamp = timestamp;  // Applies the provided timestamp
}
