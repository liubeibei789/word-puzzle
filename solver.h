#ifndef __SOLVER_H_
#define __SOLVER_H_


//#include "solver.c"
typedef enum{
   typePuzzle,
   typeInput,
   typeSolution
} displayType;

typedef enum{
   right2left,
   top2bottom,
   bottom2top,
   topleft2bottomright,
   bottomright2topleft,
   topright2bottomleft,
   bottomleft2topright
} dirOpt;
#endif //__SOLVER_H_