// Copyright 2019 [BVU CMSC491 class]
#ifndef OPERATORS_EXECUTOR_H_
#define OPERATORS_EXECUTOR_H_

class Executor {
  public:
    int numItems;
    virtual void execute() = 0;

    virtual void startThread() {};
    virtual void waitForCompletion() {};


};

#endif  // OPERATORS_EXECUTOR_H_
