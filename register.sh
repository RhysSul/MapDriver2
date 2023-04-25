# !/bin/bash

# TODO: Move into make file

sudo rmmod ./ascii.ko
sudo rm -rf /dev/asciimap
make all
sudo make register
sudo mknod -m 666 /dev/asciimap c 236 1