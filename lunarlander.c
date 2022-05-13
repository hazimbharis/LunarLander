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

Entity* player;

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
      break;
    //move down
    case KEY_DOWN:
      player->pos.y++;
      break;
    //move left
    case KEY_LEFT:
      player->pos.x--;
      break;
    //move right
    case KEY_RIGHT:
      player->pos.x++;
      break;
    default:
      break;
  }


}

int main()
{	
  int ch;
  Position start_pos = {10, 20};

  initscr();
  noecho();
  curs_set(0);
  keypad(stdscr, TRUE);
  nodelay(stdscr, TRUE);

  player = createPlayer(start_pos);
  mvaddch(player->pos.y, player->pos.x, player->ch);

  while(ch = getch())
  {
    if (ch == 'q') {
      break;
    }

    player->pos.y += 1;

    handleInput(ch);
    clear();
    mvaddch(player->pos.y, player->pos.x, player->ch);

    while(getch() != ERR) {}
    napms(1000/FPS);
  }

  endwin();

  return 0;
}
