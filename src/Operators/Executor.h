// Copyright 2019 [BVU CMSC491 class]
#ifndef OPERATORS_EXECUTOR_H_
#define OPERATORS_EXECUTOR_H_

class Executor {
  public:
    int numItems;

    /**
     * The operator scheduler invokes this method to have an operator process
     * data.
     *
     * This method retrieves a Data object from its `input` queue and passes it
     * on to the processData() method where a result Data object may or may not
     * be produced.
     */
    virtual void execute() {};

    virtual void startThread() {};
    virtual void waitForCompletion() {};


};

#endif  // OPERATORS_EXECUTOR_H_
