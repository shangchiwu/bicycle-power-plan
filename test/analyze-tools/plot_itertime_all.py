from pathlib import Path
import pandas as pd
import matplotlib.pyplot as plt

NUM_TEST = 40
TEST_ID = [x for x in range(1, NUM_TEST + 1)]
DATA_DIR = 'data'

IMAGE_DIR = 'image'
DPI = 300
PLOT_ = 0.5
PLOT_ALPHA = 1

def plot_itertime_all(plan_name: str):

    # plot image

    image_filename = Path(IMAGE_DIR).joinpath('itertime_%s_all.png' % plan_name)
    title = 'All Riding Time of "%s"' % (plan_name)

    fig, ax = plt.subplots()
    plt.title(title)
    plt.xlabel('Iteration')
    plt.ylabel('Evaluation Value')

    # plot log image

    image_filename_log = Path(IMAGE_DIR).joinpath('itertime_%s_all_log.png' % plan_name)
    title_log = 'All Riding Time of "%s" (log scale)' % (plan_name)

    fig_log, ax_log = plt.subplots()
    plt.title(title_log)
    plt.xlabel('Iteration')
    plt.ylabel('Evaluation Value')
    plt.xscale('log')

    n = NUM_TEST
    count = 0
    for test_id in TEST_ID:
        print('[%d/%d] (%d%%) plotting "%s" (%d) ... ' % (count + 1, n, ((count + 1) / n) * 100, plan_name, test_id))
        count += 1

        data_filename = Path(DATA_DIR).joinpath(f'itertime_{test_id}.csv')

        data = pd.read_csv(data_filename)

        data_x = data['iteration']
        data_y = data['evaluation_value']

        ax.plot(data_x, data_y, alpha=PLOT_ALPHA)
        ax_log.plot(data_x, data_y, alpha=PLOT_ALPHA)

    fig.savefig(image_filename, dpi=DPI)
    fig_log.savefig(image_filename_log, dpi=DPI)
    plt.close('all')

def main():

    # make folder
    Path(IMAGE_DIR).mkdir(parents=True, exist_ok=True)

    with open('title.txt', 'r', encoding='utf-8') as f:
        title = f.read()

    plot_itertime_all(title)

if __name__ == '__main__':
    main()
