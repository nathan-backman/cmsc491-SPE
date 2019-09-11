# Temperature Sensor Test
A contrived sample application that takes in temperature data from multiple 
"sites", drops certain sites, and for the tuples produces from the remainder, 
determines the temperatures distances from freezing (32.0F) and outputs only 
those within 10 degrees.

Ultimately this contrived sample performs 2 filters and 1 transformation.

### Operator Workflow
  1) Filter out (remove) tuples from odd sensor sites (Ex. remove "site_3")
  2) Transform temperatures to show their distance from freezing (32.0F)
  3) Filter out tuples with temps less than -10.0 or greater than +10.0
  4) Output (print) tuples

### Input data format
Format: ```siteID    temp```
  * **siteID:** In the format: ```"site_%d"```
  * **temp:** In the format: ```"%.1f"``` in range [-50, 120]

### Output format
Format: ```siteID    temp```
  * **siteID:** In the format: ```"site_%d"```
  * **temp:** In the format: ```"%+.1f"``` in range [-10, 10]
