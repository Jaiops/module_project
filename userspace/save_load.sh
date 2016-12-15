#!/bin/bash
sudo rmmod lkm_writer
cd ..
cd kernelspace
sudo insmod lkm_writer.ko
cd ..
cd userspace
./test_set
./lkm_script.sh save olle.txt
sudo rmmod lkm_writer
cd ..
cd kernelspace
sudo insmod lkm_writer.ko
cd ..
cd userspace
./lkm_script.sh load olle.txt
./test_get
rm olle.txt
