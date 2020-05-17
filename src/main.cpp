#include "ncurses_display.h"
#include "system.h"

int main(void) 
{
    System system;
    ncurses_display::display(system);
    return 0;
}