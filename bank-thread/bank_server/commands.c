#include "commands.h"


int getBalance(int accNum){
	FILE* dataFile;
	// open data file
	dataFile = fopen("data.txt","r");
	
	if(!dataFile) fprintf(stderr, "Failed to open file: %s \n",strerror(errno));// failed to open file
	
	// lock the data file
	int checkFlock;
	checkFlock =  flock(fileno(dataFile), LOCK_EX);
	
	if(checkFlock == -1){ fprintf(stderr, "Failed to lock file: %s \n",strerror(errno));}// failed to lock file printf()
	
	// check file line by line
	char line[100];
	char* line1;
	char* tok;
	int retrievedAccNumInt;
	int retrievedBalanceInt;
	int unFlock;
	while(fgets(line,sizeof(line),dataFile)){
		line1 = line;
		// get account number and balance from line
		tok = strtok_r(line1," ",&line1);
		// cast to int
		retrievedAccNumInt  = (int)strtol(tok, NULL, 10);
		retrievedBalanceInt = (int)strtol(line1, NULL, 10);
		
		// check if wanted account
		if(retrievedAccNumInt == accNum){
			
			// remove the file lock
			unFlock = flock(fileno(dataFile), LOCK_UN);
			// check return 
			if(unFlock == -1){
				// error unlocking file
			}
		
			return retrievedBalanceInt;
		} 
	
	}
	
	// if not found unlock file and return -1 and error message
	int unFlock1;
	unFlock1 = flock(fileno(dataFile), LOCK_UN);
	if(unFlock1 == -1){
		fprintf(stderr, "Failed to unlock file: %s \n",strerror(errno));
		// error unlocking file
	}

	// close file
	fclose(dataFile);
	return -1;

}





int deposit(int accNum, int sum){
	FILE* dataFile;
	// open data file
	dataFile = fopen("data.txt","r");
	
	if(!dataFile) fprintf(stderr, "Failed to open file: %s \n",strerror(errno));// failed to open file
	
	// lock the data file
	int checkFlock =  flock(fileno(dataFile), LOCK_EX);
	
	if(checkFlock == -1) fprintf(stderr, "Failed to lock file: %s \n",strerror(errno)); // failed to lock file 
	
	// create a temporary file which we swap at the end
	// the temporary file copies every line but changes the wanted accounts balance
	
	FILE* tmpFile;
	// open data file
	tmpFile = fopen("tmpData.txt","a");
	
	if(!tmpFile) fprintf(stderr, "Failed to open file: %s \n",strerror(errno)); // failed to open file
	
	// lock the data file
	int checkFlock1;
	checkFlock1 =  flock(fileno(tmpFile), LOCK_EX);
	
	if(checkFlock1 == -1) fprintf(stderr, "Failed to lock file: %s \n",strerror(errno));// failed to lock file 
	
	
	// check file line by line
	char line[100];
	char* line1;
	char* retrievedAccNum;
	char* retrievedBalance;
	int retrievedAccNumInt;
	int retrievedBalanceInt;
	int foundAcc = 0;
	
	while(fgets(line,sizeof(line),dataFile)){
		
		// get account number and balance from line
		line1 = line;
		retrievedAccNum = strtok_r(line1," ",&line1);
		retrievedBalance = line1;
		// cast to int
		retrievedAccNumInt  = (int)strtol(retrievedAccNum, NULL, 10);
		retrievedBalanceInt = (int)strtol(retrievedBalance, NULL, 10);
		
		// check if wanted account
		if(retrievedAccNumInt == accNum){
			// write new balance for account in temp file
			foundAcc = 1;
			fprintf(tmpFile, "%d %d\n", retrievedAccNumInt, (retrievedBalanceInt + sum));
		} 
		else{	
			// write same line in temp file
			fprintf(tmpFile, "%d %d\n", retrievedAccNumInt, retrievedBalanceInt);
		
		}
	
	}
	
	// Unlock files and switch temp file to be the original file
	
	if(flock(fileno(dataFile), LOCK_UN) == -1) fprintf(stderr, "Failed to unlock file: %s \n",strerror(errno));// error unlocking
	fclose(dataFile);
	
	// make temporary file the real one
	if(unlink("data.txt") == -1) fprintf(stderr, "Failed to unlink file: %s \n",strerror(errno));//  error unlinking
	
	if(rename("tmpData.txt", "data.txt") == -1)fprintf(stderr, "Failed to rename file: %s \n",strerror(errno)); // error renaming
	
	if(flock(fileno(tmpFile), LOCK_UN) == -1) fprintf(stderr, "Failed to unlock file: %s \n",strerror(errno));// error unlocking
	fclose(tmpFile);
	return foundAcc;
}

int withdraw(int accNum, int sum){
	FILE* dataFile;
	// open data file
	dataFile = fopen("data.txt","r");
	
	if(!dataFile) fprintf(stderr, "Failed to open file: %s \n",strerror(errno));// failed to open file
	
	// lock the data file
	int checkFlock =  flock(fileno(dataFile), LOCK_EX);
	
	if(checkFlock == -1) fprintf(stderr, "Failed to lock file: %s \n",strerror(errno)); // failed to lock file 
	
	// create a temporary file which we swap at the end
	// the temporary file copies every line but changes the wanted accounts balance
	
	FILE* tmpFile;
	// open data file
	tmpFile = fopen("tmpData.txt","a");
	
	if(!tmpFile) fprintf(stderr, "Failed to open file: %s \n",strerror(errno)); // failed to open file
	
	// lock the data file
	int checkFlock1;
	checkFlock1 =  flock(fileno(tmpFile), LOCK_EX);
	
	if(checkFlock1 == -1) fprintf(stderr, "Failed to lock file: %s \n",strerror(errno));// failed to lock file 
	
	
	// check file line by line
	char line[100];
	char* line1;
	char* retrievedAccNum;
	char* retrievedBalance;
	int retrievedAccNumInt;
	int retrievedBalanceInt;
	int foundAcc = 0;
	
	while(fgets(line,sizeof(line),dataFile)){
		
		// get account number and balance from line
		line1 = line;
		retrievedAccNum = strtok_r(line1," ",&line1);
		retrievedBalance = line1;
		// cast to int
		retrievedAccNumInt  = (int)strtol(retrievedAccNum, NULL, 10);
		retrievedBalanceInt = (int)strtol(retrievedBalance, NULL, 10);
		
		// check if wanted account
		if(retrievedAccNumInt == accNum){
			// write new balance for account in temp file
			foundAcc = 1;
			fprintf(tmpFile, "%d %d\n", retrievedAccNumInt, (retrievedBalanceInt - sum));
		} 
		else{	
			// write same line in temp file
			fprintf(tmpFile, "%d %d\n", retrievedAccNumInt, retrievedBalanceInt);
		
		}
	
	}
	
	// Unlock files and switch temp file to be the original file
	
	if(flock(fileno(dataFile), LOCK_UN) == -1) fprintf(stderr, "Failed to lock file: %s \n",strerror(errno));// error unlocking
	fclose(dataFile);
	
	// make temporary file the real one
	if(unlink("data.txt") == -1) fprintf(stderr, "Failed to unlink file: %s \n",strerror(errno));//  error unlinking
	
	if(rename("tmpData.txt", "data.txt") == -1)fprintf(stderr, "Failed to rename file: %s \n",strerror(errno)); // error unlinking
	
	if(flock(fileno(tmpFile), LOCK_UN) == -1) fprintf(stderr, "Failed to unlock file: %s \n",strerror(errno));// error unlocking
	fclose(tmpFile);
	return foundAcc;
}

int transfer(int accNum1, int accNum2, int sum){
	FILE* dataFile;
	// open data file
	dataFile = fopen("data.txt","r");
	
	if(!dataFile) fprintf(stderr, "Failed to open file: %s \n",strerror(errno)); // failed to open file
	
	// lock the data file
	int checkFlock;
	checkFlock =  flock(fileno(dataFile), LOCK_EX);
	
	if(checkFlock == -1)fprintf(stderr, "Failed to lock file: %s \n",strerror(errno)); // failed to lock file 
	
		
	// check file line by line
	char line[100];
	char* line1;
	char* retrievedAccNum;
	int retrievedAccNumInt;

	
	bool foundAcc1, foundAcc2;
	
	
	
	while(fgets(line,sizeof(line),dataFile)){
		line1 = line;
		// get account number and balance from line
		retrievedAccNum = strtok_r(line1," ",&line1);
		// cast to int
		retrievedAccNumInt  = (int)strtol(retrievedAccNum, NULL, 10);
		
		
		// check if we find both accounts
		if(retrievedAccNumInt == accNum1){	
			foundAcc1 = true;
		} 
		else if (retrievedAccNumInt == accNum2){			
			foundAcc2 = true;		
		}
	}
	// unlock and close the data file
	if(flock(fileno(dataFile), LOCK_UN) == -1) fprintf(stderr, "Failed to unlock file: %s \n",strerror(errno));// error unlocking
	fclose(dataFile);
	
	if(foundAcc1 && foundAcc2){
		withdraw(accNum1, sum);
		deposit(accNum2, sum);
		return 1;
	}
	
	printf("didnt find accounts \n");
	
	return 0;
	

}





int checkCommand(char* command)
{
    	// variables
	char* line = command;
	char *token;
	char letter;
	int acc1, acc2, sum;
	acc1 = acc2 = sum = 0;

	int tokenCount = 0;
	while((token = strtok_r(line, " ", &line))){

        	if(tokenCount == 0){
        		// take first letter of first token assuming the user has inputted a valid one
            		letter = token[0];
        	}
        	// extract all the other data from tokens
        	else if(tokenCount == 1){
            		acc1 = (int)strtol(token, NULL, 10);
        	}
        	else if(tokenCount == 2 && letter != 't'){
			sum = (int)strtol(token, NULL, 10);
		}
            	else if(tokenCount == 2 && letter == 't'){
               	acc2 = (int)strtol(token, NULL, 10);
            	}
       	else if(tokenCount == 3 && letter == 't'){
                	sum = (int)strtol(token, NULL, 10);
          	}
       	 
        	tokenCount++;
        }
	// use appropriate functions according to the command
	switch(letter){
	
		case 'l':;
			
			int balance = getBalance(acc1);
			printf("Account %d balance: %d €. \n", acc1,balance);
			return 0;
			
		case 'w':
			if(sum <= 0){
				printf("Cant withdraw negative amounts. \n");
			}
			int ret1 = withdraw(acc1,sum);		
			if(ret1 != 0){
				printf("Withdrew %d € from %d's account. \n", sum,acc1);
				return 0;
			}	
			else{
				printf("Wanted account %d was not available. \n",acc1);
				return -1;
			}
		case 'd':
			if(sum <= 0){
				printf("Cant deposit negative amounts. \n");
				return -1;
			}
			int ret2 = deposit(acc1,sum);		
			if(ret2 != 0){
				printf("Deposited %d € to %d's account. \n", sum,acc1);
				return 0;
			}	
			else{
				printf("Wanted account %d was not available. \n",acc1);
				return -1;
			}
		
		case 't':
			if(sum <= 0){
				printf("Cant transfer negative amounts. \n");
				return -1;
			}
			int ret3 = transfer(acc1,acc2,sum);		
			if(ret3 != 0){
				printf("Transfered %d € from account %d to account %d. \n", sum,acc1,acc2);
				return 0;
			}	
			else{
				printf("Wanted account %d was not available. \n",acc1);
				return -1;
			}
		
		default:
			printf("Invalid input. Command must start with l,w,d,t. \n");
			return -1;
		
	

	}   
}








