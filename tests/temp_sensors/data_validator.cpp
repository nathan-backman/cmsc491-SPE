#include <iostream>
#include <string>
#include <sstream>


int main() {

  std::string line;
  while(getline(std::cin, line)) {
    line[4] = ' ';
    std::istringstream is(line);
    std::string s;
    int sensID;
    float temp;
    is >> s >> sensID >> temp;
    if (sensID%2 == 0) {
      float t = temp - 32.0;
      if (t <= 10.0 && t >= -10.0) {
        printf("site_%d %+.1f\n", sensID, t);
      }
    }
  }

  return 0;
}
