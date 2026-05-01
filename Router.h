#pragma once
#include <string>
#include "Destination.h"
#include "Train.h"
#include "UI.h"
#include <iostream>
#include <iomanip>
#include <cmath>

enum TravelMode { TRAIN, CAR, WALK };

struct Segment {
    std::string from;
    std::string to;
    TravelMode  mode;
    double km;
    double price;
    double hrs;
};

class Router {
private:
    Destination* city;
    int cityN;
    Train* train;
    int trainN;
    std::string  walkA[200];
    std::string  walkB[200];
    int   walkN;

    double timeG[100][100];
    double priceG[100][100];

    double CAR_RATE = 5.5;
    double CAR_SPEED = 80.0;
    double WALK_MAX_KM = 1.0;
    double CAR_MAX_KM  = 300.0;

    double haversine(int a, int b) {
        const double R = 6371.0, P = 3.1415;
        double la1 = city[a].getLatitude()  * P / 180.0;
        double la2 = city[b].getLatitude()  * P / 180.0;
        double dla = (city[b].getLatitude()  - city[a].getLatitude())  * P / 180.0;
        double dlo = (city[b].getLongitude() - city[a].getLongitude()) * P / 180.0;
        double x = sin(dla/2)*sin(dla/2) + cos(la1)*cos(la2)*sin(dlo/2)*sin(dlo/2);
        return R * 2 * atan2(sqrt(x), sqrt(1-x));
    }

    bool isWalk(const std::string& a, const std::string& b) {
        for (int i = 0; i < walkN; i++)
            if ((walkA[i]==a && walkB[i]==b) || (walkA[i]==b && walkB[i]==a))
                return true;
        return false;
    }

    bool hasTrain(const std::string& a, const std::string& b) {
        for (int i = 0; i < trainN; i++)
            if ((train[i].getFrom()==a && train[i].getTo()==b) ||
                (train[i].getFrom()==b && train[i].getTo()==a))
                return true;
        return false;
    }

    void printSeg(const Segment& s, int idx) {
        std::string label, col;
        if      (s.mode == TRAIN) { label = "Поїзд"; col = UI::CYAN;   }
        else if (s.mode == CAR)   { label = "Авто "; col = UI::YELLOW; }
        else                      { label = "Пішки"; col = UI::GREEN;  }

        std::cout << col << "  " << idx << ". [" << label << "] " << UI::RESET
                  << UI::WHITE << UI::BOLD << s.from << UI::RESET
                  << UI::GRAY  << " -> " << UI::RESET
                  << UI::WHITE << UI::BOLD << s.to   << UI::RESET << "\n";

        std::cout << UI::GRAY << "       "
                  << std::fixed << std::setprecision(1)
                  << s.km << " km  |  "
                  << s.hrs << " год  |  ";
        if (s.price > 0)
            std::cout << s.price << " грн";
        else
            std::cout << "безкоштовно";
        std::cout << UI::RESET << "\n";
    }

public:
    Router(Destination* c, int cn, Train* t, int tn,
           std::string* wa, std::string* wb, int wn)
        : city(c), cityN(cn), train(t), trainN(tn), walkN(wn) {
        for (int i = 0; i < wn; i++) { walkA[i] = wa[i]; walkB[i] = wb[i]; }
    }

    void buildGraph() {
        for (int i = 0; i < cityN; i++)
            for (int j = 0; j < cityN; j++)
                timeG[i][j] = priceG[i][j] = 0;
        for (int i = 0; i < trainN; i++) {
            int a = -1, b = -1;
            for (int j = 0; j < cityN; j++) {
                if (city[j].getName() == train[i].getFrom()) a = j;
                if (city[j].getName() == train[i].getTo())   b = j;
            }
            if (a != -1 && b != -1) {
                timeG[a][b]  = timeG[b][a]  = train[i].getDur();
                priceG[a][b] = priceG[b][a] = train[i].getPrice();
            }
        }
    }

    void dijkstra(int src, int dst) {
        double dist[100];
        bool   vis[100];
        int    par[100];
        TravelMode parMode[100];

        for (int i = 0; i < cityN; i++) {
            dist[i] = 1e9; vis[i] = false; par[i] = -1; parMode[i] = CAR;
        }
        dist[src] = 0;

        for (int step = 0; step < cityN; step++) {
            int v = -1;
            for (int i = 0; i < cityN; i++)
                if (!vis[i] && (v==-1 || dist[i]<dist[v])) v = i;
            if (v==-1 || dist[v]>=100000) break;
            vis[v] = true;

            for (int j = 0; j < cityN; j++) {
                if (j == v) continue;
                double km = haversine(v, j);
                std::string nv = city[v].getName(), nj = city[j].getName();

                double cost = 10000;
                TravelMode mode = CAR;

                if (isWalk(nv, nj) || km < WALK_MAX_KM) {
                    cost = km / 5.0;
                    mode = WALK;
                } else if (hasTrain(nv, nj)) {
                    cost = timeG[v][j];
                    mode = TRAIN;
                } else if (km <= CAR_MAX_KM) {
                    cost = km / CAR_SPEED;
                    mode = CAR;
                }

                if (cost < 1e9 && dist[v] + cost < dist[j]) {
                    dist[j]    = dist[v] + cost;
                    par[j]     = v;
                    parMode[j] = mode;
                }
            }
        }

        if (dist[dst] >= 100000) {
            UI::err("Маршрут не знайдено.");
            return;
        }

        int path[100], sz = 0, cur = dst;
        while (cur != -1) { path[sz++] = cur; cur = par[cur]; }

        UI::section("Маршрут");

        double totKm = 0, totPrice = 0, totHrs = 0;
        for (int i = sz-1; i > 0; i--) {
            int a = path[i], b = path[i-1];
            double km = haversine(a, b);
            Segment s;
            s.from = city[a].getName();
            s.to   = city[b].getName();
            s.km   = km;
            s.mode = parMode[b];

            if (s.mode == TRAIN) {
                s.price = priceG[a][b];
                s.hrs = timeG[a][b];
            } else if (s.mode == CAR) {
                s.price = km * CAR_RATE;
                s.hrs = km / CAR_SPEED;
            } else {
                s.price = 0;
                s.hrs = km / 5.0;
            }

            totKm += s.km;
            totPrice += s.price;
            totHrs += s.hrs;
            printSeg(s, sz - i);
        }

        UI::line();
        std::cout << std::fixed << std::setprecision(1);
        std::cout << UI::WHITE << "  Відстань : " << UI::BOLD << totKm    << " км"  << UI::RESET << "\n";
        std::cout << UI::WHITE << "  Вартість : " << UI::BOLD << totPrice << " грн" << UI::RESET << "\n";
        std::cout << UI::WHITE << "  Час      : " << UI::BOLD << totHrs   << " год" << UI::RESET << "\n";
    }
};