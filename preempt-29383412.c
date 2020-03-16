/*  Imtiaz Bin Mujib
    29383412
    Start:13/10/2019
    Last Modified:14/10/2019
    Description: This program is a pseudo Round Robin process scheduler. It takes te input from a file and then converts that
    information into imaginary processes that can be used to simulate a process scheduler.
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "process-visualiser.h"
#include <unistd.h>

typedef enum{
//Enum structure to store process State
    READY, RUNNING, EXIT
} process_state_t;

typedef struct{
//Special Struct to store Process State and information
    char process_name[11];
    int entryTime;
    int serviceTime;
    int remainingTime;

    process_state_t state;
} pcb_t;

FILE *fptr;                     // File pointer
const char * filName;           // Variable where file offset is stored
void createPCB(FILE *filePointer);
void RR();
int n;                          // Number of processes on file
char check;                     // Check to find number of lines in file
int lines;                      // Number of lines in file
pcb_t processArray[40];

void main(int argc, char *argv []){

    filName= "process-data.txt";
    if (argc>1){
    // If filename argument is provided then filName is changed into that argument
        filName=argv[1];
    }
    fptr=fopen(filName,"r");
    while(!feof(fptr)){
    // While loop to find how many lines are there in the file
        check = fgetc(fptr);
        if(check == '\n'){
    //If check finds newline it increments lines by 1
            lines++;
        }
    }
    n=lines;
    fseek(fptr,0,SEEK_SET); //Set file offset to the beginning of the file
    createPCB(fptr);
    initInterface("black","white"); //initialise background for use
    RR();
}
void createPCB(FILE *filePointer){
//This function takes the provided data on the process and stores it into a struct as
//a type of PCB. The PCB contains the processname, arrivaltime, servtime, state and remaining time.
//Each instant of a PCB is stored into an array so it can be used for scheduling.

    char processName[11];
    int arrTime, servTime;
    char i=0;
    while(fscanf(filePointer,"%s %d %d",processName, &arrTime, &servTime )!= EOF ){
// Checks from start of file to finish to find all necessary data and stores it into variables
    pcb_t pName;
    strcpy(pName.process_name,processName);
    pName.entryTime = arrTime;
    pName.serviceTime = servTime;
    pName.remainingTime=servTime;
    pName.state=READY;
    processArray[i]=pName;
    i++;
    }

}
int compEntry (const void * process1, const void * process2) {
// Compares the 2 entryTime values to find which is smaller
    int left = ((pcb_t *)process1)->entryTime;
    int right = ((pcb_t *)process2)->entryTime;
    return (left - right);
}


void RR(){
//This function takes the PCB array and uses that as a pseudo process scheduler,
//This follows the Round Robin method. So, all the processes get the same quantam
//time slice in order to make sure that they get the same amount of time.
    qsort(processArray, n, sizeof(pcb_t), compEntry);
    int timePassed=0;       //To keep track of the amount of time that has passed between processes
    int timeSlice=2;        // Quantam time slice of 2 seconds
    int loop=0;             // To keep track of how many iterations have happened
    int processCount=0;            // To ensure that all the processes have been run
    int timeArr[n];                 // An array to store all the time that has been used up by each process
    while(processCount<n-1){
    //This loop will continue to run as long as all the processes have not completed their functions
        for(int i=0;i<=n;i++){
        // for loop to iterate through the processes and provide them with 2 second slices of processor power
            if(processArray[i].state!=EXIT){
        //If statement to ensure that only processes that havent completed their tasks enter
                if(loop<1){
                //to ensure that the first iteration runs smoothly
                printf("%s enters the system at %d seconds</n>", processArray[i].process_name,processArray[i].entryTime);
                    processArray[i].state=RUNNING;
                    appendRow(processArray[i].process_name);
                    appendBlank(i,processArray[i].entryTime);
                    if(timePassed!=0){
                    //If no time has passes then this block of code doesnt run
                        appendBar(i,(timePassed-processArray[i].entryTime) , "white","Waiting", 1);
                    }
                     if(processArray[i].remainingTime>=timeSlice){
                        //It provied the process with a time slice and removes it from its remaining time
                        appendBar(i,timeSlice,"red","Running",0);
                        timePassed+=timeSlice;
                        processArray[i].remainingTime-=timeSlice;
                        sleep(2);
                    }
                    else{
                        //This block of code executes when only one second is needed to complete the task
                        appendBar(i,timeSlice-1,"green","Running",0);
                        timePassed+=1;
                        processArray[i].remainingTime-=1;
                        sleep(1);
                    }
                    timeArr[i]=timePassed;      //Stores the timePassed into ints own timeArray

                }
                else{
                //This statement is for all iterations after the first one
                    processArray[i].state=RUNNING;
                    appendBar(i,(timePassed-timeArr[i]) , "white","Waiting", 1);
                    if(processArray[i].remainingTime>=timeSlice){
                        appendBar(i,timeSlice,"red","Running",0);
                        timePassed+=timeSlice;
                        processArray[i].remainingTime-=timeSlice;
                        sleep(2);
                    }
                    else{
                        appendBar(i,timeSlice-1,"green","Running",0);
                        timePassed+=1;
                        processArray[i].remainingTime-=1;
                        sleep(1);
                    }
                    timeArr[i]=timePassed;
                }
                if(processArray[i].remainingTime<=0){
                //Checks if a process has finished its task. If it has changes status and increases the counter otherwise moves it to Ready
                //state
                    processArray[i].state=EXIT;
                    processCount++;
                    printf("%s completed. Turnaround time: %d total wait time %d</n>", processArray[i].process_name,
                   timePassed-processArray[i].entryTime,timePassed-processArray[i].serviceTime-processArray[i].entryTime);
                }
                else{
                    processArray[i].state=READY;
                }

            }
        }

    loop++;
    }
    waitExit();
}


