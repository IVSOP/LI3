#include <stdlib.h>
#include <stdio.h>
#include "query_requests.h"
#include "query_func.h"
#include <string.h>
#include "userdata.h"
#include "driverdata.h"
#include "ridesData.h"

#define LINE_SIZE 128

query_func * queryList[9] = {query_1, query_2, NULL, query_4, NULL, NULL, NULL, NULL, NULL};

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

int queryRequests (FILE * fp, Userstruct *userData[], DriverStruct *driverData[], RidesData *ridesData) {
    char * strBuffer = malloc(sizeof(char)*LINE_SIZE); // buffer de cada linha lida
    char * querryResult = NULL; // pointer para a string resultante de cada querry
    char * queryNumber = NULL, * str1 = NULL, * str2 = NULL, * str3 = NULL; // input 1,2,3,4 da query (possivelmente não utilizados
    char * tempsegstr[4] = {queryNumber,str1,str2,str3}; // array para atribuir, a cada string acima, o segmento correto do input
    char * strHolder, *temp;
    ssize_t read; size_t len = LINE_SIZE; // para o getline
    int i,j, commandN = 1, writeRet;
    
    // lê linhas individualmente até chegar ao fim do ficheiro
	for (i=0; (read = getline(&strBuffer, &len, fp) != -1); i++, commandN++) {
		// isto é preciso pq??????????????????????????????????????
        strBuffer[strcspn(strBuffer, "\n")] = 0; // para remover o newline

        // mais rápido assim ou fazer só sscanf com vários ifs para cada query??
		// isto é preciso?????????????????????????????????????? a query que trate dos dados
		// strsep mete a string original a NULL por alguma razão
		temp = strBuffer;
        for (j = 0; j < 4 && (strHolder = strsep(&strBuffer," ")); j++) { // j<4 por segurança
            tempsegstr[j] = strHolder;
        }
		strBuffer = temp;
		
        // está a dar erro usar as queryNumber,str1,str2,str3 diretamente???
        querryResult = queryList[(*tempsegstr[0]) - 49] (tempsegstr[1],tempsegstr[2],tempsegstr[3],userData,driverData,ridesData); // -48 para dar o numero correto, -1 para a query 1 dar no lugar 0
        writeRet = writeResults(commandN, querryResult);
        if (writeRet) {
            fprintf(stderr, "error writing file");
            return 3;
        }
		free (querryResult); // free do buffer de output
		len = LINE_SIZE;
    }
    free (strBuffer); // free do buffer de input
    return 0;
}
