#!/bin/bash
echo "start"
function remod {
	sudo rmmod lkm_writer
	sudo insmod kernelspace/lkm_writer.ko
}
echo "Running unit tests:"
remod
./userspace/testsuit
echo "Unit test done."
read
echo "Running concurrency test."
remod
cd userspace
./test_conc_script.sh
echo "Concurrency test done."
read
cd ..
remod
cd userspace
echo "Running save/load test."
./save_load.sh
echo "save/load test done."

sudo rmmod lkm_writer
