// Copyright 2019 [BVU CMSC491 class]
#ifndef OPERATORS_INPUTSOURCE_H_
#define OPERATORS_INPUTSOURCE_H_

#include <thread>  // NOLINT
#include "Operators/Emitter.h"

/**
 * An abstract class that application programmers can use to insert data into
 * the workflow of operators.
 *
 * This class manages via \ref startThread() and \ref waitForCompletion()
 * threads that manage the lifetime of the InputSource. This works for both
 * batch/finite data as well as streaming/infinite data.
 */
template<typename T>
class InputSource : public Emitter<T> {
 public:
   InputSource();
  /**
   * A user-defined, virtual function whose later implementation will dictate
   * what data an InputSource subclass will provide to the workflow.
   *
   * The application programmer is free to include any kind of logic they wish
   * to provide the workflow with Data. For example, the user may read data
   * from a file on disk, a network socket, a pipe, `STDIN`, procedurally
   * generate data, or read from any `FILE*` or file descriptor they care to.
   *
   * The application programmer simply needs to call \ref emit()
   *
   * An example subclass follows which illustrates an implemented \ref
   * generateData() method used to produce and \ref emit() Data items.
   *
   * @code
   *   class NumberGenerator : public InputSource {
   *     void generateData() {
   *       for (int i = 1; i < 10; i += 2) {
   *         emit(Data(std::to_string(i)));
   *       }
   *     }
   *   };
   * @endcode
   */
  virtual void generateData() = 0;

  void execute();

  /// Launches the \ref inputThread that executes \ref generateData()
  void startThread();

  /// Calls `join()` on the \ref inputThread
  void waitForCompletion();

 private:
  /// The `std::thread` which executes \ref generateData()
  std::thread inputThread;
};

#endif  // OPERATORS_INPUTSOURCE_H_
