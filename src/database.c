#include <stdio.h>
#include <math.h>
#include <string.h>

#define MAX_PATH 255
#define USERNAME_AND_PASSWORD_LENGTH 20
#define MAX_MEMORY_DIGIT_LENGTH 8


int createDatabase(char[MAX_PATH], int);
int initDatabase(char[]);
int listData(FILE *, int);
int addData(FILE *, int);
int deleteData(FILE *, int);
int updateData(FILE *, int);
int getDataPosition(FILE *, int , int );
int getUnsignedInt(int maxLength);
void setFileCursor(FILE *, int );
void inputString(char[], int);
void printMenu(int, char[]);


typedef struct dataStructure{
	int id;
	char user[USERNAME_AND_PASSWORD_LENGTH];
	char password[USERNAME_AND_PASSWORD_LENGTH];
	
} DataStructure;


int main( void ){
	
	printMenu(0, "");
	
	int dbChoose;
	char dbPath[MAX_PATH];
	int dbDataSize;
	int state = 1;
	
	while(state){
		printf("\n");
		printMenu(1, "");
		dbChoose = getUnsignedInt(1);
		
		switch(dbChoose){
			
			case 1:
				printf("Database name : ");
				inputString(dbPath, MAX_PATH);
				
				state = initDatabase(dbPath);
				
				fflush(stdin);
				break;
				
			case 2:
				printf("Database name : ");
				inputString(dbPath, MAX_PATH);
				
				printf("Database size: ");
				dbDataSize = getUnsignedInt(8);
				
				state = createDatabase(dbPath, dbDataSize);
		
				fflush(stdin);
				break;
				
			case -1:
				printf("\n\nThe database management system has been terminated.\n\nGoodbye!\n");
				return 0;
			
			default:
				printf("\n!! invalid entryid !! \n\n");
				fflush(stdin);
				break;
				
		}
		
	}
	
	return 0;
	
}


int createDatabase(char dbPath[], int dbDataSize){
	
	FILE *dbFilePtr;
	DataStructure reg = {0, "", ""};
	
	if((dbFilePtr = fopen(dbPath, "wb")) == NULL){
		printf("\n\n!! Failed to create database. !!\n\n");
	} else{
		
		/*add the database size to the beginning of the file. 
		  Caution! It can be added with a password control in case it is changed manually. 
		  This can be done with a struct. 
		  or
		  
		  you can use 
		  
		  fseek(dbFilePtr, 0, SEEK_END);
		  size = ftell(dbFilePtr);
		  rewind(dbFilePtr); 
		*/

		fwrite(&dbDataSize, sizeof(int), 1, dbFilePtr);
		
		int i;
		for(i = 0; i < dbDataSize; i++){
			fwrite(&reg, sizeof(DataStructure), 1, dbFilePtr);
		}
		
		fclose(dbFilePtr);
	}
	
	
	return initDatabase(dbPath);
	
}


int initDatabase(char dbPath[]){
	
	FILE *dbFilePtr;
	DataStructure db;
	int dbDataSize;
	int state = 1;
	
	if((dbFilePtr = fopen(dbPath, "rb")) == NULL){
		printf("\n\n!! Failed to read database. !!\n\n");
	} else{
		
		fread(&dbDataSize, sizeof(int), 1, dbFilePtr);
		
		printMenu(2, dbPath);
		
		while(state){
			printf("\n");
			printMenu(3, "");
			int choose = getUnsignedInt(1);
				
			switch(choose){
				
				case 1:
					if((dbFilePtr = fopen(dbPath, "rb")) == NULL){
						printf("\n\n!! data could not be read !!\n\n");
					} else{
						
						setFileCursor(dbFilePtr, 0);
						state = listData(dbFilePtr, dbDataSize);
						setFileCursor(dbFilePtr, 0);
						
					}
					
					fflush(stdin);
					fclose(dbFilePtr);
					
					break;
					
				case 2:
					if((dbFilePtr = fopen(dbPath, "rb+")) == NULL){
						printf("\n\n!! data could not be write !!\n\n");
					} else{
						
						setFileCursor(dbFilePtr, 0);
						state = addData(dbFilePtr, dbDataSize);
						setFileCursor(dbFilePtr, 0);
						
					}
					
					fflush(stdin);
					fclose(dbFilePtr);
					break;
				
				case 3:
					if((dbFilePtr = fopen(dbPath, "rb+")) == NULL){
						printf("\n\n!! data could not be delete !!\n\n");
					} else{
						setFileCursor(dbFilePtr, 0);
						state = deleteData(dbFilePtr, dbDataSize);
						setFileCursor(dbFilePtr, 0);
					}
					
					fflush(stdin);
					fclose(dbFilePtr);
					
					break;
					
				case 4:
					if((dbFilePtr = fopen(dbPath, "rb+")) == NULL){
						printf("\n\n!! data could not be update !!\n\n");
					} else{
						setFileCursor(dbFilePtr, 0);
						state = updateData(dbFilePtr, dbDataSize);
						setFileCursor(dbFilePtr, 0);
					}
					
					fflush(stdin);
					fclose(dbFilePtr);
					
					break;
					
					
				case -1:
					fflush(stdin);
					return 1;
				
				default:
					printf("\nThis option not available\n");

			}
		
		}
		
	}
	
	return -1;
	
}


int listData(FILE *dbFilePtr, int dbDataSize){
	
	int choose;
	int position;
	DataStructure reg;
	int i;
	
	printf("\n");
	printMenu(4, "");
	choose = getUnsignedInt(1);
	
	switch(choose){
		case 1:
			printf("Enter id : ");
			position = getDataPosition(dbFilePtr, dbDataSize, getUnsignedInt(MAX_MEMORY_DIGIT_LENGTH));
			
			if(position != -1){
				setFileCursor(dbFilePtr, position);
				fread(&reg, sizeof(DataStructure), 1, dbFilePtr);
				
				printf("\n\n\n%*s%*s | %*s%*s | %*s%*s\n%21s|%22s|%21s\n", 
						10 + strlen("ID")/2,  "ID", 10 - strlen("ID")/2, "", 
						10 + strlen("User")/2, "User", 10 - strlen("User")/2, "",
						10 + strlen("Pass")/2, "User", 10 - strlen("Pass")/2, "",
						"--------------------", "--------------------", "--------------------");
				
				printf("%*d%*s | %*s%*s | %*s%*s\n",
						  10 + (int)((floor(log10(abs(reg.id))) + 1)/2), reg.id, 10 - (int)((floor(log10(abs(reg.id))) + 1)/2), "",
						  10 + strlen(reg.user) / 2, reg.user, 10 - strlen(reg.user) / 2, "",
						  10 + strlen(reg.password) / 2, reg.password, 10 - strlen(reg.password) / 2, ""
						  );
				setFileCursor(dbFilePtr, 0);
						  
			} else{
				
				printf("\nNo records found.\n");
				
			}
			
			fflush(stdin);
			return 1;
		
		case 2:
			
			setFileCursor(dbFilePtr, 0);
			
			printf("\n\n\n%*s%*s | %*s%*s | %*s%*s\n%21s|%22s|%21s\n", 
						10 + strlen("ID")/2,  "ID", 10 - strlen("ID")/2, "", 
						10 + strlen("User")/2, "User", 10 - strlen("User")/2, "",
						10 + strlen("Pass")/2, "User", 10 - strlen("Pass")/2, "",
						"--------------------", "--------------------", "--------------------");
			
			for(i = 0; i < dbDataSize; i++){
				fread(&reg, sizeof(DataStructure), 1, dbFilePtr);
				if(reg.id != 0){
					printf("%*d%*s | %*s%*s | %*s%*s\n",
						  10 + (int)((floor(log10(abs(reg.id))) + 1)/2), reg.id, 10 - (int)((floor(log10(abs(reg.id))) + 1)/2), "",
						  10 + strlen(reg.user) / 2, reg.user, 10 - strlen(reg.user) / 2, "",
						  10 + strlen(reg.password) / 2, reg.password, 10 - strlen(reg.password) / 2, ""
						  );
				}
			}
			
			fflush(stdin);
			setFileCursor(dbFilePtr, 0);
			
			return 1;
			
		case 0:
			fflush(stdin);
			return 1;
			
		default:
			printf("\n!! invalid entryid !! \n\n");
			fflush(stdin);
			
			break;
			
	}
	
	return -1;
	
}


int addData(FILE * dbFilePtr, int dbDataSize){
	
	int choose;
	int position;
	DataStructure reg;
	int i, id;
	char user[USERNAME_AND_PASSWORD_LENGTH], pass[USERNAME_AND_PASSWORD_LENGTH];
	
	int state = 1;	
	while(state){
		
		printf("\n--- New Record (For exit : -1)--- \n");
		
		printf("ID : ");
		id = getUnsignedInt(MAX_MEMORY_DIGIT_LENGTH);
		
		if(id == -1){
			return 1;
		}
		
		printf("\nuser : ");
		inputString(user, USERNAME_AND_PASSWORD_LENGTH);
		
		printf("\npass : ");
		inputString(pass, USERNAME_AND_PASSWORD_LENGTH);
		
		if(id > 0 && id <= dbDataSize){
			
			reg.id = id;
			strcpy(reg.user, user);
			strcpy(reg.password, pass);
			
			
			setFileCursor(dbFilePtr, id - 1);
			
			fwrite(&reg, sizeof(DataStructure), 1, dbFilePtr);
			
			printf("\n\ncompleted successfully\n\n");
			state = 0;
			
		} else{
			printf("\n\n!! ID out of range !!\n\n");
		}
		
		state = -1;
	}
	
	return state;
	
}


int deleteData(FILE *dbFilePtr, int dbDataSize){

	DataStructure reg;
	int id, position;
	
	printf("--- deletion process (For exit : -1) ---\n\nID : ");
	id = getUnsignedInt(MAX_MEMORY_DIGIT_LENGTH);
	
	if(id == -1){
		printf("\n!! invalid value !!\n");
		return 1;
	}
	
	position = getDataPosition(dbFilePtr, dbDataSize, id);
	if(position == -1){
		printf("\nNo records found.\n");
		return 1;
	}
	
	setFileCursor(dbFilePtr, position);
	fread(&reg, sizeof(DataStructure), 1, dbFilePtr);
	
	printf("User %s will be deleted (Y/N) : ", reg.user);

	char temp[2];
	inputString(temp, 2);

	if(strcmp("Y", temp) == 0){
		reg.id = 0;
		strcpy(reg.user, "");
		strcpy(reg.password, "");
		
		setFileCursor(dbFilePtr, position);
		fwrite(&reg, sizeof(DataStructure), 1, dbFilePtr);
		
		printf("\ndeleted.\n");
		
	} else{
		printf("\nThe processing has been cancelled.\n");
	}
	
	return 1;
	
}


int updateData(FILE * dbFilePtr, int dbDataSize){
	
	DataStructure reg;
	int id, position, choose;
	
	printf("--- update process (For exit : -1) ---\n\nID : ");
	id = getUnsignedInt(MAX_MEMORY_DIGIT_LENGTH);
	
	if(id == -1){
		printf("\n!! invalid value !!\n");
		return 1;
	}
	
	position = getDataPosition(dbFilePtr, dbDataSize, id);
	if(position == -1){
		printf("\nNo records found.\n");
		return 1;
	}
	
	setFileCursor(dbFilePtr, position);
	fread(&reg, sizeof(DataStructure), 1, dbFilePtr);
	
	printf("1 - Change user\n"
			"2 - Change pass\n"
			"? - For exit : -1\n"
			"\nYour choose : ");
	choose = getUnsignedInt(1);
	
	char temp[USERNAME_AND_PASSWORD_LENGTH];
	switch(choose){
		
		case 1:
			printf("\nNew user : ");
			inputString(temp, USERNAME_AND_PASSWORD_LENGTH);
			strcpy(reg.user, temp);
			
			setFileCursor(dbFilePtr, position);
			fwrite(&reg, sizeof(DataStructure), 1, dbFilePtr);
			printf("\nThe user has been changed.\n");
			
			return 1;
			
		case 2:
			printf("\nNew pass : ");
			inputString(temp, USERNAME_AND_PASSWORD_LENGTH);
			strcpy(reg.password, temp);
			
			setFileCursor(dbFilePtr, position);
			fwrite(&reg, sizeof(DataStructure), 1, dbFilePtr);
			printf("\nThe pass has been changed.\n");
			
			return 1;
			
		default:
			return 1;
		
	}

	return 1;
	
}


int getDataPosition(FILE *dbFilePtr, int dbDataSize, int id){
	
	int i;
	DataStructure reg;

	for(i = 0; i < dbDataSize; i++){
		
		fread(&reg, sizeof(DataStructure), 1, dbFilePtr);
		if(reg.id == id){
			return i;
		}
		
	}
	
	return -1;
	
}


void setFileCursor(FILE *dbFilePtr, int nextSize){
	
	fseek(dbFilePtr, sizeof(DataStructure) * nextSize + sizeof(int), SEEK_SET);

}


// safe method to get int input value in a controlled way
int getUnsignedInt(int maxLength){
	
	if(maxLength > 8)
		return -1;
	
	char input[maxLength + 1];
	fgets(input, maxLength + 2, stdin);
	
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
	
	fgets(buffer, maxLength, stdin);
	
	int i;
	for(i = 0; buffer[i] != '\0' && buffer[i] != '\n'; i++); 
	
	buffer[i] = '\0'; // add end of line
	
	fflush(stdin); // clear buffer
	
}


void printMenu(int printNumber, char message[]){
	
	char temp[] = "";
	
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
					"2 - Create a new database\n"
					"? - For exit, enter : -1\n"
					"\nYour choose : "
					);
			break;	
		
		case 2:                                                                                        
			
			printf("\n\n %30s%-30s\n\n", "####  You are using ", strcat(strcat(temp, message), "  ####"));
			break;
			
		case 3:
			printf("1 - List data\n"
				   "2 - Add data\n"
				   "3 - Delete data\n"
				   "4 - Update data\n"
				   "? - Go back : -1\n"
				   "\nYour choose : "
				   );
			break;
			
		case 4:
			printf("1 - List by id entered\n"
					"2 - List all data\n"
					"? - Go back : 0\n"
					"\nYour choose : "
					);
			break;
			
	}
	
}


