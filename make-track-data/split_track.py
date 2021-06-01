import sys
import json
from pathlib import Path
import gpxpy
import geopy.distance

def main():

    # parse command line arguments
    
    if (len(sys.argv) != 3):
        print("Usage: split_track.py <gpx> <num_segments>")
        exit(1)
    
    gpx_filepath = sys.argv[1]
    num_segments = int(sys.argv[2])

    print(f'GPX: "{gpx_filepath}"')
    print(f'num_segments: {num_segments}')

    # parse gpx file

    with open(gpx_filepath, 'r', encoding='utf-8') as f:
        gpx = gpxpy.parse(f)
    
    gpx_name = Path(gpx_filepath).stem

    # make data

    track_data = {
        'trackName': '',
        'trackBaseName': gpx_name,
        'distance': 0,
        'numSegments': 0,
        'segments': []
    }

    # find movements

    movements = []

    for track in gpx.tracks:
        for segment in track.segments:
            print(f'num points: {len(segment.points)}')
            for i in range(1, len(segment.points)):
                point_a = segment.points[i - 1]
                point_b = segment.points[i]
                distance = geopy.distance.distance((point_a.latitude, point_a.longitude), (point_b.latitude, point_b.longitude)).km
                elevation_diff = point_b.elevation - point_a.elevation
                movements.append({
                    'point_a': point_a, 'point_b': point_b, 'distance': distance, 'elevation_diff': elevation_diff
                })
    
    # split to segments

    total_distance = 0
    for movement in movements:
        total_distance += movement['distance']

    distance_per_segment = total_distance / num_segments
    print(f'distance_per_segment: {distance_per_segment}')

    current_segment = []
    current_distance = 0

    for movement in movements:

        current_segment.append(movement)
        current_distance += movement['distance']

        if current_distance >= distance_per_segment or movement == movements[-1]:
            total_climb = 0
            total_downhill = 0
            for m in current_segment:
                if m['elevation_diff'] >= 0:
                    total_climb += m['elevation_diff']
                else:
                    total_downhill -= m['elevation_diff']

            track_data['segments'].append({
                'segmentId': len(track_data['segments']),
                'distance': round(current_distance, 3),
                'totalClimb': round(total_climb, 3),
                'totalDownhill': round(total_downhill, 3),
                'elevationDiff': round(total_climb - total_downhill, 3)
            })
            track_data['distance'] += current_distance
            current_segment = []
            current_distance = 0
    
    # complete track data

    track_data['distance'] = round(total_distance, 3)
    track_data['numSegments'] = len(track_data['segments'])
    track_data['trackName'] = f'{gpx_name}_{track_data["numSegments"]}'

    # save track data

    with open(f'{track_data["trackName"]}.json', 'w', encoding='utf-8') as f:
        json.dump(track_data, f, indent=2)

    # print statictics

    track_data['segments'] = '...(omit)' # omit the long output
    print(json.dumps(track_data, indent=2))

if __name__ == '__main__':
    main()
