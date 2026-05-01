#pragma once
#include <string>
#include <iostream>
#include <iomanip>

namespace UI {

    const std::string RESET  = "\033[0m";
    const std::string BOLD   = "\033[1m";
    const std::string CYAN   = "\033[36m";
    const std::string YELLOW = "\033[33m";
    const std::string GREEN  = "\033[32m";
    const std::string RED    = "\033[31m";
    const std::string GRAY   = "\033[90m";
    const std::string WHITE  = "\033[97m";

    inline void line() {
        std::cout << GRAY << "  ──────────────────────────────────────\n" << RESET;
    }

    inline void section(const std::string& title) {
        std::cout << YELLOW << BOLD << "\n  [ " << title << " ]\n" << RESET;
    }

    inline void header() {
        std::cout << CYAN << BOLD;
        std::cout << "\n";
        std::cout << "   ======================================\n";
        std::cout << "  |          TRIP PLANNER                |\n";
        std::cout << "  |       Маршрути по Україні            |\n";
        std::cout << "   ======================================\n";
        std::cout << RESET << "\n";
    }

    inline void ok(const std::string& msg) {
        std::cout << GREEN << "  + " << msg << RESET << "\n";
    }

    inline void err(const std::string& msg) {
        std::cout << RED << "  ! " << msg << RESET << "\n";
    }

    inline void ask(const std::string& msg) {
        std::cout << WHITE << "  > " << msg << ": " << RESET;
    }

}