#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>


#include "solver.h"

extern int display(char** matrix, displayType type, int** posList);


//==========================================================
// function: findMatchMatrix
// input: 
// matrix - puzzle matrix
// userLine - user input word
// userLen - length of user input word
// posList - list of starting positions found
// numRow, numCol - dimension of puzzle matrix
// output: if succeed, return 0
//==========================================================
int findMatchMatrix(char** matrix, char* userLine, int userLen, int numRow)
{
   printf("88888\n");
   matrix[0][0] = 'm';
#if 0
   for (int i = 0; i < numRow; i++)
   {
      char *p = strstr(puzzleLine, userLine);   
   
      while(p) {
         for (int i = 0; i < userLen; i++) {
            *(p+i) = toupper(*(p+i));
         }
         printf("77777\n");
         p = strstr(p+1, userLine);
      }
   }
#endif
   return 0;
}

int main()
{
   char** puzzle = (char**)calloc(3, sizeof(char*));
   puzzle[0] = "fhsorapplewoappnapples";
   puzzle[1] = "aaappleospwappleswapp";
   puzzle[2] = "woapplawwwsagmml";

   char* userLine = "apple";
   int numRow = 3, numCol = 30;
   int userLen = 5, i = 0;
   //posList:list of pos found axises(row,col)
   int** posList = calloc(numRow, sizeof(int*));
   
   findMatchMatrix(puzzle, userLine, userLen, numRow);
   
   
   display(puzzle, typeSolution, posList);


   return 0;
}




