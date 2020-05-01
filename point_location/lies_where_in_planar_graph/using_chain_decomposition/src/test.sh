#!/bin/sh
# quick check with log output

make all;

./build/out > ./autotest/current_build_log;

diff ./autotest/stable_build_log ./autotest/current_build_log > ./autotest/build_diff;

if [ -s ./autotest/build_diff ]
then  echo "WARNING, stable build has different log output!"
else
      echo "stable build seems to have the same log output, but you have better implement a better test"
fi

# clean
rm ./build/out ./autotest/current_build_log ./autotest/build_diff;