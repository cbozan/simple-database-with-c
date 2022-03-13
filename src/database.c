#include <stdio.h>
#include <math.h>

#define MAX_PATH 255
#define USERNAME_AND_PASSWORD_LENGTH 20

void printMenu(int);
int getUnsignedInt(int maxLength);
void inputString(char[], int);
int createDatabase(char[MAX_PATH], int);


typedef struct dataStructure{
	int id;
	char user[USERNAME_AND_PASSWORD_LENGTH];
	char password[USERNAME_AND_PASSWORD_LENGTH];
	
} DataStructure;

int main( void ){
	
	printMenu(0);
	
	int dbChoose;
	char dbPath[MAX_PATH];
	int dbDataSize;
	int state = 1;
	
	
	while(state){
		
		printMenu(1);
		dbChoose = getUnsignedInt(1);
		
		switch(dbChoose){
			
			case 1:
				printf("Database name: ");
				inputString(dbPath, MAX_PATH);
				
				//state = initDatabase();
				
				break;
				
			case 2:
				printf("Database name(or path): ");
				inputString(dbPath, MAX_PATH);
				
				printf("Database size: ");
				dbDataSize = getUnsignedInt(8);
				
				state = createDatabase(dbPath, dbDataSize);
				
				state = 0;
				break;
			
			default:
				printf("\n!! invalid entryid !! \n");
		
		}
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


int createDatabase(char dbPath[], int dbDataSize){
	
	FILE *dbFilePtr;
	DataStructure reg = {0, "", ""};
	
	
	if((dbFilePtr = fopen(dbPath, "wb")) == NULL){
		printf("Failed to create database.\n");
	} else{
		
		/*add the database size to the beginning of the file. 
		  Caution! It can be added with a password control in case it is changed manually. 
		  This can be done with a struct. */
		fwrite(&dbDataSize, sizeof(int), 1, dbFilePtr);
		
		int i;
		for(i = 0; i < dbDataSize; i++){
			fwrite(&reg, sizeof(DataStructure), 1, dbFilePtr);
		}
		
		fclose(dbFilePtr);
	}
	
	
}


// safe method to get int input value in a controlled way
int getUnsignedInt(int maxLength){
	
	if(maxLength > 8)
		return -1;
	
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
	fflush(stdin); // clear buffer
	
	return value;
	
}


// safe method to string input value in a controlled way
void inputString(char buffer[], int maxLength){
	
	fgets(buffer, MAX_PATH, stdin);
	
	int i;
	for(i = 0; buffer[i] != '\n' && buffer[i] != '\0'; i++);
	
	buffer[i - 1] = '\0'; // add end of line
	
	fflush(stdin); // clear buffer
	
}


