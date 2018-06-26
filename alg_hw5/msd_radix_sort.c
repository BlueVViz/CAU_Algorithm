/**
 * "Radix Sort"
 * - ./msd_radix_sort <input_file_name> <N>
 * - measure running time of 'Radix Sort'
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


#define RADIX 10
#define DEGREE 1
#define INTEGER_LEN 10
#define LESS(x, y) x < y ? x : y
#define COMP(x, y) x > y ? 1 : 0

typedef double TIME_T;

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
 * so input number should be 0 ~ 2^32 - 1
 */
int strSize(char *str, int N){
    int len = N;
    int idx = 0;
    char intStr[INTEGER_LEN] = "4294967295";

    // Case 1: Always smaller than Max integer
    if(len < INTEGER_LEN)
        return 0;
    // Case 2:
    if(len == INTEGER_LEN){
        for(idx = 0; idx < len; idx++){
            if(COMP(str[idx] , intStr[idx])){
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

/*
 * Real sorting process
 */
unsigned int sort(unsigned int*data, unsigned int *aux, int lo, int hi, unsigned int d){

    // Variable declaration
    unsigned int count[RADIX+2] = {0, };
    int iFor = 0, tmpLo = 0, tmpHi = 0;

    // If low pivot is bigger than hi one terminate
    if(hi <= lo)
        return -1;

    // Count number
    for(iFor = lo; iFor <= hi; iFor++){
        count[((data[iFor] / d) & (RADIX-1)) + 0x2]++;
    }

    // Cumulate number
    for(iFor = 0; iFor <= RADIX-1; iFor++){
        count[iFor+1] += count[iFor];
    }

    // Save the Sort data array
    for(iFor = lo; iFor <= hi; iFor++){
        aux[count[((data[iFor] / d) & (RADIX-1)) + 1]++] = data[iFor];
    }

    // Save back to data array
    for(iFor = lo; iFor <= hi; iFor++){
        data[iFor] = aux[iFor - lo];
    }
    d>>=DEGREE; // Calculate degree ( number length )
    for(iFor = 0; iFor <= RADIX-1; iFor++){
        // When d is lower than 1, pass
        if(d <= 0){
            continue;
        }
        // When tmpHi is negative, pass
        if((tmpHi = lo + count[iFor+1] - 1) < 0){
            continue;
        }
        // Before enter the call function
        if(tmpHi <= (tmpLo = lo + count[iFor])) {
            continue;
        }
        // Recursion
        sort(data, aux, tmpLo, tmpHi, d);
    }
    return 1;
}

/*
 * MSD Radix sort
 * Radix is sat as 1024
 * shift with 10 bits
 * Basic setting
 */
unsigned int radix_sort(unsigned int *data, unsigned int n){

    // Variable declaration
    int d = 1, max = 1;
    unsigned int *aux = (unsigned int*)malloc(sizeof(unsigned int)*n);

    // loop for find maximum number
    // Using same variable to reduce memory
    for(d = 0; d < n; d++) {
        max |= data[d];
    }

    // Reset variable & and calculate max number length
    d = 1;
    while(max >= RADIX){
        max >>= DEGREE;
        d <<= DEGREE;
    }

    // Sort begin
    sort(data, aux, 0, n-1, d);
    free(aux);
    return 0;
}


int main(int argc, char* argv[]) {

    /** your code here **/
    //Declare variation
    int sBuf = 0;
    int idFor = 0;
    int counter = 0;
    int fileLine = 0;
    unsigned int N = 1;
    unsigned int *arr = (unsigned int*)malloc(sizeof(unsigned int)*N);
    unsigned int *bkarr = (unsigned int*)malloc(sizeof(unsigned int)*N);

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
        printf("ERROR: Fail to open file\n");
        fclose(f);
        free(arr);
        free(bkarr);
        return 0;
    }

    /* Argument 2 => input N
     * Input string should be a number.
     * If an any single of character is included,
     * program should be terminated
     */
    if(!(sBuf = checkstr(argv[2]))){
        fclose(f);
        free(arr);
        free(bkarr);
        return 0;
    }

    // This program consider input upt to 1000000.
    // So if that is bigger than threshold, terminate.
    if(strSize(argv[2], sBuf)){
        fclose(f);
        free(arr);
        free(bkarr);
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
        printf("ERROR: Input number is negative.\n");
        fclose(f);
        free(arr);
        free(bkarr);
        return 0;
    }
    arr = (unsigned int*)realloc(arr, sizeof(unsigned int)*N);
    bkarr = (unsigned int*)realloc(bkarr, sizeof(unsigned int)*N);

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
    arr = (unsigned int*)realloc(arr, sizeof(unsigned int*)*N);
    bkarr = (unsigned int*)realloc(bkarr, sizeof(unsigned int*)*N);
    memcpy(bkarr, arr, sizeof(unsigned int*) * N );

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

        radix_sort(arr, N);
    }while(clock() - start< 1000);

    // Finish Stop watch
    duration = ( clock() - start ) / counter / CLOCKS_PER_SEC * 1000;


    /** your code here **/
    // Print result on screen
    for(idFor = 0; idFor < N; idFor++){
        printf("%d\n", arr[idFor]);
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