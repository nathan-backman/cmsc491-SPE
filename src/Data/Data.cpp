// Copyright 2019 [BVU CMSC491 class]
#include "Data/Data.h"
#include <iostream>
#include <any>

Data::Data(std::any value) {
	std::any *p = new std::any(value);
	this->value = p;
	gettimeofday(&timestamp, NULL);  // Gets the current time
}

Data::Data(std::any value, timeval timestamp) {
	//std::cout << value << std::endl;
	std::any *p = new std::any(value);
	this->value = p;
	this->timestamp = timestamp;  // Applies the provided timestamp
}