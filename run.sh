#!/bin/sh

export LD_LIBRARY_PATH=$(pwd)/my_lib:$LD_LIBRARY_PATH
./imageplayer
