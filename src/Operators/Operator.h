// Copyright 2019 [BVU CMSC491 class]
#ifndef OPERATORS_OPERATOR_H_
#define OPERATORS_OPERATOR_H_

#include <deque>
#include "Data/Data.h"
#include "Operators/Emitter.h"
#include "Operators/Acceptor.h"

/**
 * An abstract class that denotes the interconnectivity of user-defined
 * operators within our stream processing engine workflow.
 *
 * The @b Operator class supports being scheduled for execution via execute()
 * and ultimately produces Data via the emit() method.  Application programmers
 * are meant to implement the processData() method which defines how the
 * user-defined operator intends to process data.
 */
template<typename A, typename B>
class Operator : public Emitter<B> {
 public:
  /// The default constructor for an Operator that does not use a sliding
  /// window.
  Operator() {
    this->range = 1;
    this->slide = 1;

    acceptor.executor = this;
  }

  /**
   * The constructor for an Operator that uses a sliding window. If this
   * constructor is used, then processData(deque<Data>) will be called during
   * execture rather than processData(Data)
   */
  explicit Operator(int range, int slide) {
    this->range = range;
    this->slide = slide;

    acceptor = Acceptor<A>(this);
  }

  /**
   * The operator scheduler invokes this method to have an operator process
   * data.
   *
   * This method retrieves a Data object from its `input` queue and passes it
   * on to the processData() method where a result Data object may or may not
   * be produced.
   */
  void execute();

  /**
   * A user-defined, virtual function whose later implementation will dictate
   * how an Operator subclass is intended to process data.
   *
   * The application programmer is free to include any kind of processing logic
   * within this method and free to maintain their own state within their
   * subclass as well. Should processing data result in the production of
   * output items from the operator, then the programmer may pass their Data
   * objects to the emit() method. They may even emit() multiple items, should
   * it be necessary.
   *
   * @param data A Data object representing the data to be processed.
   *
   * An example subclass follows which illustrates an implemented `processData`
   * method used to increment data values:
   * @code
   *   class IncrementOp : public Operator {
   *    public:
   *     void processData(Data data) {
   *       int num = std::stoi(data.value);  // Retrieve number in int form
   *       num++;                            // Increment number
   *       data.value = std::to_string(num); // Store number back in data
   *       emit(data);                       // Emit data as output
   *     }
   *   };
   * @endcode
   *
   * Note that in the example above, the Data object `data` was reused as it
   * was passed to the emit() method. This is not a requirement but happened to
   * preserve the internal `timestamp` variable. We could have crafted a new
   * Data object with a matching `timestamp` in the following way:
   *
   * @code
   *   Data outputData(std::to_string(num), data.timestamp);
   *   emit(outputData);
   * @endcode
   */
  virtual void processData(Data<A> data) {}
  virtual void processData() {}

  /// The range of the window, defaults to 1 if not specified
  int range;

  /// The slide of the window, defaults to 1 if not specified
  int slide;

  /** The buffer for the window. It is loaded on the first
   * executed and updated each execute thereafter
   */
  std::deque<Data<A>> window;

  Acceptor<A> acceptor;
};

#endif  // OPERATORS_OPERATOR_H_
