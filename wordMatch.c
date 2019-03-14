#include<stdio.h>
#include<stdlib.h>
#include<string.h>

//==========================================================
// function: findMatchOneLine
// input: 
// puzzleLine - one of lines in the puzzle
// userLine - user input word
// userLen - length of user input word
// posList - list of starting positions found
// output: number of positions found
//==========================================================
int findMatchOneLine(char* puzzleLine, char* userLine, int userLen, int* posList)
{
   printf("enter findMatchOneLine\n");

   int i = 0, j = 0, cnt = 0;
   for (i = 0; puzzleLine[i]; i++)
   {
      if (puzzleLine[i] == userLine[0])
      {
         char* substr = calloc(userLen, sizeof(char));
         strncpy(substr, puzzleLine+i, userLen);
         if (!strcmp(substr, userLine))
         {
            posList[cnt++] = i;
            printf("%d th position found:%d\n", cnt, i);
            continue;
         }
      }  
   }
   return cnt;
}

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
int findMatchMatrix(char** matrix, char* userLine, int userLen, int** posList, int numRow, int numCol)
{
   for (int i = 0; i < numRow; i++)
   {
      posList[i] = calloc(numCol, sizeof(int));
      findMatchOneLine(matrix[i], userLine, userLen, posList[i]);
   }
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
   
   findMatchMatrix(puzzle, userLine, userLen, posList, numRow, numCol);
   
   // print out posList
   for (i = 0; i < numRow; i++)
   {
      printf("i=%d\n", i);
      for (int j = 0; j < numCol; j++)
      {  
         printf("_%d", posList[i][j]);
      }
      printf("\n");
   }
   
   return 0;
}




