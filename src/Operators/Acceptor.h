// Copyright 2019 [BVU CMSC491 class]
#ifndef OPERATORS_ACCEPTOR_H_
#define OPERATORS_ACCEPTOR_H_

#include <mutex>  // NOLINT
#include <queue>

template<typename T>
class Acceptor {
  public:
  /**
   * A wrapper around `input.push(data)` that protects the \ref input queue
   * with a `std::mutex`.
   *
   * @param data The Data object to add to the \ref input queue
   */
    std::mutex inputMutex;
    Executor* executor;

    Acceptor() {}

    Acceptor(Executor* p) {
      executor = p;
    }

    std::queue<Data<T>> input;

    void addData(Data<T> data) {
      // Basic mutex protection when adding to the queue
      inputMutex.lock();
      input.push(data);
      inputMutex.unlock();
    }
};

#endif  // OPERATORS_ACCEPTOR_H
