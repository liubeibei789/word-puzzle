#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#include "solver.h"

#define MAX_DIGIT_N 3 //max puzzle size 999*999
#define MAX_WORD_LEN 30 //max file name length
#define PUZZLE_FILE "./puzzle3.txt" //puzzle file directory
#define USER_INPUT_FILE "./human_input3.txt" //user input directory


//==========================================================
// function: readPuzzle
// input: puzzle file directory
// e.x. puzzle3.txt
// first line: 2 numbers(number of rows followed by number of columns)
// starts from second line: 2-D char matrix
// output: 2-D puzzle matrix
//==========================================================
char** readPuzzle(char* fileDir)
{
   char *temp;   // store string temporily
   temp = calloc(MAX_DIGIT_N, sizeof(char));
   int numRow = 0, numCol = 0;
   
   // open file
   printf("Welcome to Puzzle!\n");
   FILE *fp;
   fp = fopen(fileDir, "r");

   // read row number
   fscanf(fp, "%s", temp); 
   numRow = atoi(temp);
   //printf("numRow = %d\n", numRow);

   // read column number
   fscanf(fp, "%s", temp);
   numCol = atoi(temp);
   //printf("numCol = %d\n", numCol);

   getc(fp); //collect the '\0' after 20 20

   // capture matrix row-wise   
   char **puzzle = (char**)calloc(numRow+1, sizeof(char*));  

   //read puzzle matrix
   for (int i = 0; i < numRow; i++)  //numRow
   {
      puzzle[i] = (char*)calloc(numCol+1, sizeof(char));
      fscanf(fp, "%s", puzzle[i]);
      for(int j = 0; puzzle[i][j]; j++){
         puzzle[i][j] = tolower(puzzle[i][j]);
      }
   }

   // close file pointer and clean up
   fclose(fp);
   free(temp);
   temp = NULL;


   return puzzle;
}

//==========================================================
// function: readUserInput
// input: user input file directory
// e.x. human_input3.txt
// first line: puzzle file name
// starts from second line: list of words
// output: list of words
//==========================================================
char** readUserInput(char* fileDir)
{
   // open file
   FILE *fp;
   fp = fopen(fileDir, "r");
   
   char **wordList = NULL;
   char *word = "dummy";  // can i get rid of this?
   int i = 0, cnt = 0;

   char *puzzleFile = (char*)calloc(MAX_WORD_LEN, sizeof(char));
   fscanf(fp, "%s", puzzleFile);
   if (!strcmp(puzzleFile, PUZZLE_FILE))
   {
      printf("wrong user input file\n");
   }

   int curAllocated = 100;  // init memory allocated for user input
   wordList = calloc(curAllocated, sizeof(char*));

   while(strcmp(word, "exit"))
   {
      //free(word);  // when shall i free "word"?
      //word = NULL;
      word = calloc(MAX_WORD_LEN, sizeof(char));
      fscanf(fp, "%s", word);
      for(int i = 0; word[i]; i++){
         word[i] = tolower(word[i]);
      }
      /* // this piece of code is not working.confused?
      for ( ; *word; ++word)
      {
         *word = tolower(*word);
      }*/

      wordList[cnt++] = word;

      if (cnt >= curAllocated)
      {
         curAllocated = 2 * curAllocated;
         register void *res = realloc(wordList, curAllocated);
      } 
   }

   return wordList;  
}

//==========================================================
// function: beforeFinder
// convert matrix to be ready to be fed into left2right finder
// output: if succeed, return 0
//==========================================================
char** beforeFinder(char** puzzle, dirOpt dir)
{
   char **puzzlePtr = puzzle;
   // initialize a tempPuzzle
   int numRow = 0;
   int numCol = strlen(puzzle[0])*2;  //should be root(2),1.414. round to 2.
   while (*puzzlePtr)
   {
      numRow = numRow + 2;  //should be root(2),1.414. round to 2.
      puzzlePtr = puzzlePtr + 1;
   }

   char **tempPuzzle = (char**)calloc(numRow, sizeof(char**));

   // copy the conent
   for (int i = 0; i < numRow/2; i++) //
   {
      tempPuzzle[i] = (char*)calloc(numCol, sizeof(char*));
      printf("line %d, before cnvt:%s\n", i, puzzle[i]);
      for (int j = 0; j < numCol/2; j++)
      {
         tempPuzzle[i][j] = puzzle[i][numCol/2-1-j];
      }
      printf("after cnvt:%s\n", tempPuzzle[i]);
   }

   return tempPuzzle;
}

//==========================================================
// function: afterFinder
// convert output from finder back to original matrix
// output: if succeed, return 0
//==========================================================
int afterFinder(char** puzzle, char** tempPuzzle, dirOpt dir)
{
   
   return 0;
}

//==========================================================
// function: finder
// input: 
// matrix - puzzle matrix
// userLine - user input words
// output: if succeed, return 0
//==========================================================
int finder(char** matrix, char** userInput)
{
   printf("88888\n");
   char **matrixLine = matrix;
   char **userLine = userInput;
   int i = 0;

   while (strcmp(*userLine, "exit")) //iter through userInput list
   {
      while (*matrixLine != NULL)    //iter through puzzle rows
      {
         char *p = strstr(*matrixLine, *userLine);   
         while(p) 
         {
            for (int i = 0; i < strlen(*userLine); i++) 
            {
               *(p+i) = toupper(*(p+i));
            }
            printf("77777:one match found, %s\n", *userLine);
            p = strstr(p+1, *userLine);
         }
         matrixLine = matrixLine + 1;
      }
      matrixLine = matrix; // restore matrixLine 
      userLine = userLine + 1;
   }
   return 0;
}


//==========================================================
// function: display
// input: 2-D matrix to be printed, enum display type
// output: if succeed, return 0;
//==========================================================
int display(char** matrix, displayType type)
{
   switch (type)
   {
      case typePuzzle:
         printf("******** Your Puzzle ********\n");
         break;
      case typeInput:
         printf("******** User Input ********\n");
         break;
      case typeSolution:
         printf("******** Solution ********\n");
         break;
      default:
         printf("display type error\n");
   }

   int i = 0;
   
   while (matrix[i] != NULL)
   {
      printf("%s\n", matrix[i]);
      i++;
   }

   return 0;
}

int main()
{
   char **puzzle = readPuzzle(PUZZLE_FILE);
   //display(puzzle, typePuzzle);
   char **input = readUserInput(USER_INPUT_FILE);
   //display(input, typeInput);

   // ----------- left 2 right --------
   //finder(puzzle, input);
   //display(puzzle, typeSolution);
   
   // ----------- right 2 left --------
   char **tempPuzzle = beforeFinder(puzzle, right2left);
   finder(tempPuzzle, input);
   //afterFinder(puzzle, tempPuzzle, right2left);
   //display(puzzle, typeSolution);


   return 0;
}
   

