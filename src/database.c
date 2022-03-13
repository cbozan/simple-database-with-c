#include <stdio.h>
#include <math.h>

#define MAX_PATH 255

void printMenu(int);
int getUnsignedInt(int maxLength);

int main( void ){
	
	printMenu(0);
	
	FILE *dbFilePtr;
	int dbChoose;
	char dbPath[MAX_PATH];
	int dbDataSize;
	
	printMenu(1);
	int choose = getUnsignedInt(1);
	
	switch(choose){
		
		case 1:
			printf("Database name: ");
			fgets(dbPath, MAX_PATH, stdin);
			break;
			
		case 2:
			printf("Database name(or path): ");
			fgets(dbPath, 50, stdin);
			printf("Database size: ");
			dbDataSize = getUnsignedInt(9);
			
			printf("%s | %d", dbPath, dbDataSize);
			break;
	
	}
	
	return 0;
	
}

void printMenu(int printNumber){
	
	switch(printNumber){
		
		case 0:
			printf("\n\n%30s%41s%-30s\n%30s%41s%-30s\n%30s%41s%-30s\n%96s\n\n", 
					"*************************", "*****************************************", "*************************",
					"*************************", "Professional database management system ", "*************************",
					"*************************", "*************** Welcome *****************", "*************************",
					"*******************************************************************************************");
			break;
			
		case 1:
			printf( "1 - Continue with an existing database\n"
					"2 - Create a new database\n\n"
					"Your choose : ");
			break;	
		
	}
	
}


// safe method to get int input value in a controlled way
int getUnsignedInt(int maxLength){
	
	char input[maxLength];
	fgets(input, maxLength + 1, stdin);

	int value = 0;
	int i, lastLocation = maxLength - 1;
	for(i = maxLength - 1; i >= 0; i--){
		
		if(input[i] != '\0' && input[i] != '\n'){
			
			if( (int)input[i] >= (int)'0' && (int)input[i] <= (int)'9' ){
				
				value += ((int)input[i] - (int)'0') * pow(10, (lastLocation) - i);
				
			} else{
				
				return -1;
				
			}
			
		} else{
			
			lastLocation--;
			
		}
			
	}
	
	char ch;
	while( (ch = getchar()) != '\n' && ch != EOF); // clear buffer
	
	return value;
	
}


