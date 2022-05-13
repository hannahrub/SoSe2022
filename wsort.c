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
    int *wordArray = malloc(0); // init the array with 0 memory space bc we realloc later on 
    int newChar;    // the letter we are currently reading
    int charCount = 0; //keep track of how many characters were read in this word so far

    while(1){
        newChar = fgetc(stdin); // read next letter, advances pointer by one
        if ((newChar == '\n') || (newChar == EOF)){break;} // if we reached the end of one word or of the whole file abort
        wordArray = realloc(wordArray, sizeof(int) * (charCount +1));
        wordArray[charCount] = newChar;
        charCount ++;
    }
    // always add \n at the end to avoid confusion further down the road
    wordArray = realloc(wordArray,  sizeof(int) * (charCount +1));
    wordArray[charCount] = '\n';
    charCount ++;

    goodboi newWord; // init new gooboi object
    newWord.size = charCount;
    newWord.letters = wordArray;
    return newWord;
}

betterboi *allInOne(betterboi *allWordsOneArray, goodboi word){ 
    // input: the array we want to add the word to  , word we want to append

    if(word.size < 2){return allWordsOneArray;} 	// if word empty, skip 
	if(word.size > 101) return allWordsOneArray;} 	// if word empty, skip it

    // access wordArray attribute in struct and increase size  of allocated mem
    allWordsOneArray->wordArray = realloc(allWordsOneArray->wordArray, (allWordsOneArray->size+1)*sizeof(goodboi));

    allWordsOneArray->wordArray[allWordsOneArray->size] = word; // append word into the space we just created
    allWordsOneArray->size ++; // update size
    return allWordsOneArray;
}

int compare(const void *wordA, const void *wordB) {
    goodboi * A = ((goodboi *) wordA); // recast void pointer as goodboi and dereference it
    goodboi * B = ((goodboi *) wordB);
    
    int max = ((A->size > B->size)*A->size + (B->size >= A->size)*B->size);
	int i = 0;
	
    for( i = 0; i<max; ++i){
        if ((A->letters[i] == '\n' )|| (B->letters[i] == '\n')){break;} // we have reached the end of at least one of the words
		
        if(A->letters[i] != B->letters[i]){
            return (A->letters[i] > B->letters[i]) - (A->letters[i] < B->letters[i]) ; //if A>B: return 1-0 == 1, if B>A return 0-1 == -1
        }
    }

    if ((A->letters[i] == '\n') && (B->letters[i] == '\n')) {return 0;}   //equal til the end and same length -> equal
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

    goodboi nW2; //new test subject


    betterboi *nA = malloc(sizeof(betterboi)); // malloc space for one betterboi (this will be the array we later fill with words)
    nA->size = 0;   // set size to 0
    nA->wordArray = malloc(sizeof(goodboi *)); // allocate space for the goodboi array aka the array containing all words
	            
    
    while(feof(stdin) == 0) {
		nW2 = createWordarray();				//read all words one by one
        nA = allInOne(nA, nW2);					// insert words one by one just after they have been created 
    }   
    

	qsort(nA->wordArray, nA->size, sizeof(goodboi), compare); 
    // print the array contan all words
    for(int i = 0; i < nA->size ; ++i)
    {
        //printf("[%d] ", i);
        for(int j = 0; j < nA->wordArray[i].size; ++j)
        {
            printf("%c", (char)(nA->wordArray[i].letters[j]));
            //printf("WELT\n");
        }
    }
    return 0;
}
