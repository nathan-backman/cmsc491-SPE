# Temperature Sensor Test
This is the most simple type of application -- the one that does nothing at 
all. Aside from the output operator, there is a single operator that does 
literally nothing at all other than forward the data along. If we fail this 
test then there is something seriously wrong within our infrastructure.

### Operator Workflow
  1) Receive data and emit the same data (the *identity* operator)
  2) Output (print) tuples

### Input data format
Format: ```string```
  * **string:** 10 arbitrary alpha-numeric characters

### Output format
Format: ```string```
  * **string:** 10 arbitrary alpha-numeric characters
