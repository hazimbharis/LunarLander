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

#define FPS 1

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
int hvelocity = 10; // move to player struct?
int vvelocity; // move to player struct?
int score;
int counter;

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
    mvaddch(player->pos.y, player->pos.x, ' ');
      player->pos.y--;
      // fuel decrease
      // does not visually show ship going up unless vvelocity is negative
      // delay reset
      // upward velocity increase
      break;
    //move down
    case KEY_DOWN:
    mvaddch(player->pos.y, player->pos.x, ' ');
      player->pos.y++;
      // fuel decrease
      // downward velocity increase
      break;
    //move left
    case KEY_LEFT:
    mvaddch(player->pos.y, player->pos.x, ' ');
      //player->pos.x--;
      hvelocity--;
      // fuel decrease
      // negative velocity increase
      break;
    //move right
    case KEY_RIGHT:
    mvaddch(player->pos.y, player->pos.x, ' ');
      //player->pos.x++;
      hvelocity++;
      // fuel decrease
      // positive velocity increase
      break;
    default:
    mvaddch(player->pos.y, player->pos.x, ' ');
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

void overlayCanvas(int height, int width) {
  // prints score?, time, fuel, hvelocity, vvelocity and altitude at the top of the screen
  mvprintw(0,0,"SCORE %d", score);
  mvprintw(2,0,"COUNTER %d", counter);
  mvprintw(4,0,"FUEL %d", fuel);

  mvprintw(0,width-10,"ALTITUDE");
  //mvprintw(2,width-10,"X-VELOCITY %d", hvelocity);
  mvprintw(4,width-10,"Y-VELOCITY");


}

int main()
{	
  int ch;
  int delay = 0;
  int height,width;
  Position start_pos = {10, 0};
  // add starting hvelocity, vvelocity, and altitude - maybe put in player struct

  initscr();
  noecho();
  curs_set(0);
  keypad(stdscr, TRUE);
  nodelay(stdscr, TRUE);

  getmaxyx(stdscr,height,width);
  overlayCanvas(height,width);
  player = createPlayer(start_pos);
  mvaddch(player->pos.y, player->pos.x, player->ch);

  while(ch = getch())
  {
    if (ch == 'q') { // to quit game
      break;
    }

    if (hvelocity > 0) {
      mvaddch(player->pos.y, player->pos.x, ' ');
      player->pos.x += 1;
      mvaddch(player->pos.y, player->pos.x, player->ch);
    }

    if (hvelocity < 0) {
      mvaddch(player->pos.y, player->pos.x, ' ');
      player->pos.x -= 1;
      mvaddch(player->pos.y, player->pos.x, player->ch);
    }
    

    delay++;
    
    //if (vvelocity > 0)
    if (delay == 2) {
      mvaddch(player->pos.y, player->pos.x, ' ');
      player->pos.y += 1;
      mvaddch(player->pos.y, player->pos.x, player->ch);
      delay = 0;
    }

    //if (vvelocity < 0)
    //

    handleInput(ch);
    //clear(); // need to change to either refresh() or smtg else because it will delete terrain and canvas
    mvaddch(player->pos.y, player->pos.x, player->ch);
    mvprintw(2,width-16,"X-VELOCITY %d", hvelocity);

    while(getch() != ERR) {} // emptys getch buffer so no input lag
    napms(1000/FPS);
  }

  endwin();

  return 0;
}
