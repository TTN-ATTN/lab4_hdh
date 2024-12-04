#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define SORT_BY_ARRIVAL 0
#define SORT_BY_PID 1
#define SORT_BY_BURST 2
#define SORT_BY_START 3

typedef struct {
    int iPID;
    int iArrival, iBurst;
    int iStart, iFinish, iWaiting, iResponse, iTaT;
} PCB;

void inputProcess(int n, PCB P[]);
void printProcess(int n, PCB P[]);
void exportGanttChart(int n, PCB P[]);
void pushProcess(int *n, PCB P[], PCB Q);
void removeProcess(int *n, int index, PCB P[]);
int swapProcess(PCB *P, PCB *Q);
int partition(PCB P[], int low, int high, int iCriteria);
void quickSort(PCB P[], int low, int high, int iCriteria);
void calculateAWT(int n, PCB P[]);
void calculateATaT(int n, PCB P[]);
void generateRandomProcess(int n, PCB P[]);

void generateRandomProcess(int n, PCB P[]){
    for (int i = 0; i < n; i++) {
        P[i].iPID = i + 1;
        P[i].iArrival = rand() % 21;
        P[i].iBurst = rand() % 11 + 2;
        P[i].iStart = 0;
        P[i].iFinish = 0;
        P[i].iWaiting = 0;
        P[i].iResponse = 0;
        P[i].iTaT = 0;
    }
}

int swapProcess(PCB *P, PCB *Q){
    PCB Temp = *P;
    *P = *Q;
    *Q = Temp;

    return 0;
}

void inputProcess(int n, PCB P[]){
    for (int i = 0; i < n; i++) {
        printf("Input Process %d\n", i + 1);
        printf("PID: ");
        scanf("%d", &P[i].iPID);
        printf("Arrival Time: ");
        scanf("%d", &P[i].iArrival);
        printf("Burst Time: ");
        scanf("%d", &P[i].iBurst);
        printf("\n");
    }
}

int partition(PCB P[], int low, int high, int iCriteria){
    int pivot = P[high].iArrival;
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (iCriteria == SORT_BY_ARRIVAL) {
            if (P[j].iArrival < pivot) {
                i++;
                swapProcess(&P[i], &P[j]);
            }
        } else if (iCriteria == SORT_BY_PID) {
            if (P[j].iPID < pivot) {
                i++;
                swapProcess(&P[i], &P[j]);
            }
        } else if (iCriteria == SORT_BY_BURST) {
            if (P[j].iBurst < pivot) {
                i++;
                swapProcess(&P[i], &P[j]);
            }
        } else if (iCriteria == SORT_BY_START) {
            if (P[j].iStart < pivot) {
                i++;
                swapProcess(&P[i], &P[j]);
            }
        }
    }

    swapProcess(&P[i + 1], &P[high]);
    return i + 1;
}

void quickSort(PCB P[], int low, int high, int iCriteria){
    if (low < high) {
        int pi = partition(P, low, high, iCriteria);
        quickSort(P, low, pi - 1, iCriteria);
        quickSort(P, pi + 1, high, iCriteria);
    }
}

void pushProcess(int *n, PCB P[], PCB Q){
    P[*n] = Q;
    (*n)++;
}

void removeProcess(int *n, int index, PCB P[]){
    for (int i = index; i < *n - 1; i++) {
        P[i] = P[i + 1];
    }
    (*n)--;
}

void printProcess(int n, PCB P[]){
    for (int i = 0; i < n; i++) {
        printf("P%d: Arrival: %d, Burst: %d\n", P[i].iPID, P[i].iArrival, P[i].iBurst);
    }
}

void exportGanttChart(int n, PCB P[]) {
    printf("\nGantt Chart\n");
   
    int start = (P[0].iArrival != 0);
    
    puts("");
    char *s = calloc(20, 1);
    printf("|");

    if (start)
        printf("%4s", "|");

    for (int i = 0; i < n; i++) {
        snprintf(s, 20, "P%d", P[i].iPID);
        printf("%2s%2s", s, "|");
        if (i < n - 1 && P[i].iFinish != P[i + 1].iStart)
            printf("%4s", "|");
    }
    
    printf("\n0");
    for (int i = 0; i < n; i++) {
        if (P[i].iStart != 0 && (i == 0 || P[i].iStart != P[i - 1].iFinish))
            printf("%4d", P[i].iStart);
        printf("%4d", P[i].iFinish);
    }
    puts("\n");
    return;
}

void calculateAWT(int n, PCB P[]){
    float fAWT = 0;
    for (int i = 0; i < n; i++) {
        fAWT += P[i].iWaiting;
    }
    fAWT /= n;
    printf("AWT: %.2f\n", fAWT);
}

void calculateATaT(int n, PCB P[]){
    float fATaT = 0;
    for (int i = 0; i < n; i++) {
        fATaT += P[i].iTaT;
    }
    fATaT /= n;
    printf("ATaT: %.2f\n", fATaT);
}

int main() {
    srand(time(NULL));

    PCB Input[10];
    PCB ReadyQueue[10];
    PCB TerminatedArray[10];
    memset(ReadyQueue, 0, sizeof(PCB)*10);
    memset(TerminatedArray, 0, sizeof(PCB)*10);
    memset(Input, 0, sizeof(PCB)*10);
    int iNumberOfProcess;

    printf("Please input number of Process: ");
    scanf("%d", &iNumberOfProcess);

    int iRemain = iNumberOfProcess, iReady = 0, iTerminated = 0;

    // inputProcess(iNumberOfProcess, Input);
    generateRandomProcess(iNumberOfProcess, Input);
    printf("\nGenerated Process: \n");
    printProcess(iNumberOfProcess, Input);

    quickSort(Input, 0, iNumberOfProcess - 1, SORT_BY_ARRIVAL);
    pushProcess(&iReady, ReadyQueue, Input[0]);
    removeProcess(&iRemain, 0, Input);

    ReadyQueue[0].iStart = ReadyQueue[0].iArrival;
    ReadyQueue[0].iFinish = ReadyQueue[0].iStart + ReadyQueue[0].iBurst;
    ReadyQueue[0].iResponse = ReadyQueue[0].iStart - ReadyQueue[0].iArrival;
    ReadyQueue[0].iWaiting = ReadyQueue[0].iResponse;
    ReadyQueue[0].iTaT = ReadyQueue[0].iFinish - ReadyQueue[0].iArrival;

    printf("\nReady Queue: ");
    printProcess(1, ReadyQueue);

    while (iTerminated < iNumberOfProcess) {

        while (iRemain > 0) {
            pushProcess(&iReady, ReadyQueue, Input[0]);
            removeProcess(&iRemain, 0, Input);
            if (Input[0].iArrival <= ReadyQueue[0].iFinish) {    
                continue;
            } 
            else {
                break;
            }

        }

        if (iReady > 0) {
            pushProcess(&iTerminated, TerminatedArray, ReadyQueue[0]);
            removeProcess(&iReady, 0, ReadyQueue);

            if(ReadyQueue[0].iArrival <= TerminatedArray[iTerminated - 1].iFinish) {
                ReadyQueue[0].iStart = TerminatedArray[iTerminated - 1].iFinish;
            } else {
                ReadyQueue[0].iStart = ReadyQueue[0].iArrival;
            }

           
            ReadyQueue[0].iFinish = ReadyQueue[0].iStart + ReadyQueue[0].iBurst;
            ReadyQueue[0].iResponse = ReadyQueue[0].iStart - ReadyQueue[0].iArrival;
            ReadyQueue[0].iWaiting = ReadyQueue[0].iResponse;
            ReadyQueue[0].iTaT = ReadyQueue[0].iFinish - ReadyQueue[0].iArrival;
        }
    }

    printf("\n===== FCFS Scheduling =====\n");
    exportGanttChart(iTerminated, TerminatedArray);
    quickSort(TerminatedArray, 0, iTerminated - 1, SORT_BY_PID);
    calculateAWT(iTerminated, TerminatedArray);
    calculateATaT(iTerminated, TerminatedArray);

    return 0;
}
