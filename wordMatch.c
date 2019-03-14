#include<stdio.h>
#include<stdlib.h>
#include<string.h>

//==========================================================
// function: findWordMatch
// input: 
// puzzleLine - one of lines in the puzzle
// userLine - user input word
// len - length of user input word
// pos - list of starting positions found
// output: if succeed, return 0
//==========================================================
int findWordMatch(char* puzzleLine, char* userLine, int len, int* pos)
{
   printf("enter findWordMatch\n");

   int i = 0, j = 0, k = 0;
   for (i = 0; puzzleLine[i] != '\0'; i++)
   {
      if (puzzleLine[i] == userLine[0])
      {
         char* substr = calloc(len, sizeof(char));
         strncpy(substr, puzzleLine+i, len);
         if (!strcmp(substr, userLine))
         {
            pos[k++] = i;
            printf("%d th position found:%d\n", k, i);
            continue;
         }
      }  
   }
   return 0;
}



int main()
{
   char* puzzleLine = "fhsorapplewoappnapples";
   char* userLine = "apple";
   int len = 5, pos = 0;
   
   findWordMatch(puzzleLine, userLine, len, &pos);
   
   return 0;
}