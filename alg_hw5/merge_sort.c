//
// Created by JunHyuckWoo on 2018. 3. 3..
// Student ID: 20145337
// Merge sort  with Descending order
//

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#define INTSIZE 10
#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))

//Function
void mergeSort(int *arr, int *wrk, int element, int arrSize, int interval);
void merge(int *arr, int *wrk, int left, int right, int arrSize);
void copyArr(int *arr, int *wrk, int end);
int inputCheck(char *argv[]);
int detectNum(char *argv[], int *inputSize);
int compareNumSize(char *argv[], char *intMax, int inputSize);
int checkNum(char *argv);

int main(int argc, char* argv[]){

    // Variable declation
    int counter = 0;
    int element = 1;
    int interval =0;
    int tmp=0;
    int fileSize = 0;
    int arrSize = 1;
    char buf[20] = {0, };
    char *Sbuf = NULL;
    double elapTime = 0,buftime = 0;
    double start = 0, stop = 0;
    int *arr = (int*)malloc(sizeof(int)*arrSize);
    int *wrk = (int*)malloc(sizeof(int)*arrSize);
    int *bkup = (int*)malloc(sizeof(int)*arrSize);

    /*=============================
    // Command line input check
       - Enter wrong numbers of parameters
       - If user give correct input,
         input number should be 3*/
    if(argc != 3){
        printf("ERROR: Wrong Input\n");
        return 0;
    }
    // - Check first parameter
    //   Check input type, size
    if(inputCheck(argv)){
        return 0;
    }
    // Command line input check finish
    //=============================


    // Reset variable
    // Input N could be different, it could be bigger than file content number K or smaller
    arrSize = atoi(argv[1]);
    arr = (int*)realloc(arr, sizeof(int)*arrSize);
    wrk = (int*)realloc(wrk, sizeof(int)*arrSize);
    bkup = (int*)realloc(bkup, sizeof(int)*arrSize);


    //=============================
    // Text file check
    // File open == checking second parameter
    FILE * f= fopen(argv[2], "rt");

    // File check
    if(f == NULL){
        printf("ERROR: Fail to open file\n");
        return 0;
    }

    // File read
    // Read a string
    while(fgets(buf,sizeof(buf),f) && fileSize < arrSize) {
        // Divide it when it find a " ", "\r", "\n" or "\r\n"
        // Until it return NULL
        Sbuf = strtok(buf, " \r\n");
        while(Sbuf != NULL){
            if(checkNum(Sbuf))
                return 0;
            arr[fileSize] = atoi(Sbuf);
            fileSize++;
            Sbuf = strtok(NULL, " \r\n");
        }
    }
    // If there isn't number, then file size might be a 0
    if(fileSize <1){
        printf("ERROR: No Contents in file\n");
        return 0;
    }
    // Text file check finish
    //=============================


    // Error Control
    // Case when input N is® bigger than file contents K
    if(fileSize < arrSize){
        arrSize = fileSize;
        bkup = (int*)realloc(bkup, sizeof(int)*arrSize);  // To save space waste
        arr = (int*)realloc(arr, sizeof(int)*arrSize);  // To save space waste
        wrk = (int*)realloc(wrk, sizeof(int)*arrSize);  // To save space waste
    }

    // Copy arr for small elapse
    memcpy(bkup,arr, sizeof(int)*arrSize);

    // Timer Start
    start = clock();

    // Make a kinds of while for elapsed < 10000
    do{
        // sort
        counter++;
        buftime = clock();
        memcpy(arr,bkup, sizeof(int)*arrSize);
        buftime = clock() - buftime;
        start += buftime + (3*0.143000); // 0.143 == clock time spend
        mergeSort(arr, wrk, element, arrSize, interval);
    }while(clock() - start< 1000);

    // Timer Stop
    stop = clock();

    elapTime = (stop - start) / counter / CLOCKS_PER_SEC * 1000;

    // Output
    for(tmp = 0; tmp < arrSize; tmp++){
        printf("%d\n", arr[tmp]);
    }
    printf("Running time = %lf ms\n",elapTime);


    //free data
    free(arr);
    free(wrk);
    free(bkup);
    // Close File
    fclose(f);

    return 0;
}


void mergeSort(int *arr, int *wrk, int element, int arrSize, int interval){

    for(element=1; element<arrSize; element*=2 ){
        for(interval=0;interval<arrSize ;interval += 2*element){
            merge(arr,wrk, interval, MIN(interval + element,arrSize), MIN(interval + 2*element,arrSize));
            //merge(arr, interval, element, wrk, arrSize);
        }
        copyArr(arr,wrk,arrSize);
    }
}

void merge(int *arr, int *wrk, int left, int right, int arrSize){
    // Merge two sorted array
    int tmp = left;
    int ileft = left;
    int iright = right;
    for(;tmp < arrSize;tmp++){
        if(ileft == right){
            while(iright != arrSize){
                wrk[tmp] = arr[iright];
                iright++; tmp++;
            }
        }
        else if((arr[ileft] > arr[iright]) && (ileft != right && iright != arrSize) || ileft == right){
            wrk[tmp] = arr[iright];
            if(iright != arrSize){
                iright++;
            }
        }
        else{
            wrk[tmp] = arr[ileft];
            if(ileft != right){
                ileft++;
            }
        }
    }
}

void copyArr(int *arr, int *wrk, int end){
    // Copy
    int tmp=0;
    for(; tmp < end;tmp++){
        arr[tmp] = wrk[tmp];
    }

}

int inputCheck(char *argv[]){

    int inputSize = 0;
    char intMax[] = "2147438647";

    // 1. First input should be number
    //    Check input size also
    // 2. If Number is too big to save in integer
    if(detectNum(argv, &inputSize)){
        return 1;
    }
    if(compareNumSize(argv, intMax, inputSize)){
        return 1;
    }
    return 0;

}


int checkNum(char *argv){
    int size = 1;
    while(argv[size] != '\0'){
        // Loop until finish the sentence
        // If detect other things(ex character), show error and finish
        if(argv[size] > 58 || argv[size] < 47){
            printf("ERROR: This string is not a number\n");
            return 1;
        }
        size++; // Count string size
    }
    if((argv[0] > '9' || argv[0] < '0') && argv[0] != '-'){
        printf("ERROR: This string is not a number\n");
        return 1;
    }
    return 0;
}

int detectNum(char *argv[], int *inputSize){	
    int size = *inputSize;
    while(argv[1][size] != '\0'){
        // Loop until finish the sentence
        // If detect other things(ex character), show error and finish
        if(argv[1][size] > 58 || argv[1][size] < 47){
            printf("ERROR: Second parameter isn't number\n");
            return 1;
        }
        size++; // Count string size
    }
    *inputSize = size;
    return 0;
}

int compareNumSize(char *argv[], char *intMax, int inputSize){
    int tmp = 0;
    // Out boundary of integer
    if(inputSize > INTSIZE ){
        printf("Error: Your parameter is too big\n");
        return 1;
    }// In boundary of integer
    else if(inputSize < INTSIZE){

        return 0;
    }
    else{
        // Check the value
        while(intMax[tmp] != '\0'){
            if(intMax[tmp] < argv[1][tmp]){
                printf("Error: Your parameter is too big\n");
                return 1;
            }
            tmp++;
        }
        return 0;
    }
}