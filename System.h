#pragma once
#include <string>
#include "Destination.h"
#include "Train.h"
#include "Router.h"
#include "CityInput.h"

class System {
private:
    Destination city[100];
    int cityN = 0;
    Train train[100];
    int trainN = 0;
    std::string walkA[200];
    std::string walkB[200];
    int  walkN = 0;

public:
    void loadCities(std::string file);
    void loadTrains(std::string file);
    void loadWalks(std::string file);
    void listCities();
    void findRoute();
    void showMenu();
};