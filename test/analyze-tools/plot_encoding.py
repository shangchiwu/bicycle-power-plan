import json
from pathlib import Path
import pandas as pd
import matplotlib.pyplot as plt

NUM_TEST = 40
TEST_ID = [x for x in range(1, NUM_TEST + 1)]
ENCODING_DIR = 'encoding'
TRACK_FILE = 'track.json'

IMAGE_DIR = Path('image').joinpath('image_encoding')
DPI = 300

def plot_onefile(track: dict, plan_name: str, test_id: int):

    # read segmenmts

    segments = track['segments']
    segment_id = [s['segmentId'] for s in segments]
    elevation = [s['elevation'] for s in segments]
    wind_speed = [s['windSpeed'] for s in segments]

    # read encoding

    encoding_filename = Path(ENCODING_DIR).joinpath(f'{test_id}.enc')
    with open(encoding_filename, 'r', encoding='utf-8') as f:
        encoding = [float(x) * 100 for x in f.read().split()]

    # plot image

    title = 'Encoding of "%s" (%d)' % (plan_name, test_id)

    fig, host = plt.subplots(figsize=(10, 5))
    plt.title(title)

    par1 = host.twinx()
    par2 = host.twinx()

    host.set_xlabel('Segment')
    host.set_ylabel('Elevation (m)')
    par1.set_ylabel('Wind Speed (m/s)')
    par2.set_ylabel('Encoding (Power Ratio) (%)')

    host.xaxis.set_ticks(segment_id)
    par2.yaxis.set_ticks([x for x in range(0, 101, 10)])
    host.set_ylim(min(elevation), min(elevation) + 3 * (max(elevation) - min(elevation)))
    par1.set_ylim(4 * min(wind_speed), 4 * max(wind_speed))
    par2.set_ylim(0, 100)

    host.fill_between(segment_id, elevation, y2=min(elevation), label='Elevation', color='#8f8')
    line_elevation, = host.plot(segment_id, elevation, label='Elevation', color='#8f8')
    line_wind_speed, = par1.plot(segment_id, wind_speed, label='Wind Speed', color='#6bf')
    par1.plot(segment_id, [0 for _ in range(len(segment_id))], linestyle='--', color='#6bf', linewidth=1)
    line_encoding, = par2.plot(segment_id, encoding, label='Encoding', color='red', linewidth=2)

    host.legend(handles=[line_elevation, line_encoding, line_wind_speed], loc='upper left')

    par2.spines['right'].set_position(('outward', 60))

    fig.tight_layout()

    # save image

    image_filename = Path(IMAGE_DIR).joinpath('encoding_%s_%d.png' % (plan_name, test_id))
    plt.savefig(image_filename, dpi=DPI)
    plt.close('all')

def main():

    # make folder
    Path(IMAGE_DIR).mkdir(parents=True, exist_ok=True)

    with open('title.txt', 'r', encoding='utf-8') as f:
        title = f.read()

    # read track data

    with open(TRACK_FILE, 'r', encoding='utf-8') as f:
        track = json.load(f)

    n = NUM_TEST
    count = 0
    for test_id in TEST_ID:
        print('[%d/%d] (%d%%) plotting "%s" (%d) ... ' % (count + 1, n, ((count + 1) / n) * 100, title, test_id))
        plot_onefile(track, title, test_id)
        count += 1

if __name__ == '__main__':
    main()
