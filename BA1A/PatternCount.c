#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>

/* Program that satisfies the Challenge 1A of Bioinformatics Algorithms: An Active L
 * Learning Approach, 2nd Ed, Vol1, by Phillip Compeau and Pavel Pevzner.
 * Program will read in a parent DNA string, text, and given a smaller DNA string, pattern,
 * gives us the count of the number of occurrences that pattern occurs in text, including
 * overlaps.
 * Developed by Augustine Koh, 3 April 2016 
 * ak.augustine.koh@gmail.com  
 * */ 

int PatternCount(char text[], char pattern[]);
char * Text(char text[], int pos, int k); 

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
    char  in_label[5], dna_pattern[100], out_label[6];

    int  gold_count=0;
    int file_object_count;    

    // TODO: Explore using a different file reading function, like fgets() instead
    // it is known that fscanf() exposes some vulnerabilities. 
    // See my stackoverflow post: http://stackoverflow.com/questions/36382238/difficulty-in-reading-in-a-dna-sequence-file-using-c-code
    file_object_count = fscanf(dataset, "%s %s %s %s %d", in_label, dna_text,dna_pattern, out_label,&gold_count);
   
    if(file_object_count == 5)
    {
      printf("Successfully read in all the file contents!\n");
      printf("We will count the number of occurrences of pattern %s in the parent DNA sequence, including overlaps.\n", dna_pattern);
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
    printf("No input file provided, proceeding to use hardcoded array in the source code.\n");

    char text[] = {'A','T','C','C','G','A','T','C','C','C','A','T','G','C','C','C','A','T','G', '\0'};
    char pattern[] = {'C','C','\0'};
  
    /* Some other hardcoded test arrays */
    //char text[] = {'G','C','G', 'C', 'G', '\0'};
    //char pattern[] = {'G', 'C', 'G' , '\0'};

    //char text[]={'A', 'C', 'G', 'T', 'A', 'C', 'G', 'T', 'A', 'C','G','T','\0'};
    //char pattern[] = {'C','G','\0'};

    int c = PatternCount(text, pattern);
    printf ("My count is %d.\n", c);
  }

return 0;
}

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
  //char * k_mer = (char *)malloc(strlen(pattern)+1);
  int i;  // iteration variable that indicates the position on DNA seq in current iteration
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
  char * k_mer = (char *)malloc((k + 1)*sizeof(char));
  for(i=0; i<= k-1; i++)
  {
    k_mer[i] = text[pos+i];
    //printf("iteration: %d, k_mer: %c\n", i,k_mer[i]);
    if(i == k-1)
      k_mer[k] = '\0';
  }
  return k_mer;
}


/* Some remarks to myself as I was developing this code */

/* At the start, I was contemplating whether to use string data type (array of characters
 * terminated by \0 character) to contain my DNA sequences, but after consulting
 * my textbook, I found that it might be better to use an array because 
 * it is not possible to modify a string data type in C, in general.
 * Also, as the input size of the DNA sequence could be varying, I want to
 * be able to do dynamic memory allocation of an array.
 * */

/* Ok, I reconsider my statement... if I use strings data type, I do
 * get to have access to a lot of convenient string standard library 
 * functions that make my life easier. But never mind, first I'll 
 * implement with array directly, to learn how to work with array, then later
 * I will see if I can turn my code into string..
 */

/* Ok this is crazy, but after realizing that when it comes to arrays... there
 * are no easy ways to determine their sizes (well there is the K&R C method, but that
 *   fails to work when an array's pointer gets passed into a function, which will
 *   happen a few times here, because the array is no longer in the same scope 
 *   and decays to a pointer, where at that stage its just impossible to even
 *   determine its size. Better to just leverage on std library, and immediately use library's string function.
 *   But I must declare my string as a char array that has \0 for last element, rather than
 *   as a literal like "Hello", because declaring it as a literal makes it unmodifiable, which cannot be for my use case here.
 * */
