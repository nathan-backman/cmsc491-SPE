// Copyright 2019 [BVU CMSC491 class]
#include "Engine/StreamProcessingEngine.h"

void StreamProcessingEngine::run() {
  // Launch all of the input threads
  inputMonitor = std::thread(&StreamProcessingEngine::launchInputs, this);

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
        if (opPtr->numItems != 0) {
          dataInWorkflow = true;
          break;
        }
      }
    }
  } while (receivedDataThisRound || dataInWorkflow);

  // Clean up the inputMonitor thread
  inputMonitor.join();
}

void StreamProcessingEngine::launchInputs() {
  // Launch threads
  for (auto inputSource : inputSources) inputSource->startThread();

  // Join threads
  for (auto inputSource : inputSources) inputSource->waitForCompletion();

  // Flag the operator execution code to signal that no more Data is coming
  receivingInput = false;
}
