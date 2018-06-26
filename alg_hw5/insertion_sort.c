/**
 * "Quick Sort"
 * - ./basic_quick_sort <input_file_name> <N>
 * - measure running time of 'Insertion Sort'
 *
 * name: Junhyuck Woo
 * ID: 20145337
  **/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>

#define INTEGER_LEN 10
#define LESS(x, y) x < y ? x : y
#define COMP(x, y) x > y ? 1 : 0
#define SWAP(a, b, type) type temp = a; a = b; b = temp;

typedef double TIME_T;





//Function
void insertSort(int *arr, int arrSize);
int inputCheck(char *argv[]);
int detectNum(char *argv[], int *inputSize);
int compareNumSize(char *argv[], char *intMax, int inputSize);
int checkNum(char *argv);



int main(int argc, char* argv[]) {

    /** your code here **/
    //Declare variation
    int N = 1;
    int sBuf = 0;
    int idFor = 0;
    int counter = 0;
    int fileLine = 0;
    int *arr = (int*)malloc(sizeof(int)*N);
    int *bkarr = (int*)malloc(sizeof(int)*N);

    char *gCol;
    char buf[20];
    char *Sbuf = NULL;

    FILE * f;
    TIME_T start = 0;
    TIME_T elapBuf = 0;
    TIME_T duration = 0;

    srand(time(NULL));

    // Argument check
    /* Argument 1 => Text file name
     * Open File
     */
    f = fopen(argv[1], "rt");

    /* If file is not opened,
     * then f still remain as null.
     * => Shutdown the program.
     */
    if(f == NULL){
        free(arr);
        free(bkarr);
        fclose(f);
        printf("ERROR: Fail to open file\n");
        return 0;
    }

    /* Argument 2 => input N
     * Input string should be a number.
     * If an any single of character is included,
     * program should be terminated
     */
    if(!(sBuf = checkstr(argv[2]))){
        free(arr);
        free(bkarr);
        fclose(f);
        return 0;
    }

    // This program consider input upt to 1000000.
    // So if that is bigger than threshold, terminate.
    if(strSize(argv[2], sBuf)){
        free(arr);
        free(bkarr);
        fclose(f);
        return 0;
    }

    /*
     * Reset Variable
     * Argument error checking is done,
     * so some of variable need to reset
     * for saving memory or needless action.
     */
    N = strtol(argv[2], &gCol, 10);
    if(N < 0){
        free(arr);
        free(bkarr);
        fclose(f);
        printf("ERROR: Input number is negative.\n");
        return 0;
    }
    arr = (int*)realloc(arr, sizeof(int)*N);
    bkarr = (int*)realloc(bkarr, sizeof(int)*N);

    /* Read File
     * If file readable, then keep read
     * This program should read file up to N line,
     * If N is bigger than fileLine, it doesn't matter.
     * However, in opposite condition, we should break.
     */
    while(fgets(buf,sizeof(buf),f) && fileLine < N) {
        // Divide it when it find a " ", "\r", "\n" or "\r\n"
        // Until it return NULL
        Sbuf = strtok(buf, " \r\n");
        while(Sbuf != NULL){
            if(!(sBuf = checkstr(Sbuf)))
                break;
            if(strSize(Sbuf, sBuf))
                return 0;
            arr[fileLine] = strtol(Sbuf, &gCol,10);
            fileLine++;
            Sbuf = strtok(NULL, " \r\n");
        }
    }

    // Backup data
    N = LESS(N, fileLine);
    arr = (int*)realloc(arr, sizeof(int*)*N);
    bkarr = (int*)realloc(bkarr, sizeof(int*)*N);
    memcpy(bkarr, arr, sizeof(int*) * N );

    /** your code here **/
    // Sort with measuring time
    // Start Stop watch
    start = clock();
    do{
        counter++;
        elapBuf = clock();
        memcpy(arr, bkarr, sizeof(int*) * N );
        elapBuf = clock() - elapBuf;
        start += elapBuf;

        insertSort(arr, N);
    }while(clock() - start< 1000);

    // Finish Stop watch
    duration = ( clock() - start ) / counter / CLOCKS_PER_SEC * 1000;


    /** your code here **/
    // Print result on screen
    for(idFor = 0; idFor < N - 1; idFor++){
        printf("%d\n", arr[idFor] - arr[idFor + 1]);
    }

    // Please keep these printf statements!
    printf("N = %7d,\tRunning_Time = %.3f ms\n", N, duration);

    /** your code here **/
    // free memory
    fclose(f);
    free(arr);
    free(bkarr);

    return 0;
}

void insertSort(int *arr, int arrSize){
    // Normal insertion sort
    int buf = 0, pin =0, min = 0;
    for(pin = 1; pin < arrSize; pin++){
        // Select number to sort
        buf = arr[pin];
        // if buf(selected num) is smaller than arr, swap it until it's not
        for(min = pin-1;min>=0 && arr[min] > buf; min--){
            arr[min + 1] = arr[min];
        }
        // find a correct location, then save it
        arr[min+1] = buf;
    }
}

/* Function */
/*
 * Check input
 * Every input should be a number and
 * If string has character then return 0
 * If string is perfectly number then return string size
 */
int checkstr(char* str){
    int idx = 0;
    // Search until string until end of string
    while(str[idx] != '\0'){
        // If string is negative number then first character might be a '-'
        // In this case, just pass
        if(str[idx] == '-' && idx == 0)
            idx++;
        // File has a character
        if(str[idx] > '9' || str[idx] < '0'){
            // Except negative case
            printf("ERROR: Second parameter isn't number\n");
            return 0; // Terminate
        }
        idx++;
    }
    return  idx; // Terminate func
}

/*
 * Check String size and value.
 * This program just get inter type value,
 * so input number should be -2,147,483,648 ~ 2,147,483,647
 */
int strSize(char *str, int N){
    int len = N;
    int idx = 0;
    int flag = 0;
    char intStr[10] = "2147483647";

    // If string is negative set the flag
    if(str[0] == '-'){
        intStr[9]++;
        flag = 1;
    }

    // Case 1: Always smaller than Max integer
    if(len < INTEGER_LEN + flag)
        return 0;
    // Case 2:
    if(len == INTEGER_LEN + flag){
        for(idx = flag; idx < len; idx++){
            if(COMP(str[idx] , intStr[idx-flag])){
                printf("ERROR: This string is not fit omn integer type variable.\n");
                return 1;
            }
        }
        return 0;
    }

    // Case 3: string is bigger than integer type number
    // Terminate
    printf("ERROR: Overflow is occurred.\n");
    return 1;
}