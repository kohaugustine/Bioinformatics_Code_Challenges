#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "vector.h"

int PatternCount(char text[], char pattern[]);
char * Text(char text[], int pos, int k); 
vector_t * FrequentWords(char text[], int k);   
vector_t * RemoveDuplicates(vector_t * freqpat_arr);

int main(int argc, char **argv)
{

  if(argc>1)
  {
    FILE * dataset = fopen(argv[1], "r");
      if(dataset==NULL)
      {
        printf("File count not be opened or found!\n");
        return 1;
      }
    int datades = fileno(dataset); // obtain data file descriptor from the data file pointer
    struct stat f_info; // declare a stat structure to hold info about file
    fstat(datades, &f_info); // get the data file information from filedescriptor, and store it in stat structure
    // Dynamically allocate memory DNA text based on the size of the file 
    // since we directly pass the file sizing info, f_info.st_size, to malloc()
    // there is no need to perform sizeof(f_info.st_size) call on passing it in
    // As most of the contriubtion to file size comes from the dna text itself, it
    // should not be too naive to say that dna_text array be allocated a space that 
    // is equal to the size of the file.
    char * dna_text = (char *) malloc(f_info.st_size);
    // Should be safe to assume DNA pattern is about 100 characters long, but we may
    // need to modify it depending on the search we want to perform. in_label and 
    // out_label just hold the fix labels that we will need to read in from
    // the file and are known beforehand to take up this fixed number of characters.

    // Size of identified DNA seqs is as such because we need to account for the spaces too
    char  in_label[5], out_label[5] ;
    char id_dna_seqs[14][13]; // Make sure to store 13, and not 12 characters because on more character is needed for null termination

    // size of k-mer that we want to identify
    int  k_size=0;

    // TODO: Explore using a different file reading function, like fgets() instead
    // it is known that fscanf() exposes some vulnerabilities. 
    // See my stackoverflow post: http://stackoverflow.com/questions/36382238/difficulty-in-reading-in-a-dna-sequence-file-using-c-code
    int file_object_count_1 = fscanf(dataset, "%s %s %d %s", in_label, dna_text,&k_size, out_label);


    //Initially I encountered problems with printing out the strings, until I realized
    // I needed to allocate space for null termination, see http://stackoverflow.com/questions/37632857/difficulty-in-reading-a-series-of-whitespace-separated-dna-string-into-different?noredirect=1#comment62747735_37632857
    int file_object_count_2 = fscanf(dataset, "%s %s %s %s %s %s %s %s %s %s %s %s %s %s", id_dna_seqs[0], id_dna_seqs[1], id_dna_seqs[2], id_dna_seqs[3], id_dna_seqs[4], id_dna_seqs[5], id_dna_seqs[6], id_dna_seqs[7], id_dna_seqs[8], id_dna_seqs[9], id_dna_seqs[10], id_dna_seqs[11], id_dna_seqs[12], id_dna_seqs[13]);

   
    if(file_object_count_1 + file_object_count_2== 18)
    {
      printf("Successfully read in all the file contents!\n");
      printf("We will identify the sequences of size %d, with the most frequent occurrences, in the parent DNA sequence.\n", k_size);
    }
    else
    {
      printf("Did not successfully read in all the file contents!\n");
      return 1;
    }

    vector_t * frequentpat = FrequentWords(dna_text, k_size);

    printf("This is what final frequent patterns array looks like!\n");
    int match_count = 0;
    int i,j;
    for(i=0; i<=frequentpat->size-1 ; i++)
    {
      printf("%s\n", frequentpat->array[i]);
      // Now we check our answer with the golden output
    }
    printf("Now we'll check this with the golden output!\n");
    if(frequentpat->size == 14)
    {
      printf("The size matches!\n");
      for(i=0; i<=frequentpat->size-1 ; i++)
      {      
        for(j=0;j<=13;j++)
        {
          if(strcmp(frequentpat->array[i], id_dna_seqs[j])==0)
            match_count++;
        }
      }
    }
    else
    {
      printf("Size does not match!\n");
      return 1;
    }
    if(match_count == 14)
      printf("We got perfect match for each and every string! Code is correct!\n");
    else
    {
      printf("We did not get perfect match for each element!\n");
      return 1;
    }
    destroyVector(frequentpat);  
  }
  else
  {
    printf("No input file provided! Please provide input file!\n");
    return 1;
  }

return 0;
}


vector_t * FrequentWords(char text[], int k)
// Implementation closely follows the pseudocode
{
  // In think of how to implement a dynamically grown array for holding frequent patterns
  // Try using http://stackoverflow.com/questions/3536153/c-dynamically-growing-array
  // I was reminded me of ECE220, and so I reused the code from lab 9 but had to modify
  // the vector struct to handle strings instead of simple integers
  vector_t * freqpat_arr = createVector(1);
  
  // We allocate a new memory block each iteration that holds pattern 
  // identified from Text() function, and get the number of times it appears
  // in the original DNA sequence, and assign it to a value in the count array
  unsigned int i;
  int * count = (int *)malloc(sizeof(int) * (strlen(text) - k +1));
  for(i = 0; i<= (strlen(text)/sizeof(char)) - k; i++)
  {
    char * pattern = Text(text, i, k);
    count[i] = PatternCount(text, pattern);
    free(pattern);
  }

  // Simple remark: I can reuse the iteration variable i here again without 
  // declaring another separate variable
  // Naive implementation of finding max value in array
  // TODO: Figure out a better implementation, based on exercises from
  // Algorithms in C++ textbook by Sedgewick
  int maxcount = count[0];
  for(i = 0; i< (strlen(text)/sizeof(char))-k+1; i++)
  {
    if(maxcount < count[i])
      maxcount = count[i];
  }

  
  // Loop for adding the identified frequent patterns into our frequent pattern
  // collection array
  for(i=0; i< (strlen(text)/sizeof(char))-k+1; i++)
  {
    if(count[i] == maxcount)
    {
      char * freqpat = Text(text, i ,k);
      printf("Found frequent pattern: %s, adding it to freqpat_arr.\n", freqpat);
      push_back(freqpat_arr, freqpat);
      
    }

  }
  free(count);
  vector_t * freqpat_no_dup = RemoveDuplicates(freqpat_arr);
  destroyVector(freqpat_arr); // Free the old array after the new no duplicate array is generated
  return freqpat_no_dup;
}


// Reusing the PatternCount function from problem BA1A

int PatternCount(char text[], char pattern[])
/* Function accepts a complete DNA sequence, which is char array text[] and
 * the pattern array which we want to identify for k-mer, which is char array
 * pattern[].
 * Returns the count of the number of time array pattern[] was present in 
 * text[] including overlaps.
 * */
{
  int count = 0;
  /* Purpose of using malloc to define the array that will  hold
   * my k_mer sequence is because I need to be passing my k_mer array into 
   * Text() function by reference. I can't be returning a pointer to the k_mer
   * array from within the Text() function. When I initially tried that, I 
   * was getting severely indeterminate bugs, because the run time stack would 
   * be torn down after the function call was made and when I wanted to use
   * the char string k_mer, the char values in it had changed by the time
   * I make use of it within this PatternCount function.
   * */
  unsigned int i;  // iteration variable that indicates the position on DNA seq in current iteration
  //TODO: Figure out why the compiler gives me a warning here if I don't declare
  // i as unsigned... in problem BA1A, I didn't need to declare as unsigned, and 
  // still would not get error...
  for(i=0; i<= (strlen(text)/sizeof(char) - strlen(pattern)/sizeof(char)); i++)
  {
    char * k_str = Text(text, i, strlen(pattern));
    if( strcmp(k_str, pattern) == 0)
      count++;
    else
      continue;
    free(k_str); //After each iteration, we gonna generate a new k_str from a new index, so free the no longer needed current k_str
  }
  return count;
}

// Text() is a helper function for PatternCount()
char * Text(char text[], int pos, int k)
/* Function input accepts pointer to a DNA char array, text, the position along text, 
 * pos, and length of the k_mer pattern, k
 *  Returns a pointer to a sub-sequence of bouded between indexes
 *  pos and pos+k-1.
 */
{
  int i;
  // allocate one more additional character space in order to allow for null char
  // to be inserted when constructing sub-sequence string
  char * k_mer = (char *)malloc((k + 1)*sizeof(char));
  for(i=0; i<= k-1; i++)
  {
    k_mer[i] = text[pos+i];
    //printf("iteration: %d, k_mer: %c\n", i,k_mer[i]);
    if(i == k-1)
      k_mer[k] = '\0'; //insert a null character at the end to terminate string
  }
  return k_mer;
}

// Helper function to remove duplicates from frequentpatterns array
vector_t * RemoveDuplicates(vector_t * freqpat_arr)
{
  vector_t * no_dup = createVector(2);
  int i,j;
  for(i=0; i<= freqpat_arr->size -1; i++)
  {
    int dup_count = 0;
    // For a given element in freqpat array, compare it to each and every element
    // in the no_duplicates array. If there is a repetition of element, increment
    // the duplicate counter. 
    for(j=0; j<=no_dup->size-1;j++)
    {
      if(strcmp(freqpat_arr->array[i], no_dup->array[j]) == 0)
        dup_count++;
    }
    // If there is no duplicates after comparison, then it is safe to push
    // the pattern into no_dup
    if(dup_count==0)
      push_back(no_dup, freqpat_arr->array[i]);
  }
  return no_dup;
}



