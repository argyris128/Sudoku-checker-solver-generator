/*
   Argyris Patramanis csd4379
   Assignment 4 2023
*/
#include "sudoku.h"

Grid_T backtrack;
int counter = 0;

Grid_T sudoku_generate(int nelts, int unique) {
	Grid_T newGrid;
	int r1, r2, v, i = 0, j = 0;
	char dir[16];
	FILE* in;
	
	r1 = rand() % 3;
	sprintf(dir, "solved/solved.%d", r1);
	
	in = fopen(dir, "r");
	for(i = 0; i < 9; i++) {
		for(j = 0; j < 9; j++) {
			fscanf(in, "%d", &v);
			newGrid.cell[i][j].choices[0] = v;
		}
	}
	fclose(in);
	
	for(i = 81-nelts; i >= 0; i--) {
		r1 = rand() % 9;
		r2 = rand() % 9;
		while(newGrid.cell[r1][r2].choices[0] == 0) {
			r1 = rand() % 9;
			r2 = rand() % 9;
		}
		newGrid.cell[r1][r2].choices[0] = 0;
	}
	newGrid.unique = unique;
	
	return newGrid;
}

Grid_T sudoku_read(void) {
	Grid_T newGrid;
	int v, i = 0, j = 0;
	
	for(i = 0; i < 9; i++) {
		for(j = 0; j < 9; j++) {
			scanf("%d", &v);
			newGrid.cell[i][j].choices[0] = v;
		}
	}
	
	return newGrid;
}

void sudoku_print(FILE *s, Grid_T g) {
	int i = 0, j = 0;
	
	for(i = 0; i < 9; i++) {
		for(j = 0; j < 9; j++) {
			if(j != 8)
				fprintf(s, "%d ", g.cell[i][j].choices[0]);
			else
				fprintf(s, "%d\n", g.cell[i][j].choices[0]);
		}
	}
}

Grid_T sudoku_solve(Grid_T g, Choice_T c) {
	Grid_T newGrid = g;
	Choice_T temp;
	int i = 0, j = 0;
	
	for(i = 0; i < 9; i++) {	/* Check if puzzle is wrong/impossible */
		for(j = 0; j < 9; j++) {	
			if(g.cell[i][j].choices[0] == 0 && g.cell[i][j].count == 0) {
				newGrid.unique = -1;
				return newGrid;
			}
		}
	}
	
	if(sudoku_is_correct(newGrid) == 0) {
		c = grid_iterate(newGrid, c);
		
		if(c.i == 0 && c.j == 0 && c.n == 0)
			return sudoku_solve(backtrack, c);
		if(c.i == -1 && c.j == -1 && c.n == -1) {
			newGrid.unique = -1;
			return newGrid;
		}
		
		newGrid = grid_update(newGrid, c);
		
		if(newGrid.cell[c.i][c.j].count >= 3)
			backtrack = newGrid;
		
		temp = grid_exist_unique(newGrid);
		if(temp.i == 0 && temp.j == 0 && temp.n == 0 && counter > 30)
			newGrid.unique = 0;
		counter++;
		return sudoku_solve(newGrid, c);
	}
	
	return newGrid;
}

int sudoku_is_correct(Grid_T g) {
	int i = 0, j = 0, k = 0, m = 0;
	
	for(i = 0; i < 9; i++) {
		for(j = 0; j < 9; j++) {			
			int subgrid_i = (i / 3) * 3;
			int subgrid_j = (j / 3) * 3;
			
			if(g.cell[i][j].choices[0] == 0)	/* check if cell is empty */
				return 0;
			
			for(k = 0; k < 9; k++) {	/* check every cell on row */
				if(i!=k) {
					if(g.cell[i][j].choices[0] == g.cell[k][j].choices[0])
						return 0;
				}					
			}
			
			for(k = 0; k < 9; k++) {	/* check every cell on column */
				if(j!=k) {
					if(g.cell[i][j].choices[0] == g.cell[i][k].choices[0])
						return 0;
				}
			}
			
			for(k = subgrid_i; k < subgrid_i+3; k++) {	/* check every cell on 3x3 subgrid */
				for(m = subgrid_j; m < subgrid_j+3; m++) {
					if(i!=k && j!=m) {
						if(g.cell[i][j].choices[0] == g.cell[k][m].choices[0])
							return 0;
					}
				}
			}
			
		}
	}
	
	return 1;
}

int main(int argc, char *argv[]) {
	Grid_T newGrid;
	Choice_T newChoice;
	int i = 0, j = 0, v[9][9], nelts;
	srand(getpid());
	
	if(argc == 1) { 
		fprintf(stderr, "New puzzle:\n");
		newGrid = sudoku_read();
		sudoku_print(stderr, newGrid);
		
		for(i = 0; i < 9; i++) {
			for(j = 0; j < 9; j++) {
				v[i][j] = newGrid.cell[i][j].choices[0];
			}
		}

		newGrid = grid_init(newGrid, v);
		backtrack = newGrid;
		
		newGrid = sudoku_solve(newGrid, newChoice);	
		
		if(grid_unique(newGrid) == 1) {
			fprintf(stderr, "Puzzle has a unique choice solution:\n");
			sudoku_print(stdout, newGrid);
		} else if(grid_unique(newGrid) == 0) {
			fprintf(stderr, "Puzzle has multiple solutions:\n");
			sudoku_print(stdout, newGrid);
		} else
			fprintf(stderr, "Puzzle has no solution\n");
	} else if(strcmp(argv[1], "-c") == 0) {
		newGrid = sudoku_read();
		sudoku_print(stderr, newGrid);
		
		if(sudoku_is_correct(newGrid) == 1)
			fprintf(stderr, "Puzzle solution is correct\n");
		else
			fprintf(stderr, "Puzzle solution is not correct\n");
	} else if(strcmp(argv[1], "-g") == 0) {
		sscanf(argv[2], "%d", &nelts);
		
		newGrid = sudoku_generate(nelts, 1);
		sudoku_print(stdout, newGrid);		
	}
	
	return 0;
}