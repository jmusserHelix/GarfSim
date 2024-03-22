#!/bin/csh

setenv DCSimNtrack 100
setenv DCSimtrackx -7.0

setenv DCSimtrackang 0.1
setenv DCSimRootfile DCsimX7p0A0p1.root
./smalljet

setenv DCSimtrackang 0.2
setenv DCSimRootfile DCsimX7p0A0p2.root
./smalljet

setenv DCSimtrackang 0.3
setenv DCSimRootfile DCsimX7p0A0p3.root
./smalljet

setenv DCSimtrackang 0.4
setenv DCSimRootfile DCsimX7p0A0p4.root
./smalljet

setenv DCSimtrackang 0.5
setenv DCSimRootfile DCsimX7p0A0p5.root
./smalljet

