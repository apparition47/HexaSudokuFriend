/*
 * main.c
 * HexaSudokuFriend
 * 2013-07-29
 *
 * Engine for a backtracking-based sudoku solver.
 * Resolves recursively by finding the first free
 * cell with the least possibilities then loop
 * through the list of values until a deadend is
 * found. In this case, backtrack to parent and
 * continue to resolve until all cells are satisfied.
 * This is a brute-force approach that has been
 * improved by eliminating impossible candidates
 * from the list of to-try attempts.
 *
 * Based on a backtracking algorithm devised
 * by Steven Skiena (2006).
 */

#include "solver.h"

/*
 * Load a space-separated grid of DIMxDIM
 * as a sudoku grid as a file.
 *
 * Cross-platform where fopen() is used.
 */
void read_board(const char *filename, sudoku *board)
{
	int i,j;
	int value;
	FILE *file;
    
	if ((file = fopen(filename, "r")) == NULL) {
		perror("Cannot open file");
		return;
	}
    
	init_board(board);
    
	for (i=0; i<DIMENSION; i++) {
		for (j=0; j<DIMENSION; j++) {
			fscanf(file,"%d",&value);
			if (value != 0) {
				fill_square(i,j,value,board);
			}
		}
	}
}

/*
 * Heuristic analysis of grid to determine if
 * it is possible to solve (before performing
 * intensive recursion operations).
 */
void possible_values(int x, int y, sudoku *board, int possible[]) {
	int i,j;
	int xlow,ylow;/* origin of box with (x,y) */
	int init;
    
    /* is anything/everthing possible? */
	if ((board->m[x][y] != 0) || ((x<0)||(y<0))) {
		init = FALSE;
	}
	else {
		init = TRUE;
	}
    
	for (i=1; i<=DIMENSION; i++) {
		possible[i] = init;
	}
    
	for (i=0; i<DIMENSION; i++) {
		if (board->m[x][i] != 0) {
			possible[ board->m[x][i] ] = FALSE;
		}
	}
    
	for (i=0; i<DIMENSION; i++) {
		if (board->m[i][y] != 0) {
			possible[ board->m[i][y] ] = FALSE;
		}
	}
    
	xlow = BASED * ((int) (x / BASED));
	ylow = BASED * ((int) (y / BASED));
    
	for (i=xlow; i<xlow+BASED; i++) {
		for (j=ylow; j<ylow+BASED; j++) {
			if (board->m[i][j] != 0) {
                possible[ board->m[i][j] ] = FALSE;
			}
		}
	}
}

int possible_count(int x, int y, sudoku *board) {
	int i;
	int cnt;/* number of open squares */
	int possible[DIMENSION+1];     /* what is possible for the square */
    
	possible_values(x, y, board, possible);
	cnt = 0;
	for (i=0; i<=DIMENSION; i++){
		if (possible[i] == TRUE) {
			cnt++;
		}
	}
    
	return(cnt);
}

/*
 * Populate grid with value and update free count.
 */
void fill_square(int x, int y, int v, sudoku *board) {
	if (board->m[x][y] == 0) {
		board->freecount--;
	}
    
	board->m[x][y] = v;
}

/*
 * Empty cell of grid and update free count.
 */
void free_square(int x, int y, sudoku *board) {
	if (board->m[x][y] != 0) {
		board->freecount=board->freecount+1;
	}
    
	board->m[x][y] = 0;
}


void next_square(int *x, int *y, sudoku* board) {
    int i,j;
    int bestcnt, newcnt;
    int doomed;
    
    bestcnt = DIMENSION + 1;
    doomed = FALSE;
    
    *x = *y = -1;
    
	for (i=0; i<DIMENSION; i++) {
		for (j=0; j<DIMENSION; j++) {
			newcnt = possible_count(i,j,board);
            
            /* walked into a dead end */
			if ((newcnt==0) && (board->m[i][j]==0)) {
				doomed = TRUE;
			}
            
            /* the best and latest square counts */
			if ((newcnt < bestcnt) && (newcnt >= 1)) {
				bestcnt = newcnt;
				*x = i;
				*y = j;
			}
		}
	}
    
    /* no more moves left */
	if (doomed) {
        /* initialize to non-position */
		*x = *y = -1;
	}
}

void init_board(sudoku* board) {
    int i,j;
    
	for (i=0; i<DIMENSION; i++) {
		for (j=0; j<DIMENSION; j++) {
			board->m[i][j] = 0;
		}
        
		board->freecount = NCELLS;
	}
    
    // main defaults
    finished = FALSE;
    steps = 0;
}


void process_solution(int a[], int k, sudoku *board) {
    /* signal/callback to view that we're done */
	finished = TRUE;
}

/*
 * If cell empty count is 0, we're done.
 */
int is_a_solution(int a[], int k, sudoku *board) {
	steps++;
    
    return (board->freecount == 0) ? TRUE : FALSE;
}

void make_move(int a[], int k, sudoku *board) {
	fill_square(board->move[k].x,board->move[k].y,a[k],board);
}


void unmake_move(int a[], int k, sudoku *board) {
	free_square(board->move[k].x,board->move[k].y,board);
}

void construct_candidates(int a[], int k, sudoku *board, int c[], int *ncandidates) {
	int x,y;/* position of next move */
	int i;
	int possible[DIMENSION+1];     /* what is possible for the square */
    
    /* which square should we fill next? */
	next_square(&x,&y,board);
    
    /* store our choice of next position */
	board->move[k].x = x;
	board->move[k].y = y;
    
	*ncandidates = 0;
    
    /* error condition, no moves possible */
	if ((x<0) && (y<0)) {
		return;
	}
    
	possible_values(x,y,board,possible);
	for (i=1; i<=DIMENSION; i++) {
		if (possible[i] == TRUE) {
			c[*ncandidates] = i;
			*ncandidates = *ncandidates + 1;
		}
	}
}



void backtrack(int a[], int k, sudoku *input) {
	int c[MAXCANDIDATES];           /* candidates for next position */
	int ncandidates;                /* next position candidate count */
	int i;
    
	if (is_a_solution(a,k,input)) {
		process_solution(a,k,input);
	}
	else {
		k++;
		construct_candidates(a,k,input,c,&ncandidates);
		
		for (i=0; i<ncandidates; i++) {
			a[k] = c[i];
			make_move(a,k,input);
            
			backtrack(a,k,input);
            
            /* are we done? terminate early */
			if (finished) {
				return;
			}
            
            /* else we're stuck and we should go back */
			unmake_move(a,k,input);
		}
	}
}