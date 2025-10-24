/*
   Argyris Patramanis csd4379
   Assignment 4 2023
*/
#include "grid.h"

Grid_T grid_init(Grid_T g, int v[9][9]) {
	int i = 0, j = 0, m = 0, k = 0;
	int subgrid_i;
	int subgrid_j;
	g.unique = 1;
	
	for(i = 0; i < 9; i++) {
		for(j = 0; j < 9; j++) {
			g.cell[i][j].count = 0;
			g.cell[i][j].choices[0] = v[i][j];
			if(g.cell[i][j].choices[0] == 0) {	/* initializing empty cells */
				for(m = 1; m < 10; m++) {
					g.cell[i][j].choices[m] = 1;
					g.cell[i][j].count++;
				}
			}
		}
	}
	
	for(i = 0; i < 9; i++) {
		for(j = 0; j < 9; j++) {
			subgrid_i = (i / 3) * 3;
			subgrid_j = (j / 3) * 3;
			if(g.cell[i][j].choices[0] == 0) {	/* if cell is empty find all possible inputs */
				
				for(m = 0; m < 9; m++) {		/* removing wrong inputs from every row */
					if(g.cell[m][j].choices[0] != 0) {
						if(g.cell[i][j].choices[g.cell[m][j].choices[0]] == 1)
							g.cell[i][j].count--;
						g.cell[i][j].choices[g.cell[m][j].choices[0]] = 0;
					}
				}
				
				for(m = 0; m < 9; m++) {		/* removing wrong inputs from every column */
					if(g.cell[i][m].choices[0] != 0) {
						if(g.cell[i][j].choices[g.cell[i][m].choices[0]] == 1)
							g.cell[i][j].count--;
						g.cell[i][j].choices[g.cell[i][m].choices[0]] = 0;
					}
				}
				
				for(m = subgrid_i; m < subgrid_i+3; m++) {	/* removing the choice from 3x3 subgrid */
					for(k = subgrid_j; k < subgrid_j+3; k++) {
						if(g.cell[m][k].choices[0] != 0) {
							if(g.cell[i][j].choices[g.cell[m][k].choices[0]] == 1)
								g.cell[i][j].count--;
							g.cell[i][j].choices[g.cell[m][k].choices[0]] = 0;
						}
					}
				}
				
				if(g.cell[i][j].count == 0)
					g.unique = -1;
				
			}

		}
	}
	
	return g;
}

Grid_T grid_update(Grid_T g, Choice_T c) {
	Grid_T newGrid = g;
	int i = 0, j = 0;
	int subgrid_i = (c.i / 3) * 3;
	int subgrid_j = (c.j / 3) * 3;
	newGrid.cell[c.i][c.j].choices[0] = c.n;
	
	for(i = 0; i < 9; i++) {	/* removing the choice from every column */
		if(newGrid.cell[c.i][i].choices[c.n] == 1)
			newGrid.cell[c.i][i].count--;
		newGrid.cell[c.i][i].choices[c.n] = 0;	
	}
	
	for(i = 0; i < 9; i++) {	/* removing the choice from every row */
		if(newGrid.cell[i][c.j].choices[c.n] == 1)
			newGrid.cell[i][c.j].count--;
		newGrid.cell[i][c.j].choices[c.n] = 0;	
	}
	
	for(i = subgrid_i; i < subgrid_i+3; i++) {	/* removing the choice from 3x3 subgrid */
		for(j = subgrid_j; j < subgrid_j+3; j++) {
			if(newGrid.cell[i][j].choices[c.n] == 1)
				newGrid.cell[i][j].count--;
			newGrid.cell[i][j].choices[c.n] = 0;
		}
	}

	return newGrid;
}

Choice_T grid_iterate(Grid_T g, Choice_T t) {
	int i = 0, j = 0, k = 0, rchoice, emptyCellFound = 0;
	
	for(k = 1; k < 10; k++) {	
		for(i = 0; i < 9; i++) {
			for(j = 0; j < 9; j++) {
				t.i = i;
				t.j = j;
				if(g.cell[i][j].choices[0] == 0) {
					emptyCellFound = 1;
					if(g.cell[i][j].count == k) {	/* try cells from lowest counts first to biggest last */
						rchoice = (rand() % 9) + 1;
						while(g.cell[i][j].choices[rchoice] != 1)	/* looking for random choice */
							rchoice = (rand() % 9) + 1;		
						t.n = rchoice;
						return t;
					}
				}
			}
		}
	}
	
	if(emptyCellFound == 1) {
		t.i = 0;	/* if no choice is found return 0,0,0 */
		t.j = 0;
		t.n = 0;
	} else {
		t.i = -1;	/* if puzzle is incorrect return -1,-1,-1 */
		t.j = -1;
		t.n = -1;
	}		
	
	return t;
}

int grid_unique(Grid_T g) {
	return g.unique;
}

Choice_T grid_exist_unique(Grid_T g) {
	Choice_T c;
	int i = 0, j = 0;
	
	for(i = 0; i < 9; i++) {
		for(j = 0; j < 9; j++) {
			c.i = i;
			c.j = j;
			if(g.cell[i][j].count == 1) {
				c.n = g.cell[i][j].choices[0];
				return c;
			}
		}
	}
	
	c.i = 0;
	c.j = 0;
	c.n = 0;
	return c;
}

Grid_T grid_clear_unique(Grid_T g) {
	g.unique = 0;
	return g;
}