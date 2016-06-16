#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>


int PatternCount(char text[], char pattern[]);
char * Text(char text[], char k_mer[], int pos, int k); 


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
    char  in_label[5], dna_pattern[100], out_label[5] ;
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

    //printf("%s\n", id_dna_seqs[5]);
    
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

    

    //printf("The input label looks like %s\n", in_label);
    //printf("The dna text looks like %s\n", dna_text);
    //printf("The dna pattern looks like %s\n", dna_pattern);
    //printf("The out label looks like %s\n", dna_pattern);
    //printf("The count looks like %d\n", count);

    // TODO: Correct the below checking code
    int my_count = 0;
    my_count = PatternCount(dna_text,dna_pattern);
    free(dna_text);
    if(my_count == gold_count)
      printf("My count is %d which equals the gold count of %d! Correct!\n", my_count, gold_count);
    else
      printf("My count does not match the gold count!\n");
  }
  
  // If no input file is provided, then we directly use hardcoded arrays
  else 
  {

  // TODO: Implement hardcoded arrays here, see the challenge qn PDF 
  
  }
  
return 0;
}


int FrequentWords(text[], k)
// Implementation closely follows the pseudocode
{

  //TODO: think of how to implement a dynamically grown array for holding frequent patterns
  // Try using http://stackoverflow.com/questions/3536153/c-dynamically-growing-array
  // and this reminds me of ECE220, so go and relook at the past labs
  // and scavenge that code to create my knowledge base!
  char ** freqpattern = (char **)malloc(sizeof(char) * (strlen(text)));

  int i;
  char * pattern  = (char *)malloc(sizeof(k));
  int * count = (int *)malloc(sizeof(int) * (strlen(text) - k +1));
  for(i = 0; i<= strlen(text) - k; i++)
  {
      pattern = Text(i, k);
      count[i] = PatternCount(text, pattern);
  }

  //TODO: Ideintify the largest integer value in the count array, need to write function for finding that maximum
  int maxcount = count[0];
  // can reuse the variable i again?
  // Naive implementation of finding max value in array
  for(i = 0; i< strlen(text)-k+1; i++)
  {
    if(maxcount < count[i])
      maxcount = count[i];
  }

  for(i=0; i< strlen(text)-k+1; i++)
  {
    if(count[i] == maxcount)
      
  }

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
  char * k_mer = (char *)malloc(strlen(pattern)+1);
  int i;  // iteration variable that indicates the position on DNA seq in current iteration
  for(i=0; i<= strlen(text) - strlen(pattern); i++)
  {
    if( strcmp(Text(text, k_mer, i, strlen(pattern)), pattern) == 0)
      count++;
    else
      continue;
  }
  free(k_mer); // remember to free up the memory here, a safe thing to do since I no longer need my k_mer array
  return count;
}

// Text() is a helper function for PatternCount()
char * Text(char text[], char k_mer[], int pos, int k)
/* Function input accepts pointer to a DNA char array, text, a k-mer char array, 
 * k_mer, the position along text, and length of the k_mer pattern
 *  Returns a pointer to a sub-sequence of characters bounded between indexes
 *  pos and pos+k-1.
 */
{
  int i;
  for(i=0; i<= k-1; i++)
  {
    k_mer[i] = text[pos+i];
    //printf("iteration: %d, k_mer: %c\n", i,k_mer[i]);
    if(i == k-1)
      k_mer[k+1] = '\0';
  }
  return k_mer;
}
