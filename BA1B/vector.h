/* The vector datatype is defined as such.
 * It contains members:
 * size, which specifies the number of elements it is currently holding
 * maxSize, which specifies the largest number of elements that it can actually hold
 * array, which is dnamically allocated to hold an array of integers whose size
 * at any point is given by size and which can never be larger that maxSize
 */
typedef struct {
    int size;
    int maxSize;
    int k_size; // This is the k-mer size 
    // the array variable is a pointer to an array of string character pointers
    // basically an array of pointers. This gives us the flexibility of not needing
    // forming an array of objects whose sizes we may not know now (but only later,
    // which happens when we are doing dynamic memory allocation) and also when
    // the sizes of the various objects in the array are not going to be the same
    // with one another.
    char ** array;
} vector_t;

vector_t * createVector(int initialSize, int k);
void destroyVector(vector_t * vector);
void resize(vector_t * vector);
void push_back(vector_t * vector, char * string);
char * pop_back(vector_t * vector);
char * access(vector_t * vector, int index);
