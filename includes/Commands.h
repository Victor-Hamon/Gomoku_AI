#ifndef PBRAIN_INCEPHALON_COMMANDS_H
#define PBRAIN_INCEPHALON_COMMANDS_H

#include <vector>
#include <string>
#include <functional>
#include <iostream>
#include "Incephalon.h"

struct Command {
    std::string key;
    std::function<void(Vector &, Vector&, int &, Incephalon&)> handler;
};

void Command_Start(Vector &input, Vector &best, int &size, Incephalon &incephalon);
void Command_Takeback(Vector &input, Vector &best, int &size, Incephalon &incephalon);
void Command_Restart(Vector &input, Vector &best, int &size, Incephalon &incephalon);
void Command_Begin(Vector &input, Vector &best, int &size, Incephalon &incephalon);
void Command_Turn(Vector &input, Vector &best, int &size, Incephalon &incephalon);
void Command_Board(Vector &input, Vector &best, int &size, Incephalon &incephalon);
void Command_Info(Vector &input, Vector &best, int &size, Incephalon &incephalon);
void Command_End(Vector &input, Vector &best, int &size, Incephalon &incephalon);
void Command_About(Vector &input, Vector &best, int &size, Incephalon &incephalon);

std::vector<Command> commands = {
        {"START", &Command_Start},
        {"RESTART", &Command_Restart},
        {"TAKEBACK", &Command_Takeback},
        {"BEGIN", &Command_Begin},
        {"TURN", &Command_Turn},
        {"BOARD", &Command_Board},
        {"INFO", &Command_Info},
        {"END", &Command_End},
        {"ABOUT", &Command_About},
};

#endif //PBRAIN_INCEPHALON_COMMANDS_H
