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
    PCB pivot = P[high];
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (iCriteria == SORT_BY_ARRIVAL ) {
            if (P[j].iArrival < pivot.iArrival || (pivot.iArrival == P[j].iArrival && P[j].iBurst < pivot.iBurst)) {
                i++;
                swapProcess(&P[i], &P[j]);
            }
        } else if (iCriteria == SORT_BY_PID) {
            if (P[j].iPID < pivot.iPID) {
                i++;
                swapProcess(&P[i], &P[j]);
            }
        } else if (iCriteria == SORT_BY_BURST) {
            if (P[j].iBurst < pivot.iBurst) {
                i++;
                swapProcess(&P[i], &P[j]);
            }
        } else if (iCriteria == SORT_BY_START) {
            if (P[j].iStart < pivot.iStart) {
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
    PCB OriginalProcess[10]; 
    memset(&Input, 0, sizeof(PCB) * 10); 
    memset(&TerminatedArray, 0, sizeof(PCB) * 10); 
    memset(&ReadyQueue, 0, sizeof(PCB) * 10); 
    memset(&OriginalProcess, 0, sizeof(PCB) * 10); 
    int iNumberOfProcess, iQuantumTime; 
    printf("Please input the quantum time: "); 
    scanf("%d", &iQuantumTime); 
    printf("Please input the number of Process: "); 
    scanf("%d", &iNumberOfProcess); 
    int n = iNumberOfProcess;
    int iRemain = iNumberOfProcess, iReady = 0, iTerminated = 0; 
    // inputProcess(iNumberOfProcess, Input); 
    generateRandomProcess(iNumberOfProcess, Input);
    printf("\nGenerated Process: \n");
    printProcess(iNumberOfProcess, Input);

    quickSort(Input, 0, iNumberOfProcess - 1, SORT_BY_ARRIVAL); 
    pushProcess(&iReady, ReadyQueue, Input[0]); 
    OriginalProcess[Input[0].iPID].iBurst = Input[0].iBurst; 
    removeProcess(&iRemain, 0, Input); 
    ReadyQueue[0].iStart = ReadyQueue[0].iArrival; 
    ReadyQueue[0].iFinish = ReadyQueue[0].iStart + ReadyQueue[0].iBurst; 
    ReadyQueue[0].iResponse = ReadyQueue[0].iStart - ReadyQueue[0].iArrival; 
    ReadyQueue[0].iWaiting = ReadyQueue[0].iResponse; 
    ReadyQueue[0].iTaT = ReadyQueue[0].iFinish - ReadyQueue[0].iArrival; 
 
    int currentTime = ReadyQueue[0].iArrival; 
 
    while (iRemain > 0 || iReady > 0) 
    { 
        if (iReady == 0) currentTime = Input[0].iArrival; 
        for (int i = 0; i < iRemain; i++) 
        { 
            int compareTime; 
            if (ReadyQueue[0].iBurst > iQuantumTime) compareTime = currentTime + iQuantumTime; 
            else compareTime = currentTime + ReadyQueue[0].iBurst; 
            if (Input[i].iArrival <= compareTime) 
            { 
                pushProcess(&iReady, ReadyQueue, Input[i]); 
                OriginalProcess[ReadyQueue[iReady - 1].iPID].iBurst = Input[i].iBurst; 
                removeProcess(&iRemain, i, Input); 
                i--; 
            } 
        } 
        if (ReadyQueue[0].iBurst > iQuantumTime && iReady > 0){
            currentTime += iQuantumTime; 
            ReadyQueue[0].iBurst -= iQuantumTime; 
            ReadyQueue[0].iFinish += iQuantumTime; 
            for (int i = 1; i < iReady; i++) 
            { 
                ReadyQueue[i].iWaiting += iQuantumTime; 
            } 
            
            PCB temp = ReadyQueue[0]; 
            temp.iStart = currentTime - iQuantumTime; 
            temp.iFinish = currentTime; 
            temp.iBurst = iQuantumTime; 
            pushProcess(&iTerminated, TerminatedArray, temp); 
 
            pushProcess(&iReady, ReadyQueue, ReadyQueue[0]); 
            removeProcess(&iReady, 0, ReadyQueue); 
        } 
        else if (iReady > 0) 
        { 
            ReadyQueue[0].iStart = currentTime; 
            currentTime += ReadyQueue[0].iBurst; 
            ReadyQueue[0].iFinish = currentTime; 
            ReadyQueue[0].iTaT = ReadyQueue[0].iFinish - ReadyQueue[0].iArrival; 
 
            pushProcess(&iTerminated, TerminatedArray, ReadyQueue[0]); 
            ReadyQueue[0].iBurst = OriginalProcess[ReadyQueue[0].iPID].iBurst; 
            pushProcess(&ReadyQueue[0].iPID, OriginalProcess, ReadyQueue[0]); 
            removeProcess(&iReady, 0, ReadyQueue); 
        } 
    }
        

    printf("\n===== Round Robin Scheduling =====\n");
    exportGanttChart(iTerminated, TerminatedArray);
    quickSort(TerminatedArray, 0, iTerminated - 1, SORT_BY_PID);
    calculateAWT(n, TerminatedArray);
    calculateATaT(n, TerminatedArray);

    return 0;
}
