#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>
#include <math.h>
#include <sys/time.h>
#include "cs402.h"
#include "transaction.h"
#include "my402list.h"
#define LargeP "?,???,???.??"
long long Ba = 0;
void initialization(My402List* list, FILE* fp) {
	char newLine[1025];
	while(fgets(newLine, sizeof(newLine), fp)!= NULL){
		if (strlen(newLine)> 1024){
			fprintf(stderr, "this line contains more than 80 characters");
			exit(0);
		}else{
			My402Transaction* newTran =(My402Transaction*)malloc(sizeof(My402Transaction));
			char *type;
			char *time;
			char *amount;
			char *des;
			char *extra;
			type = strtok(newLine, "\t");
			if (type == NULL){
				fprintf(stderr, "the format of this line is wrong");
				exit(0);
			}
			time = strtok(NULL, "\t");
			if (time == NULL){
				fprintf(stderr, "the format of this line is wrong");
				exit(0);
			}
			amount = strtok(NULL, "\t");
			if (amount == NULL){
				fprintf(stderr, "the format of this line is wrong");
				exit(0);
			}
			des = strtok(NULL, "\t");
			if (des == NULL){
				fprintf(stderr, "the format of this line is wrong");
				exit(0);
			}
			extra = strtok(NULL, "\t");

			///type
			if(type!=NULL){
				if(strcmp(type, "-")!=0 && strcmp(type, "+") != 0){
					fprintf(stderr, "the sign is invalid");
					exit(0);
				}else{
					if (strcmp(type, "+") == 0){
						newTran->trans_type = 1;
					}else{
						newTran->trans_type = 2;
					}
					

				}
			}else{
				fprintf(stderr, "the format of this line is wrong");
				exit(0);
			}

			///time
			if(time!=NULL){
				if(strlen(time)>=11 || atol(time)<0){
					fprintf(stderr, "the timestamp is bad");
					exit(0);
				}else{
					newTran->trans_time = atol(time);
				}
			}else{
				fprintf(stderr, "the format of this line is wrong");
				exit(0);
			}

			///amount
			char *nonDecimals;
			int decimals = 0;

			char *copyamount = (char *)malloc(sizeof(char)*11);
			strncpy(copyamount, amount,10);
			nonDecimals = strtok(copyamount, ".");
			decimals = strlen(amount)-strlen(nonDecimals)-1;
			if(strlen(nonDecimals) > 7){
				fprintf(stderr, "The number to the left of the decimal point can be at most 7 digits");
				exit(0);
			}else if(decimals != 2){
				fprintf(stderr, "a number followed by a period must followed by exactly two digits");
				exit(0);
			}else if(atof(amount) < 0){
				fprintf(stderr, "The number is smaller 0");
				exit(0);
			}else {
				
				newTran->trans_amount = (char*)malloc(sizeof(char)*11);
				strncpy(newTran->trans_amount, amount,10);
			}
			

			///description
			if(des!=NULL){
				if(strlen(des)==0){
					fprintf(stderr, "the description is empty");
					exit(0);
				}else{
					while (*des == ' '){
						des++;
					}
					newTran->trans_des = (char*)malloc(sizeof(char)*25);
					strncpy(newTran->trans_des, des, 24);
				}
			}else{
				fprintf(stderr, "the format of this line is wrong");
				exit(0);
			}

			///extra
			if (extra != NULL){
				fprintf(stderr, "there are extra fields");
				exit(0);
			}else{
				int append;
				append = My402ListAppend(list, newTran);
				if (append==0){
					fprintf(stderr, "Cannot append new transaction to 402list");
					exit(0);
				}
			}
		}

	}
}

void sort(My402List* list) {
	My402ListElem *curr, *next;
	My402Transaction *currT, *nextT;
	for (curr = My402ListFirst(list); My402ListNext(list, curr) != NULL;curr = My402ListNext(list, curr)){
		for (next = My402ListNext(list, curr); next != NULL;next = My402ListNext(list, next)){
			currT = (My402Transaction *)curr->obj;
			nextT = (My402Transaction *)next->obj;
			if (currT->trans_time > nextT->trans_time){
				curr->obj = nextT;
				next->obj = currT;
			}
			if (currT->trans_time == nextT->trans_time){
				fprintf(stderr, "we have same transaction time here");
				exit(0);
			}
		}
	}
}

void printTrans(My402List* list) {
	
	printf("+-----------------+--------------------------+----------------+----------------+\n");
    printf("|       Date      | Description              |         Amount |        Balance |\n");
	printf("+-----------------+--------------------------+----------------+----------------+\n");
	My402ListElem *curr;
	My402Transaction *currT;
	
	
	for (curr = My402ListFirst(list); curr !=NULL;curr = My402ListNext(list, curr)){
		
		currT = (My402Transaction *) curr->obj;
		time_t tm = currT->trans_time;
		char *ctimeString = ctime(&tm);
		char *date[5];
		date[0] = strtok(ctimeString, " ");
		date[1] = strtok(NULL, " ");
		date[2] = strtok(NULL, " ");
		date[3] = strtok(NULL, " ");
		date[4] = strtok(NULL, " ");
		char last[4];
		long lastlong;
		strncpy(last, ctimeString+20, 4);
		lastlong = atol(last);
		if (strlen(date[2])==1){
			printf("%s%s%s%s%s%s%s%s%s%ld%s%s","|", " ",date[0]," ", date[1], " ", " ",date[2], " ", lastlong, " ", "|");
		}else{
			printf("%s%s%s%s%s%s%s%s%ld%s%s","|", " ",date[0]," ", date[1], " ", date[2], " ", lastlong, " ", "|");
		}

		//description
		char x[]= "                        ";
		int din;

		for (din = 0; din<strlen(currT->trans_des)-1;din++){
			x[din] = currT->trans_des[din];
		}
		if (strlen(currT->trans_des)==24){
			x[23] = currT->trans_des[23];
		}
		printf(" %s ", x);
		printf("|");


		//amount
		double c=0;
		c = atof(currT->trans_amount);
		c = c*100;
		long w = (long) round(c);
		double tempaa=0;
		tempaa = atof(currT->trans_amount);

		if (currT->trans_type ==1){
			if (w< 1000000000){
				printf("  %'12.2f  ", tempaa);
			}else{
				printf("  %s  ",LargeP);
			}
		}else{
			if (w< 1000000000){
				printf(" (%'12.2f) ", tempaa);
			}else{
				printf(" (%s) ", LargeP);
			}
		}
		printf("|");

		//balance
		if (currT->trans_type ==1){
			Ba = Ba + w;
		}else if (currT->trans_type ==2){
			Ba = Ba - w;
		}

		if (Ba>1000000000){
			printf("  %s  ",LargeP);
		}else if (Ba<-1000000000){
			printf(" (%s) ", LargeP);
		}else{
			double tempf = (double) Ba;
			if (Ba<0){
				tempf = -tempf/100;
			}else{
				tempf = tempf/100;
			}
			if (Ba>0){
				printf("  %'12.2f  ", tempf);

			}else{
				printf(" (%'12.2f) ", tempf);
			}
		}
		printf("|\n");
		

	}
	printf("+-----------------+--------------------------+----------------+----------------+\n");
}

