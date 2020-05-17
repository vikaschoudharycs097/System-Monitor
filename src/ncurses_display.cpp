#include <curses.h>
#include <chrono>
#include <string>
#include <thread>
#include <vector>
#include <cstdio>
#include <cstring>
#include "format.h"
#include "ncurses_display.h"
#include "system.h"

using std::string;
using std::to_string;

// 50 bars uniformly displayed from 0 - 100 %
// 2% is one bar(|)
std::string ncurses_display::progressBar(float percent) 
{
    std::string result{"0%"};
    int size{50};
    float bars{percent * size};

    for (int i{0}; i < size; i++) 
    {
        result += i <= bars ? '|' : ' ';
    }

    std::string display{to_string(percent * 100).substr(0, 4)};
    if (percent < 0.1 || percent == 1.0)
    {
        display = " " + to_string(percent * 100).substr(0, 3);
    }

    return result + " " + display + "/100%";
}

// Display data of System class
void ncurses_display::displaySystem(System& system, WINDOW* window) 
{
    int row{0};
    mvwprintw(window, ++row, 2, ("OS: " + system.operatingSystem()).c_str());
    mvwprintw(window, ++row, 2, ("Kernel: " + system.kernelVersion()).c_str());
    mvwprintw(window, ++row, 2, "CPU: ");
    wattron(window, COLOR_PAIR(1));
    mvwprintw(window, row, 10, "");
    wprintw(window, progressBar(system.cpu().utilization()).c_str());
    wattroff(window, COLOR_PAIR(1));
    mvwprintw(window, ++row, 2, "Memory: ");
    wattron(window, COLOR_PAIR(1));
    mvwprintw(window, row, 10, "");
    wprintw(window, progressBar(system.memoryUtilization()).c_str());
    wattroff(window, COLOR_PAIR(1));
    mvwprintw(window, ++row, 2, ("Total Processes: " + to_string(system.totalProcesses())).c_str());
    mvwprintw(window, ++row, 2, ("Running Processes: " + to_string(system.runningProcesses())).c_str());
    mvwprintw(window, ++row, 2, ("Up Time: " + format::elapsedTime(system.upTime())).c_str());
    wrefresh(window);
}

void ncurses_display::displayProcesses(std::vector<Process>& processes, WINDOW* window, int n) 
{
    int row{0};
    int const PID_COLUMN{2};
    int const USER_COLUMN{8};
    int const CPU_COLUMN{15};
    int const RAM_COLUMN{23};
    int const TIME_COLUMN{31};
    int const COMMAND_COLUMN{40};
    wattron(window, COLOR_PAIR(2));
    mvwprintw(window, ++row, PID_COLUMN, "PID");
    mvwprintw(window, row, USER_COLUMN, "USER");
    mvwprintw(window, row, CPU_COLUMN, "CPU[%%]");
    mvwprintw(window, row, RAM_COLUMN, "RAM[MB]");
    mvwprintw(window, row, TIME_COLUMN, "TIME+");
    mvwprintw(window, row, COMMAND_COLUMN, "COMMAND");

    char line[200];
    char temp[200];
    wattroff(window, COLOR_PAIR(2));
    for (int i = 0; i < n; ++i) 
    {
        sprintf(line, "%-5.5s %-6.6s %-7.7s %-7.7s %-9.9s %-100.100s", 
                to_string(processes[i].processId()).c_str(),
                processes[i].processUser().c_str(), 
                to_string(processes[i].cpuUtilization() * 100).substr(0, 4).c_str(),
                processes[i].processRam().c_str(), 
                format::elapsedTime(processes[i].processUpTime()).c_str(), 
                processes[i].processCommand().c_str());
    
        std::strncpy(temp, line, window->_maxx-5);
        mvwprintw(window, ++row, PID_COLUMN, temp);
    }
}

void ncurses_display::display(System& system, int n) 
{
    initscr();      // start ncurses
    noecho();       // do not print input values
    cbreak();       // terminate ncurses on ctrl + c
    start_color();  // enable color

    int x_max{getmaxx(stdscr)};
    WINDOW* system_window = newwin(9, x_max - 1, 0, 0);
    WINDOW* process_window = newwin(3 + n, x_max - 1, system_window->_maxy + 1, 0);

    while (1) 
    {
        init_pair(1, COLOR_BLUE, COLOR_BLACK);
        init_pair(2, COLOR_GREEN, COLOR_BLACK);
        box(system_window, 0, 0);
        box(process_window, 0, 0);
        displaySystem(system, system_window);
        displayProcesses(system.processes(), process_window, n);
        wrefresh(system_window);
        wrefresh(process_window);
        refresh();
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
    
    endwin();
}