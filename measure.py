import time
import os

def measure_program_seconds(program: str):
	program_full_path = "./build/" + program

	if os.path.exists(program_full_path) is False:
		print("Error: program", program_full_path, "does not exist")
		exit()

	start_time = time.time()
	os.system(program_full_path)
	return time.time() - start_time

def save_measures(file_name: str, measures: list[float]):
	with open(file_name, "w") as txt_file:
		for measure in measures:
			txt_file.write(str(measure) + "\n")

programs_to_measure = ["initial", "wReserve", "wEncode", "wCustomString", "wModuleOpt", "wLoopUnroll", "wToString", "wToString2", "wToString3", "wMultiThread"]

for program in programs_to_measure:
	measures = []
	for i in range(20):
		measures.append(measure_program_seconds(program))
	save_measures("./performance-tests/" + program + ".time", measures)