// Copyright 2019 [BVU CMSC491 class]
#ifndef ENGINE_STREAMPROCESSINGENGINE_H_
#define ENGINE_STREAMPROCESSINGENGINE_H_

#include <set>
#include <thread>  // NOLINT
#include <vector>
#include "Data/Data.h"
#include "Operators/InputSource.h"
#include "Operators/Operator.h"

/**
 * This class facilitates the execution of the stream processing environment.
 *
 * Ultimately, this class manages the workflow of \ref Operator and \ref
 * InputSource references and schedules the stream operators and input source
 * threads as well.  By calling \ref run(), this entire process starts and
 * continues as long as the input sources provided data or there is data in the
 * pipeline.
 */
class StreamProcessingEngine {
 public:
  /**
   * Registers a connection between an Operator and any downstream
   * <span>Operator</span>s that it is connected to.
   *
   * The method also registers each operator within the \ref ops private member
   * so that they are accessible to the internal operator schedule.
   *
   * @param upstreamOp A reference to the upstream Operator in the defined
   * relationship
   *
   * @param downstreamOps A vector of references to any immediate downstream
   * <span>Operator</span>s that are connected to the upstream Operator
   */
  void connectOperators(Operator* upstreamOp,
                        std::vector<Operator*> downstreamOps);

  /**
   * Registers a connection between an InputSource and any downstream Operator
   * that it is connected to.
   *
   * The method also registers the InputSource within the \ref inputSources
   * private member so that their threads can be launched later before we begin
   * scheduling <span>Operator</span>s.
   *
   * @param upStreamOp A reference to the InputSource in the defined
   * relationship
   *
   * @param downstreamOps A vector of references to any immediate downstream
   * <span>Operator</span>s that are connected to the InputSource
   */
  void addInputSource(InputSource* inputSource,
                      std::vector<Operator*> downstreamOps);

  /**
   * Starts execution of the workflow.
   *
   * This involves launching threads for each InputSource as well as beginning
   * execution of the <span>Operator</span>s.
   */
  void run(int numThreads);

 private:
  /**
   * Starts execution of the user-provided InputSource code.
   *
   * This method launches a thread for each of the InputSource references
   * stored within \ref inputSources.
   */
  void launchInputs();
  void threadRunner();

  /// A collection of references to each Operator in the workflow
  std::set<Operator*> ops;

  /// A collection of references to each InputSource in the workflow
  std::vector<InputSource*> inputSources;

  /// Thread that starts & joins the <span>InputSource</span> threads.
  std::thread inputMonitor;

  /// Guard condition that's set to false when <span>InputSource</span>s finish
  bool receivingInput = true;

  std::vector<std::thread> workerThreads;
};

#endif  // ENGINE_STREAMPROCESSINGENGINE_H_
