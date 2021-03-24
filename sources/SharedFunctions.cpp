#include <iostream>
#include <SharedFunctions.h>
#include <fstream>

void toupper(std::string &str) {
    for (char & c : str) {
        if (c >= 'a' && c <= 'z') {
            c += 'A' - 'a';
        }
    }
}

void logToFile(std::string str) {
    std::ofstream myfile;
    myfile.open(R"(C:\Users\VictorHamon\Epitech_repositories\B-AIA-500-PAR-5-1-gomoku-victor.hamon\log.txt)", std::ios::app);
    myfile << str << std::endl;
    myfile.close();
}
