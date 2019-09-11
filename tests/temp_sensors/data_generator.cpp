// Copyright 2019 [BVU CMSC491 class]
#include <stdlib.h>
#include <time.h>
#include <iostream>

int main(int argc, char** argv) {
  // Verify command line parameter format
  if (argc != 3) {
    std::cerr << "[Error] Usage: " << argv[0] << " <numData> <numSensors>"
              << std::endl;
    return 1;
  }

  // Verify numData parameter
  int D = atoi(argv[1]);
  if (D <= 0 || D > 10000000) {
    std::cerr << "[Error] Input: " << argv[0] << " <numData = " << D
              << "> <numSensors>" << std::endl;
    std::cerr << "[Error] numItems parameter must be in range [1, 10000000]"
              << std::endl;
    return 1;
  }

  // Verify numSensors parameter
  int S = atoi(argv[2]);
  if (S <= 0 || S > 100) {
    std::cerr << "[Error] Input: " << argv[0]
              << " <numData> <numSensors = " << S << ">" << std::endl;
    std::cerr << "[Error] numSensors parameter must be in range [1, 100]"
              << std::endl;
    return 1;
  }

  int seedp = time(NULL);
  const float minF = -50.0;
  const float maxF = 120.0;
  for (int i = 1; i <= D; i++) {
    float r = (static_cast<float>(rand_r(&seedp))) /
              (static_cast<float>(RAND_MAX));
    float temp = minF + r * (maxF - minF);
    int sensID = (rand_r(&seedp) % S) + 1;
    printf("site_%d %.1f\n", sensID, temp);
  }

  return 0;
}
