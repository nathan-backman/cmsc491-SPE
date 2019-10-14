// Copyright 2019 [BVU CMSC491 class]
#include <string.h>
#include <cstdio>
#include "Data/Data.h"

Data::Data(void* value, uint32_t valueSize) : valueSize(valueSize) {
  // Provision space for value data and copy it over
  this->value = malloc(valueSize);
  memcpy(this->value, value, valueSize);

  gettimeofday(&timestamp, NULL);  // Gets the current time

  refCount = new std::atomic<unsigned int>(1);
}

Data::Data(void* value, uint32_t valueSize, timeval timestamp) : valueSize(valueSize), timestamp(timestamp) {
  // Provision space for value data and copy it over
  this->value = malloc(valueSize);
  memcpy(this->value, value, valueSize);

  refCount = new std::atomic<unsigned int>(1);
}

Data::~Data() {
  // If refCount is 1 prior to decrement (postfix decrement) then free it up
  if ((*refCount)-- == 1) {
    free(value);
    delete refCount;
  }
}

Data::Data(const Data &old) {
  value = old.value;
  valueSize = old.valueSize;
  timestamp = old.timestamp;
  refCount = old.refCount;

  (*refCount)++;
}
