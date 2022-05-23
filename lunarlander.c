#include "lunarlander.h"

// player initialization function
Entity* createPlayer(Position start_pos, int starting_fuel)
{
  Entity* newPlayer = calloc(1, sizeof(Entity));

  newPlayer->pos.y = start_pos.y;
  newPlayer->pos.x = start_pos.x;
  newPlayer->hvelocity = 2;
  newPlayer->vvelocity = 2;
  newPlayer->fuel = starting_fuel;
  newPlayer->ch = '&';

  return newPlayer;
}

// player movement function
void handleInput(Entity* player, int input)
{
  switch(input)
  {
    //move up
    case KEY_UP:
    if (player->fuel != 0) {
      mvaddch(player->pos.y, player->pos.x, ' ');
      player->vvelocity-=2;
      player->fuel--;
    }
    break;
    //move left
    case KEY_LEFT:
    if (player->fuel != 0) {
      mvaddch(player->pos.y, player->pos.x, ' ');
      player->hvelocity--;
      player->fuel--;
    }
    break;
    //move right
    case KEY_RIGHT:
    if (player->fuel != 0) {
      mvaddch(player->pos.y, player->pos.x, ' ');
      player->hvelocity++;
      player->fuel--;
    }
    break;
    default:
    mvaddch(player->pos.y, player->pos.x, ' ');
      break;
  }
}

// 2d array pointer used to store tile ids
Tile** createTileArray(int height, int width) {
    Tile* values = calloc(height*width, sizeof(Tile));
    Tile** rows = malloc(height*sizeof(Tile*));
    for (int i=0;i<height;i++)
    {
        rows[i] = values + i*width;
    }
    return rows;
}

//random mountain terrain generation function
void generateTerrain(Tile** tiles,int height, int width) {
  int i=0;
  int j=0;
  int h=3; // current y position of mountain terrain gen
  int x; // current x position of mountain terrain gen
  int r; // random variable
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

    // if mountain height at ground level AND previous tile was not flat land, add flat land
    if ( r == 2 && d != 2) {
      // make landing pads - min length 2 - also score multiplier based on depth value/h
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

      // tile_id and score_mult setting to TileArray
      mvaddch(height-h,x,'_');
      mvaddch(height-h,x+1,'_');
      tiles[height-h][x].tile_id = 2;
      tiles[height-h+1][x].tile_id = 2; // incase player somehow skips a tile
      tiles[height-h][x].score_mult = score_mult;
      tiles[height-h+1][x].score_mult = score_mult;
      tiles[height-h][x+1].tile_id = 2;
      tiles[height-h+1][x+1].tile_id = 2; // incase player somehow skips a tile
      tiles[height-h][x+1].score_mult = score_mult;
      tiles[height-h+1][x+1].score_mult = score_mult;

      x+=2;
      for (x=x;x<width;x++) {
        mvaddch(height-h,x,'_');
        tiles[height-h][x].tile_id = 2;
        tiles[height-h+1][x].tile_id = 2; // incase player somehow skips a tile
        tiles[height-h][x].score_mult = score_mult;
        tiles[height-h+1][x].score_mult = score_mult;

        r = rand() % 3;
        if (r != 0) {
          d=2;
          score_mult = 2;
          break;
        }
      }
    }

    mvaddch(height-h,x,'/');
    tiles[height-h][x].tile_id = 1;
    tiles[height-h+1][x].tile_id = 1; // incase player somehow skips a tile
    // store position in array
    h++;
    d=1;
    
    r = rand() % 3;
    if ( h >= height/1.5 || r == 1) {
      for (x=x+1;x<width;x++) {
        h--;
        mvaddch(height-h,x,'\\');
        tiles[height-h][x].tile_id = 1;
        tiles[height-h+1][x].tile_id = 1; // incase player somehow skips a tile
        // store position in tile array
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
            tiles[height-h+1][x].tile_id = 2;
            tiles[height-h][x].score_mult = score_mult;
            tiles[height-h+1][x].score_mult = score_mult;
            tiles[height-h][x+1].tile_id = 2;
            tiles[height-h+1][x+1].tile_id = 2;
            tiles[height-h][x+1].score_mult = score_mult;
            tiles[height-h+1][x+1].score_mult = score_mult;
            mvprintw(height-h+1,x,"x%d",score_mult);
          } 

          if (x == width - 1) {
            tiles[height-h][x].tile_id = 1;
            tiles[height-h+1][x].tile_id = 1;
            //set current tile and tile below unlandable
          }
          x++;
          for (x=x;x<width;x++) {
            mvaddch(height-h,x,'_');
            tiles[height-h][x].tile_id = 2;
            tiles[height-h+1][x].tile_id = 2;
            tiles[height-h][x].score_mult = score_mult;
            tiles[height-h+1][x].score_mult = score_mult;

            r = rand() % 3;
            if (r != 0) {
              d=2;
              score_mult = 2;
              break;
            }
          }
          break;
        }
        if (r == 3) {
          break;
        }
      }
    }
  }
}

// prints title, score, time, fuel, hvelocity, vvelocity and altitude at the top of the screen
void overlayCanvas(Entity* player, int difficulty, int score, int counter, int height, int width) {
  attron(A_BOLD);
  mvprintw(0,1,"SCORE %04d", score);
  mvprintw(1,1,"TIME  %04d", counter);
  mvprintw(2,1,"FUEL  %04d", player->fuel);

  mvprintw(0,width-15,"ALTITUDE   %03d", height - player->pos.y);
  mvprintw(1,width-15,"X-VELOCITY %+03d", player->hvelocity);
  mvprintw(2,width-15,"Y-VELOCITY %+03d", player->vvelocity);

  if (difficulty == 1) {
    mvprintw(0,width/2 - 6,"LUNAR LANDER");
  }
  
  if (difficulty == 2) {
    mvprintw(0,width/2 - 9,"LUNAR LANDER: ACE");
  }
  
  if (difficulty == 5) {
    mvprintw(0,width/2 - 10,"LUNAR LANDER: MAYDAY");
  }
  
  mvprintw(1,width/2 - 4,"Q : quit");

  if (player->fuel < 20 && player->fuel != 0) {
    attron(A_BLINK);
    mvprintw(2,1,"FUEL  %04d", player->fuel);
  }
  //mvprintw(1,width/2,"TILE ID: %+03d", tiles[player->pos.y][player->pos.x].tile_id);

  refresh();
  attroff(A_BLINK);
  attroff(A_BOLD);

  int i;
  for (i=0;i<width;i++) {
    mvaddch(3,i,'~');
  }
}

int main()
{	
  srand(time(NULL));
  Entity* player; 
  int score = 0;
  int counter = 0;
  int fuel = 100;
  int FPS = 2;
  int ch;
  int key_press;
  int delay = 0;
  int difficulty = 1;
  int starting_vX = 2;
  int starting_vY = 2;
  int height,width;
  Tile **tiles;
  Position start_pos = {6, 0};

  initscr();
  noecho();
  curs_set(0);
  keypad(stdscr, TRUE);
  nodelay(stdscr, TRUE);
  start_color();
  attron(COLOR_PAIR(1));
  refresh();
  attroff(COLOR_PAIR(1));

  getmaxyx(stdscr,height,width);
  player = createPlayer(start_pos,fuel);

  // start menu while loop
  while (1) {
    mvprintw(height/5 - 1,width/2 - 6,"LunarLander");
    mvprintw(height/3 + 1,width/2 - 42,"To land: Your X and Y velocity needs to be less than 7 when landing on a landing pad");
    mvprintw(height/3 + 2,width/2 - 10,"Controls: [<][^][>]");
    mvprintw(height/3 + 3,width/2 - 8,"Press Q to quit");

    mvprintw(height/2 + 3,width/2 - 12,"Choose your difficulty:");

    mvprintw(height/2 + 5,width/4 - 2,"[1]");
    mvprintw(height/2 + 6,width/4 - 3,"Normal");
    mvprintw(height/2 + 7,width/4 - 4,"Fuel 100");
    mvprintw(height/2 + 8,width/4 - 10,"Score Multiplier x1");

    mvprintw(height/2 + 5,width/2 - 2,"[2]");
    mvprintw(height/2 + 6,width/2 - 2,"Ace");
    mvprintw(height/2 + 7,width/2 - 4,"Fuel 80");
    mvprintw(height/2 + 8,width/2 - 10,"Score Multiplier x2");

    mvprintw(height/2 + 5,width/1.4 - 2,"[3]");
    mvprintw(height/2 + 6,width/1.4 - 3,"Mayday");
    mvprintw(height/2 + 7,width/1.4 - 4,"Fuel 50");
    mvprintw(height/2 + 8,width/1.4 - 11,"Higher Starting Speed");
    mvprintw(height/2 + 9,width/1.4 - 10,"Score Multiplier x5");

    key_press = getch();

    if (key_press == '1') {
      clear();
      refresh();
      break;
    }

    if (key_press == '2') {
      fuel = 80;
      difficulty = 2;
      clear();
      refresh();
      break;
    }

    if (key_press == '3') {
      fuel = 50;
      difficulty = 5;
      starting_vX = 7;
      starting_vY = 7;
      player->vvelocity = starting_vY;
      player->hvelocity = starting_vX;
      clear();
      refresh();
      break;
    }

    if (key_press == 'q') {
      endwin();
      return 0;
    }
  }

  player->fuel = fuel;
  tiles = createTileArray(height,width);
  overlayCanvas(player,difficulty,score,counter,height,width);
  generateTerrain(tiles,height, width);
  
  mvaddch(player->pos.y, player->pos.x, player->ch);

  while(ch = getch())
  {
    if (ch == 'q') { // to quit game
      break;
    }

    // positive x-velocity movement
    if (player->hvelocity > 0 && player->pos.x != width) {

      //checks if player at the right edge of screen
      if (player->pos.x != width) {
        mvaddch(player->pos.y, player->pos.x, ' ');
        player->pos.x += 1;
        mvaddch(player->pos.y, player->pos.x, player->ch);
      }
    }

    //negative x-velocity movement
    if (player->hvelocity < 0 && player->pos.x != 0) {

      //checks if player is at the left edge of screen
      if (player->pos.x != 0) {
        mvaddch(player->pos.y, player->pos.x, ' ');
        player->pos.x -= 1;
        mvaddch(player->pos.y, player->pos.x, player->ch);
      }
    }

    // if player is slow, gravity delay is slow
    if (player->vvelocity < 5 || player->vvelocity > -5) {
      delay++;
    }

    // if player is fast, gravity delay is not slow
    if (player->vvelocity >= 5 || player->vvelocity <= -5) {
      delay += 2;
    }
    
    //positive y-velocity movement
    if (player->vvelocity > 0) {
      if (delay >= 2) {
        mvaddch(player->pos.y, player->pos.x, ' ');
        player->pos.y += 1;
        mvaddch(player->pos.y, player->pos.x, player->ch);
        player->vvelocity = player->vvelocity + 1;
        delay = 0;
        mvprintw(2,width/2 - 5,"          ");
      }
    }

    //zero velocity movement
    if (player->vvelocity == 0) {
      if (delay >= 1) {
        player->vvelocity = player->vvelocity + 1;
        delay = 0;
      }
    }

    //negative y-velocity movement
    if (player->vvelocity < 0) {
      //if touching top of screen
      //do not go up and print message Hey now the moon is down there - stop trying to break into the scoreboard!
      if (delay >= 2 && !(player->pos.y < 5)) {
          mvaddch(player->pos.y, player->pos.x, ' ');
          player->pos.y -= 1;
          mvaddch(player->pos.y, player->pos.x, player->ch);
          player->vvelocity = player->vvelocity + 1.5;
          delay = 0;
          mvprintw(2,width/2 - 5,"          ");
      }
      //if touching top of screen
      else if (delay >= 2 && player->pos.y < 5) {
        mvprintw(2,width/2 - 5,"No entry!!");
        player->vvelocity = player->vvelocity + 1.5;
        delay = 0;
      }
    }

    //if player is going fast, FPS fast
    if (player->hvelocity > 5 || player->hvelocity < -5) {
      FPS = 3;
    }

    //if player is going normal speed, FPS return to normal
    if (player->hvelocity <= 5 || player->hvelocity >= -5) {
      FPS = 2;
    }

    handleInput(player,ch);
    overlayCanvas(player,difficulty,score,counter,height,width);
    mvaddch(player->pos.y, player->pos.x, player->ch);

    if (tiles[player->pos.y][player->pos.x].tile_id == 2 && player->vvelocity < 7 && player->hvelocity < 7) {
      score += (difficulty)*(1000-player->fuel)*(counter)*tiles[player->pos.y][player->pos.x].score_mult;
      //mvaddch(player->pos.y, player->pos.x, ' ');
      //mvaddch((player->pos.y)-1, player->pos.x, player->ch);
      player->pos = start_pos;
      init_pair(1,COLOR_GREEN,COLOR_BLACK);
      attron(COLOR_PAIR(1));
      refresh();
      mvprintw(height/2,(width/2) - 5,"YOU WIN!!!");
      mvprintw(height/2 + 1,(width/2) - 17,"Press E to go to the next level!");
      player->vvelocity = 0;
      player->hvelocity = 2;
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
      attroff(COLOR_PAIR(1));
      clear();
      overlayCanvas(player,difficulty,score,counter,height,width);
      generateTerrain(tiles, height, width);
      refresh();
    }

    if ((tiles[player->pos.y][player->pos.x].tile_id == 1) || (tiles[player->pos.y][player->pos.x].tile_id == 2 && player->vvelocity >= 7) || (tiles[player->pos.y][player->pos.x].tile_id == 2 && player->hvelocity >= 7)) {
      //mvaddch(player->pos.y, player->pos.x, ' ');
      //mvaddch((player->pos.y)-1, player->pos.x, player->ch);
      player->pos = start_pos;
      init_pair(1,COLOR_RED,COLOR_BLACK);
      attron(COLOR_PAIR(1));
      mvprintw(height/2,(width/2) - 6,"YOU LOSE!!!");
      mvprintw(height/2 + 1,(width/2) - 21,"Press Q to quit the game or E to restart!");
      player->vvelocity = starting_vY;
      player->hvelocity = starting_vX;
      score = 0;
      counter = 0;
      player->fuel = fuel;
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
      attroff(COLOR_PAIR(1));
      clear();
      overlayCanvas(player,difficulty,score,counter,height,width);
      generateTerrain(tiles, height, width);
      refresh();
    }

    counter++;
    while(getch() != ERR) {} // emptys getch buffer so no input lag
    napms(1000/FPS);
  }

  endwin();

  return 0;
}
