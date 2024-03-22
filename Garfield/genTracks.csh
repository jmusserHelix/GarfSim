#!/bin/csh

setenv DCSimNtrack 100
setenv DCSimtrackx 6.0
setenv DCSimtrackang 0.0

rm  Besignal1.out
setenv DCSimOutFile "Besignal1.root"
taskset 0x1 ./smalljet >> Besignal1.out &

rm  Besignal2.out
setenv DCSimOutFile "Besignal2.root"
taskset 0x2 ./smalljet >> Besignal2.out &

rm  Besignal3.out
setenv DCSimOutFile "Besignal3.root"
taskset 0x4 ./smalljet >> Besignal3.out &

rm  Besignal4.out
setenv DCSimOutFile "Besignal4.root"
taskset 0x8 ./smalljet >> Besignal4.out &

rm  Besignal5.out
setenv DCSimOutFile "Besignal5.root"
taskset 0x10 ./smalljet >> Besignal5.out &



