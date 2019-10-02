// Copyright 2019 [BVU CMSC491 class]
#include "Engine/StreamProcessingEngine.h"

/*
template<typename A, typename B>
void StreamProcessingEngine::connectOperators(
    Operator<A,B>* upstreamOp, std::vector<Acceptor<B>*> downstreamOps) {
  // Register the downstream operators with the upstream operator
  upstreamOp->downstreamOperators = downstreamOps;

  // Add these operators to our running set of all workflow operators which
  // will be later used by the operator scheduler
  ops.insert(upstreamOp);
  for (auto a : downstreamOps) {
    ops.insert(a->executor);
  }
}
*/

/*
template<typename A>
void StreamProcessingEngine::addInputSource(
    InputSource<A>* inputSource, std::vector<Acceptor<A>*> downstreamOps) {
  // Register the downstream operators with the upstream operator
  inputSource->downstreamOperators = downstreamOps;

  // Register the inputSource with other input sources to be scheduled later
  inputSources.push_back(inputSource);
}
*/

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
