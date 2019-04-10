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
   int numCol = strlen(puzzle[0]);
   while (*puzzlePtr)
   {
      numRow = numRow + 1;
      puzzlePtr = puzzlePtr + 1;
   }

   char **tempPuzzle;
   int cnt, i;

   // copy the conent
   switch(dir)
   {
      case right2left:  // flip horizontally
         tempPuzzle = (char**)calloc(numRow+1, sizeof(char**));
         for (int i = 0; i < numRow; i++)
         {
            tempPuzzle[i] = (char*)calloc(numCol+1, sizeof(char*));
            //printf("line %d, before cnvt:%s\n", i, puzzle[i]);
            for (int j = 0; j < numCol; j++)
            {
               tempPuzzle[i][j] = puzzle[i][numCol-1-j];
            }
            //printf("after cnvt:%s\n", tempPuzzle[i]);
         }
         break;
      case top2bottom:  // flip diaonally
         tempPuzzle = (char**)calloc(numCol+1, sizeof(char**)); 
         //printf("numCol=%d, numRow=%d\n",numCol, numRow);
         for (int i = 0; i < numCol; i++)
         {
            tempPuzzle[i] = (char*)calloc(numRow+1, sizeof(char*));
            for (int j = 0; j < numRow; j++)
            {
               tempPuzzle[i][j] = puzzle[j][i];
            }
            //printf("after cnvt:%s\n", tempPuzzle[i]);
         }
         break;
      case bottom2top:  // flip diaonally followed by flip horizontally
         tempPuzzle = (char**)calloc(numCol+1, sizeof(char**));
         // flip diaonally
         for (int i = 0; i < numCol; i++)
         {
            tempPuzzle[i] = (char*)calloc(numRow+1, sizeof(char*));
            for (int j = 0; j < numRow; j++)
            {
               tempPuzzle[i][j] = puzzle[j][i];
            }
         }
         // flip horizontally in place
         for (int i = 0; i < numRow; i++)
         {
            for (int j = 0; j < (numCol+1)/2; j++)
            {
               char tmp = tempPuzzle[i][j];
               tempPuzzle[i][j] = tempPuzzle[i][numCol-1-j];
               tempPuzzle[i][numCol-1-j] = tmp;
            }
            //printf("after cnvt:%s\n", tempPuzzle[i]);
         }
         break;

      //following cases:only considering suqare puzzle at the moment.
      case topleft2bottomright:
         // create triangle
         tempPuzzle = (char**)calloc(numRow*2, sizeof(char*)); 
         cnt = 1;  // number of elements on current row(tempPuzzle)
         i = 0;

         // cnt increases
         for (; i < numRow-1; i++,cnt++)
         {
            tempPuzzle[i] = (char*)calloc(numCol, sizeof(char));
            //printf("*******i=%d, cnt=%d*******\n",i,cnt); //print of traversal
            for (int j = 0; j < cnt; j++)
            {
               //printf("tempPuzzle[%d][%d] = puzzle[%d][%d]\n",i,j,numRow-1-i+j,j); //print of traversal
               tempPuzzle[i][j] = puzzle[numRow-1-i+j][j];
            }
            //printf("after cnvt:%s\n", tempPuzzle[i]);
         }

         // cnt decreases
         for (; i < numRow*2-1; i++,cnt--)
         {
            tempPuzzle[i] = (char*)calloc(numCol, sizeof(char));
            //printf("-------i=%d, cnt=%d---------\n",i,cnt);  //print of traversal
            for (int j = 0; j < cnt; j++)
            {
               //printf("tempPuzzle[%d][%d] = puzzle[%d][%d]\n",i,j,j,j+i-numRow+1);//print of traversal
               tempPuzzle[i][j] = puzzle[j][j+i-numRow+1];
            }
            //printf("after cnvt:%s\n", tempPuzzle[i]);
         }
         /* draft: 
            i=0, numRow=3, j=0
            tempPuzzle[0][0] = puzzle[2][0];

            i=1, numRow=3, j=0,1

            tempPuzzle[1][0] = puzzle[1][0];
            tempPuzzle[1][1] = puzzle[2][1];
            
            i=2, numRow=3, j=0,1,2
            tempPuzzle[2][2] = puzzle[0][0];
            tempPuzzle[2][1] = puzzle[1][1];
            tempPuzzle[2][0] = puzzle[2][2];

            i=3, numRow=3, j=0,1
            tempPuzzle[3][0] = puzzle[0][1];
            tempPuzzle[3][1] = puzzle[1][2];

            i=4, numRow=3, j=0
            tempPuzzle[3][1] = puzzle[0][2];
         */
         break;
      case bottomright2topleft:  // create triangle followed by flip triangle horizontally
         // create triangle
         tempPuzzle = (char**)calloc(numRow*2, sizeof(char*)); 
         cnt = 1;  // number of elements on current row(tempPuzzle)
         i = 0;

         // cnt increases
         for (; i < numRow-1; i++,cnt++)
         {
            tempPuzzle[i] = (char*)calloc(numCol, sizeof(char));
            //printf("*******i=%d, cnt=%d*******\n",i,cnt); //print of traversal
            for (int j = 0; j < cnt; j++)
            {
               //printf("tempPuzzle[%d][%d] = puzzle[%d][%d]\n",i,j,numRow-1-i+j,j); //print of traversal
               tempPuzzle[i][j] = puzzle[numRow-1-i+j][j];
            }
            //printf("after cnvt:%s\n", tempPuzzle[i]);
         }

         // cnt decreases
         for (; i < numRow*2-1; i++,cnt--)
         {
            tempPuzzle[i] = (char*)calloc(numCol, sizeof(char));
            //printf("-------i=%d, cnt=%d---------\n",i,cnt);  //print of traversal
            for (int j = 0; j < cnt; j++)
            {
               //printf("tempPuzzle[%d][%d] = puzzle[%d][%d]\n",i,j,j,j);//print of traversal
               tempPuzzle[i][j] = puzzle[j][j+i-numRow+1];
            }
            //printf("after cnvt:%s\n", tempPuzzle[i]);
         }

         // flip triangle horizontally in place
         for (i = 0; i < numRow*2-1; i++)
         {
            for (int j = 0; j < (cnt+1)/2; j++)
            {
               char tmp = tempPuzzle[i][j];
               tempPuzzle[i][j] = tempPuzzle[i][cnt-j];
               tempPuzzle[i][cnt-j] = tmp;
            }
            if (i < numRow-1) { cnt++;}
            else { cnt--; }
            //printf("i=%d:after cnvt:%s\n", i, tempPuzzle[i]);
         }


         break;
      case topright2bottomleft:
         // create triangle
         tempPuzzle = (char**)calloc(numRow*2, sizeof(char*)); 
         cnt = 1;  // number of elements on current row(tempPuzzle)
         i = 0;

         // cnt increases
         for (; i < numRow-1; i++,cnt++)
         {
            tempPuzzle[i] = (char*)calloc(numCol, sizeof(char));
            //printf("*******i=%d, cnt=%d*******\n",i,cnt); //print of traversal
            for (int j = 0; j < cnt; j++)
            {
               //printf("tempPuzzle[%d][%d] = puzzle[%d][%d]\n",i,j,j,i-j); //print of traversal
               tempPuzzle[i][j] = puzzle[j][i-j];
            }
            //printf("after cnvt:%s\n", tempPuzzle[i]);
         }

         // cnt decreases
         for (; i < numRow*2-1; i++,cnt--)
         {
            tempPuzzle[i] = (char*)calloc(numCol, sizeof(char));
            //printf("-------i=%d, cnt=%d---------\n",i,cnt);  //print of traversal
            for (int j = 0; j < cnt; j++)
            {
               //printf("tempPuzzle[%d][%d] = puzzle[%d][%d]\n",i,j,i-numRow+1+j,numRow-1-j);
               tempPuzzle[i][j] = puzzle[i-numRow+1+j][numRow-1-j];
            }
            //printf("after cnvt:%s\n", tempPuzzle[i]);
         }

         break;
      case bottomleft2topright:   // create triangle and flip horizontally in place
         // create triangle
         tempPuzzle = (char**)calloc(numRow*2, sizeof(char*)); 
         cnt = 1;  // number of elements on current row(tempPuzzle)
         i = 0;

         // cnt increases
         for (; i < numRow-1; i++,cnt++)
         {
            tempPuzzle[i] = (char*)calloc(numCol, sizeof(char));
            //printf("*******i=%d, cnt=%d*******\n",i,cnt); //print of traversal
            for (int j = 0; j < cnt; j++)
            {
               //printf("tempPuzzle[%d][%d] = puzzle[%d][%d]\n",i,j,j,i-j); //print of traversal
               tempPuzzle[i][j] = puzzle[j][i-j];
            }
            //printf("after cnvt:%s\n", tempPuzzle[i]);
         }

         // cnt decreases
         for (; i < numRow*2-1; i++,cnt--)
         {
            tempPuzzle[i] = (char*)calloc(numCol, sizeof(char));
            //printf("-------i=%d, cnt=%d---------\n",i,cnt);  //print of traversal
            for (int j = 0; j < cnt; j++)
            {
               //printf("tempPuzzle[%d][%d] = puzzle[%d][%d]\n",i,j,i-numRow+1+j,numRow-1-j);
               tempPuzzle[i][j] = puzzle[i-numRow+1+j][numRow-1-j];
            }
            //printf("after cnvt:%s\n", tempPuzzle[i]);
         }
         
         // flip triangle horizontally in place
         for (i = 0; i < numRow*2-1; i++)
         {
            for (int j = 0; j < (cnt+1)/2; j++)
            {
               char tmp = tempPuzzle[i][j];
               tempPuzzle[i][j] = tempPuzzle[i][cnt-j];
               tempPuzzle[i][cnt-j] = tmp;
            }
            if (i < numRow-1) { cnt++;}
            else { cnt--; }
            //printf("i=%d:after cnvt:%s\n", i, tempPuzzle[i]);
         }

         break;
      default:
         break;
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
   char **puzzlePtr = puzzle;

   int numRow = 0;
   int numCol = strlen(puzzle[0]);
   while (*puzzlePtr)
   {
      numRow = numRow + 1;
      puzzlePtr = puzzlePtr + 1;
   }

   int cnt, i;

   switch(dir)
   {
      case right2left:  // flip horizontally
         for (int i = 0; i < numRow; i++)
         {
            //printf("line %d, before cnvt back:%s\n", i, tempPuzzle[i]);
            for (int j = 0; j < numCol; j++)
            {
               puzzle[i][j] = tempPuzzle[i][numCol-1-j];
            }
            //printf("after cnvt back:%s\n", puzzle[i]);
         }
         break;
      case top2bottom: // flip diaonally
         for (int i = 0; i < numCol; i++)
         {
            for (int j = 0; j < numRow; j++)
            {
               puzzle[i][j] = tempPuzzle[j][i];
            }
            //printf("after cnvt back:%s\n", puzzle[i]);
         }
         break;
      case bottom2top: // flip horizontally followed by flip diaonally(reversed order) 
         // flip horizontally in place
         for (int i = 0; i < numRow; i++)
         {
            for (int j = 0; j < (numCol+1)/2; j++)
            {
               char tmp = tempPuzzle[i][j];
               tempPuzzle[i][j] = tempPuzzle[i][numCol-1-j];
               tempPuzzle[i][numCol-1-j] = tmp;
            }
         }
         // flip diaonally
         for (int i = 0; i < numCol; i++)
         {
            for (int j = 0; j < numRow; j++)
            {
               puzzle[i][j] = tempPuzzle[j][i];
            }
            //printf("after cnvt back:%s\n", puzzle[i]);
         }
         break;

      //following cases:only considering suqare puzzle at the moment.
      case topleft2bottomright:
         // convert triangle back to rectangle
         cnt = 1;  // number of elements on current row(tempPuzzle)
         i = 0;

         for (; i < numRow; i++,cnt++)
         {
            //printf("*******i=%d, cnt=%d*******\n",i,cnt); //print of traversal
            for (int j = 0; j < cnt; j++)
            {
               //printf("tempPuzzle[%d][%d] = puzzle[%d][%d]\n",i,j,numRow-1-i+j,j); //print of traversal
               puzzle[numRow-1-i+j][j] = tempPuzzle[i][j];
            }
            //printf("after cnvt back:%s\n", puzzle[i]);
         }
         break;
      case bottomright2topleft:  // flip triangle horizontally followed by convert triangle back to rectangle
         // flip triangle horizontally in place
         cnt = 0;
         for (i = 0; i < numRow*2-1; i++)
         {
            for (int j = 0; j < (cnt+1)/2; j++)
            {
               char tmp = tempPuzzle[i][j];
               tempPuzzle[i][j] = tempPuzzle[i][cnt-j];
               tempPuzzle[i][cnt-j] = tmp;
            }
            if (i < numRow-1) { cnt++;}
            else { cnt--; }
            //printf("i=%d:after horizontally flip:%s\n", i, tempPuzzle[i]);
         }

         cnt = 1;  // number of elements on current row(tempPuzzle)
         i = 0;
         for (; i < numRow; i++,cnt++)
         {
            //printf("*******i=%d, cnt=%d*******\n",i,cnt); //print of traversal
            for (int j = 0; j < cnt; j++)
            {
               //printf("tempPuzzle[%d][%d] = puzzle[%d][%d]\n",i,j,numRow-1-i+j,j); //print of traversal
               puzzle[numRow-1-i+j][j] = tempPuzzle[i][j];
            }
            //printf("after cnvt back:%s\n", puzzle[i]);
         }
         break;
      case topright2bottomleft:
         // convert triangle back to rectangle
         cnt = 1;  // number of elements on current row(tempPuzzle)
         i = 0;

         /////////......strange here....///////////
         //all topleft2bottomright, topright2bottomleft, etc.(all 4) case...
         //not to reverse changes made "beforeFinder",
         //but only use the cnt increase part, just change limit from numRow-1 to numRow.
         /////////////////////////////////////////

         // cnt increases
         for (; i < numRow; i++,cnt++) 
         {
            //printf("*******i=%d, cnt=%d*******\n",i,cnt); //print of traversal
            for (int j = 0; j < cnt; j++)
            {
               //printf("tempPuzzle[%d][%d] = puzzle[%d][%d]\n",i,j,j,i-j); //print of traversal
                puzzle[j][i-j] = tempPuzzle[i][j];
            }
            //printf("after cnvt back:%s\n", puzzle[i]);
         }

         break;
      case bottomleft2topright: // flip horizontally in place followed by convert triangle to rectangle
         // flip triangle horizontally in place
         cnt = 0;
         for (i = 0; i < numRow*2-1; i++)
         {
            for (int j = 0; j < (cnt+1)/2; j++)
            {
               char tmp = tempPuzzle[i][j];
               tempPuzzle[i][j] = tempPuzzle[i][cnt-j];
               tempPuzzle[i][cnt-j] = tmp;
            }
            if (i < numRow-1) { cnt++;}
            else { cnt--; }
            //printf("i=%d:after cnvt:%s\n", i, tempPuzzle[i]);
         }

         // convert triangle to rectangle
         cnt = 1;  // number of elements on current row(tempPuzzle)
         i = 0;

         // cnt increases
         for (; i < numRow; i++,cnt++)
         {
            //printf("*******i=%d, cnt=%d*******\n",i,cnt); //print of traversal
            for (int j = 0; j < cnt; j++)
            {
               //printf("tempPuzzle[%d][%d] = puzzle[%d][%d]\n",i,j,j,i-j); //print of traversal
               puzzle[j][i-j] = tempPuzzle[i][j];
            }
            //printf("after cnvt back:%s\n", puzzle[i]);
         }
         break;
   }


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
   char **matrixLine = matrix;
   char **userLine = userInput;
   int i = 0;

   while (strcmp(*userLine, "exit")) //iter through userInput list
   {
      while (*matrixLine != NULL)    //iter through puzzle rows
      {
         //make an all-lower *matrixLine copy first, 
         //find *userLine in this all-lower copy,
         //if found, matched part will be ALL-CAP finally,
         //if not found, use the original *matrixLine

         // something wrong here.....the character at the intersection is lower in solution....
         char *allLowerCopy = calloc(strlen(*matrixLine), sizeof(char));
         strcpy(allLowerCopy, *matrixLine);
         char *ptr = allLowerCopy;

         for (int i = 0; i < strlen(*matrixLine); i++)
         {
            *(ptr+i) = tolower(*(ptr+i));
         } 
         char *p = strstr(allLowerCopy, *userLine);
         //char *p = strstr(*matrixLine, *userLine);   
         while(p) 
         {
            for (int i = 0; i < strlen(*userLine); i++) 
            {
               *(p+i) = toupper(*(p+i));
            }
            printf("one match found, %s\n", *userLine);
            strcpy(*matrixLine, allLowerCopy);
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
   display(puzzle, typePuzzle);
   char **input = readUserInput(USER_INPUT_FILE);
   display(input, typeInput);

   printf("===== looking for words =====\n");


   // ------- left to right (original case) --------
   // no conversion needed before fed into finder  
   finder(puzzle, input); 
   
   // following cases needs conversion......
   char **tempPuzzle;

#if 1
   // --------------- right to left --------------
   tempPuzzle = beforeFinder(puzzle, right2left);
   finder(tempPuzzle, input); 
   afterFinder(puzzle, tempPuzzle, right2left);
#endif
   // --------------- top to bottom --------------
   tempPuzzle = beforeFinder(puzzle, top2bottom);
   finder(tempPuzzle, input);
   afterFinder(puzzle, tempPuzzle, top2bottom);
#if 1
   // --------------- bottom to top --------------
   tempPuzzle = beforeFinder(puzzle, bottom2top);
   finder(tempPuzzle, input);
   afterFinder(puzzle, tempPuzzle, bottom2top);

   // ---------- topleft to bottomright ----------
   tempPuzzle = beforeFinder(puzzle, topleft2bottomright);
   finder(tempPuzzle, input);
   afterFinder(puzzle, tempPuzzle, topleft2bottomright);

   // ---------- bottomright to topleft ----------
   tempPuzzle = beforeFinder(puzzle, bottomright2topleft);
   finder(tempPuzzle, input);
   afterFinder(puzzle, tempPuzzle, bottomright2topleft);

   // ---------- topright to bottomleft ----------
   tempPuzzle = beforeFinder(puzzle, topright2bottomleft);
   finder(tempPuzzle, input);
   afterFinder(puzzle, tempPuzzle, topright2bottomleft);

   // ---------- bottomleft to topright ----------
   tempPuzzle = beforeFinder(puzzle, bottomleft2topright);
   finder(tempPuzzle, input);
   afterFinder(puzzle, tempPuzzle, bottomleft2topright);
#endif

   display(puzzle, typeSolution);

   return 0;
}
   
// careful with interfere after one found--> turn upper case..

