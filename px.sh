#!/bin/sh

RUN_FLAGS="-mf"

p.sh $1 
echo $1 | run.sh $RUN_FLAGS  | grep 3T >/dev/null && \
	cat words/oxford_3000_keys/oxford3000_uniq.txt | grep -n $1
echo $1 | run.sh $RUN_FLAGS  | grep 4T >/dev/null && \
	cat words/ielts_general_4000.txt               | grep -n $1
echo $1 | run.sh $RUN_FLAGS  | grep 5T >/dev/null && \
	cat words/toefl_5000.txt                       | grep -n $1
echo $1 | run.sh $RUN_FLAGS  | egrep "xT|yT" >/dev/null && \
	cat words/google-20000-english.txt             | grep -n $1

