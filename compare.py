import os
import seaborn as sns
import matplotlib.pyplot as plt
import pandas as pd
import numpy as np

def read_measures(file_name: str):
	if os.path.exists(file_name) is False:
		print("Error: file", file_name, " does not exist")
		exit()

	measures = []
	with open(file_name, "r") as txt_file:
		for line in txt_file.readlines():
			measures.append(float(line))	
	return measures

programs_to_measure = ["initial", "wReserve", "wEncode", "wCustomString", "wModuleOpt", "wLoopUnroll", "wToString", "wToString2", "wToString3", "wMultiThread"]

programs_time = {}
for program in programs_to_measure:
	measures = read_measures("./performance-tests/" + program + ".time")
	programs_time[program] = measures
	print("Mean of " + program + ": " + str(np.nanmean(measures)))
df = pd.DataFrame(programs_time)




boxplot = sns.boxplot(data=df)

plt.xlabel('Program')
plt.ylabel('Time in Seconds')
plt.title('Measure FIZZ BUZZ programs')
plt.show()