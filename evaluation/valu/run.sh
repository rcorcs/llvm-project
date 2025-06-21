#./${1}.baseline > tmp.1.txt
#./${1}.luslp    > tmp.2.txt
#diff tmp.1.txt tmp.2.txt
echo "==BASELINE"
time ./${1}.baseline >/dev/null
echo "==LV"
time ./${1}.lv >/dev/null
echo "==SLP"
time ./${1}.slp >/dev/null
echo "==DU+SLP"
time ./${1}.du-slp >/dev/null
echo "==VALU+SLP"
time ./${1}.valu >/dev/null
echo "==VALU/S+SLP"
time ./${1}.valu-seeds >/dev/null
