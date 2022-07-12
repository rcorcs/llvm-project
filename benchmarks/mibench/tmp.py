
import sys
import numpy as np
with open(sys.argv[1]) as f:
    arr = [int(line.split(':')[1].strip()) for line in f if line.startswith('#FSize')]
    print(np.min(arr),np.mean(arr),np.max(arr))
