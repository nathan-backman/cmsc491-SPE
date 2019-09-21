// Copyright 2019 [BVU CMSC491 class]
#ifndef OPERATORS_EMITTER_H_
#define OPERATORS_EMITTER_H_

#include <vector>
#include "Data/Data.h"

// Forward declartion since Operator includes Emitter
class Operator;

/**
 * Abstract class that provides functionality to Operator and InputSource
 * classes to \ref emit() Data to downstream <span>Operator</span>s.
 */
class Emitter {
 public:
  /**
   * Invoking this method allows the application programmer to produce data
   * from within the Operator::processData and InputSource::generateData
   * methods.
   *
   * A Data object passed to this method will be placed into the input queues
   * of the immediate downstream <span>Operator</span>s.
   *
   * @param data The Data object that the Operator produces as a result of
   * processing its input Data.
   */
  void emit(const Data &data);

  /// A collection of pointers to downstream <span>Operator</span>s.
  std::vector<Operator *> downstreamOperators;
};

#endif  // OPERATORS_EMITTER_H_
