// Copyright 2019 [BVU CMSC491 class]
#include "Data/Data.h"

Data::Data(std::string value) {
  this->value = value;
  timestamp = std::chrono::system_clock::now(); // Gets the current time
}

Data::Data(std::string value,
           std::chrono::time_point<std::chrono::system_clock> timestamp) {
  this->value = value;
  this->timestamp = timestamp;  // Applies the provided timestamp
}
