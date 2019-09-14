# 10x Identity Operators w/ 1 Million Tuples
This is a single identity operator that only emits data. The second operator is 
an output operator which prints every 100,000th tuple to indicate progress 
towards completion of the test. This should roughly gives us an idea of how 
many tuples we can process per second. While it would be nice to perform this 
test with only a single operator, we at least need to measure the costs 
associated with emit hence the two operators.

### Operator Workflow
  1) Receive data and emit the same data (the *identity* operator)
  2) Output (print) tuples

### Input data format
Format: *Empty Data File*

### Output format
Format: ```integer```
  * **integer:** Periodic values up to 1,000,000
