#!/bin/bash

EXE_PATH=../../build/tmp/test
${EXE_PATH}/test_worker "node1" &
${EXE_PATH}/test_worker "node2" &
${EXE_PATH}/test_worker "node3" &