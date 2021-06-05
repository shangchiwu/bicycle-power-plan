import shutil
import subprocess
import os
import random
import json
import time

PROGRAM_LOCATION = 'build/debug/BicyclePowerPlan.exe'
EXPERIMENT_DATA_LOCATION = "./datasets/"
PLAN_LIST = ['plan_merida_hard_50.json', 'plan_merida_hard_10.json'] # 'plan_river_easy.json',

TIMES = 40

CPU_AMOUNT = 12

OUTPUT_DATA_PATH = "./notebook/data/"

# Expect file layout
# - data
# |- tai_50_5_1
#  |- GA_with_LOC_FPS
#   |- statistic
#   |- encoding
# |- tai_50_10_1
#  |- GA_with_LOC_FPS
#   |- statistic
#   |- encoding

process_dict = {}

def find_valid_cpu():
    i = 1
    while True:
        if i not in process_dict:
            return i

        if process_dict[i].poll() == None:
            time.sleep(0.02)
        else:
            del process_dict[i]
            return i

        if i == CPU_AMOUNT-1:
            i = 1
            continue

        i += 1
    return random.randint(1, CPU_AMOUNT-1)

def execute_program_with_json(config_dict):
    cpu_to_run = find_valid_cpu()

    config_file_name = "config_" + str(cpu_to_run) + ".json"
    with open(config_file_name, "w") as outfile:
        json.dump(config_dict, outfile)

    process_dict[cpu_to_run] = subprocess.Popen(PROGRAM_LOCATION + " " + config_file_name +" " + str(cpu_to_run))
#    subprocess.run([PROGRAM_LOCATION, 'config.json'])  # doesn't capture output

def experiment_with_basic_config(experiment_path, basic_config_dict):
    statistic_output_path = os.path.join(experiment_path, "statistic")
    encoding_output_path = os.path.join(experiment_path, "encoding")
    if not os.path.exists(statistic_output_path):
        os.mkdir(statistic_output_path)
    if not os.path.exists(encoding_output_path):
        os.mkdir(encoding_output_path)

    for i in range(TIMES):
        config_dict = basic_config_dict.copy()
        config_dict["statistic_filename"] = os.path.join(statistic_output_path, (str(i+1) + ".csv"))
        config_dict["output_encoding_filename"] = os.path.join(encoding_output_path, (str(i+1) + ".enc"))
        config_dict["seed"] = random.randint(0, 2147483647)
        execute_program_with_json(config_dict)

def setup_basic_config(experiment_setup_path, experiment_code, basic_config_dict):
    experiment_code_path = os.path.join(experiment_setup_path, experiment_code)
    if not os.path.exists(experiment_code_path):
        os.makedirs(experiment_code_path)

    experiment_with_basic_config(experiment_code_path, basic_config_dict)

if __name__ == "__main__":

    for experiment_filename in PLAN_LIST:
        experiment_setup_name = experiment_filename[:experiment_filename.rfind('.')]
        experiment_setup_path = os.path.join(OUTPUT_DATA_PATH, experiment_setup_name)
        if not os.path.exists(experiment_setup_path):
            os.makedirs(experiment_setup_path)

        # 1st experiment
        # What you need to modify is..
        # ---------- MODIFY part START --------------
        experiment_code = "TEST_CONFIG"
        basic_config_dict = {
            "bicyclePowerPlanLocation": os.path.join(EXPERIMENT_DATA_LOCATION, experiment_filename),
            "parentPopulationSize": 64,
            "selectPopulationSize": 1,
            "offspringPopulationSize": 128,
            "iteration": 10000,
            "tauFactor": 8.0,
            "tauPrimeFactor": 8.0,
            "epsilonFactor": 0.3
            # Optional config if you want to constraint the evaluation time
            # "constraint_evaluation_time": 1000,
            # Optional config if you want to constraint the computation time(in micro seconds)
            # "constraint_computation_time": 10000,
        }
        # ---------- MODIFY part END --------------
        # Call this if you've setup the experiment_code and basic_config_dict to perform experiment
        setup_basic_config(experiment_setup_path, experiment_code, basic_config_dict)
