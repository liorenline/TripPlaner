#pragma once
#include <string>

class Destination {
protected:
    std::string name;
    double lat;
    double lon;

public:
    std::string getName() const { return name; }
    double getLatitude()  const { return lat;}
    double getLongitude() const { return lon;}

    void setName(std::string n)  { name = n; }
    void setLatitude(double la)  { lat  = la; }
    void setLongitude(double lo) { lon  = lo; }
};