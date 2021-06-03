#include "Track.h"
#include <iostream>
#include <fstream>
#include <string>
#include "nlohmann/json.hpp"

Segment::Segment() :
        m_segmentId(0), m_distance(0), m_totalClimb(0), m_totalDownhill(0),
        m_elevationDiff(0), m_elevation(0), m_temperature(0), m_windSpeed(0) {}

Track::Track() : m_numSegments(0), m_distance(0), m_segments(nullptr) {}

Track::Track(const Track &other) :
        m_trackName(other.m_trackName), m_trackBaseName(other.m_trackBaseName),
        m_numSegments(other.m_numSegments), m_distance(other.m_distance) {

    m_segments = new Segment[m_numSegments];

    for (int i = 0; i < m_numSegments; ++i) {
        m_segments[i] = other.m_segments[i];
    }
}

Track::~Track() {
    delete [] m_segments;
}

bool Track::readConfig(const std::string &configFilePath) {
    std::ifstream ifs(configFilePath);

    if (!ifs.good()) {
        std::cerr << "Error: Track::readConfig(): faild to read config file \"" <<
                     configFilePath << "\"!" << std::endl;
        return false;
    }

    nlohmann::json config;
    ifs >> config;
    ifs.close();

    m_numSegments = config["numSegments"];
    m_distance = 0;  // compute myself for the accuracy
    m_segments = new Segment[m_numSegments];

    for (int i = 0; i < m_numSegments; ++i) {
        m_segments[i].m_segmentId     = config["segments"][i]["segmentId"];
        m_segments[i].m_distance      = config["segments"][i]["distance"];
        m_segments[i].m_totalClimb    = config["segments"][i]["totalClimb"];
        m_segments[i].m_totalDownhill = config["segments"][i]["totalDownhill"];
        m_segments[i].m_elevationDiff = config["segments"][i]["elevationDiff"];
        m_segments[i].m_elevation     = config["segments"][i]["elevation"];
        m_segments[i].m_temperature   = config["segments"][i]["temperature"];
        m_segments[i].m_windSpeed     = config["segments"][i]["windSpeed"];
        m_distance += m_segments[i].m_distance;
    }

    return true;
}

const Segment &Track::operator[](int index) const {
    return m_segments[index];
}

Segment &Track::operator[](int index) {
    return m_segments[index];
}
