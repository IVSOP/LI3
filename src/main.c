#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "userdata.h"
#include "driverdata.h"
#include "ridesData.h"
#include "query_requests.h"
#include "files.h"

int main (int argc, char **argv) {
	clock_t start, end;
	double cpu_time_used;
	start = clock();

    // se não for dado o ficheiro e as querries no terminal - fase 2
    if (argc < 2) {
        fprintf(stderr, "Modo interativo por implementar!\n");
		exit(1);
	}

	//caso seja facultado o ficheiro e as querries no terminal - fase 1
    FILE ** files = open_cmdfiles(argv); // array com pointers para os ficheiros {users,drivers,rides,querries}

	DATA users = getUserData(files[0]);
	
	DATA drivers = getDriverData(files[1]);

	DATA rides = getRidesData(files[2]);
	// DATA rides = NULL;
	
	// int ret = queryRequests(files[3], users, drivers, rides);
    // if (ret) {
    // 	perror("error reading query requests");
    //     return 2;
    // }

	end = clock();
	cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
	printf("1:%g\n", cpu_time_used);

	fclose(files[0]);
	fclose(files[1]);
	fclose(files[2]);
	fclose(files[3]);

	freeUserData(users);
	freeDriverData(drivers);
	freeRidesData(rides);
	free(files);
	
	end = clock();
	cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
	printf("2:%g\n", cpu_time_used);
	return 0;
}
