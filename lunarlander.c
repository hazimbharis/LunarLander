#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

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
// maybe change FPS if hvelocity or vvelocity is high to give illusion of speed


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

typedef struct
{
  Position pos;
  bool landable;
  int score_mult;
} Tile;

//const Tile default = {.landable = FALSE;}

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
int hvelocity = 2; // move to player struct?
int vvelocity = 2; // move to player struct?
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
      // if (tiles[player->pos.y][player->pos.x].landable == FALSE)
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

void generateTerrain(int height, int width) {
  int i=2;
  int j=0;
  int x;
  int r;
  int d; // 0 if previous tile was up, 1 if down and 2 if pad.

  Tile tiles[height][width];

  for (x=0;x<width;x++) 
  { 
    r = rand() % 3;

    // new gen func with landing
    /*
    r = (rand() % 3) + 1;

    if (r == 1) {
      //go up
      for (x=x;x<width;x++) {
        mvaddch(height-i,x,'/');
        // store position in array
        i++;
        r = rand() % 3;
        if (i == height/2 || r == 1) {
          break;
        }
      }
    }
    if (i == height/2 || r == 2 && i != 0) {
      //go down
      for (x=x;x<width;x++) {
        i--;
        mvaddch(height-i,x,'\\');
        // store position in array
        r = rand() % 3;
        if (i == 0 || r == 1) {
          break;
        }
      }
    }
    if (i == 1 || r == 3) {
      //create landing pad
      for (x=x;x<width;x++) {
        mvaddch(height-i,x,'_');
        // store position in array
        r = rand() % 2;
        if (r == 1) {
          break;
        }
      }
    }
    */

    // old gen func without landing
    //r = (rand() % 3) + 1;
    /*
      if (r == 2 && d != 2) {
        if (d == 0) {
          i++;
        }
        for (x=x;x<width;x++) {
          mvaddch(height-i,x,'_');
          // store position in array
          r = rand() % 3;
          if (r == 1) {
            r = rand() % 4;
            d = 2;
            break;
          }
        }
      }
    */

    /*
    mvaddch(height-i,x,'/');
    // store position in array
    i++;
    d=0;
    */
    
    /*
    if (r == 2) {
      for (x=x;x<width;x++) {
        mvaddch(height-i,x,'_');
        // store position in array
        r = rand() % 2;
        if (r == 1) {
          break;
        }
    }
    */

    /*
    if ( i == height/2 || r == 1) {
      //if (i < height/4) continue; // makes mountains more uniform but less diverse and interesting patterns appears
      for (x=x+1;x<width;x++) {  
        i--;
        mvaddch(height-i,x,'\\');
        // store position in array
        r = rand() % 3;

        if (i == 0 || r == 1) {
          d=1;
          break;
        }
      }
    }
    */

    // if mountain height at ground level, add flat land
    if ( r == 2 ) {
      for (x=x;x<width;x++) {
        mvaddch(height-i,x,'_');
        tiles[height-i][x].landable = TRUE;
        // store position in array

        r = rand() % 3;
        if (r != 0) {
          break;
        }
      }
    }

    mvaddch(height-i,x,'/');
    // store position in array
    i++;
    r = rand() % 3;
    if ( i >= height/2 || r == 1) {
      for (x=x+1;x<width;x++) {
        i--;
        mvaddch(height-i,x,'\\');
        // store position in array
        r = rand() % 4;
        if (i == 2 || r == 1) {
          i++;
          mvaddch(height-i,x,'_');
          mvaddch(height-(i-1),x,' ');
          break;
        }
      }
    }
  }
  
  // draws terrain per space
  // store y,x values of terrain draw into a matrix
  // the y,x values will be checked each movement tick to see if 
  // player y,x pos is overlapping
  
}

void overlayCanvas(int height, int width) {
  // prints score?, time, fuel, hvelocity, vvelocity and altitude at the top of the screen
  mvprintw(0,1,"SCORE %04d", score);
  mvprintw(1,1,"TIME  %04d", counter);
  mvprintw(2,1,"FUEL  %04d", fuel);

  //mvprintw(0,width-14,"ALTITUDE  %04d", player->pos.y);
  //mvprintw(2,width-10,"X-VELOCITY %d", hvelocity);
  mvprintw(2,width-15,"Y-VELOCITY %03d",vvelocity);

  int i;
  for (i=0;i<width;i++) {
    mvaddch(3,i,'~');
  }


}

int main()
{	
  srand(time(NULL));
  int ch;
  int delay = 0;
  int height,width;
  Position start_pos = {10, 0};
  // add starting hvelocity, vvelocity, and altitude - maybe put in player struct

  initscr();
  noecho();
  //init_pair(1, COLOR_CYAN, COLOR_BLACK);
  curs_set(0);
  keypad(stdscr, TRUE);
  nodelay(stdscr, TRUE);
  start_color();
  attron(COLOR_PAIR(1));
  //overlayCanvas(height,width);
  refresh();
  attroff(COLOR_PAIR(1));

  getmaxyx(stdscr,height,width);
  player = createPlayer(start_pos);
  overlayCanvas(height,width);
  generateTerrain(height, width);
  
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
    mvprintw(0,width-15,"ALTITUDE   %03d", height - player->pos.y);
    mvprintw(1,width-15,"X-VELOCITY %03d", hvelocity);

    while(getch() != ERR) {} // emptys getch buffer so no input lag
    napms(1000/FPS);
  }

  endwin();

  return 0;
}
