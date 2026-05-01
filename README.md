## Trip Planner - routes across Ukraine

A console app for finding the optimal route between cities in Ukraine.
Supports train, car, and walking. Pathfinding via Dijkstra's algorithm.
### Run with Docker
```
bashdocker build -t trip_planner .
docker run -it trip_planner
```
### Or build manually (g++ 17+)
```
bashg++ -std=c++17 -o trip_planner main.cpp System.cpp
./trip_planner
```
### Data files
Place these in the same folder as the executable:
- destinations.txt - CityName Latitude Longitude
- trains.txt - From To DepartureTime Duration(hrs) Price(UAH)
- walks.txt - CityA CityB
