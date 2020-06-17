p.sh $1
cat words/toefl_5000.txt | run.sh -fmu | grep $1 
