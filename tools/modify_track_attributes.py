import sys
import json
from pathlib import Path

def main():

    # parse command line arguments
    
    if (len(sys.argv) != 2):
        print(f'Usage: {Path(sys.argv[0]).name} <track_file_json>')
        exit(1)
    
    track_filepath = sys.argv[1]

    print(f'track_file: "{track_filepath}"')

    # parse track file

    with open(track_filepath, 'r', encoding='utf-8') as f:
        track_data = json.load(f)

    print(f'trackName:     \'{track_data["trackName"]}\'')
    print(f'trackBaseName: \'{track_data["trackBaseName"]}\'')
    print(f'distance:      {track_data["distance"]}')
    print(f'numSegments:   {track_data["numSegments"]}')

    # modify track

    print('usage: <func> [<first=0> <last=n-1>] <value>')
    print('<func>: (t)emperature, (w)indSpeed, (s)ave, (q)uit')
    print('example: t 25.5    # set temperature to 25.5 for all segment')
    print('example: w 2 4 13  # set wind speed to 13 for segment 2~4')

    while True:
        print('> ', end='')
        try:
            input_line = input().strip().split()
            if len(input_line) == 0:
                continue
            func = input_line[0].lower()
        except EOFError:
            func = 'q'

        if func == 'q':
            break

        if func == 's':
            with open(track_filepath, 'w', encoding='utf-8') as f:
                json.dump(track_data, f, indent=2)
            continue

        key = {'t': 'temperature', 'w': 'windSpeed'}[func]
        value = float(input_line[-1])
        first = int(input_line[1]) if len(input_line) > 2 else 0
        last = int(input_line[2]) if len(input_line) > 2 else track_data['numSegments'] - 1

        for i in range(first, last + 1):
            track_data['segments'][i][key] = value

    # save track data

    print('save...')

    with open(track_filepath, 'w', encoding='utf-8') as f:
        json.dump(track_data, f, indent=2)

if __name__ == '__main__':
    main()
