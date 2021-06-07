from pathlib import Path
import pandas as pd
import matplotlib.pyplot as plt

NUM_TEST = 40
TEST_ID = [x for x in range(1, NUM_TEST + 1)]
DATA_DIR = 'data'

IMAGE_DIR = Path('image').joinpath('image_itertime')
DPI = 300

def plot_onefile(plan_name: str, test_id: int):
    data_filename = Path(DATA_DIR).joinpath(f'itertime_{test_id}.csv')

    data = pd.read_csv(data_filename)
    data_x = data['iteration']
    data_y = data['evaluation_value']

    # plot image
    
    image_filename = Path(IMAGE_DIR).joinpath('itertime_%s.png' % plan_name)
    title = 'Riding Time of "%s" (%d)' % (plan_name, test_id)

    plt.figure()
    plt.title(title)
    plt.xlabel('Iteration')
    plt.ylabel('Evaluation Value')
    plt.plot(data_x, data_y)
    plt.savefig(image_filename, dpi=DPI)
    plt.close('all')

    # plot log image
    
    image_filename_log = Path(IMAGE_DIR).joinpath('itertime_%s_%d_log.png' % (plan_name, test_id))
    title_log = 'Riding Time of "%s" (%d) (log scale)' % (plan_name, test_id)

    plt.figure()
    plt.title(title_log)
    plt.xlabel('Iteration')
    plt.ylabel('Evaluation Value')
    plt.xscale('log')
    plt.plot(data_x, data_y)
    plt.savefig(image_filename_log, dpi=DPI)
    plt.close('all')

def main():

    # make folder
    Path(IMAGE_DIR).mkdir(parents=True, exist_ok=True)

    with open('title.txt', 'r', encoding='utf-8') as f:
        title = f.read()

    n = NUM_TEST
    count = 0
    for test_id in TEST_ID:
        print('[%d/%d] (%d%%) plotting "%s" (%d) ... ' % (count + 1, n, ((count + 1) / n) * 100, title, test_id))
        plot_onefile(title, test_id)
        count += 1

if __name__ == '__main__':
    main()
