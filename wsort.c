#include<stdio.h>
#include<error.h>
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
    // remember that we have 0 byte 1 block mem alloc after this. if motivated, fix this
    // word array contains A SINGLE WORD
    int *wordArray = malloc(0);			 	// init the array with 0 memory space bc we realloc later on 
    // TODO HOW TO DO FEHLERBEHANDLUNG BC SIZE IS  or is it caught later on 
    int newChar;    						// the letter we are currently reading
    int charCount = 0; 						//keep track of how many characters were read in this word so far

    while(1){
        newChar = fgetc(stdin); // read next letter, advances pointer by one
        if(ferror(stdin) != 0){
			perror("soemthing went wrong with reading next character");
			exit(EXIT_FAILURE);
			}
        if ((newChar == '\n') || (newChar == EOF)){break;} // if we reached the end of one word or of the whole file abort
        wordArray = realloc(wordArray, sizeof(int) * (charCount +1));
        if(wordArray == NULL){
			perror("memory realloc failed");
			exit(EXIT_FAILURE);
		}
        wordArray[charCount] = newChar;
        charCount ++;
    }
    // always add \n at the end to avoid confusion further down the roadman 
    wordArray = realloc(wordArray,  sizeof(int) * (charCount +1));
    // our realloc array size is always > 0 so NULL return indicates errror?????????
    if(wordArray == NULL){
 //realloc has ENOMEM error what to do with it ???????????????
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

    // print the letters test
    goodboi nW;
    while( (nW = createWordarray()).size > 1) //check if we reached end of file
        for(int i = 0; i < nW.size; ++i)
            //printf("%c", nW.letters[i]); // print letters one by one
            break;
    
    rewind(stdin); //go back to beginning of file
    // rewind returns no value therefore no error handling??????????????

    goodboi nW2; //new test subject


    betterboi *nA = malloc(sizeof(betterboi)); // malloc space for one betterboi (this will be the array we later fill with words)
    if(nA == NULL){ 
		perror("memory allocation failed");
        exit(EXIT_FAILURE);
	}
    
    nA->size = 0;  
    nA->wordArray = malloc(sizeof(goodboi *)); // allocate space for the goodboi array aka the array containing all words
	if(nA->wordArray == NULL){
		perror("memory allocation failed");
        exit(EXIT_FAILURE);
	}            
    
    while(feof(stdin) == 0) {					// feof tests end of file indicator, do not set errno should not fail according to manual
		nW2 = createWordarray();				// qread all words one by one
        nA = allInOne(nA, nW2);					// insert words one by one just after they have been created 
    }   
    

	qsort(nA->wordArray, nA->size, sizeof(goodboi), compare); // use genreric sort with own compare method
	
    // print the array containing all words
    for(int i = 0; i < nA->size ; ++i){
        //printf("[%d] ", i);
        for(int j = 0; j < nA->wordArray[i].size; ++j){
			// does this still print??????????????
            int printTest = printf("%c", (char)(nA->wordArray[i].letters[j]));
            if(printTest < 0){
				perror("print failed");
				exit(EXIT_FAILURE);
			}
         }
    }
    return 0;
}

// todo: maybe free all memory if we hit an error case ???????????????????
