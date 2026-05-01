#include "System.h"

int main() {
    System app;
    app.loadCities("destinations.txt");
    app.loadTrains("trains.txt");
    app.loadWalks("walks.txt");
    app.showMenu();
    return 0;
}