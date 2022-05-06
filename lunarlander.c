
#include <ncurses.h>
#include <stdlib.h>
/**
 * The main loop of your lander is roughly:

    set lander starting parameters: position, ydelta, acceleration, etc.
    loop forever
         add acceleration to ydelta
         subtract thrust from ydelta
         add ydelta to y position
         display current position
         if lander position is on ground
            display success / failure messsage
            reset to starting parameters
         endif
    endloop
 *
 */


typedef struct
{
  int y;
  int x;
} Position;

typedef struct
{
  Position pos;
  char ch;
} Entity;

int main()
{	
  initscr();
  noecho();
  //curs_set(0);


  while(getch() != 'q')
  {
    mvaddch(10, 20, '@');
  }

  endwin();

  return 0;
}