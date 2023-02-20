#!/bin/bash

start=$(date +%s) # 记录开始时间
for (( i=1; i<=100000; i++ ))
do
    ./a.out # 运行C程序
done
end=$(date +%s) # 记录结束时间

runtime=$((end-start)) # 计算总运行时间
echo "Total runtime: $runtime seconds"
