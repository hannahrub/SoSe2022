#include <stdio.h>
#include <stdbool.h>

int main(int argc, char **argv) {

    const int MAX = 100;
    bool prime[MAX + 1]; // init array
    // deal with special cases 0,1
    prime[0] = false;
    prime[1] = false;
    // fill array with true
    for(int i = 2; i <= MAX; i++){
        prime[i] = true;
    }
    for(int i=2; i<=MAX; i++){
        if (prime[i] == true){
            // set multiples to false
            int j = 2*i;
            while(j <= MAX) {
                prime[j] = false;
                j += i;
            }
            }
        // have we reached square(n+1) yet?
        if (i*i >= MAX){
            break;
        }
        }
    for(int i =0; i<= MAX; i++){
        if(prime[i] == false){continue;}
        printf("%d \n", i);
    }
    return 0;
    }
