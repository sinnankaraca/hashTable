/***************************************************************************************
* File Name          : hashAssignmentV3.cpp
* Description        : Implemenation of a hashTable with linkedlst
*					   Collusion inside hash function has been resolved with linkedlist
*
* Author:              PROG8130 / Sinan KARACA
* Date:                Nov 24, 2021
****************************************************************************************
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

unsigned int putIntoHashTable(char* ptrInputData, unsigned int bufferLength);    // function to add to hash table
unsigned int getFromHashTable(char* ptrOutputData, unsigned int bufferLength);   // function to retrieve data from hash table

#define INPUT_BUFFER_SIZE         200             // local buffer used for adding data to the hash table (there is no reason in this assignment to change this value)

#define HASH_SIZE    10                          // size of hash table to be used (for testing I suggest making this number much lower)

												  // data structure used to keep track of hashed data
struct myHashStruct {
	char* ptrBuffer = NULL;                       // pointer to data stored in hash (you will need to malloc space for string to be stored)
	struct myHashStruct* ptrNextHashData = NULL;  // pointer to next item in this hash bucket (or NULL if no more)
};

struct myHashStruct* myHash[HASH_SIZE];           // create an empty hash table structure (note this is basically an arrary of linked list heads)

int main()
{
	char    inputBuffer[INPUT_BUFFER_SIZE];

	// initialize the hash table to empty one
	for (int i = 0; i < HASH_SIZE; i++)
	{
		if ((myHash[i] = (struct myHashStruct*)calloc(1, sizeof(struct myHashStruct))) == NULL)
		{
			printf("calloc failed!\n");
			return(-1);
		}
	}

	// add to hash table loop
	while (1)
	{
		printf("enter data to be added to hash table or exit when done\n");

		// get strings from the console and place in hash until nothing entered
		scanf_s("%199s", inputBuffer, INPUT_BUFFER_SIZE);
		inputBuffer[INPUT_BUFFER_SIZE - 1] = '\0';

		// stop adding data into hash table when "exit" is entered
		if (strcmp(inputBuffer, "exit") == 0)
			break;

		if (putIntoHashTable(inputBuffer, strlen(inputBuffer)) == HASH_SIZE)
			printf("Error putting into hash table\n");
	}

	// check if data is in hash table
	while (1)
	{
		unsigned int hashIndexNumber = 0;

		printf("Enter data to find, done when complete\n");

		// get strings from the console and check if in hash table
		scanf_s("%199s", inputBuffer, INPUT_BUFFER_SIZE);
		inputBuffer[INPUT_BUFFER_SIZE - 1] = '\0';

		// stop adding data into hash table when "done" is entered
		if (strcmp(inputBuffer, "done") == 0)
			break;

		/*
		if ((hashIndexNumber = getFromHashTable(inputBuffer, strlen(inputBuffer))) == HASH_SIZE)
			printf("%s not found in hash table\n", inputBuffer);
		else
			printf("%s found in hash table at %u\n", inputBuffer, hashIndexNumber);

			*/

		//Changed the function for print out
		hashIndexNumber = getFromHashTable(inputBuffer, strlen(inputBuffer));

		if (hashIndexNumber == 11) {

			printf("% s not found in hash table\n", inputBuffer);

		}
		else {
			printf("%s found in hash table at %u\n", inputBuffer, hashIndexNumber);

		}

	}

	return 0;
}

// FUNCTION      : myHashFunction
// DESCRIPTION   :
//   Calculate a hash value to use in storing the data into the hash table
// PARAMETERS    :
//   ptrInputBuffer - a pointer to the buffer to be put into the hash table
//
// RETURNS       :
//   Hash value calculated ( HASH_SIZE on failure)

/*
Algorithm description : 
P: The value of P can be any prime number roughly equal to the number of different characters used.
For example : if the input string contains only lowercase letters of the English alphabet, then P = 31 is the appropriate value of P.
If the input string contains both uppercase and lowercase letters, then P = 53 is an appropriate option.
M : the probability of two random strings colliding is inversely proportional to m, Hence m should be a large prime number.
M = 10 ^ 9 + 9 is a good choice.

Ref : https://www.geeksforgeeks.org/string-hashing-using-polynomial-rolling-hash-function/

*/

unsigned int myHashFunction(char* ptrInputBuffer)
{
	unsigned int calculatedHashValue = 0;

	int p = 53;
	int m = pow(10,9) + 9;
	long long power_of_p = 1;
	int hashPower = 1;

	for (int i = 0; i < sizeof(ptrInputBuffer); i++) {

		calculatedHashValue = (calculatedHashValue + (ptrInputBuffer[i] - 'a' + 1) * hashPower) % m;

		hashPower = (hashPower * p) % m;
	}


	// make sure if hash value is bigger than the table size, the value wraps
	return calculatedHashValue % HASH_SIZE;
}

// FUNCTION      : putIntoHashTable
// DESCRIPTION   :
//   Put the supplied data into a hash table
// PARAMETERS    :
//   ptrInputBuffer - a pointer to the buffer to be put into the hash table
//   bufferLength   - how many characters are in the buffer being put into the hash table
//
// RETURNS       :
//   Hash value used ( HASH_SIZE on failure)

unsigned int putIntoHashTable(char* ptrInputData, unsigned int bufferLength)
{
	unsigned int hashValue = myHashFunction(ptrInputData);

	// check if the value is in the hash
	// First element of collusion linkedlist

	if (myHash[hashValue]->ptrBuffer == NULL) {

		// Malloc for buffer input
		// +1 for '/0'
		myHash[hashValue]->ptrBuffer = (char*)malloc(bufferLength + 1);
		
		// Check if malloc worked

		if (myHash[hashValue]->ptrBuffer == NULL) {

			printf("Malloc Error myHash char allocation!\n");
			return(-1);
		}

	}

	//If first element exists malloc for next element

	else {

		struct myHashStruct* ptrNext = myHash[hashValue];

		while (ptrNext->ptrNextHashData != NULL) {

			//printf("%s", ptrNext->ptrBuffer);
			ptrNext = ptrNext->ptrNextHashData;
		}
		//
		struct myHashStruct* ptrNew = (struct myHashStruct*)malloc(sizeof(struct myHashStruct));


		// Check if malloc worked
		if (ptrNew == NULL) {

			printf("Malloc error PtrNew struc allocation!\n");
			return(-1);
		}

		ptrNew->ptrNextHashData = NULL;

		ptrNext->ptrNextHashData = ptrNew;

		

	}

	strncpy_s(myHash[hashValue]->ptrBuffer, bufferLength + 1, ptrInputData, bufferLength + 1);
	

return hashValue;
}

// FUNCTION      : getFromHashTable
// DESCRIPTION   :
//   Read as much data as there is room for from the hash table
// PARAMETERS    :
//   ptrOutputBuffer - a pointer to the buffer to place the data read from hash table
//   bufferLength    - maxiumum number of characters that can be read
//
// RETURNS       :
//   Hash value used ( return HASH_SIZE value on failure)

unsigned int getFromHashTable(char* ptrOutputData, unsigned int bufferLength)
{
	unsigned int hashValue = myHashFunction(ptrOutputData);

	unsigned countColomn = 1;


	// check if the data is in the hash table here!!!

	struct myHashStruct* ptrCheck = myHash[hashValue];

	//Check if first element of hastable[index] is not zero
	//If it is not, check if the first element match with the input data

	if (ptrCheck->ptrBuffer != NULL) {

		if (strcmp(ptrCheck->ptrBuffer, ptrOutputData) == 0) {

			return hashValue;
		}

	}

	//Check other elements of linkedlist inside the hash functions

	for (ptrCheck = myHash[hashValue]; ptrCheck->ptrNextHashData != NULL; ptrCheck = ptrCheck->ptrNextHashData) {

		if (strcmp(ptrCheck->ptrBuffer, ptrOutputData) == 0) {

			return hashValue;
		}

	}

	return 11;
}

// end code *     *     *   
