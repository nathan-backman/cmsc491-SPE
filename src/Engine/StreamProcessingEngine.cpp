// Copyright 2019 [BVU CMSC491 class]
#include "Engine/StreamProcessingEngine.h"

void StreamProcessingEngine::connectOperators(
    Operator* upstreamOp, std::vector<Operator*> downstreamOps) {
  // Register the downstream operators with the upstream operator
  upstreamOp->downstreamOperators = downstreamOps;

  // Add these operators to our running set of all workflow operators which
  // will be later used by the operator scheduler
  ops.insert(upstreamOp);
  ops.insert(downstreamOps.begin(), downstreamOps.end());
}

void StreamProcessingEngine::addInputSource(
    InputSource* inputSource, std::vector<Operator*> downstreamOps) {
  // Register the downstream operators with the upstream operator
  inputSource->downstreamOperators = downstreamOps;

  // Register the inputSource with other input sources to be scheduled later
  inputSources.push_back(inputSource);
}

void StreamProcessingEngine::run(int numThreads) {
  // Launch all of the input threads
  inputMonitor = std::thread(&StreamProcessingEngine::launchInputs, this);

  // Create worker Threads
  workerThreads = std::vector<std::thread>(numThreads);
  for(int i=0; i<numThreads; i++) {
    workerThreads[i] = std::thread(&StreamProcessingEngine::threadRunner, this);
  }

  // Clean up the inputMonitor thread
  inputMonitor.join();

  // Clean up workerThreads
  for(int i=0; i<numThreads; i++) {
    workerThreads[i].join();
  }
}

void StreamProcessingEngine::launchInputs() {
  // Launch threads
  for (auto inputSource : inputSources) inputSource->startThread();

  // Join threads
  for (auto inputSource : inputSources) inputSource->waitForCompletion();

  // Flag the operator execution code to signal that no more Data is coming
  receivingInput = false;
}

void StreamProcessingEngine::threadRunner() {
  // Schedule the operators where there is Data remaining
  bool dataInWorkflow = false;
  bool receivedDataThisRound = false;
  do {
    // Try to execute each operator
    for (auto opPtr : ops) {
      opPtr->execute();
    }
    receivedDataThisRound = receivingInput;
    // If the inputs are done, look for leftover data in the workflow
    if (!receivedDataThisRound) {
      dataInWorkflow = false;
      for (auto opPtr : ops) {
        if (!opPtr->input.empty()) {
          dataInWorkflow = true;
          break;
        }
      }
    }
  } while (receivedDataThisRound || dataInWorkflow);
}
