#ifndef _COMMANDS_H
#define _COMMANDS_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/file.h>
#include <stdbool.h>



int checkCommand(char* command);
int getBalance(int accNum);
int withdraw(int accNum, int sum);
int deposit(int accNum, int sum);
int transfer(int accNum1, int accNum2, int sum);

#endif
