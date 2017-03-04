/*
 ============================================================================
 Name        : factory.c
 Author      : BerkGulay
 Version     :
 Copyright   : All rights of this code is reserved
 Description : Factory Producing Line Console Application in Ansi-C Standart
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Node for processed cars.It includes info about car. Used to add them in departments structure's processed cars part.
struct ProcessedCarNode{
	char CarInfo[25];
	struct ProcessedCarNode *prev;

};

//Node for every Car in system. It keeps every cars info in it.
struct Car{
	char model[21];
	char unicode[5];
	int StartTime;
	char CurrentDepartment[20];
	int CurrentDeptTime;
	int CompletedDeptTime;
	char CurrentStatue[15];
	char in_process;

};

//input: Car pointer
//to initialize a car structure.
void start_car(struct Car *car){
	car->CurrentDeptTime=0;
	car->CompletedDeptTime=0;
	car->in_process='N';
	strcpy(car->CurrentStatue,"Not Complete");
}

//Node for every Department in system. It keeps every departments info in it.
struct Department{
	char GeneralName[20];
	char SpecialName[25];
	int ProcessTime;
	struct Car *CurrentCar;
	struct Department *PreviousDept;
	struct Department *NextSibling;
	struct ProcessedCarNode *top;

};

//input: Department pointer
//to initialize a department structure.
void start_department(struct Department *department){
	department->CurrentCar=NULL;
	department->NextSibling=NULL;
	department->top=NULL;
}

// Factory structure keeps all factory process time info and all departments place in it
typedef struct {
	int TotalProcessTime;
	struct Department *last;

}Factory;

//input: Factory pointer
//to initialize a factory structure
void start_factory(Factory *factory){
	factory->TotalProcessTime=0;
	factory->last=NULL;
}

// Node for a report. It keeps all info about a report in it
struct ReportNode{
	int ReportTime;
	char Type[12];
	char Unicode[5];
	char Statue;
	struct ReportNode *prevReportNode;

};

// AllReports structure keeps all reports place in it and also keeps the report's number.
typedef struct {
	int counter;
	struct ReportNode *top;

}AllReports;

//input: AllReports pointer
//It is for initializing AllReports structure.
void start_AllReports(AllReports *allRep){
	allRep->counter=0;
	allRep->top=NULL;
}


//Our Queue node to keep cars in it.
struct NodeQue{
	struct NodeQue *next;
	struct Car *car;
};

//Our Queue structure to keep all queue node's places with
//head(very first item) , front(first item according to queue) and rear(last item according to queue).
typedef struct {
	int counter;
	struct NodeQue *head;
	struct NodeQue *front;
	struct NodeQue *rear;
}Queue;

//input: myQue , Queue pointer which is keeping the que we will check
//return: if true 1 else 0
//Checks if queue is empty
int queue_is_empty(Queue *myQue) {
	if(myQue -> counter == 0){
		return 1;
	}
	else{return 0;}
}

//input: myQue,car --> myQue is queue pointer which is keeping the queue we will add car pointer inside it
//add an car element to queue
void enqueue(Queue *myQue, struct Car *car) {

	struct NodeQue *ptr = (struct NodeQue *)malloc(sizeof(struct NodeQue));

	ptr -> next = NULL;
	ptr -> car = car;

	if(myQue -> head == NULL){
		myQue -> head = ptr;
	}

	if(queue_is_empty(myQue)==1){
		myQue -> front = myQue -> rear = ptr;
	}
	else {
		myQue -> rear -> next = ptr;
		myQue -> rear = ptr;
	}
	myQue -> counter ++;

}

//input: myQueue, queue which we will delete an car element from front
//dequeue an element from string queue
struct Car* dequeue(Queue *myQue){
	if(queue_is_empty(myQue)==0) {
		struct Car *rtrnPtr=myQue -> front -> car;
		struct NodeQue *ptr = myQue -> front;
		myQue -> front = ptr -> next;
		myQue -> counter--;

		return rtrnPtr;
	}

	return NULL;
}


//input: myQueue, which will be initialized
//Func. initializes the car queue
void initial_queue(Queue *myQue){
	myQue -> counter = 0;
	myQue -> front = myQue -> rear = myQue -> head = NULL;
}

struct StringNode{  //String Node for dynamic sized string stack
	struct StringNode* next;
	char data[50];
};

typedef struct{  //Dynamic String Stack
	struct StringNode *top;
	int counter;
}StringStack;

//input: string stack
//return: if true 1 else 0
//Checks if string stack is empty
int string_stack_is_empty(StringStack *myStack){
	if(myStack -> counter == 0){
		return 1;
	}
	else{return 0;}
}

//input: string stack, string data
//push an element to string stack
void string_push(StringStack *myStack, char data[]){
	struct StringNode *ptr = (struct StringNode *)malloc(sizeof(struct StringNode));

	strcpy(ptr -> data,data);
	ptr -> next = myStack -> top;
	myStack -> top = ptr;
	myStack -> counter++;
}

//input: string stack
//pop an element from string stack
char* string_pop(StringStack *myStack){
	if(string_stack_is_empty(myStack)==0) {
		char *rtrnValue=myStack -> top -> data;
		myStack -> top = myStack -> top -> next;
		myStack -> counter--;
		return rtrnValue;
	}

	return "String Stack is empty!!!";
}

//input: string stack
//initialize the string stack
void initial_StringStack(StringStack *myStack){
	myStack -> top = NULL;
	myStack -> counter = 0;
}


// This function copies the given string dynamically as strdup() function do
char* mycopy (const char *str) {
    char *tok = (char *) malloc (strlen(str)+1);

    if (tok==NULL){
    	return NULL;
    }

    strcpy (tok,str);
    return tok;
}

//input: pointer a and pointer b
//output: if a<b return -1 else return 1
//compares to pointer value to sort an array
int compare_int( const void* a, const void* b )
{
	if( *(int*)a == *(int*)b ) return 0;
	return *(int*)a < *(int*)b ? -1 : 1;
}


//input: string path,char array pointer commands
//output commands array pointer
//reads the file dynamically and returns a command which include comment lines
void text_reader(const char* path,char * commands[]){
	FILE *f=fopen(path,"r");
	char line[81];

	int i=0;
	char* eof ;
	while (eof = fgets(line, sizeof(line), f)) {
		line[strcspn(line,"\n")]=0;
		line[strcspn(line,"\r")]=0;
		commands[i++]=mycopy(eof);

	}
	commands[i]=NULL;

	fclose(f);
}

//input: LastDepartment and space(needed space value from previous prints) pointers
//This function prints factory's hierarchy according to it's departments position.
void PrintFactory(struct Department *LastDepartment,int *space){

	if(LastDepartment->PreviousDept == NULL){
		printf("\n- ");
		*space+=1;
		struct Department *SiblingTraveller=LastDepartment;
		while(SiblingTraveller != NULL){
			printf("%s ",SiblingTraveller->SpecialName);
			*space += strlen(SiblingTraveller->SpecialName)+1;
			SiblingTraveller=SiblingTraveller->NextSibling;
		}
		printf("%s","\n");
		return;
	}
	else{
		PrintFactory(LastDepartment->PreviousDept,space);

		char spchar[*space+1];
		memset(spchar,' ',*space);
		spchar[*space]=0;
		printf("%s- ",spchar);
		*space+=1;
		struct Department *SiblingTraveller=LastDepartment;
		while(SiblingTraveller != NULL){
			printf("%s ",SiblingTraveller->SpecialName);
			*space+=strlen(SiblingTraveller->SpecialName)+1;
			SiblingTraveller=SiblingTraveller->NextSibling;
		}
		printf("%s","\n");
		return;
	}

}

//input: Car , Factory , Stack pointers
//This method takes a car pointer inside it and writes report about this car acording to given factory pointer. At this time it uses
//a stack to reorder our outputs.
void CarReportWriter(struct Car *CarPtr,Factory *factory,StringStack *stack){
	char line[strlen(CarPtr->model)+18];
	memset(line,'-',strlen(CarPtr->model)+18);
	line[strlen(CarPtr->model)+18]=0;

	printf("%s\n",line);
	printf("|Report for %s %s|\n",CarPtr->model,CarPtr->unicode);
	printf("%s\n",line);
	if(strcmp(CarPtr->CurrentStatue,"Not Complete")==0){
		struct Department *DepPtr=factory->last;
		while(strcmp(DepPtr->GeneralName,CarPtr->CurrentDepartment)!=0){
			char str[22]; strcpy(str,DepPtr->GeneralName); strcat(str,":0");
			string_push(stack,str);
			DepPtr=DepPtr->PreviousDept;
		}
		char str[22]; strcpy(str,DepPtr->GeneralName); char time[3]; sprintf(time,"%d",CarPtr->CurrentDeptTime); strcat(str,":"); strcat(str,time);
		string_push(stack,str);
		DepPtr=DepPtr->PreviousDept;
		while(DepPtr != NULL){
			char str[22]; strcpy(str,DepPtr->GeneralName); char time[3]; sprintf(time,"%d",DepPtr->ProcessTime); strcat(str,":"); strcat(str,time);
			string_push(stack,str);
			DepPtr=DepPtr->PreviousDept;
		}
		while(string_stack_is_empty(stack)==0){
			printf("%s, ",string_pop(stack));
		}
		float comp=(float)((CarPtr->CompletedDeptTime+CarPtr->CurrentDeptTime)*100)/(float)factory->TotalProcessTime;
		printf("| Start Time: %d | Complete: %.2f%% | Not complete\n",CarPtr->StartTime,comp);
	}
	else{
		struct Department *DepPtr=factory->last;
		while(DepPtr != NULL){
			char str[22]; strcpy(str,DepPtr->GeneralName); char time[3]; sprintf(time,"%d",DepPtr->ProcessTime); strcat(str,":"); strcat(str,time);
			string_push(stack,str);
			DepPtr=DepPtr->PreviousDept;
		}
		while(string_stack_is_empty(stack)==0){
			printf("%s, ",string_pop(stack));
		}
		printf("| Start Time: %d | Complete: 100%% | Complete\n",CarPtr->StartTime);
	}
}

//input: AllReports pointer, T(as time value) , Factory pointer , CarsQueue(which keeps all cars in it) pointer
//This function's usage is giving reports about our factory and cars. This func. runs with "report nodes" and takes them from
//AllReports pointer to give asked report acc. to type(from report nodes) and TİME(T) values.
void Report(AllReports *AllReports,int T,Factory *factory,Queue *CarsQue){
	StringStack stck; StringStack *stack=&stck;  initial_StringStack(stack);
	struct ReportNode *TravelPtr=AllReports->top;
	char ReportType[12];
	char CarUnicode[5];

	while(TravelPtr!=NULL){
		if(TravelPtr->ReportTime==T){
			if(TravelPtr->Statue=='N'){
				strcpy(ReportType,TravelPtr->Type);
				if(strcmp(ReportType,"Car")==0){strcpy(CarUnicode,TravelPtr->Unicode);}
				TravelPtr->Statue='D';
				break;
			}
		}
		TravelPtr=TravelPtr->prevReportNode;
	}

	if(strcmp(ReportType,"Car")==0){
		printf("\nCommand: Report Car %d %s\n",T,CarUnicode);

		struct NodeQue *Node=CarsQue->head;
		while(Node != NULL){
			struct Car *CarPtr=Node->car;

			if(strcmp(CarPtr->unicode,CarUnicode)==0){
				CarReportWriter(CarPtr,factory,stack);
				break;
			}

			Node=Node->next;
		}

	}
	else if(strcmp(ReportType,"Cars")==0){
		printf("\nCommand: Report Cars %d\n",T);

		struct NodeQue *Node=CarsQue->head;
		while(Node != NULL){
			struct Car *CarPtr=Node->car;
			if(CarPtr->in_process=='Y'){
				CarReportWriter(CarPtr,factory,stack);
			}
			Node=Node->next;
		}
	}
	else if(strcmp(ReportType,"Departments")==0){
		printf("\nCommand: Report Departments %d\n",T);

		struct Department *DeptPtr=factory->last;

		while(DeptPtr != NULL){
			struct Department *SiblingPtr=DeptPtr;
			while(SiblingPtr != NULL){

				char line[strlen(SiblingPtr->SpecialName)+26];
				memset(line,'-',strlen(SiblingPtr->SpecialName)+26);
				line[strlen(SiblingPtr->SpecialName)+26]=0;

				printf("%s\n",line);
				printf("|Report for Department '%s'|\n",SiblingPtr->SpecialName);
				printf("%s\n",line);

				if(SiblingPtr->CurrentCar!=NULL){
					printf("I am currently processing %s %s\n",SiblingPtr->CurrentCar->model,SiblingPtr->CurrentCar->unicode);
				}
				else{
					printf("%s is now free.\n",SiblingPtr->SpecialName);
				}
				if(SiblingPtr->top != NULL){
					printf("%s","Processed Cars\n");

					struct ProcessedCarNode *temp=SiblingPtr->top;
					while(temp != NULL){
						string_push(stack,temp->CarInfo);
						temp=temp->prev;
					}
					int q=1;
					while(string_stack_is_empty(stack)==0){
						printf("%d. %s\n",q,string_pop(stack));
						q++;
					}
				}
				SiblingPtr=SiblingPtr->NextSibling;
			}
			DeptPtr=DeptPtr->PreviousDept;
		}
	}
}

//input: factory pointer
//Returns 1 if factory's all departments are empty else returns 0
int departments_is_empty(Factory *factory){

	struct Department *dept=factory->last;
	while(dept != NULL){
		struct Department *sib=dept;
		while(sib != NULL){
			if(sib->CurrentCar != NULL){return 0;}
			sib=sib->NextSibling;
		}
		dept=dept->PreviousDept;
	}

	return 1;
}

//input: TransmitterDept pointer , ReceiverDept pointer
//This function finds right transmitter department to take its car to Receiver Department and updates the info of car and both departments
void TransportFunction(struct Department *TransmitterDept,struct Department *ReceiverDept){
	struct Department *TransmitterSibling=TransmitterDept;
	struct Department *ChoosenTransmitter=NULL;

	while(TransmitterSibling != NULL){
		if(TransmitterSibling->CurrentCar != NULL && TransmitterSibling->CurrentCar->CurrentDeptTime+1>=TransmitterSibling->ProcessTime){
			if(ChoosenTransmitter==NULL){ChoosenTransmitter=TransmitterSibling;}
			else{
				if(ChoosenTransmitter->CurrentCar->StartTime>TransmitterSibling->CurrentCar->StartTime){
					ChoosenTransmitter=TransmitterSibling;
				}
			}
		}
		TransmitterSibling=TransmitterSibling->NextSibling;
	}

	if(ChoosenTransmitter!=NULL){
		struct ProcessedCarNode *ProCar= (struct ProcessedCarNode *)malloc(sizeof(struct ProcessedCarNode));
		char CarInf[25]; strcpy(CarInf,ChoosenTransmitter->CurrentCar->model); strcat(CarInf," "); strcat(CarInf,ChoosenTransmitter->CurrentCar->unicode);
		strcpy(ProCar->CarInfo,CarInf);
		ProCar->prev=ChoosenTransmitter->top; ChoosenTransmitter->top=ProCar;
		ReceiverDept->CurrentCar=ChoosenTransmitter->CurrentCar;
		ChoosenTransmitter->CurrentCar=NULL;
		ReceiverDept->CurrentCar->CurrentDeptTime=0;
		ReceiverDept->CurrentCar->CompletedDeptTime+=ChoosenTransmitter->ProcessTime;
		strcpy(ReceiverDept->CurrentCar->CurrentDepartment,ReceiverDept->GeneralName);
	}

}

//input: factory pointer, CarsQueue(which keeps all cars inside it) pointer , ReportsArray(sorted version of given report's times), AllReports pointer
//This function is basic function for factory process. It checks if there is any car to put department process from beginning
//or any car to move on other departments and any car to take from department process to processed cars. This func. uses transport func.
//for transporting cars one department to another. Checks all cars queue to take given cars inside it and checks the last dept for ready cars
void DepartmentsProcess(Factory *factory,Queue *CarsQue,int ReportsArray[],AllReports *AllReports){
	int T=1;
	int RepNum=0;
	Queue *CarLine=(Queue *)malloc(sizeof(Queue));
	initial_queue(CarLine);

	while(queue_is_empty(CarsQue)==0 || queue_is_empty(CarLine)==0 || departments_is_empty(factory)==0){

		if(queue_is_empty(CarsQue)==0){
			while(CarsQue->front->car->StartTime<=T){
				enqueue(CarLine,dequeue(CarsQue));
				if(queue_is_empty(CarsQue)==1){break;}
			}
		}

		struct Department *Department=factory->last;
		struct Department *SiblingDept=Department;
		struct Department *PreviousDepartment=Department->PreviousDept;
		while(SiblingDept != NULL){
			if(SiblingDept->CurrentCar !=NULL){
				if(SiblingDept->CurrentCar->CurrentDeptTime+1==SiblingDept->ProcessTime){
					strcpy(SiblingDept->CurrentCar->CurrentStatue,"Complete");
					struct ProcessedCarNode *ProCar=(struct ProcessedCarNode *)malloc(sizeof(struct ProcessedCarNode));
					char CarInf[25]; strcpy(CarInf,SiblingDept->CurrentCar->model); strcat(CarInf," "); strcat(CarInf,SiblingDept->CurrentCar->unicode);
					strcpy(ProCar->CarInfo,CarInf);
					ProCar->prev=SiblingDept->top; SiblingDept->top=ProCar;
					SiblingDept->CurrentCar=NULL;
				}
				else{
					SiblingDept->CurrentCar->CurrentDeptTime++;
				}
			}
			else{
				TransportFunction(PreviousDepartment,SiblingDept);
			}
			SiblingDept=SiblingDept->NextSibling;
		}

		while(Department->PreviousDept->PreviousDept != NULL){
			Department=Department->PreviousDept;
			SiblingDept=Department;
			PreviousDepartment=Department->PreviousDept;

			while(SiblingDept != NULL){
				if(SiblingDept->CurrentCar !=NULL){
					if(SiblingDept->CurrentCar->CurrentDeptTime+1<=SiblingDept->ProcessTime){
						SiblingDept->CurrentCar->CurrentDeptTime++;
					}
				}
				else{
					TransportFunction(PreviousDepartment,SiblingDept);
				}
				SiblingDept=SiblingDept->NextSibling;
			}
		}
		SiblingDept=Department->PreviousDept;

		while(SiblingDept != NULL){
			if(SiblingDept->CurrentCar !=NULL){
				if(SiblingDept->CurrentCar->CurrentDeptTime+1<=SiblingDept->ProcessTime){
					SiblingDept->CurrentCar->CurrentDeptTime++;
				}
			}
			else{
				if(queue_is_empty(CarLine)==0){
					SiblingDept->CurrentCar=dequeue(CarLine);
					strcpy(SiblingDept->CurrentCar->CurrentDepartment,SiblingDept->GeneralName);
					SiblingDept->CurrentCar->in_process='Y';
				}
			}
			SiblingDept=SiblingDept->NextSibling;
		}


		while(ReportsArray[RepNum]==T){
			Report(AllReports,T,factory,CarsQue);
			RepNum++;
		}
		T++;
	}
}

//input: commands array pointer
//This function takes the comments and transmit it proper functions and processes.
void FactoryFunction(char *commands[]){

	Queue *CarsQue=(Queue *)malloc(sizeof(Queue));
	initial_queue(CarsQue);
	Factory *factory=(Factory *)malloc(sizeof(Factory));
	start_factory(factory);
	AllReports *AllReports=malloc(sizeof(AllReports));
	start_AllReports(AllReports);

	int i=-1;
	while(commands[++i]!=NULL){
		char * splittedCommands[4];
		char *tok;
		int c=0;

		tok = strtok(commands[i], " ");
		while( tok != NULL )
		{
			splittedCommands[c++]=strdup(tok);
			tok = strtok(NULL, " ");
		}

		if(strcmp(splittedCommands[0],"AddDept")==0){
			struct Department *department=(struct Department *)malloc(sizeof(struct Department)); start_department(department);

			strcpy(department->GeneralName,splittedCommands[2]);
			department->ProcessTime=atoi(splittedCommands[3]);
			strcpy(department->SpecialName,strcat(mycopy(splittedCommands[2]),"1"));
			department->PreviousDept=factory->last;
			factory->last=department;
			factory->TotalProcessTime+=department->ProcessTime;

			printf("Department %s has been created.\n",department->GeneralName);

			int i;
			for(i=1;i<atoi(splittedCommands[1]);i++){
				struct Department *sibling=(struct Department *)malloc(sizeof(struct Department)); start_department(sibling);

				strcpy(sibling->GeneralName,department->GeneralName);
				sibling->ProcessTime=atoi(splittedCommands[3]);
				char num[3]; sprintf(num,"%d",i+1);
				strcpy(sibling->SpecialName,strcat(mycopy(splittedCommands[2]),num));

				department->NextSibling=sibling;
				department=sibling;
			}
		}

		else if(strcmp(splittedCommands[0],"Produce")==0){
			struct Car *car=(struct Car *)malloc(sizeof(struct Car)); start_car(car);

			car->StartTime=atoi(splittedCommands[1]);
			strcpy(car->model,splittedCommands[2]);
			strcpy(car->unicode,splittedCommands[3]);

			enqueue(CarsQue,car);
		}

		else if(strcmp(splittedCommands[0],"PrintFactory")==0){
			int spc=0; int *space=&spc;
			PrintFactory(factory->last,space);
		}

		else if(strcmp(splittedCommands[0],"Report")==0){
			struct ReportNode *report= (struct ReportNode *)malloc(sizeof(struct ReportNode));
			report->Statue='N';
			report->ReportTime=atoi(splittedCommands[1]);
			strcpy(report->Type,splittedCommands[2]);
			if(strcmp(splittedCommands[2],"Car")==0){strcpy(report->Unicode,splittedCommands[3]);}
			report->prevReportNode=AllReports->top;
			AllReports->top=report;
			AllReports->counter++;
		}

	}

	int ReportsArray[AllReports->counter];
	struct ReportNode *traveler=AllReports->top;
	int t;
	for(t=0;t<AllReports->counter;t++){
		ReportsArray[t]=traveler->ReportTime;
		traveler=traveler->prevReportNode;
	}
	qsort(ReportsArray,AllReports->counter,sizeof(int),compare_int);

	DepartmentsProcess(factory,CarsQue,ReportsArray,AllReports);
}


//input: argv(take i/o from console) array pointer, argc
//Our Main Function to control program from console(This program is a console application)
int main(int argc, char* argv[]) {

	char * commands[300];
	text_reader("input.txt",commands);
	FactoryFunction(commands);

}
