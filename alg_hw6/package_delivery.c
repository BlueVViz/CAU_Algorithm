/**
 * "Package Delivery"
 * ./package_delivery <input_file_name>
 *
 * Date: 6/10/18
 * name: Junhyuck Woo
 * ID: 20145337
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <unistd.h>

#define CHILD 10
#define ITERATION 10000

#define SWAP(a, b, type) type temp = (a); (a) = (b); (b) = temp;

/* Function */
/**
 * Modified Quick sort
 * Sort dump data(list of item) associated
 */
void choose_pivot (unsigned int *data, unsigned int *dump, unsigned int n) {
    int buf;
    if(n > 0 ){
        buf  = rand()%n;
        SWAP(data[0], data[buf], unsigned int);
        SWAP(dump[0], dump[buf], );
    }
}
void quick_sort (unsigned int *data, unsigned int *dump, unsigned int n) {
    int idxFor = 0;
    int left = 1;
    int right = n - 1;
    int p = 0;

    choose_pivot(data, dump, n);
    p = data[0];

    if(n > 1) {
        for (idxFor = 1; idxFor <= right; idxFor++) {
            if (data[idxFor] > p) {
                SWAP(data[left], data[idxFor], unsigned int);
                SWAP(dump[left], dump[idxFor], );
                left++;
            }
        }

        SWAP(data[0], data[left - 1], int);
        SWAP(dump[0], dump[left - 1], );

        quick_sort(&data[0], &dump[0], left - 1);
        quick_sort(&data[left], &dump[left], n - (left));
    }
}

/**
 * Create First generation
 * @param w: Warehouse number
 * @param l: location number
 * @param n: total input item number
 * @param dumpPressed: accumulate dump item
 * @param cpy: Copy of dump data
 * @param chromosome: data set
 */
void gene(unsigned int w, unsigned int l, unsigned int n,
          unsigned int *dumpPressed, unsigned int *cpy, unsigned int ***chromosome){
    int location = 0, destination = 0;
    int idFor = 0;
    int jdFor = 0;

    for(idFor = 0; idFor < CHILD; idFor++){
        memcpy(dumpPressed, cpy, sizeof(unsigned int) * l);
        for(jdFor = 0; jdFor < n; jdFor++){
            location = rand() % w;
            do{
                destination = rand() % l;
            }while(dumpPressed[destination] == 0);
            chromosome[idFor][location][destination]++;
            dumpPressed[destination]--;
        }
    }
}

/**
 * Create offspring using best object
 * @param w: Warehouse number
 * @param l: location number
 * @param n: total input item number
 * @param order: order of iteration
 * @param evaluation: list of offset delivery time array
 * @param dumpPressed: accumulate dump item
 * @param cpy: Copy of dump data
 * @param chromosome: data set
 */
void offSpring(unsigned int w, unsigned int l, unsigned int n, unsigned int order,
               unsigned int *evaluation, unsigned int *dumpPressed, unsigned int *cpy, unsigned int ***chromosome){
    unsigned int iFor = 0, jFor = 0, kFor = 0, index = 0, tmp = 0;
    unsigned int pick = 0, len = 0, num = n;
    unsigned int cover = 0;
    unsigned int buf = evaluation[0];

    /// For fast calculation - compromise spped & accurate
    if(n < 1000){
        cover = 1;
    }
    else if(n < 100000){
        cover = 10;
    }
    else{
        cover = 100;
    }



    // Select elite
    for(iFor = 1; iFor < CHILD; iFor++){
        if(buf > evaluation[iFor]) {
            buf = evaluation[iFor];
            index = iFor;
        }
    }

    for(jFor = 0; jFor < w; jFor++){
        for(kFor = 0; kFor < l; kFor++)
            chromosome[0][jFor][kFor] = chromosome[index][jFor][kFor];
    }


    for(iFor = 1; iFor < CHILD; iFor++){
        for(jFor = 0; jFor < w; jFor++){
            free(chromosome[iFor][jFor]);
            chromosome[iFor][jFor] = calloc(l, sizeof(unsigned int));
        }
    }

    if(order < ((ITERATION / cover) / 5)){
        for(iFor = 1; iFor < CHILD; iFor++){
            memcpy(dumpPressed, cpy, sizeof(unsigned int) * l);
            for(jFor = 0; jFor < n; jFor++){
                len = rand() % w;
                do{
                    pick = rand() % l;
                }while(dumpPressed[pick] == 0);
                chromosome[iFor][len][pick]++;
                dumpPressed[pick]--;
            }
        }
    }
    else {
        for(iFor = 1; iFor < CHILD; iFor++){
            pick = rand() % w;
            len = (rand() % w );
            num = n;
            memcpy(dumpPressed, cpy, sizeof(unsigned int) * l);

            // Copy
            for(jFor = 0; jFor < len ;jFor++) {
                memcpy(chromosome[iFor][(jFor + pick)%w], chromosome[0][(jFor + pick)%w], sizeof(unsigned int) * l);
                for(kFor = 0; kFor < l; kFor++){
                    dumpPressed[kFor] -= chromosome[0][(jFor + pick)%w][kFor];
                    num -= chromosome[0][(jFor + pick)%w][kFor];
                }
            }

            if(order < 2 * ((ITERATION / cover) / 5)){
                for(jFor = 0; jFor < num; jFor++){
                    len = rand() % w;
                    do{
                        pick = rand() % l;
                    }while(dumpPressed[pick] == 0);
                    chromosome[iFor][len][pick]++;
                    dumpPressed[pick]--;
                }
            }
            else{
                index = pick;
                for(jFor = 0; jFor + len <  w; jFor++){
                    if(num != 0){
                        tmp = rand() % num;
                    }
                    if(jFor + len + 1 == w){
                        tmp = num;
                    }
                    for(kFor = 0; kFor < tmp;kFor++){
                        do{
                            pick = rand() % l;
                        }while(dumpPressed[pick] == 0);
                        chromosome[iFor][(jFor + len + index) % w][pick]++;
                        dumpPressed[pick]--; num--;
                    }
                }
            }
        }
    }
}

/**
 * Calculate delivery time
 * @param w: Warehouse number
 * @param l: location number
 * @param dump: item array
 * @param evaluation: list of offset delivery time array
 * @param timeTale: graph about delivery time
 * @param chromosome: data set
 */
void delivery(unsigned int w, unsigned int l, unsigned int *drone, unsigned int *evaluation,
              unsigned int **timeTable, unsigned int ***chromosome) {
    int iFor = 0, jFor = 0, kFor = 0, lFor = 0, buf = 0, tmp = 0, timeBuf = 0;
    int count = 0, flag = 0;
    unsigned int time = 0;
    unsigned int *box = NULL;
    unsigned int *backup1 = (unsigned int*)calloc(l, sizeof(unsigned int));
    unsigned int *backup2 = (unsigned int*)calloc(l, sizeof(unsigned int));

    for(iFor = 0; iFor < CHILD; iFor++){
        // Reset variable
        count = 0;
        for(jFor = 0; jFor < w; jFor++){

            memcpy(backup1, chromosome[iFor][jFor], l * sizeof(unsigned int));
            memcpy(backup2, timeTable[jFor], l * sizeof(unsigned int));
            quick_sort(timeTable[jFor], chromosome[iFor][jFor], l);

            box = (unsigned int*)calloc(drone[jFor], sizeof(unsigned int));
            buf = 0, count = 0, kFor = 0; // Reset data for the other
            while(1){
                // Find
                while(chromosome[iFor][jFor][buf] == 0 && buf < l){
                    buf++;
                }
                if(buf == l){
                    break;
                }

                /// Allocate item to drone, and calculate delivery time
                while(count != chromosome[iFor][jFor][buf]){
                    timeBuf = box[kFor] + timeTable[jFor][buf];
                    flag = 0;
                    for(tmp = 0; tmp < drone[jFor]; tmp++){
                        if(box[kFor] > box[tmp] + timeTable[jFor][buf]){
                            box[tmp] += timeTable[jFor][buf];
                            flag = 1; break;
                        }
                        else if(timeBuf >= box[tmp] + timeTable[jFor][buf]){
                            box[tmp] += timeTable[jFor][buf];
                            flag = 1; break;
                        }
                    }
                    if(flag == 0){
                        box[kFor] = timeBuf;
                    }

                    count++;
                    if((kFor < drone[jFor] - 1) && (kFor >= 0) && (drone[jFor] != 1)){
                        kFor++;
                    }
                    else if(drone[jFor] != 1){
                        kFor--;
                    }
                }
                count = 0; buf++;
            }
            /// each drone has delivery time and find worst time
            for(lFor = 0; lFor < drone[jFor]; lFor++){
                if(time < box[lFor]){
                    time = box[lFor];
                }
            }
            // Restore data set
            memcpy(chromosome[iFor][jFor], backup1, l * sizeof(unsigned int));
            memcpy(timeTable[jFor], backup2, l * sizeof(unsigned int));

            free(box); // Free mremory
        }

        evaluation[iFor] = time;
        time = 0;
    }
    free(backup1); // Free memory
    free(backup2); // Free memory
}

/**
 * main function - Actually work
 * @param w: Warehouse number
 * @param location: location number
 * @param numItem: total input item number
 * @param dump: item array
 * @param dumpPressed: accumulate dump item
 * @param drone: number of drone array
 * @param timeTale: graph about delivery time
 * @return best time
 */
unsigned int packageAssign(unsigned int warehouse, unsigned int location, unsigned int numItem,
                           unsigned int *dump, unsigned int *dumpPressed, unsigned int *drone,
                           unsigned int **timeTale){
    /// Variable declaration
    unsigned int best = 0;
    unsigned int time = 0;
    unsigned int iFor = 0;
    unsigned int jFor = 0;
    unsigned int kFor = 0;
    unsigned int cover = 0;
    unsigned int evaluation[CHILD] = {0, };
    unsigned int cpy[location];
    unsigned int itemLoc[numItem];
    unsigned int ***chromosome = (unsigned int***)calloc(CHILD, sizeof(unsigned int**));

    /// For fast calculation - compromise spped & accurate
    if(numItem < 1000){
        cover = 1;
    }
    else if(numItem < 100000){
        cover = 10;
    }
    else{
        cover = 100;
    }

    /// Backup dumpPressed data to restore later
    memcpy(cpy, dumpPressed, sizeof(unsigned int ) * location);

    /// Data set to save child
    for(iFor = 0; iFor < CHILD; iFor++){
        chromosome[iFor] = (unsigned int**)calloc(warehouse, sizeof(unsigned int*));
        for(jFor = 0; jFor < warehouse; jFor++){
            chromosome[iFor][jFor] = (unsigned int*)calloc(location, sizeof(unsigned int));
        }
    }
    /// Create First generation of chromosome set
    gene(warehouse, location, numItem, dumpPressed, cpy, chromosome);

    /// Iteration begin => Upto 1000 times
    for(iFor = 0; ; iFor++){
        /// Restore backup contents
        memcpy(dumpPressed, cpy, sizeof(unsigned int ) * location);
        memset(evaluation, 0, sizeof(unsigned int) * CHILD);
        best = 0;

        /// calculate
        delivery(warehouse, location, drone, evaluation, timeTale, chromosome);

        /// Find Best one in data set - have least delivery time
        time = evaluation[0];
        for(jFor = 0; jFor < CHILD; jFor++){
            if(time > evaluation[jFor]){
                time = evaluation[jFor];
                best = jFor;
            }
        }
        /// When match the Iteration number
        if(iFor == ITERATION / cover ){
            break;
        }

        /// Evolution - generate offset chromosome data set
        memcpy(dumpPressed, cpy, sizeof(unsigned int ) * location);
        offSpring(warehouse, location, numItem, iFor, evaluation, dumpPressed, cpy, chromosome);
    }

    /// Print item location
    for(iFor = 0; iFor < numItem; iFor++){
         kFor = dump[iFor] - 1;
        for(jFor = 0; jFor < warehouse; jFor++){
            if(chromosome[best][jFor][kFor] == 0) {
                continue;
            }
            else {
                chromosome[best][jFor][kFor]--;
                itemLoc[iFor] = jFor + 1;
                break;
            }
        }
        printf("%d %d\n", iFor + 1, itemLoc[iFor]);
    }

    /// Free allocated memory
    for(iFor = 0; iFor < CHILD; iFor++){
        for(jFor = 0; jFor < warehouse; jFor++){
            free(chromosome[iFor][jFor]);
        }
    }
    for(iFor = 0; iFor < CHILD; iFor++){
        free(chromosome[iFor]);
    }
    free(chromosome);

    return time;
}

int main(int argc, char* argv[]) {

    //Declare variation
    int iFor = 0;
    int numItem = 0;
    int fileLine = 0;
    unsigned int w = 0;
    unsigned int l = 0;
    unsigned int buf = 0;
    unsigned int deliveryTime = 0;
    unsigned int *dump = NULL;
    unsigned int *drone = NULL;
    unsigned int *dumpPressed = NULL;
    unsigned int **timeTable = NULL;

    char cBuf[20];
    char *sBuf = NULL;
    char *gCol = NULL;

    FILE * f;

    // Argument check
    /* Argument 1 => Text file name
     * Open File
     */
    f = fopen(argv[1], "rt");

    srand(time(NULL));

    /* If file is not opened,
     * then f still remain as null.
     * => Shutdown the program.
     */
    if(f == NULL){
        printf("ERROR: Fail to open file\n");
        fclose(f);
        free(dumpPressed);
        free(timeTable);
        free(drone);
        free(dump);
        free(sBuf);
        return 0;
    }

    /* Read File
     * If file readable, then keep read
     * This program should read file up to N line,
     * If N is bigger than fileLine, it doesn't matter.
     * However, in opposite condition, we should break.
     */
    while(fgets(cBuf,sizeof(cBuf),f)) {
        // Divide it when it find a " ", "\r", "\n" or "\r\n"
        // Until it return NULL
        sBuf = strtok(cBuf, " \r\n");
        while(sBuf != NULL){
            if(fileLine == 0){
                w = (unsigned int)strtol(sBuf, &gCol, 10);
                drone = (unsigned int*)calloc(w, sizeof(unsigned int));
                timeTable = (unsigned int**)calloc(w, sizeof(unsigned int*));
            }
            else if(fileLine == 1){
                l = (unsigned int)strtol(sBuf, &gCol,10);
                dumpPressed = (unsigned int*)calloc(l, sizeof(unsigned int));
                for(iFor = 0; iFor < w; iFor++){
                    *(timeTable + iFor) = (unsigned int*)calloc(l, sizeof(unsigned int));
                }
            }
            else if(fileLine <= w + 1){
                buf = (unsigned int)strtol(sBuf, &gCol, 10) - 1;
                sBuf = strtok(NULL, " \r\n");
                drone[buf] = (unsigned int)strtol(sBuf, &gCol, 10);
                sBuf = strtok(NULL, " \r\n");
                for(iFor = 0; sBuf != NULL; iFor++){
                    timeTable[buf][iFor] = (unsigned int)strtol(sBuf, &gCol, 10);
                    sBuf = strtok(NULL, " \r\n");
                }
                iFor = 0;
            }
            else if(fileLine == w + 2){
                numItem = (unsigned int)strtol(sBuf, &gCol,10);
                dump = (unsigned int*)calloc(numItem, sizeof(unsigned int));
            }
            else{
                sBuf = strtok(NULL, " \r\n");
                buf = (unsigned int)strtol(sBuf, &gCol, 10);
                dump[iFor] = buf;
                dumpPressed[buf-1]++;
                iFor++;
            }
            fileLine++;
            sBuf = strtok(NULL, " \r\n");
        }
    }

    // Base Setting
    printf("%d\n", numItem);
    /**
     *
     * @pram w: Number of Warehouse
     * @pram l: Number of Location
     * @pram numItem: Number of item
     * @pram dump: Array of item
     * @pram dumpPressed: Accumulate dump array
     * @pram drone: Nmber of drone array
     * @pram timeTable: Graph about delivery time
     */
    deliveryTime = packageAssign(w, l, numItem, dump, dumpPressed, drone, timeTable);

    // Please keep these print statements!
    printf("Total delivery time =  %d\n", deliveryTime);

    // free memory
    fclose(f);
    for(iFor = 0; iFor < w; iFor++){
        free(timeTable[iFor]);
    }
    free(dumpPressed);
    free(timeTable);
    free(drone);
    free(dump);
    free(sBuf);

    return 0;
}
