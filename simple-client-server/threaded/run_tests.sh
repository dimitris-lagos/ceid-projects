#!/bin/bash
for iter in {1..10}
do
./client $iter &
done

for iter in {11..20}
do
./client $iter &
done

for iter in {21..30}
do
./client $iter &
done


exit