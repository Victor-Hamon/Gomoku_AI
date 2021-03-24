#include<iostream>
#include<string>
#include "Commands.h"
#include "SharedFunctions.h"
Incephalon incephalon;

[[noreturn]] int play() {
    std::string command;
    Vector input, best;
    int size = 20;
    incephalon.setSize(20);
    while (true) {
        std::cin >> command;
        toupper(command);

        for (auto& current_command : commands) {
            if (current_command.key == command) {
                current_command.handler(input, best, size, incephalon);
            }
        }
    }
}

int main() {
    play();
}
