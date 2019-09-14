# 10x Identity Operators w/ 1 Million Tuples
This is a serial chain of 10 identity operators that only emit data. The final 
operator is an output operator which prints every 100,000th tuple to indicate 
progress towards completion of the test.  The idea is that we have a workflow 
of decent length so that we can incur primarily overhead from our system to 
measure.

### Operator Workflow
  1-10) Receive data and emit the same data (the *identity* operator)
  11) Output (print) tuples

### Input data format
Format: *Empty Data File*

### Output format
Format: ```integer```
  * **integer:** Periodic values up to 1,000,000
