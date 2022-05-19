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

#define FPS 2

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

/**
 * typedef struct
 * {
 *   Position pos;
 *   char tile;
 * } MountainTile;
 * 
 * lol its just another entity struct - should just reuse
 */



Entity* player; // change this somehow
int fuel;      // move to player struct?
int hvelocity; // move to player struct?
int vvelocity; // move to player struct?

// change this player initialization bit too, hmm
Entity* createPlayer(Position start_pos)
{
  Entity* newPlayer = calloc(1, sizeof(Entity));

  newPlayer->pos.y = start_pos.y;
  newPlayer->pos.x = start_pos.x;
  newPlayer->ch = '&';

  return newPlayer;
}

void handleInput(int input)
{
  switch(input)
  {
    //move up
    case KEY_UP:
      player->pos.y--;
      // mvaddch " " over current spot
      // fuel decrease
      // does not visually show ship going up unless vvelocity is negative
      // delay reset
      // upward velocity increase
      break;
    //move down
    case KEY_DOWN:
      player->pos.y++;
      // mvaddch " " over current spot
      // fuel decrease
      // downward velocity increase
      break;
    //move left
    case KEY_LEFT:
      player->pos.x--;
      // mvaddch " " over current spot
      // fuel decrease
      // negative velocity increase
      break;
    //move right
    case KEY_RIGHT:
      player->pos.x++;
      // mvaddch " " over current spot
      // fuel decrease
      // positive velocity increase
      break;
    default:
      break;
  }


}

void generateTerrain() {
  // draws terrain per space
  // store y,x values of terrain draw into a matrix
  // the y,x values will be checked each movement tick to see if 
  // player y,x pos is overlapping
  ;
}

void overlayCanvas() {
  // prints score?, time, fuel, hvelocity, vvelocity and altitude at the top of the screen

}

int main()
{	
  int ch;
  int delay = 0;
  Position start_pos = {10, 0};
  // add starting hvelocity, vvelocity, and altitude - maybe put in player struct

  initscr();
  noecho();
  curs_set(0);
  keypad(stdscr, TRUE);
  nodelay(stdscr, TRUE);

  player = createPlayer(start_pos);
  mvaddch(player->pos.y, player->pos.x, player->ch);

  while(ch = getch())
  {
    if (ch == 'q') { // to quit game
      break;
    }

    //if (hvelocity > 0)
    player->pos.x += 1;
    
    //if (hvelocity < 0)
    // go left

    delay++;
    
    //if (vvelocity > 0)
    if (delay == 2) {
      player->pos.y += 1;
      delay = 0;
    }

    //if (vvelocity < 0)
    //

    handleInput(ch);
    clear(); // need to change to either refresh() or smtg else because it will delete terrain
    mvaddch(player->pos.y, player->pos.x, player->ch);

    while(getch() != ERR) {} // emptys getch buffer so no input lag
    napms(1000/FPS);
  }

  endwin();

  return 0;
}
