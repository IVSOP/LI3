#include <stdlib.h>
#include <stdio.h>
#include "query_requests.h"
#include "query_func.h"
#include <string.h>
#include "userdata.h"
#include "driverdata.h"
#include "ridesData.h"
#include <unistd.h>

#define LINE_SIZE 128

char * query_not_implemented(char *trash0, char *trash1, char *trash2, UserData *userData, DriverStruct *driverData[], RidesData *ridesData) {
	fprintf(stderr, "ERROR: querry not implemented\n");
	return NULL;
}


void sub_timespec(struct timespec t1, struct timespec t2, struct timespec *td)
{
    td->tv_nsec = t2.tv_nsec - t1.tv_nsec;
    td->tv_sec  = t2.tv_sec - t1.tv_sec;
    if (td->tv_sec > 0 && td->tv_nsec < 0)
    {
        td->tv_nsec += NS_PER_SECOND;
        td->tv_sec--;
    }
    else if (td->tv_sec < 0 && td->tv_nsec > 0)
    {
        td->tv_nsec -= NS_PER_SECOND;
        td->tv_sec++;
    }
}

int compareResult(char *resultStr, char *resultPath) {
	int i;
	int chr;
	FILE *fpout = fopen(resultPath, "r");
	for (i = 0; resultStr[i] != '\0' && (chr = fgetc(fpout)) != EOF; i++) {
		if ((char)chr != resultStr[i]) {
			// fprintf(stderr, "Error on character [%d]", i);
			return 3;
		}
	}
	fclose(fpout);
	return 0;
}

int writeResults (int commandN, char * strResult) {
	char resultPath[64];
	snprintf(resultPath, 64, "exemplos_de_queries/tests_1/command%d_output.txt", commandN);
    
	return compareResult(strResult, resultPath);
}

int queryRequests (FILE * fp, UserData *userData, DriverStruct *driverData[], RidesData *ridesData) {
	clock_t cpu_start, cpu_end;
	double cpu_time_used;
	struct timespec start, finish, delta;

	query_func * queryList[9] = {query_1, query_2, query_not_implemented, query_4, query_not_implemented, query_not_implemented, query_not_implemented, query_not_implemented, query_not_implemented};
    char * strBuffer = malloc(sizeof(char)*LINE_SIZE); // buffer de cada linha lida
    char * querryResult = NULL; // pointer para a string resultante de cada querry
    char * tempsegstr[4]; // array para atribuir o segmento correto do input
    char * strHolder, *temp;
    ssize_t read; size_t len = LINE_SIZE; // para o getline
    int i,j, commandN = 1, writeRet;
	char full_command[LINE_SIZE];
    
    // lê linhas individualmente até chegar ao fim do ficheiro
	for (i=0; (read = getline(&strBuffer, &len, fp) != -1); i++, commandN++) {
		cpu_start = clock();
		clock_gettime(CLOCK_REALTIME, &start);

        strBuffer[strcspn(strBuffer, "\n")] = 0; // para remover o newline

		strncpy(full_command, strBuffer, LINE_SIZE - 1);

        // mais rápido assim ou fazer só sscanf com vários ifs para cada query??
		temp = strBuffer;
        for (j = 0; j < 4 && (strHolder = strsep(&strBuffer," ")); j++) { // j<4 por segurança
            tempsegstr[j] = strHolder;
        }
		strBuffer = temp;

		printf("Answering query %d:\n", (int)((*tempsegstr[0]) - 48));
        querryResult = queryList[(*tempsegstr[0]) - 49] (tempsegstr[1],tempsegstr[2],tempsegstr[3],userData,driverData,ridesData); // -48 para dar o numero correto, -1 para a query 1 dar no lugar 0
		
		cpu_end = clock();
		clock_gettime(CLOCK_REALTIME, &finish);
		sub_timespec(start, finish, &delta);
		cpu_time_used = ((double) (cpu_end - cpu_start)) / CLOCKS_PER_SEC;
		
		if (querryResult != NULL) {
			writeRet = writeResults(commandN, querryResult);
			if (writeRet == 1) {
				fprintf(stderr, "error writing file");
				return 3;
			}
			
			if (writeRet == 3) {
				fprintf(stderr, "-->ERROR: Results differ\nCommand:%s\nExpected:%s\nGot:%s\nError file:exemplos_de_queries/tests_1/command%d_output.txt\n", full_command, "see file :)", querryResult, commandN);
			} else {
				printf("Correct answer\n");
			}
		}
		printf("CPU time:%g\n", cpu_time_used);
		printf("Wall clock time:%d.%.9ld\n\n", (int)delta.tv_sec, delta.tv_nsec);

		free (querryResult); // free do buffer de output
		len = LINE_SIZE; // após um getline, len é alterado para o tamanho da linha; tem de ser reset, a próxima linha pode ter len maior

    }
    free (strBuffer); // free do buffer de input
    return 0;
}