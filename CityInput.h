#pragma once
#include <string>
#include "Destination.h"
#include "UI.h"

class CityInput {
private:
    Destination* cities;
    int          cityN;

    int levenshtein(std::string a, std::string b) {
        int la = a.size(), lb = b.size();
        int dp[101][101] = {};
        for (int i = 0; i <= la; i++) dp[i][0] = i;
        for (int j = 0; j <= lb; j++) dp[0][j] = j;
        for (int i = 1; i <= la; i++)
            for (int j = 1; j <= lb; j++) {
                int cost = (a[i-1] != b[j-1]) ? 1 : 0;
                int a = dp[i-1][j]+1, b = dp[i][j-1]+1, c = dp[i-1][j-1]+cost;
                dp[i][j] = a < b ? (a < c ? a : c) : (b < c ? b : c);
            }
        return dp[la][lb];
    }

    std::string toLower(std::string s) {
        for (char& c : s) if (c >= 'A' && c <= 'Z') c += 32;
        return s;
    }

public:
    CityInput(Destination* c, int n) : cities(c), cityN(n) {}

    int findIdx(const std::string& name) {
        for (int i = 0; i < cityN; i++)
            if (cities[i].getName() == name) return i;
        return -1;
    }

    std::string suggest(const std::string& inp) {
        std::string best = "";
        int bestD = 100;
        std::string inpL = toLower(inp);
        for (int i = 0; i < cityN; i++) {
            std::string nameL = toLower(cities[i].getName());
            int d = levenshtein(inpL, nameL);
            if (d < bestD) { bestD = d; best = cities[i].getName(); }
        }
        if (bestD == 0)  return inp;
        if (bestD <= 3)  return best;
        return "";
    }

    std::string resolve(const std::string& inp) {
        if (findIdx(inp) != -1) return inp;

        std::string s = suggest(inp);
        if (s == "") {
            UI::err("Місто \"" + inp + "\" не знайдено.");
            return "";
        }

        std::cout << UI::YELLOW << "  ? Можливо ви мали на увазі \""
                  << UI::BOLD << s << UI::RESET
                  << UI::YELLOW << "\"? (y/n): " << UI::RESET;
        char ans; std::cin >> ans;
        if (ans == 'y' || ans == 'Y') return s;

        UI::err("Місто не знайдено.");
        return "";
    }
};