#include <stdio.h>
#include <stdlib.h>
#include "query_func.h"

//compara se date1 é menor que date2
int compDates (char * date1, char * date2) {
    int day1,month1,year1,day2,month2,year2;
    sscanf(date1, "%d/%d/%d", &day1,&month1,&year1);
    sscanf(date2, "%d/%d/%d", &day2,&month2,&year2);
    int result = ((year1 - year2) * 64) + ((month1-month2) * 16) + ((day1-day2)); // peso do ano arbitrariamente maior e do mês segundo maior
    return (result < 0);
}




//TODO: query_requests para escrever no ficheiro tem de receber mais de uma string usar void * em vez de char * !!!
char * query_2 (char * number, char * trash1, char * trash2, Userstruct *userData[], DriverStruct *driverData[], RidesData *ridesData) {
    //int number = atoi(number);
    char a[11] = "29/11/2021";
    char b[11] = "15/12/2070";
    printf("%d\n", compDates(a,b));
    return NULL;
}