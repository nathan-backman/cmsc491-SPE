#!/bin/bash

echo "Starting Execution of Test Suite"
echo "--------------------------------"

successes=$((0))
failures=$((0))

cd profiler
for testDir in */; do
  testName="${testDir%?}"
  echo -n "Running test [$testName]: "

  cd $testName
  /usr/bin/time -f "\tUser Time:   %U\n\tSystem Time: %S\n\tWall Clock:  %E" --output=test.time \
    ./driver.app < input.data > test.output
  diff output.data test.output > /dev/null
  if [ "$?" -ne "0" ] ; then
    echo -e "\e[1;31mFailed\e[0m"
    failures=$((failures+1))
  else
    echo -e "\e[1;32mPassed\e[0m"
    successes=$((successes+1))
  fi
  cat test.time
  rm -f test.output test.time

  cd ..
done

echo "--------------------------------"
echo "Finished Tests"
echo -e "  \e[1;32mPassed $successes\e[0m"
echo -e "  \e[1;31mFailed $failures\e[0m"

