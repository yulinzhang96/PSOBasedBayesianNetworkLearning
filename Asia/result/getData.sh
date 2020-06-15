# !/bin/bash

#提取value
valueFile=value.txt
grep value network.txt > $valueFile
AccuracyFile=Accuracy.txt
grep Accuracy network.txt > $AccuracyFile