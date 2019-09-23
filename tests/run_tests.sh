#!/bin/bash


echo "Starting Execution of Test Suite"
echo "--------------------------------"

successes=$((0))
failures=$((0))

cd tests
for testDir in */; do
  testName="${testDir%?}"
  echo -n "Running test [$testName]: "

  cd $testName

  ../../build/bin/tests/$testName < input.data > test.output

  #./driver.app < input.data > test.output
  diff output.data test.output > /dev/null
  if [ "$?" -ne "0" ] ; then
    echo -e "\e[1;31mFailed\e[0m"
    failures=$((failures+1))
  else
    echo -e "\e[1;32mPassed\e[0m"
    successes=$((successes+1))
  fi
  rm -f test.output

  cd ..
done

echo "--------------------------------"
echo "Finished Tests"
echo -e "  \e[1;32mPassed $successes\e[0m"
echo -e "  \e[1;31mFailed $failures\e[0m"

