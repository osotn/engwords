p.sh $1
cat words/ielts_general_4000.txt | run.sh -fmu | grep $1 
