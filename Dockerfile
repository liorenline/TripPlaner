FROM gcc:13

WORKDIR /app

COPY . .

RUN g++ -std=c++17 -o trip_planner main.cpp System.cpp

CMD ["./trip_planner"]
