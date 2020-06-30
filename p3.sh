p.sh $1
cat words/oxford_3000_keys/oxford3000_uniq.txt | run.sh -fmu | grep $1 
