#ifndef __SOLVER_H_
#define __SOLVER_H_


//#include "solver.c"
typedef enum{
   typePuzzle,
   typeInput,
   typeSolution
} displayType;


int display(char** matrix, displayType type, int** posList);

#endif //__SOLVER_H_