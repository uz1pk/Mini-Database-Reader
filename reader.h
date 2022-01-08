/*
Name: Usman Zaheer
Class: CIS 2520 (Dr. Stefan Kremer)
Helper function for reader.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
    
#include "hash.h" //given helper library for hash tables


int entries(char *filebase);
void createSet(char *code, int until, char *setData, int setDataIndex, char *setDataFile);
long getIdx(char *fileName, char *hashKey);
void setIntersection(char *setOne, char *setTwo, char *setCreation);
int setIndex(char *fileName, int *indices);
void getString(char *dataType, int index, char *dataSpecific);
char * getData(int index, int type);
