//
// Created by JunHyuckWoo on 2018. 3. 3..
// Student ID: 20145337
// Insertion sort with Descending order
//

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#define INTSIZE 10

//Function
void insertSort(int *arr, int arrSize);
int inputCheck(char *argv[]);
int detectNum(char *argv[], int *inputSize);
int compareNumSize(char *argv[], char *intMax, int inputSize);
int checkNum(char *argv);


int main(int argc, char* argv[]){

    // Variable declaration
    int tmp = 0;
    int counter = 0;
    int fileSize = 0;
    int arrSize = 1;
    char *Sbuf = NULL;
    char buf[20] = {0,};
    double elapTime = 0, buftime = 0;
    double start = 0, stop = 0;
    int *arr = (int*)malloc(sizeof(int)*arrSize);
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
            if(checkNum(Sbuf)){
                return 0;
            }
            arr[fileSize] = atoi(Sbuf);
            fileSize++;                     // Count number
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
    // Case when input N is bigger than file contents K
    if(fileSize < arrSize){
        arrSize = fileSize;
        arr = (int*)realloc(arr, sizeof(int)*arrSize);  // To save space waste
        bkup = (int*)realloc(bkup, sizeof(int)*arrSize);  // To save space waste
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
        insertSort(arr, arrSize);
    }while(clock() - start< 1000 );


    insertSort(arr, arrSize);

    // Timer Stop
    stop = clock();

    elapTime = (stop - start) / counter / CLOCKS_PER_SEC * 1000;

    // Print result
    for(tmp = 0; tmp < arrSize; tmp++){
        printf("%d\n",arr[tmp]);
    }
    printf("Running time = %lf ms\n", elapTime);

    //free data
    free(arr);
    free(bkup);
    // Close File
    fclose(f);

    return 0;
}

void insertSort(int *arr, int arrSize){
    // Normal insertion sort
    int buf = 0;
    int pin =0;
    int min = 0;
    for(pin = 1; pin < arrSize; pin++){
        // Select number to sort
        buf = arr[pin];
        // if buf(selected num) is bigger than arr, swap it until it's not
        for(min = pin-1;min>=0 && arr[min] < buf;min--){
            arr[min + 1] = arr[min];
        }
        // find a correct location, then save it
        arr[min+1] = buf;
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
            printf("Error: Second parameter isn't number\n");
            return 1;
        }
        size++; // Count string size
    }
    *inputSize = size;
    return 0;
}

int compareNumSize(char *argv[], char *intMax, int inputSize){
    int tmp = 0;
    if(inputSize > INTSIZE ){
        printf("Error: Your parameter overflow\n");
        return 1;
    }
    else if(inputSize < INTSIZE){
        return 0;
    }
    else{
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