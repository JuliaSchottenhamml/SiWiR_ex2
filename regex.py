import numpy as np
import commands as subprocess
import re
import os

cores = range(1,33)
sizes = [32, 33, 1024, 1025, 2048, 2049]

pattern1 = r'time,([\d\.e+-]*)'
pattern2 = r'residuum,([\d\.e+-]*)'

def	extract(input, pattern):
	return re.search(pattern, output).group(1)
	
result = []

for s in sizes:
	for c in cores:
		print("{0}   {1}".format(s,c))
		os.environ["OMP_NUM_THREADS"] = str(c)
		cpus = "0"
		for i in range(1,c):
			cpus += ", {0}".format((i*8)%32 + (i//4))
		#print(cpus)
		os.environ["GOMP_CPU_AFFINITY"] = cpus
		output = subprocess.getoutput("./rbgs {0} {0} 500".format(s))
		#print(output)
		#print(float(extract(output, pattern2)))
		result.append([s, c, float(extract(output, pattern1)), float(extract(output, pattern2))])
		print(result[-1])

np.savetxt("data/result.txt", np.array(result))
