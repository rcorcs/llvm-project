/usr/bin/time -f "${1},O3,%E" sh -c "./${1}.baseline 1>/dev/null 2>/dev/null"
/usr/bin/time -f "${1},VALU,%E" sh -c "./${1}.valu 1>/dev/null 2>/dev/null"
/usr/bin/time -f "${1},VALU/S,%E" sh -c "./${1}.valu-seeds 1>/dev/null 2>/dev/null"
/usr/bin/time -f "${1},LV,%E" sh -c "./${1}.lv 1>/dev/null 2>/dev/null"
/usr/bin/time -f "${1},U+SLP,%E" sh -c "./${1}.du-slp 1>/dev/null 2>/dev/null"
#for Count in 2 4; do
#/usr/bin/time -f "${1},U${Count}+SLP,%E" sh -c "./${1}.unroll-${Count}-slp 1>/dev/null 2>/dev/null"
#done
/usr/bin/time -f "${1},SLP,%E" sh -c "./${1}.slp 1>/dev/null 2>/dev/null"

