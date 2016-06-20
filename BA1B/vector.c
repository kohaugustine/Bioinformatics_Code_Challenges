#include "vector.h"
#include <stdlib.h>
#include <stdio.h>

// Reusing some code from ECE220 lab 9

vector_t * createVector(int initialSize)
/*
 * Function that creates a vector object type from an inital size 
 * initialSize is the number of elements that we want to vector to hold 
 * as a starting maximum value
 */
{
	vector_t * vector=(vector_t*)malloc(sizeof(vector_t));
	vector->size=0;
	vector->maxSize=initialSize;
	// We allocate an array of character pointers because we will need to have
	// multiple frequent patterns to access and they could be of varying sizes.
	// The benefit of an array of pointers is that we would not need to worry
	// about not knowing the sizes ahead of time, and also being able to collectively
	// address non-consistent sizes throughout an array
	vector->array=(char**)calloc(initialSize, sizeof(char*));
	return vector;
}

void destroyVector(vector_t * vector)
{
	// Free up the dynamically allocated array in vector structure first before
	// freeing up the structure, because if we free up the structure first, we'll
	// lose the pointer to the dynamically allocated array and have no way to free
	// that memory, causing a memory leak.
	free(vector->array);
	free(vector);
}

void resize(vector_t * vector)
{
	//Anytime we need to resize, it means we double the size as what its original is
	// This is somewhat aribitrary and crude, as we may not necessary need to expand
	// our vector by that much in order to accomodate new objects, but still this
	//works so we do it.
	printf("did a resize\n");
	vector->array=(char**)realloc(vector->array,vector->maxSize*2*sizeof(char *));
	// Update the max size within the vector struct
	vector->maxSize= vector->maxSize*2;
	
}

void push_back(vector_t * vector, char * string)
/* This function treats the vector as a stack in which new pointers to frequent
 * pattern strings that we want to get added to it get pushed at the very end 
 * of the stack.  
 */ 
{
	// If our vector's actual size is at least equal or larger than the maxSize
	// then we shall resize our vector just to give more redundancy
	if(vector->size>=vector->maxSize)
		resize(vector);
	// After first resizing, we have new space that will let us accomodate 
	// some new elements. The reason why we can allocate the element to the 
	// array at position given by the actual size of the vector itself is 
	// because the arrays are indexed from 0. So the actual size of the vector
	// when used as an index, means that we are reaching the position right
	// after the previous last element before the vector was resized.
	printf("Pushed the string: %s to the vector at position %d!\n", string, vector->size);

	vector->array[vector->size]=string;
	vector->size++;
	
}

char * pop_back(vector_t * vector)
/* This function again acts on the vector like a stack. It pops the last element
 * from the array in the vector and returns it to the caller, but does not 
 * shrink the size of the vector.
 */ 
{
	if(vector->size<=0)
		return 0;
	vector->size--;
	return vector->array[vector->size];

}

char * access(vector_t * vector, int index)
{
	if(index>=vector->size)	
		return 0;
	return	vector->array[index];
}
