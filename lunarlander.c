#include "lunarlander.h"

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

//#define FPS 2
// maybe change FPS if hvelocity or vvelocity is high to give illusion of speed

Entity* player; // change this somehow
int fuel = 100;      // move to player struct?
int hvelocity = 2; // move to player struct?
int vvelocity = 2; // move to player struct?
int score = 0;
int counter = 0;

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
      vvelocity-=2;
      fuel--;
      // delay reset
      break;
    //move left
    case KEY_LEFT:
    mvaddch(player->pos.y, player->pos.x, ' ');
      hvelocity--;
      fuel--;
      break;
    //move right
    case KEY_RIGHT:
    mvaddch(player->pos.y, player->pos.x, ' ');
      hvelocity++;
      fuel--;
      break;
    default:
    mvaddch(player->pos.y, player->pos.x, ' ');
      break;
  }
}

Tile** createTileArray(int height, int width) {
    Tile* values = calloc(height*width, sizeof(Tile));
    Tile** rows = malloc(height*sizeof(Tile*));
    for (int i=0;i<height;i++)
    {
        rows[i] = values + i*width;
    }
    return rows;
}

/*
Tile** resetTileArray(Tile** tiles) {
  free(*tiles);
  free(tiles);
}

or

Tile** resetTileArray(Tile** tiles, int height, int width) {
  int i;
  int j;

  for (i=0;i<height;i++) {
    for (j=0;j<width;j++) {
      tiles[i][j].
    }
  }

}
*/


void generateTerrain(Tile** tiles,int height, int width) {
  int i=0;
  int j=0;
  int h=3;
  int x;
  int r;
  int d; // 0 if previous tile was up, 1 if down and 2 if pad.
  int score_mult = 2;

  for (i=0;i<height;i++) {
    for (j=0;j<width;j++) {
      tiles[i][j].tile_id = 0;
      tiles[i][j].score_mult = score_mult;
    }
  }

  for (x=0;x<width;x++) 
  { 
    r = rand() % 5;

    // if mountain height at ground level, add flat land
    if ( r == 2 && d != 2) {
      // make landing pads - min length 2 - also multiplier based on depth value/i
      if (x != width - 1) {

        if (h > height/6) {
          score_mult = 3;
        }
          
        if (h > height/3) {
          score_mult = 4;
        }
          
        if (h > height/2) {
          score_mult = 5;
        }
        
        mvprintw(height-h+1,x,"x%d",score_mult);
      }

      mvaddch(height-h,x,'_');
      mvaddch(height-h,x+1,'_');
      tiles[height-h][x].tile_id = 2;
      //set tile underneath
      tiles[height-h][x].score_mult = score_mult;
      tiles[height-h][x+1].tile_id = 2;
      //set tile underneath
      tiles[height-h][x+1].score_mult = score_mult;

      x+=2;
      for (x=x;x<width;x++) {
        mvaddch(height-h,x,'_');
        tiles[height-h][x].tile_id = 2;
        //set tile underneath
        tiles[height-h][x].score_mult = score_mult;
        // store position in array

        r = rand() % 3;
        if (r != 0) {
          d=2;
          score_mult = 2;
          break;
        }
      }
    }

    //if (r == 1) {
      mvaddch(height-h,x,'/');
      tiles[height-h][x].tile_id = 1;
      tiles[height-h-1][x].tile_id = 1;
      // store position in array
      h++;
      d=1;
      //x++;
    //}
    
    
    r = rand() % 3;
    if ( h >= height/1.5 || r == 1) {
      for (x=x+1;x<width;x++) {
        h--;
        mvaddch(height-h,x,'\\');
        tiles[height-h][x].tile_id = 1;
        tiles[height-h-1][x].tile_id = 1;
        // store position in array
        r = rand() % 8;
        if (h == 2 || r == 2) {
          // make landing pads - min length 2 - also multiplier based on depth value/h
          
          h++;
          mvaddch(height-h,x,'_');
          mvaddch(height-(h-1),x,' ');
          mvaddch(height-h,x+1,'_');

          if (x != width - 1) {

            if (h > height/6) {
              score_mult = 3;
            }
              
            if (h > height/3) {
              score_mult = 4;
            }
              
            if (h > height/2) {
              score_mult = 5;
            }

            tiles[height-h][x].tile_id = 2;
            //set tile underneath
            tiles[height-h][x].score_mult = score_mult;
            tiles[height-h-1][x].tile_id = 0;
            tiles[height-h][x+1].tile_id = 2;
            //set tile underneath
            tiles[height-h][x+1].score_mult = score_mult;
            mvprintw(height-h+1,x,"x%d",score_mult);
          } 

          if (x == width - 1) {
            tiles[height-h][x].tile_id = 1;
            //set current tile unlandable
          }
          x++;
          for (x=x;x<width;x++) {
            mvaddch(height-h,x,'_');
            tiles[height-h][x].tile_id = 2;
            // set tile underneath
            tiles[height-h][x].score_mult = score_mult;
            // store position in array

            r = rand() % 3;
            if (r != 0) {
              d=2;
              score_mult = 2;
              break;
            }
          }
          
          //h++;
          //mvaddch(height-h,x,'_');
          //mvaddch(height-(h-1),x,' ');
          break;
        }
        if (r == 3) {
          break;
        }
      }
      //continue;
    }
  }
  
  //x--;

  // draws terrain per space
  // store y,x values of terrain draw into a matrix
  // the y,x values will be checked each movement tick to see if 
  // player y,x pos is overlapping
  //return tiles[][];
}

void overlayCanvas(int height, int width) {
  // prints score?, time, fuel, hvelocity, vvelocity and altitude at the top of the screen
  mvprintw(0,1,"SCORE %04d", score);
  mvprintw(1,1,"TIME  %04d", counter);
  mvprintw(2,1,"FUEL  %04d", fuel);

  mvprintw(0,width-15,"ALTITUDE   %03d", height - player->pos.y);
  mvprintw(1,width-15,"X-VELOCITY %+03d", hvelocity);
  mvprintw(2,width-15,"Y-VELOCITY %+03d", vvelocity);
  //mvprintw(1,width/2,"TILE ID: %+03d", tiles[player->pos.y][player->pos.x].tile_id);

  //mvprintw(0,width-14,"ALTITUDE  %04d", player->pos.y);
  //mvprintw(2,width-10,"X-VELOCITY %d", hvelocity);
  //mvprintw(2,width-15,"Y-VELOCITY %03d",vvelocity);

  int i;
  for (i=0;i<width;i++) {
    mvaddch(3,i,'~');
  }
}

int main()
{	
  srand(time(NULL));
  int FPS = 2;
  int ch;
  int key_press;
  int delay = 0;
  int height,width;
  Tile **tiles;
  Position start_pos = {6, 0};
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
  tiles = createTileArray(height,width);
  player = createPlayer(start_pos);
  //tiles[height][width];
  overlayCanvas(height,width);
  //tiles = generateTerrain(height, width);
  generateTerrain(tiles,height, width);
  
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
    

    //delay++;

    if (vvelocity < 5 || vvelocity > -5) {
      delay++;
    }

    if (vvelocity >= 5 || vvelocity <= -5) {
      delay += 2;
    }

    //if (delay == 1) {
      //vvelocity = vvelocity * 1.62;
      //delay = 0;
    //}
    
    
    if (vvelocity >= 0) {
      if (delay >= 2) {
        mvaddch(player->pos.y, player->pos.x, ' ');
        player->pos.y += 1;
        mvaddch(player->pos.y, player->pos.x, player->ch);
        vvelocity = vvelocity + 1;
        delay = 0;
      }
    }

    if (vvelocity <= 0) {
      //if touching top of screen
      //do not go up and print message Hey now the moon is down there - stop trying to break into the scoreboard!

      if (delay >= 2) {
          mvaddch(player->pos.y, player->pos.x, ' ');
          player->pos.y -= 1;
          mvaddch(player->pos.y, player->pos.x, player->ch);
          vvelocity = vvelocity + 1.5;
          delay = 0;
      }
    }

    if (hvelocity > 5 || hvelocity < -5) {
      FPS = 3;
    }

    handleInput(ch);
    overlayCanvas(height,width);
    //clear(); // need to change to either refresh() or smtg else because it will delete terrain and canvas
    mvaddch(player->pos.y, player->pos.x, player->ch);

    if (tiles[player->pos.y][player->pos.x].tile_id == 2 && vvelocity < 5 && hvelocity < 5) {
      score += (1000-fuel)*(counter)*tiles[player->pos.y][player->pos.x].score_mult;
      player->pos = start_pos;
      mvprintw(height/2,(width/2) - 5,"YOU WIN!!!");
      mvprintw(height/2 + 1,(width/2) - 17,"Press E to go to the next level!");
      vvelocity = 0;
      hvelocity = 2;
      refresh();
      while(1) {
        key_press = getch();

        if (key_press == 'q') {
          endwin();
          return 0;
        }

        if (key_press == 'e') {
          break;
        }
      }
      clear();
      overlayCanvas(height,width);
      generateTerrain(tiles, height, width);
      refresh();
      //napms(4000);
      //nodelay(stdscr,FALSE);
    }

    if (tiles[player->pos.y][player->pos.x].tile_id == 1) {
      player->pos = start_pos;
      mvprintw(height/2,(width/2) - 6,"YOU LOSE!!!");
      mvprintw(height/2 + 1,(width/2) - 21,"Press Q to quit the game or E to restart!");
      vvelocity = 0;
      hvelocity = 2;
      score = 0;
      counter = 0;
      fuel = 100;
      refresh();
      while(1) {
        key_press = getch();

        if (key_press == 'q') {
          endwin();
          return 0;
        }

        if (key_press == 'e') {
          break;
        }
      }
      clear();
      overlayCanvas(height,width);
      generateTerrain(tiles, height, width);
      refresh();
      //napms(4000);
      //nodelay(stdscr,FALSE);
    }

    counter++;
    while(getch() != ERR) {} // emptys getch buffer so no input lag
    napms(1000/FPS);
    //check win or lose
  }

  endwin();

  return 0;
}
