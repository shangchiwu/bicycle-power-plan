#ifndef BICYCLEPOWERPLAN_TRACK_H
#define BICYCLEPOWERPLAN_TRACK_H

#include <string>

class Segment {
public:
    Segment();
    Segment(const Segment &other) = default;

    int m_segmentId;
    float m_distance;       // unit in (m)
    float m_totalClimb;     // unit in (m)
    float m_totalDownhill;  // unit in (m)
    float m_elevationDiff;  // unit in (m)
    float m_elevation;      // unit in (m)
    float m_temperature;    // unit in (*C)
    float m_windSpeed;      // unit in (m/s)
};

class Track {
public:
    Track();
    Track(const Track &other);
    ~Track();
    
    bool readConfig(const std::string &configFilePath);

    const Segment &operator[](int index) const;
    Segment &operator[](int index);

    std::string m_trackName;
    std::string m_trackBaseName;
    int m_numSegments;
    float m_distance;

private:
    Segment *m_segments;
};

#endif // BICYCLEPOWERPLAN_TRACK_H
