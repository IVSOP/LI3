#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "userdata.h"
#include "driverdata.h"
#include "ridesData.h"
#include "query_requests.h"
#include "files.h"
#include <unistd.h>

enum { NS_PER_SECOND = 1000000000 };

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

int main (int argc, char **argv) {
    clock_t cpu_start, cpu_end;
	double cpu_time_used;
	struct timespec start, finish, delta;

	// se não for dado o ficheiro e as querries no terminal - fase 2
    if (argc < 2) {
        fprintf(stderr, "Modo interativo por implementar!\n");
		exit(1);
	}
	//caso seja facultado o ficheiro e as querries no terminal - fase 1
    FILE ** files = open_cmdfiles(argv); // array com pointers para os ficheiros {users,drivers,rides,querries}

	cpu_start = clock();
	clock_gettime(CLOCK_REALTIME, &start);

	DATA users = getUserData(files[0]);
	
	DATA drivers = getDriverData(files[1]);

	DATA rides = getRidesData(files[2]);

	cpu_end = clock();
	clock_gettime(CLOCK_REALTIME, &finish);
	sub_timespec(start, finish, &delta);
	cpu_time_used = ((double) (cpu_end - cpu_start)) / CLOCKS_PER_SEC;
	printf("CPU time-loading data:%g\n", cpu_time_used);
	printf("Wall clock time-loading data:%d.%.9ld\n", (int)delta.tv_sec, delta.tv_nsec);
	
	// int ret = queryRequests(files[3], users, drivers, rides);
    // if (ret) {
    // 	fprintf(stderr, "Error reading query requests");
    //     return 2;
    // }

	fclose(files[0]);
	fclose(files[1]);
	fclose(files[2]);
	fclose(files[3]);

	cpu_start = clock();
	clock_gettime(CLOCK_REALTIME, &start);

	freeUserData(users);
	freeDriverData(drivers);
	freeRidesData(rides);
	free(files);

	cpu_end = clock();
	clock_gettime(CLOCK_REALTIME, &finish);
	sub_timespec(start, finish, &delta);
	cpu_time_used = ((double) (cpu_end - cpu_start)) / CLOCKS_PER_SEC;
	printf("CPU time-freeing data:%g\n", cpu_time_used);
	printf("Wall clock time-freeing data:%d.%.9ld\n", (int)delta.tv_sec, delta.tv_nsec);

	return 0;
}
