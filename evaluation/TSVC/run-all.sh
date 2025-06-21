for i in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15; do
#for i in 16 17 18 19 20 21 22 23 24 25; do
echo $i
 echo "GCC: $i"
   ./tsvc.std-gcc 1> GCC.${i}.txt
 echo "STD: $i"
   ./tsvc.std 1> STD.${i}.txt
 echo "STD+VALU/NS: $i"
   ./tsvc.std-valu 1> STD+VALU-NS.${i}.txt
 echo "STD+VALU/S: $i"
   ./tsvc.std-valu-seeds 1> STD+VALU.${i}.txt
 #echo "O3: $i"
 #  ./tsvc.o3novec 1> O3.${i}.txt
 #echo "SLP-NU: $i"
 #  ./tsvc.slp-nounroll 1> SLP-NU.${i}.txt
 echo "DU+SLP: $i"
   ./tsvc.slp-du 1> DU+SLP.${i}.txt
 echo "VALU/NS+SLP: $i"
   ./tsvc.slp-valu 1> VALU-NS+SLP.${i}.txt
 echo "VALU/S+SLP: $i"
   ./tsvc.slp-valu-seeds 1> VALU+SLP.${i}.txt
 echo "LV: $i"
   ./tsvc.vec 1> LV.${i}.txt
done
