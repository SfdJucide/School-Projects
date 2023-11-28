#!/usr/bin/bash

scp -o StrictHostKeyChecking=no -P 2222 src/cat/s21_cat src/grep/s21_grep lostwill@192.168.50.110:/usr/local/bin

