#include <stdio.h>

/* block width within a sudoku grid */
#define BASED   4

/* total size of a sudoku grid */
#define DIMENSION  BASED*BASED
#define NCELLS     DIMENSION*DIMENSION

/* max possible next extensions */
/* low count seems to stall ARM devices? */
#define MAXCANDIDATES   100

/* maximum solution size */
#define NMAX            100

#define TRUE    1
#define FALSE   0

/* x and y coordinates of point */
typedef struct {
    int x, y;
} point;

typedef struct {
    int m[DIMENSION+1][DIMENSION+1];
    int freecount;/* how many open squares remain? */
    
    point move[NCELLS+1];
} sudoku;

int finished; /* solution found, stop search */

int steps; /* how many total move insertions? */

void read_board(const char *, sudoku *);
void possible_values(int, int, sudoku *, int []);
int possible_count(int, int, sudoku *);
void fill_square(int, int, int, sudoku *);
void next_square(int *, int *, sudoku *);
void init_board(sudoku *);
void backtrack(int [], int, sudoku *);
void process_solution(int [], int, sudoku *);
int is_a_solution(int [], int, sudoku *);
void make_move(int [], int, sudoku *);
void unmake_move(int [], int, sudoku *);
void construct_candidates(int [], int, sudoku *, int [], int *);
