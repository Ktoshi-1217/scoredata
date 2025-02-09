#define DIALOG puts("ok")
/**************************************************************************************************/
#define FILENAME "scoredata.DAT"
#define AMOUNT 20
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<ctype.h>
#include	<time.h>

FILE *fp;
time_t t;
struct tm *lt;

typedef struct{
	int no;
	char name[10];
	char date[20];
	int score;
}DATA;
/**************************************************************************************************/
int intscan(void)
{
int a;
char c[20];

	if(gets(c) == NULL)	return -2;
	if(!isdigit(c[0]))	return -1;
	a = atoi(c);

	return a;
}
/**************************************************************************************************/
void reeddata(DATA *data)
{
int i;
char temporary[20] = {" "};

	if((fp = fopen(FILENAME,"r")) != NULL){

		for(i=0;i<AMOUNT;i++){
			if((fscanf(fp,"%d %s %s" , &data->no , data->name , data->date) == EOF)||(fscanf(fp,"%s %d",&temporary[1] , &data->score) == EOF)){
				data->no = 0;
				break;
			}
			strcat(data->date,temporary);
			data++;
		}
		fclose(fp);

	}else{
		data->no = 0;
		data->score = 0;
	}

}
/**************************************************************************************************/
void datadis(DATA *data)
{
int i;

	printf("-------------------------------------------------------------------------------\n");
	printf("SCOREDATE\n");
	printf("No.   NAME    DATE              SCORE\n");
	for(i=0;(data->no != 0)&&(i<AMOUNT);i++){
		printf("%2d  %-8s  %16s  %8d\n" , data->no , data->name , data->date , data->score);
		data++;
	}
	for(i;i<AMOUNT;i++){
		printf(" 0  ________  0000/00/00 00:00         0\n");
	}
	printf("-------------------------------------------------------------------------------\n");

}
/**************************************************************************************************/
void adddata(DATA *data)
{
int i;
int conditions;
DATA newdata;
DATA savedata;

	printf("Return with [Ctrl + Z]\n");
	conditions = 1;
	do{
		printf("  NAME\n");
		printf("________\r");
		if(gets(newdata.name) == NULL)	return;
		if(isdigit(newdata.name[0]))	printf("The name must not have a number at the beginning\n");
		else if(strlen(newdata.name) >= 8)	printf("The name is too long\n");
		else{
			conditions = 0;
			for(i=0;((data+i)->no!=0)&&(i<AMOUNT);i++){
				if(!strcmp((data+i)->name,newdata.name)){
					printf("Cannot be given the same name\n");
					conditions = 1;
					break;
				}
			}
		}
	}while(conditions);

	newdata.score = 0;
	while(1){
		printf(" Score\n");
		printf("________\r");
		if((newdata.score = intscan()) == -2)	return;
		if((newdata.score > 0)&&(newdata.score <= 99999000))	break;
		printf("Score Error\n");
	}

	t = time(NULL);
	lt = localtime(&t);
	sprintf(newdata.date,"%04d/%02d/%02d %02d:%02d",
		lt->tm_year+1900,
		lt->tm_mon + 1,
		lt->tm_mday,
		lt->tm_hour,
		lt->tm_min
	);

	for(i=0;(data->score > newdata.score)&&(data->no!=0)&&(i<AMOUNT);i++)	data++;

	for(i;i<AMOUNT;i++){
		if(data->no==0){
			if(i == 0){
				newdata.no = 1;
				*data = newdata;
				break;
			}
			newdata.no = (data-1)->no +1;
			*data = newdata;
			break;
		}							//d=1 nd=2 sd=?
		savedata = *data;			//d=1 nd=2 sd=1
		newdata.no = savedata.no;	//d=1 nd=2 sd=1
		*data = newdata;			//d=2 nd=2 sd=1

		newdata = savedata;			//d=2 nd=1 sd=1
		data++;						//d=? nd=1 sd=1
	}
	if(i<AMOUNT)	(data+1) -> no = 0;

	printf("\nUpdate completed!!\n");

}
/**************************************************************************************************/
void deldata(DATA *data)
{
int yesno=1;
int no;
int i;
char name[10];

	if(!data->no){
		printf("Error\n");
		return;
	}

	printf("Return with [Ctrl + Z]\n");
	do{
		no = 0;
		printf("Which data do you want to delete? (Enter number or name)\n");
		printf("________\r");
		if(gets(name) == NULL)	return;
		if(isdigit(name[0])){
			for(i=0;((data+i)->no!=0)&&(i<AMOUNT);i++);
			if((atoi(name) > 0)&&(atoi(name) <= i))	no = atoi(name);
		}else{
			for(i=0;((data+i)->no!=0)&&(i<AMOUNT);i++){
				if(!strcmp((data+i)->name,name)){
					no = i+1;
					break;
				}
			}
		}

		if(no){
			no--;
			printf("\n%2d  %-8s  %16s  %8d\n" , (data+no)->no , (data+no)->name , (data+no)->date , (data+no)->score);
			printf("Do you really want to delete this?\n");
			printf("Yes=>0 No=>1\n");
			yesno = intscan();
		}else	printf("Error\n\n");

	}while(yesno);

	data+=no;

	for(no;((data+1)->no!=0)&&(no<AMOUNT);no++){
		*data = *(data+1);
		data->no -= 1;
		data++;
	}

	data->no = 0;

	printf("\nUpdate completed!!\n");

}
/**************************************************************************************************/
void changedata(DATA *data)
{
int no;
int yesno = 0;
int select;
int conditions;
int score;
int i;
char name[10];
DATA savedata;
DATA savedata2;
DATA *initialdata = data;
DATA *dataaddress;

	if(!data->no){
		printf("Error\n");
		return;
	}

	printf("Return with [Ctrl + Z]\n");
	do{
		no = 0;
		printf("Which data do you want to change? (Enter number or name)\n");
		printf("________\r");
		if(gets(name) == NULL)	return;
		if(isdigit(name[0])){
			for(i=0;((data+i)->no!=0)&&(i<AMOUNT);i++);
			if((atoi(name) > 0)&&(atoi(name) <= i))	no = atoi(name);
		}else{
			for(i=0;((data+i)->no!=0)&&(i<AMOUNT);i++){
				if(!strcmp((data+i)->name,name)){
					no = i+1;
					break;
				}
			}
		}

		if(!no)	printf("Error\n\n");

	}while(!no);

	data += no-1;

	do{

		printf("%2d  %-8s  %16s  %8d\n" , data->no , data->name , data->date , data->score);
		printf("What do you want to change?\n");
		printf("NAME=>1 Score=>2 Date_update=>3 End=>Ctrl+Z\n");
		if((select = intscan()) == -2)	return;
		switch(select){
			case 1:
				do{
					printf("NEWNAME\n");
					printf("________\r");
					if(gets(name) == NULL){
						conditions = 1;
						break;
					}
					if(isdigit(name[0]))	printf("The name must not have a number at the beginning\n");
					else if(strlen(name) >= 8)	printf("The name is too long\n");
					else{
						conditions = 0;
						for(i=0;((initialdata+i)->no!=0)&&(i<AMOUNT)&&((initialdata+i)->no!=no);i++){
							if(!strcmp((initialdata+i)->name,name)){
								printf("Cannot be given the same name\n");
								conditions = 1;
								break;
							}
						}
					}
				}while(conditions);
				if(!conditions){
					strcpy(data->name,name);
					printf("\nUpdate completed!!\n");
				}
				yesno = 9;
				break;

			case 2:
				score = 0;
				while(1){
					printf(" Score\n");
					printf("________\r");
					if((score = intscan()) == -2)	break;
					if((score > 0)&&(score <= 99999000))	break;
					printf("Score Error\n");
				}
				if(score != -2){
					data->score = score;
					savedata = *data;

					for(i=no-1;((data+1)->no!=0)&&(i<AMOUNT);i++){
						*data = *(data+1);
						data->no -= 1;
						data++;
					}

					data->no = 0;

					data = initialdata;
					for(i=0;(data->score > savedata.score)&&(data->no!=0)&&(i<AMOUNT);i++)	data++;

					dataaddress = data;

					for(i;i<AMOUNT;i++){
						if(data->no==0){
							if(i == 0){
								savedata.no = 1;
								*data = savedata;
								break;
							}
							savedata.no = (data-1)->no +1;
							*data = savedata;
							break;
						}
						savedata2 = *data;
						savedata.no = savedata2.no;
						*data = savedata;

						savedata = savedata2;
						data++;
					}
					if(i<AMOUNT)	(data+1) -> no = 0;

					data = dataaddress;

					printf("\nUpdate completed!!\n");

				}
				yesno = 9;
				break;

			case 3:
				t = time(NULL);
				lt = localtime(&t);
				sprintf(data->date,"%04d/%02d/%02d %02d:%02d",
					lt->tm_year+1900,
					lt->tm_mon + 1,
					lt->tm_mday,
					lt->tm_hour,
					lt->tm_min
				);
				printf("\nUpdate completed!!\n");
				yesno=9;
				break;

			default:
				printf("Error\n\n");
				break;
		}

		if(yesno == 9){
			printf("Do you want to change anything else?\n");
			printf("Yes=>0 No=>1\n");
			yesno = intscan();
		}

	}while(!yesno);

}
/**************************************************************************************************/
void alldelfile(DATA *data)
{

	printf("Do you want to delete all data?\n");
	printf("Yes=>0\n");
	if(intscan())	return;
	printf("Do you really want to erase it?\n");
	printf("Yes=>0\n");
	if(intscan())	return;
	printf("You won't regret it, right?\n");
	printf("Yes=>0\n");
	if(intscan())	return;

	data->no = 0;

	printf("\nUpdate completed.\n");

}
/**************************************************************************************************/
int filesave(DATA *data)
{
int i;
int err=0;
int success = 1;

	if((fp = fopen(FILENAME,"w")) != NULL){

		for(i=0;(data->no != 0)&&(i<AMOUNT)&&(success == 1);i++){
			if(fprintf(fp,"%d %s %s %d\n",data->no , data->name , data->date , data->score) == 0){
				printf("Failed to save the file\n\n");
				printf("Do you want to quit?\n0=>End 1=>Return\n");
				err = intscan();
				success = 0;
				break;
			}
			data++;
		}
		fclose(fp);

	}else{
		printf("Failed to open file\n\n");
		printf("Do you want to quit?\n0=>End 1=>Return\n");
		err = intscan();
		success = 0;
	}

	if(success)	printf("The file was saved successfully!!\n");

	return err;

}
/**************************************************************************************************/
int main(void)
{

int err=0;
int select;
int change = 0;
DATA data[AMOUNT];

	/* 読み込み部 */
	reeddata(data);

	do{

		do{

			/* スコア表示 */
			datadis(data);

			/* 選択 */
			printf("what will you do?\n");
			printf("1=>Add_to 2=>Delete 3=>Change 99=>Delete_all 0=>Save&Exit 9=>Exit without saving\n");
			select = intscan();

			/* 処理 */
			switch(select){
				case  0:	break;
				case  1:	adddata(data);		change = 1;	break;
				case  2:	deldata(data);		change = 1;	break;
				case  3:	changedata(data);	change = 1;	break;
				case 99:	alldelfile(data);	change = 1;	break;
				case  9:	select = 0;	change *= 2;	break;
				default:	puts("Error");	break;
			}
			printf("\n");

		}while(select);

		/* ファイル保存 */
		if(change == 2){
			printf("Do you really want to exit without saving?\n");
			printf("Yes=>0 No=>1\n");
			err = intscan();
			change = 1;
		}else if(change)	err = filesave(data);

	}while(err);

	return 0;

}
