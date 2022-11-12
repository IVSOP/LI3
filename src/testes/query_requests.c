#include <stdlib.h>
#include <stdio.h>
#include "query_requests.h"
#include "query_func.h"
#include <string.h>
#include "userdata.h"
#include "driverdata.h"
#include "ridesData.h"
#include <unistd.h>

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

#define LINE_SIZE 128

int writeResults (int commandN, char * strResult) {
    int ret;
    char outputFileName[20] = "commandX_output.txt"; // path do ficheiro com o resultado do comando
    outputFileName[7] = (commandN+48);
    char resultPath[31] = "Resultados/"; //31 é a soma dos caracteres de resultados com o nome do ficheiro a criar
    strcat(resultPath,outputFileName);
    FILE *fpout = fopen(resultPath,"w");
    if (!fpout) {
		perror("Unable to open/create output file");
        return 1;
    }
    ret = fputs(strResult, fpout);
    if (ret == EOF) {
        return 2;
    }
	fclose(fpout);
    return 0;
}

int queryRequests (FILE * fp, UserData *userData, DriverStruct *driverData[], RidesData *ridesData) {
	clock_t cpu_start, cpu_end;
	double cpu_time_used;
	struct timespec start, finish, delta;

	query_func * queryList[9] = {query_1, query_2, NULL, query_4, NULL, NULL, NULL, NULL, NULL};
    char * strBuffer = malloc(sizeof(char)*LINE_SIZE); // buffer de cada linha lida
    char * querryResult = NULL; // pointer para a string resultante de cada querry
    char * tempsegstr[4]; // array para atribuir o segmento correto do input
    char * strHolder, *temp;
    ssize_t read; size_t len = LINE_SIZE; // para o getline
    int i,j, commandN = 1, writeRet;
    
    // lê linhas individualmente até chegar ao fim do ficheiro
	for (i=0; (read = getline(&strBuffer, &len, fp) != -1); i++, commandN++) {
		cpu_start = clock();
		clock_gettime(CLOCK_REALTIME, &start);

        strBuffer[strcspn(strBuffer, "\n")] = 0; // para remover o newline

        // mais rápido assim ou fazer só sscanf com vários ifs para cada query??
		temp = strBuffer;
        for (j = 0; j < 4 && (strHolder = strsep(&strBuffer," ")); j++) { // j<4 por segurança
            tempsegstr[j] = strHolder;
        }
		strBuffer = temp;

        querryResult = queryList[(*tempsegstr[0]) - 49] (tempsegstr[1],tempsegstr[2],tempsegstr[3],userData,driverData,ridesData); // -48 para dar o numero correto, -1 para a query 1 dar no lugar 0
		writeRet = writeResults(commandN, querryResult);
        if (writeRet) {
            fprintf(stderr, "error writing file");
            return 3;
        }
		free (querryResult); // free do buffer de output
		len = LINE_SIZE; // após um getline, len é alterado para o tamanho da linha; tem de ser reset, a próxima linha pode ter len maior

		cpu_end = clock();
		clock_gettime(CLOCK_REALTIME, &finish);
		sub_timespec(start, finish, &delta);
		cpu_time_used = ((double) (cpu_end - cpu_start)) / CLOCKS_PER_SEC;
		printf("Answering query %d:\nCPU time:%g\n", (int)((*tempsegstr[0]) - 48), cpu_time_used);
		printf("Wall clock time:%d.%.9ld\n\n", (int)delta.tv_sec, delta.tv_nsec);

    }
    free (strBuffer); // free do buffer de input
    return 0;
}