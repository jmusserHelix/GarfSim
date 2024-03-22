#!/bin/csh

setenv DCSimNtrack 20
setenv DCSimtrackx 6.0
setenv DCSimtrackang 0.0

rm  Besignal6.out
setenv DCSimOutFile "Besignal6.root"
taskset 0x20 ./smalljet >> Besignal6.out &

rm  Besignal7.out
setenv DCSimOutFile "Besignal7.root"
taskset 0x40 ./smalljet >> Besignal7.out &

rm  Besignal8.out
setenv DCSimOutFile "Besignal8.root"
taskset 0x80 ./smalljet >> Besignal8.out &

rm  Besignal9.out
setenv DCSimOutFile "Besignal9.root"
taskset 0x100 ./smalljet >> Besignal9.out &

rm  Besignal10.out
setenv DCSimOutFile "Besignal10.root"
taskset 0x200  ./smalljet >> Besignal10.out &

rm  Besignal11.out
setenv DCSimOutFile "Besignal11.root"
taskset 0x400 ./smalljet >> Besignal11.out &

rm  Besignal12.out
setenv DCSimOutFile "Besignal12.root"
taskset 0x800 ./smalljet >> Besignal12.out &

rm  Besignal13.out
setenv DCSimOutFile "Besignal13.root"
taskset 0x1000 ./smalljet >> Besignal13.out &

rm  Besignal14.out
setenv DCSimOutFile "Besignal14.root"
taskset 0x2000 ./smalljet >> Besignal14.out &

rm  Besignal15.out
setenv DCSimOutFile "Besignal15.root"
taskset 0x4000 ./smalljet >> Besignal15.out &



