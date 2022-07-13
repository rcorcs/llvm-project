
for N in $(cat BenchNames); do
  echo $N
  #make fm2 2>../tmp.txt 1>/dev/null
  #grep "NumFunctions" ../tmp.txt
  grep "Merged" $N.fm.txt | wc -l
  grep "Merged" $N.fm2.txt | wc -l
done
