#include <iostream>
#include <sstream>
#include "Incephalon.h"
#include "SharedFunctions.h"

void Command_Start(Vector &input, Vector &best, int &size, Incephalon &incephalon) {
    std::cin >> size;
    if (size > MaxSize || size <= 5) {
        std::cout << "ERROR" << std::endl;
    } else {
        incephalon.setSize(size);
        std::cout << "OK" << std::endl;
    }
}

void Command_Takeback(Vector &input, Vector &best, int &size, Incephalon &incephalon) {
    incephalon.removeLastStep();
    std::cout << "OK" << std::endl;
}

void Command_Restart(Vector &input, Vector &best, int &size, Incephalon &incephalon) {
    incephalon.restart();
    std::cout << "OK" << std::endl;
}

void Command_Begin(Vector &input, Vector &best, int &size, Incephalon &incephalon) {
    best = incephalon.getBestMove();
    incephalon.putPawn(best);
    std::cout << best.x << "," << best.y << std::endl;
}

void Command_Turn(Vector &input, Vector &best, int &size, Incephalon &incephalon) {
    char dot;
    std::cin >> input.x >> dot >> input.y;
    if (input.x < 0 || input.x >= size || input.y < 0 || input.y >= size
        || incephalon.pSingleton[input.x + 4][input.y + 4].piece != Empty) {
        std::cout << "ERROR" << std::endl;
    } else {
        incephalon.putPawn(input);
        best = incephalon.getBestMove();
        incephalon.putPawn(best);
        std::cout << best.x << "," << best.y << std::endl;
    }
}

void Command_Board(Vector &input, Vector &best, int &size, Incephalon &incephalon) {
    int c;
    Vector m;
    char dot;
    std::string command;
    std::stringstream ss;
    incephalon.restart();
    std::cin >> command;
    toupper(command);
    while (command != "DONE") {
        ss.clear();
        ss << command;
        ss >> m.x >> dot >> m.y >> dot >> c;
        if (m.x < 0 || m.x >= size || m.y < 0 || m.y >= size
            || incephalon.pSingleton[m.x + 4][m.y + 4].piece != Empty) {
            std::cout << "ERROR" << std::endl;
        } else {
            incephalon.putPawn(m);
        }
        std::cin >> command;
        toupper(command);
    }
    best = incephalon.getBestMove();
    incephalon.putPawn(best);
    std::cout << best.x << "," << best.y << std::endl;
}

void Command_Info(Vector &input, Vector &best, int &size, Incephalon &incephalon) {
    int value;
    std::string key;
    std::cin >> key;
    toupper(key);
    if (key == "TIMEOUT_TURN") {
        std::cin >> value;
        if (value != 0)
            incephalon.timeout_turn = value;

    } else if (key == "TIMEOUT_MATCH") {
        std::cin >> value;
        if (value != 0)
            incephalon.timeout_match = value;

    } else if (key == "TIME_LEFT") {
        std::cin >> value;
        if (value != 0)
            incephalon.time_left = value;

    } else if (key == "MAX_MEMORY") {
        std::cin >> value;
    } else if (key == "GAME_TYPE") {
        std::cin >> value;
    } else if (key == "RULE") {
        std::cin >> value;
    } else if (key == "FOLDER") {
        std::string t;
        std::cin >> t;
    }
}

void Command_End(Vector &input, Vector &best, int &size, Incephalon &incephalon) {
    exit(0);
}

void Command_About(Vector &input, Vector &best, int &size, Incephalon &incephalon) {
    std::cout << R"(name="Incephalon", version="2.5", author="Yapamon", country="France")" << std::endl;
}