#!/bin/csh

setenv DCSimNtrack 100
setenv DCSimtrackx -7.0

#setenv DCSimtrackang 0.6
#setenv DCSimRootfile DCsimX7p0A0p6.root
#./smalljet -b

setenv DCSimtrackang 0.7
setenv DCSimRootfile DCsimX7p0A0p7.root
./smalljet -b

setenv DCSimtrackang 0.8
setenv DCSimRootfile DCsimX7p0A0p8.root
./smalljet -b

setenv DCSimtrackang 0.9
setenv DCSimRootfile DCsimX7p0A0p9.root
./smalljet -b

setenv DCSimtrackang 1.0
setenv DCSimRootfile DCsimX7p0A1p0.root
./smalljet -b

