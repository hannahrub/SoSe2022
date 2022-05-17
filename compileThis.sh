#! /bin/bash

echo "lets compile this"

gcc -std=c11 -pedantic -D_XOPEN_SOURCE=700 -Wall -Werror -o wsort wsort.c

echo "Sorting all files"
valgrind --leak-check=full -s ./wsort < sortierlisten/wlist0 > sortierlisten/wlist0.mine
valgrind --leak-check=full -s ./wsort < sortierlisten/wlist1 > sortierlisten/wlist1.mine
valgrind --leak-check=full -s ./wsort < sortierlisten/wlist2 > sortierlisten/wlist2.mine
valgrind --leak-check=full -s ./wsort < sortierlisten/wlist3 > sortierlisten/wlist3.mine
valgrind --leak-check=full -s ./wsort < sortierlisten/wlist4 > sortierlisten/wlist4.mine
valgrind --leak-check=full -s ./wsort < sortierlisten/wlist5 > sortierlisten/wlist5.mine
valgrind --leak-check=full -s ./wsort < sortierlisten/wlist6 > sortierlisten/wlist6.mine

echo "Done sorting"
