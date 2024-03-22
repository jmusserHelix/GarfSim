#!/bin/csh

setenv DCSimNtrack 100
setenv DCSimtrackx 6.0
setenv DCSimtrackang 0.0

rm  signalBe6.out
setenv DCSimOutFile "signalBe6.root"
taskset 0x20 ./smalljet >> signalBe6.out &

rm  signalBe7.out
setenv DCSimOutFile "signalBe7.root"
taskset 0x40 ./smalljet >> signalBe7.out &

rm  signalBe8.out
setenv DCSimOutFile "signalBe8.root"
taskset 0x80 ./smalljet >> signalBe8.out &

rm  signalBe9.out
setenv DCSimOutFile "signalBe9.root"
taskset 0x100 ./smalljet >> signalBe9.out &

rm  signalBe10.out
setenv DCSimOutFile "signalBe10.root"
taskset 0x200  ./smalljet >> signalBe10.out &

rm  signalBe11.out
setenv DCSimOutFile "signalBe11.root"
taskset 0x400 ./smalljet >> signalBe11.out &

rm  signalBe12.out
setenv DCSimOutFile "signalBe12.root"
taskset 0x800 ./smalljet >> signalBe12.out &

rm  signalBe13.out
setenv DCSimOutFile "signalBe13.root"
taskset 0x1000 ./smalljet >> signalBe13.out &

rm  signalBe14.out
setenv DCSimOutFile "signalBe14.root"
taskset 0x2000 ./smalljet >> signalBe14.out &

rm  signalBe15.out
setenv DCSimOutFile "signalBe15.root"
taskset 0x4000 ./smalljet >> signalBe15.out &



