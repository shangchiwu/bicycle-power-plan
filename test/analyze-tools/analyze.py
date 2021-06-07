from pathlib import Path
import pandas as pd

NUM_TEST = 40
TEST_ID = [x for x in range(1, NUM_TEST + 1)]
DATA_DIR = 'data'

ANALYZE_DIR = 'analyze'
STATISTICS_CSV = Path(ANALYZE_DIR).joinpath('statistics.csv')

def analyze():

    # read data

    with open('title.txt', 'r', encoding='utf-8') as f:
        plan_name = f.read()

    data = pd.DataFrame()

    n = NUM_TEST
    count = 0
    for test_id in TEST_ID:
        print('[%d/%d] (%d%%) reading "%s" (%d) ... ' % (count + 1, n, ((count + 1) / n) * 100, plan_name, test_id))
        count += 1

        itertime_filename = Path(DATA_DIR).joinpath(f'itertime_{test_id}.csv')
        data_test = pd.read_csv(itertime_filename)
        evaluation_value = data_test.iloc[len(data_test) - 1]['evaluation_value']
        
        exectime_filename = Path(DATA_DIR).joinpath(f'exectime_{test_id}.txt')
        with open(exectime_filename, 'r', encoding='utf-8') as f:
            execution_time = float(f.read())

        row = {
            'test_id': test_id,
            'evaluation_value': evaluation_value,
            'execution_time': execution_time
        }
        data = data.append(row, ignore_index=True)

    data.to_csv(Path(ANALYZE_DIR).joinpath('data.csv'), index=False)

    # sort
    data.insert(0, 'no', range(1, len(data) + 1))
    data.to_csv(Path(ANALYZE_DIR).joinpath('data_sorted.csv'), index=False)
    
    # compute statistics for data
    statistics_series = {
        'evaluation_value_avg': data['evaluation_value'].mean(),
        'evaluation_value_min': data['evaluation_value'].min(),
        'evaluation_value_max': data['evaluation_value'].max(),
        'evaluation_value_std': data['evaluation_value'].std(),
        'execution_time_avg': data['execution_time'].mean(),
        'execution_time_min': data['execution_time'].min(),
        'execution_time_max': data['execution_time'].max(),
        'execution_time_std': data['execution_time'].std()
    }

    statistics = pd.DataFrame([statistics_series])

    print('Save statistics to \"%s\"' % STATISTICS_CSV)
    statistics.to_csv(Path(ANALYZE_DIR).joinpath('statistics.csv'), index=False)

def main():

    # make directory
    Path(ANALYZE_DIR).mkdir(parents=True, exist_ok=True)

    # find best configuration
    analyze()

if __name__ == '__main__':
    main()
