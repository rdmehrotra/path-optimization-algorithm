//=================================================================
// Copyright 2026 Georgia Tech.  All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must not be made publicly available anywhere.
//=================================================================

/*     
Student Name: Rohan Mehrotra
Date: 03/02/2026

ECE 2035 Project 1-1

Diamond Path
This program finds the minimum-cost 5-step path from the center
of an 11x11 diamond grid to any edge square.
*/

#include <stdio.h>
#include <stdlib.h>

#define DEBUG 0  // Must be 0 for submission
#define ArraySize 121

int Load_Mem(char *InputFileName, int *IntArray);

/* ---------------- Helper Functions ---------------- */

/* Convert 1D array index to row number */
int getRow(int index) {
    return index / 11;
}

/* Convert 1D array index to column number */
int getCol(int index) {
    return index % 11;
}

/* 
Check whether a square lies inside the diamond.
The diamond is defined by Manhattan distance <= 5
from the center (row 5, col 5).
*/
int validSquare(int index) {
    int row = getRow(index);
    int col = getCol(index);
    int dist = abs(row - 5) + abs(col - 5);
    return (dist <= 5);
}

/* 
Check whether a square is on the edge of the diamond.
Edge squares have Manhattan distance exactly equal to 5.
*/
int isEdge(int index) {
    int row = getRow(index);
    int col = getCol(index);
    int dist = abs(row - 5) + abs(col - 5);
    return (dist == 5);
}

/* 
Depth-first search to explore all possible 5-step paths.
Parameters:
- Array: weight map
- index: current square index
- steps: number of moves taken so far
- cost: accumulated path cost
- MinCost: pointer to current minimum path cost
*/
void dfs(int *Array, int index, int steps, int cost, int *MinCost) {

    /* Base case: exactly 5 moves taken */
    if (steps == 5) {
        /* Only update minimum if we ended on an edge square */
        if (isEdge(index)) {
            if (cost < *MinCost) {
                *MinCost = cost;
            }
        }
        return;
    }

    /* Possible movement directions: up, down, left, right */
    int moves[4] = {-11, 11, -1, 1};

    for (int i = 0; i < 4; i++) {
        int newIndex = index + moves[i];

        /* Ensure move stays inside array bounds and diamond */
        if (newIndex >= 0 && newIndex < 121 &&
            validSquare(newIndex) &&
            Array[newIndex] != 10000) {

            dfs(Array, newIndex, steps + 1,
                cost + Array[newIndex], MinCost);
        }
    }
}

/* ---------------- Main Program ---------------- */

int main(int argc, char *argv[]) {

    int Array[ArraySize];
    int Length, MinCost = 10000;

    /* Validate command-line argument */
    if (argc != 2) {
        printf("usage: P1-1 valuefile\n");
        exit(1);
    }

    /* Load weight map from file */
    Length = Load_Mem(argv[1], Array);
    if (Length != ArraySize) {
        printf("valuefile does not contain valid data.\n");
        exit(1);
    }

    /* Start DFS from center square (index 60) */
    int center = 60;
    dfs(Array, center, 0, 0, &MinCost);

    /* Required output format for autograder */
    printf("The shortest path cost is [%d]\n", MinCost);
    return 0;
}

/* 
Load_Mem:
Reads up to 121 newline-delimited integers from file.
Stores them into IntArray and returns number of values read.
*/
int Load_Mem(char *InputFileName, int IntArray[]) {

   int N, Addr, Value, NumVals;
   FILE *FP;

   FP = fopen(InputFileName, "r");

   if (FP == NULL) {
      printf("%s could not be opened; check the filename\n", InputFileName);
      return 0;
   } else {
      for (N = 0; N < ArraySize; N++) {
         NumVals = fscanf(FP, "%d: %d", &Addr, &Value);
         if (NumVals == 2)
            IntArray[N] = Value;
         else
            break;
      }
      fclose(FP);
      return N;
   }
}

