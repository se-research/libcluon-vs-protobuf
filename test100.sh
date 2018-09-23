#!/bin/sh

:>log
for i in `seq 1 100`; do
    sleep 1; sudo make test | tee -a log
done

cat log | grep "libcluon encoding" | cut -f2 -d":" > lce
cat log | grep "libcluon decoding" | cut -f2 -d":" > lcd
cat log | grep "Protobuf encoding" | cut -f2 -d":" > pbe
cat log | grep "Protobuf decoding" | cut -f2 -d":" > pbd

gnuplot chart.gnuplot
