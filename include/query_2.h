#ifndef QUERRY_2
#define QUERRY_2

#include "ridesData.h"
#include "driverdata.h"
#include "userdata.h"
#include "commonParsing.h"

typedef struct {
    unsigned int  * ratingChart;
    char * mostRecRideDate;
    short int driverNumber; // talvez meter em int o valor, ocupa menos espaço com char?
} driverRatingInfo;

void freeRidesRating (void * DriverRidesRating);
driverRatingInfo * newDriverRating (char *, short int, short int);
// int compDates (char * date1, char * date2);
char * query_2(char *, char *, char *, UserData *userData, DriverStruct *driverData[], RidesData *ridesData);

#endif
