/**
 * "3-way Quick Sort"
 * - ./3way_quick_sort <input_file_name> <N>
 * - measure running time of 'three-way Quick Sort'
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

/** Function */
/*
 * Check input
 * Every input should be a number and
 * If string has character then return 0
 * If string is perfectly number then return string size
 */
int checkstr(char* str){
    int idx = 0;
    int flag = 0;
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
    if(N < INTEGER_LEN + flag)
        return 0;
    // Case 2:
    if(N == INTEGER_LEN + flag){
        for(idx = flag; idx < N; idx++){
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

/*
* Rand function call consumes too much times,
* so my algorithm calls twice.
* Compare two value and take a smaller one.
*/
void choose_pivot (int *data, unsigned int n) {
	/** your code here **/
    int r1 = 0, r2 = 0;
    if(n > 0 ){
        r1 = rand()%n;
        r2 = rand()%n;
        r1 = LESS(r1, r2);
        SWAP(data[0], data[r1], int);
    }
}

unsigned long three_way_quick_sort (int *data, unsigned int n) {
    unsigned long cnt = (n - 1); // number of comparisons

    /** your code here **/
    int left = 1;
    int indx = 1;
    int right = n-1;
    int p = 0;

    // If right index is negative or 0 then terminate.
    if(n <= 1)
        return cnt;

    // choose pivot and  always place it at first element of array
    choose_pivot(data, n);
    p = data[0];

    /** your code here **/
    while(indx <= right){
        if(data[indx] < p){
            SWAP(data[indx],data[left], int);
            indx++, left++;
        }
        else if(data[indx] == p){
            indx++;
        }
        else if(data[indx] > p){
            SWAP(data[right], data[indx], int);
            right--;
        }
    }

    SWAP(data[0], data[left-1], int);
    three_way_quick_sort(&data[0], left - 1);
    three_way_quick_sort(&data[indx], n - indx);

    return cnt;
}



int main (int argc, char* argv[]) {

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
        printf("ERROR: Fail to open file\n");
        return 0;
    }

    /* Argument 2 => input N
     * Input string should be a number.
     * If an any single of character is included,
     * program should be terminated
     */
    if(!(sBuf = checkstr(argv[2])))
        return 0;

    // This program consider input upt to 1000000.
    // So if that is bigger than threshold, terminate.
    if(strSize(argv[2], sBuf))
        return 0;

    /*
     * Reset Variable
     * Argument error checking is done,
     * so some of variable need to reset
     * for saving memory or needless action.
     */
    N = strtol(argv[2], &gCol, 10);
    if(N < 0){
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

        three_way_quick_sort(arr, N);
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

