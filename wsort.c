#include<stdio.h>
#include<errno.h>
#include<stdlib.h>

struct disgustingWord{
    int size;
    int *letters;
};
typedef struct disgustingWord goodboi;

struct horrendousArray{
    int size;
    goodboi *wordArray;
};
typedef struct horrendousArray betterboi;

goodboi createWordarray(void){
    // word array contains A SINGLE WORD
    int *wordArray = malloc(1);			 	// init the array
    int newChar;    						// the letter we are currently reading
    int charCount = 0; 						//keep track of how many characters were read in this word so far

    while(1){
        newChar = fgetc(stdin); // read next letter, advances pointer by one
        if(ferror(stdin) != 0){
            free(wordArray);
			perror("soemthing went wrong with reading next character");
			exit(EXIT_FAILURE);
		}
        wordArray = realloc(wordArray, sizeof(int) * (charCount +1));    
        if ((newChar == '\n') || (newChar == EOF)){break;} // if we reached the end of one word or of the whole file abort
        if(wordArray == NULL){
            free(wordArray);
			perror("memory realloc failed");
			exit(EXIT_FAILURE);
		}
        wordArray[charCount] = newChar;
        charCount ++;
    }
    // always add \n at the end to avoid confusion further down the roadman 
    //wordArray = realloc(wordArray,  sizeof(int) * (charCount +1));
    if(wordArray == NULL){
		free(wordArray);
        perror("memory realloc failed");
        exit(EXIT_FAILURE);
	}
    wordArray[charCount] = '\n';
    charCount ++;

    goodboi newWord; // init new goodboi object
    newWord.size = charCount;
    newWord.letters = wordArray;
    return newWord;
}

betterboi *allInOne(betterboi *allWordsOneArray, goodboi word){ 
    // input: the array we want to add the word to  , word we want to append

    if(word.size < 2){return allWordsOneArray;} 	// if word empty, skip 
	if(word.size > 101){ return allWordsOneArray;} 	// if word too big skip it

    // access wordArray attribute in struct and increase size  of allocated mem
    allWordsOneArray->wordArray = realloc(allWordsOneArray->wordArray, (allWordsOneArray->size+1)*sizeof(goodboi));
	if(allWordsOneArray->wordArray == NULL){
		perror("memory realloc failed");
        exit(EXIT_FAILURE);
	}
    allWordsOneArray->wordArray[allWordsOneArray->size] = word; // append word into the space we just created
    allWordsOneArray->size ++; 									// update size
    return allWordsOneArray;
}

int compare(const void *wordA, const void *wordB) {
    goodboi * A = ((goodboi *) wordA); // recast void pointer as goodboi and dereference it
    goodboi * B = ((goodboi *) wordB);
    
    int max = ((A->size > B->size)*A->size + (B->size >= A->size)*B->size); 	// if A>B then 1*A + 0, if B>=A then  0+1*B
	int i = 0;
	
    for( i = 0; i<max; ++i){
        if ((A->letters[i] == '\n' )|| (B->letters[i] == '\n')){break;} 		// we have reached the end of at least one of the words
		
        if(A->letters[i] != B->letters[i]){
            return (A->letters[i] > B->letters[i]) - (A->letters[i] < B->letters[i]) ; //if A>B: return 1-0 == 1, if B>A return 0-1 == -1
        }
    }

    if ((A->letters[i] == '\n') && (B->letters[i] == '\n')) {return 0;}   // equal til the end and same length -> equal
    if ((A->letters[i] == '\n') && (B->letters[i] != '\n')) {return -1;}  // B longer than A -> B is bigger than A
    return 1;    
}


int main( int argc, char** argv){

    goodboi newWord2; //new test subject


    betterboi *newArray = malloc(sizeof(betterboi)); // malloc space for one betterboi (this will be the array we later fill with words)
    if(newArray == NULL){ 
		perror("memory allocation failed");
        exit(EXIT_FAILURE);
	}
    
    newArray->size = 0;  
    newArray->wordArray = malloc(sizeof(goodboi *)); // allocate space for the goodboi array aka the array containing all words
	if(newArray->wordArray == NULL){
		perror("memory allocation failed");
        exit(EXIT_FAILURE);
	}            
    
    while(feof(stdin) == 0) {					                    // feof tests end of file indicator, do not set errno should not fail according to manual
        newWord2 = createWordarray();				                // read all words one by one
        newArray = allInOne(newArray, newWord2);					// insert words one by one just after they have been created    }   
    }

	qsort(newArray->wordArray, newArray->size, sizeof(goodboi), compare); // use genreric sort with own compare method
	
    // print the array containing all words
    for(int i = 0; i < newArray->size ; ++i){
        //printf("[%d] ", i);
        for(int j = 0; j < newArray->wordArray[i].size; ++j){
            int printTest = printf("%c", (char)(newArray->wordArray[i].letters[j]));
            if(printTest < 0){
				perror("print failed");
				exit(EXIT_FAILURE);
			}
         }
    }
    
   
    // free everything (theoretically at least, there are 4 bytes in one block that are nowhere to be found and refuse to be free)
    free(newWord2.letters);
    
    for(int i = 0; i < newArray->size; ++i)
    {
        free((newArray->wordArray[i]).letters);
    }

    free(newArray->wordArray);
    free(newArray);

    return 0;
}


