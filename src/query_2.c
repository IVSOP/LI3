#include <stdio.h>
#include <stdlib.h>
#include "query_2.h"
#include "commonParsing.h"

//  ./programa-principal Dataset_Fase1 teste.txt

//NOTA funções utlizadas por várias funções devem devolver, por exemplo strings, que são cópias, para caso sejam usadas noutras funções não alterarem essas strings

void freeRidesRating (void * DriverRidesRating) {
    driverRatingInfo * currentArrayStruct = DriverRidesRating;
    free(currentArrayStruct->ratingChart);
    free(currentArrayStruct->mostRecRideDate);
}

driverRatingInfo * newDriverRating (char * rideDate, short int driverNumber, short int rating) {
    driverRatingInfo * new = malloc(sizeof(driverRatingInfo));
    new->ratingChart = calloc(5,sizeof(unsigned int));
    new->ratingChart[rating-1] ++;
    new->mostRecRideDate = rideDate; // devo copiar a string para a nova struct, para não haver dependências no futuro??
    new->driverNumber = driverNumber;
    return new;
}


//TODO: free dos elementos do g_ptr_array
char * query_2 (char * number, char * trash1, char * trash2, UserData *userData, DriverStruct *driverData[], RidesData *ridesData) {
    unsigned int i, driverStatus;
    gint driverNumber;
    gint elemNumber = (gint) DRIVER_ARR_SIZE * SIZE; // numero de elem do array
    driverRatingInfo * newStruct = NULL,// pointer para struct que vai ser guardada em cada pos do array
    * currentArrayStruct = NULL;
    RidesStruct * currentRide = NULL; // pointer para a ride correspondente no ficheiro csv
    DriverStruct * currentDriver = NULL; // pointer para o driver correspondente à ride atual, no ficheiro csv
    GPtrArray * driverRatingArray = g_ptr_array_new(); // novo garraypointer
    g_ptr_array_set_size(driverRatingArray,elemNumber);
    driverRatingArray->len = elemNumber;
    for (i=0;i<elemNumber;i++) {
        currentRide = getRideByID(ridesData->ridesArray,i+1);
        //printf("|||rideuser: %s ridedriver:%d\n", currentRide->user,currentRide->driver);
        driverNumber = (gint) currentRide->driver; // o array tem posições de 0 a 9999, os driverID vão de 1 a 10000, daí o driverNumber-1
        currentDriver = getDriverByID(driverData,driverNumber);
        driverStatus = currentDriver->status;
        currentArrayStruct = (driverRatingInfo *) g_ptr_array_index(driverRatingArray, driverNumber-1); 
        //printf("driver status: %d\n", currentDriver->status);
        //printf("00 is array pos %d not occupied: %d, iteration: %d\n",driverNumber-1,currentArrayStruct == 0,i+1);
        //printf(">>>current ride date:%s\n", currentRide->date);
        if (currentArrayStruct == 0 && driverStatus == 1) { // verifica se no local atual ainda n existe informação de um driver, e se este tem o estado ativo
            //printf(">>> entrou no if\n");
            newStruct = newDriverRating(currentRide->date,(short int) driverNumber,currentRide->score_d);
            //printf(">>>newstruct driver saved:%d\n",newStruct->driverNumber);
            g_ptr_array_index(driverRatingArray, driverNumber-1) = newStruct; // para debug só
            //printf(">>>000 is array pos %d not occupied: %d\n",driverNumber-1,g_array_index(driverRatingArray, driverRatingInfo *, driverNumber-1) == 0);
            //printf(">>>|||ridedriver:%d\n", currentRide->driver);
            currentArrayStruct = (driverRatingInfo *) g_ptr_array_index(driverRatingArray, driverNumber-1); 
            //printf("driver various ratings: %d %d %d %d %d\n\n",currentArrayStruct->ratingChart[0],currentArrayStruct->ratingChart[1],currentArrayStruct->ratingChart[2],currentArrayStruct->ratingChart[3],currentArrayStruct->ratingChart[4]);
        }
        else if (driverStatus == 1) { // se já existir informação de um driver (de rides prévias); supõe-se que tem estado ativo
            //printf("<<< entrou no else if\n");
            //printf("\nrepetição aqui\n\n");
            (currentArrayStruct->ratingChart)[(currentRide->score_d)-1] += 1; // dependendo da avaliação na ride atual, no array ratingChart , incrementa em 1 o número da avalições de valor 1,2,3,4 ou 5. 
            if (compDates(currentRide->date,currentArrayStruct->mostRecRideDate) >= 0) {
                currentArrayStruct->mostRecRideDate = currentRide->date;
            }
            currentArrayStruct = (driverRatingInfo *) g_ptr_array_index(driverRatingArray, driverNumber-1); 
            //printf("driver various ratings: %d %d %d %d %d; saved ride date: %s\n\n",currentArrayStruct->ratingChart[0],currentArrayStruct->ratingChart[1],currentArrayStruct->ratingChart[2],currentArrayStruct->ratingChart[3],currentArrayStruct->ratingChart[4], currentArrayStruct->mostRecRideDate);
        }
    }

    char * result = malloc(sizeof(char)*5);
    strcpy(result,"oula");
    return result;
}


// // //TODO: para dar output de várias linhas, meter /n entre linhas . query_requests para escrever no ficheiro tem de receber mais de uma string usar void * em vez de char * !!!
// char * query_2 (char * number, char * trash1, char * trash2, UserData *userData, DriverStruct *driverData[], RidesData *ridesData) {
//     unsigned int i, driverStatus;
//     guint driverNumber;
//     guint elemSize = sizeof(driverRatingInfo *); // tamanho de um elem do array
//     guint elemNumber = DRIVER_ARR_SIZE * SIZE; // numero de elem do array
//     driverRatingInfo * newStruct = NULL,// pointer para struct que vai ser guardada em cada pos do array
//     * currentArrayStruct = NULL;
//     RidesStruct * currentRide = NULL; // pointer para a ride correspondente no ficheiro csv
//     DriverStruct * currentDriver = NULL; // pointer para o driver correspondente à ride atual, no ficheiro csv
//     GArray * driverRatingArray = g_array_sized_new(FALSE,TRUE,elemSize,elemNumber); // novo garray
//     for (i=0;i<10;i++) {
//         currentRide = getRideByID(ridesData->ridesArray,i+1);
//         printf("|||rideuser: %s ridedriver:%d\n", currentRide->user,currentRide->driver);
//         driverNumber = (guint) currentRide->driver; // o array tem posições de 0 a 9999, os driverID vão de 1 a 10000, daí o driverNumber-1
//         currentDriver = getDriverByID(driverData,driverNumber);
//         driverStatus = currentDriver->status;
//         currentArrayStruct = g_array_index(driverRatingArray, driverRatingInfo *, driverNumber-1); 
//         //printf("driver status: %d\n", currentDriver->status);
//         printf("00 is array pos %d not occupied: %d, iteration: %d\n",driverNumber-1,currentArrayStruct == 0,i+1);
//         //printf(">>>current ride date:%s\n", currentRide->date);
//         if (currentArrayStruct == 0 && driverStatus == 1) { // verifica se no local atual ainda n existe informação de um driver, e se este tem o estado ativo
//             printf(">>> entrou no if\n");
//             newStruct = newDriverRating(currentRide->date,(short int) driverNumber,currentRide->score_d);
//             printf(">>>newstruct driver saved:%d\n",newStruct->driverNumber);
//             g_array_index(driverRatingArray, driverRatingInfo *, driverNumber-1) = newStruct;
//             //printf(">>>000 is array pos %d not occupied: %d\n",driverNumber-1,g_array_index(driverRatingArray, driverRatingInfo *, driverNumber-1) == 0);
//             //printf(">>>|||ridedriver:%d\n", currentRide->driver);
//             //currentArrayStruct = g_array_index(driverRatingArray, driverRatingInfo *, driverNumber-1); 
//             // printf("driver various ratings: %d %d %d %d %d\n",currentArrayStruct->ratingChart[0],currentArrayStruct->ratingChart[1],currentArrayStruct->ratingChart[2],currentArrayStruct->ratingChart[3],currentArrayStruct->ratingChart[4]);
//         }
//         else if (driverStatus == 1) { // se já existir informação de um driver (de rides prévias); supõe-se que tem estado ativo
//             printf("<<< entrou no else if\n");
//             printf("\nrepetição aqui\n\n");
//             (currentArrayStruct->ratingChart)[(currentRide->score_d)-1] += 1; // dependendo da avaliação na ride atual, no array ratingChart , incrementa em 1 o número da avalições de valor 1,2,3,4 ou 5. 
//             if (compDates(currentRide->date,currentArrayStruct->mostRecRideDate) >= 0) {
//                 currentArrayStruct->mostRecRideDate = currentRide->date;
//             }
//         }
//     }

//     char * result = malloc(sizeof(char)*5);
//     strcpy(result,"oula");
//     return result;
// }
