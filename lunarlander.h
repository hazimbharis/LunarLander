#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

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
  int tile_id; // 0 is air, 1 is mountain, 2 is pad
  int score_mult;
} Tile;