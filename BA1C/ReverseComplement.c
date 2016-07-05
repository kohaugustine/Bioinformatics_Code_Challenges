#include <stdio.h> 
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>

char * reverseComplement(char * pattern);

int main(int argc, char ** argv)
{
  if(argc > 1)
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
    // should not be too naive to say that dna_input and dna_output arrays be 
    // each allocated a space that is equal to the size of the file.
    char * dna_input = (char *) malloc(f_info.st_size);
    char * dna_output = (char *) malloc(f_info.st_size);
    char  in_label[5], out_label[5] ;

    int file_object_count = fscanf(dataset, "%s %s %s %s", in_label, dna_input, out_label, dna_output);
    
    if(file_object_count == 4)
    {
	printf("Successfully read in all file contents!\n");
	printf("We will now compute the reverse complement of the input string:\n");
	printf("%s\n", dna_input);
    }
    else
    {
      printf("Did not successfully read in all file contents!\n");
      return 1;
    }
  
    // Immediate construct the reverse complement string just as I complement each of the input
    char * rev = reverseComplement(dna_input);
    printf("My output is:\n");
    printf("%s\n", rev);
    
    //Now check that that our output matches the gold output
    int i;
    int mismatch = 0;
    for(i=0; i<=strlen(rev)/sizeof(char) - 1; i++)
    {
      if(rev[i] != dna_output[i])
	mismatch++;
    }
    if(mismatch>0)
    {
      printf("My output does not match the gold output! The gold output should be:\n");
      printf("%s\n", dna_output);
    } 
    else
      printf("My output matches the gold output and is correct!!!\n");
    free(dna_input);
    free(dna_output);
    // Lesson learnt: Initially the call to free(rev) below threw me a double free error
    // only for me to find that the problem was not because I was double freeing, but
    // because I did not allocate enough space for revcomplpattern earlier in the
    // reverseComplement() function. See the post http://stackoverflow.com/questions/38189244/freeing-up-memory-outside-of-the-function-causes-a-double-free-or-corruption-err
    free(rev);
  }
  else
    printf("Please specify the input BA1C_extra_dataset.txt file!\n");
  return 0;
}


// Function reverseComplement

char * reverseComplement(char * pattern)
{
  // create space in memory for the reverse complement array to be returned
  // its size would equal that of input pattern string. Note that I need to add
  // in an additional sizeof(char) because by default, strlen() will ignore the 
  // null character in pattern when returning the size to me, but when I allocated
  // space for revcomplpattern, I need to have that additional space for the 
  // null character that I manually assign later below. 
  char * revcomplpattern = (char *)malloc(strlen(pattern) + 1 * sizeof(char));
  int i;
  // compute index value for end of the string
  int strend = strlen(pattern)/sizeof(char) -1 ;
  // Don't forget to make revcomplpattern a null terminated string
  revcomplpattern[strend+1] = '\0';
  
  // In each iteration over the input array, we check the character at that position
  // and from that, assign its complement into the reverse position on the other
  // reverse complement array. This is more efficient step wise than first constructing the 
  // pure complement array and then doing another step to reverse its order. 
  for(i=0; i<= strend; i++)
  {
    // When comparing non-integers, the switch-case statements do not work
    // so have to use if-else
    if(pattern[i]=='A')
    {
      revcomplpattern[strend-i] = 'T';
      continue;
    }
      
    else if(pattern[i] == 'T')
    {
      revcomplpattern[strend-i] = 'A';
      continue;
    }
    
    else if(pattern[i] == 'C')
    {
      revcomplpattern[strend-i] = 'G';
      continue;
    }
    
    else if(pattern[i] == 'G')
    {
      revcomplpattern[strend-i] = 'C';
      continue;
    }
  }
  return revcomplpattern;
}
  
  
  
  
  
  
      
    
