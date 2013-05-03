#!/bin/bash

odir=~/ZMeepOut/
mkdir $odir > /dev/null 2>&1
fname='gausrc_flux_adddrop'
rm $odir$fname-power.dat > /dev/null 2>&1

time ../../zmpimeep $fname $1 
grep flux1: >> $odir$fname-power.dat $odir$fname.log
octave graph_adddrop.m  $odir$fname-power.dat > /dev/null 2>&1
mv graph.png $fname-graph.png > /dev/null 2>&1
