// Copyright 2019 [BVU CMSC491 class]
#include "Engine/StreamProcessingEngine.h"

void StreamProcessingEngine::connectOperators(
    Operator* upstreamOp,
    std::vector<Operator*> downstreamOps) {
  // Register the downstream operators with the upstream operator
  upstreamOp->downstreamOperators = downstreamOps;

  // Add these operators to our running set of all workflow operators which
  // will be later used by the operator scheduler
  ops.insert(upstreamOp);
  ops.insert(downstreamOps.begin(), downstreamOps.end());
}

void StreamProcessingEngine::addInputSource(
    InputSource *inputSource,
    std::vector<Operator*> downstreamOps) {

  // Register the downstream operators with the upstream operator
  inputSource->downstreamOperators = downstreamOps;

  // Register the inputSource with other input sources to be scheduled later
  inputSources.push_back(inputSource);
}

void StreamProcessingEngine::run() {
  // Launch all of the input threads
  inputMonitor = std::thread(&StreamProcessingEngine::launchInputs, this);

  // Schedule the operators where there is Data remaining
  bool stillProcessing = false;
  while (receivingInput || stillProcessing) {
    stillProcessing = false;

    for (auto opPtr : ops) {
      if (opPtr->execute() == true) stillProcessing = true;
    }
  }

  // Clean up the inputMonitor thread
  inputMonitor.join();
}

void StreamProcessingEngine::launchInputs() {
  // Launch threads
  for (auto inputSource : inputSources)
    inputSource->startThread();

  // Join threads
  for (auto inputSource : inputSources)
    inputSource->waitForCompletion();

  // Flag the operator execution code to signal that no more Data is coming
  receivingInput = false;
}