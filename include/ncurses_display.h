#ifndef NCURSES_DISPLAY_H
#define NCURSES_DISPLAY_H

#include <curses.h>

#include "process.h"
#include "system.h"

namespace ncurses_display 
{
    void display(System& system, int n = 10);
    void displaySystem(System& system, WINDOW* window);
    void displayProcesses(std::vector<Process>& processes, WINDOW* window, int n);
    std::string progressBar(float percent);
};  // namespace ncurses_display

#endif