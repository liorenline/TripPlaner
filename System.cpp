#include "System.h"
#include "UI.h"
#include <iostream>
#include <fstream>
#include <iomanip>

void System::loadCities(std::string file) {
    std::ifstream f(file);
    std::string name; double la, lo;
    while (f >> name >> la >> lo) {
        city[cityN].setName(name);
        city[cityN].setLatitude(la);
        city[cityN].setLongitude(lo);
        cityN++;
    }
}

void System::loadTrains(std::string file) {
    std::ifstream f(file);
    std::string from, to, dep; double dur, price;
    while (f >> from >> to >> dep >> dur >> price) {
        train[trainN].setFrom(from);
        train[trainN].setTo(to);
        train[trainN].setDep(dep);
        train[trainN].setDur(dur);
        train[trainN].setPrice(price);
        trainN++;
    }
}

void System::loadWalks(std::string file) {
    std::ifstream f(file);
    std::string a, b;
    while (f >> a >> b) {
        walkA[walkN] = a;
        walkB[walkN] = b;
        walkN++;
    }
}

void System::listCities() {
    UI::section("Доступні міста");
    int cols = 3, w = 18;
    for (int i = 0; i < cityN; i++) {
        if (i % cols == 0) std::cout << "  ";
        std::cout << UI::CYAN << "* " << UI::RESET
                  << std::left << std::setw(w) << city[i].getName();
        if ((i+1) % cols == 0 || i == cityN-1) std::cout << "\n";
    }
}

void System::findRoute() {
    CityInput input(city, cityN);
    Router    router(city, cityN, train, trainN, walkA, walkB, walkN);

    std::string from, to;

    UI::section("Пошук маршруту");
    UI::ask("Звідки");
    std::cin >> from;
    from = input.resolve(from);
    if (from == "") return;

    UI::ask("Куди");
    std::cin >> to;
    to = input.resolve(to);
    if (to == "") return;

    int src = input.findIdx(from);
    int dst = input.findIdx(to);

    router.buildGraph();
    router.dijkstra(src, dst);
}

void System::showMenu() {
    UI::header();
    int choice = 0;
    do {
        UI::line();
        std::cout << UI::BOLD << UI::WHITE << "  Меню:\n" << UI::RESET;
        std::cout << UI::CYAN   << "  [1]" << UI::RESET << " Список міст\n";
        std::cout << UI::CYAN   << "  [2]" << UI::RESET << " Знайти маршрут\n";
        std::cout << UI::GRAY   << "  [0]" << UI::RESET << " Вихід\n";
        UI::line();
        UI::ask("Вибір");
        std::cin >> choice;

        if      (choice == 1) listCities();
        else if (choice == 2) findRoute();
        else if (choice != 0) UI::err("Невірний вибір.");

    } while (choice != 0);

    std::cout << UI::CYAN << UI::BOLD << "\n  До побачення!\n\n" << UI::RESET;
}