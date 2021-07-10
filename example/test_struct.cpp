#include <mpi.h> /* PROVIDES THE BASIC MPI DEFINITION AND TYPES */
#include <stdio.h>
#include <sys/time.h>
#include <string.h>
#include <stdlib.h>
#include "config.h"

double difftimeval(const struct timeval *start, const struct timeval *end)
{
	double d;
	time_t s;
	suseconds_t u;

	s = start->tv_sec - end->tv_sec;
	u = start->tv_usec - end->tv_usec;

	d = s;
	d *= 1000000.0;//1 秒 = 10^6 微秒
	d += u;

	return d;
}


struct Data
{
	struct timeval tv;
	char data[DATA_SIZE];
};

int main(int argc, char **argv)
{


	int my_rank;
	int partner;
	int size, i, t;
	MPI_Status stat;

	MPI_Init(&argc, &argv);					 /*START MPI */
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); /*DETERMINE RANK OF THIS PROCESSOR*/
	MPI_Comm_size(MPI_COMM_WORLD, &size);	 /*DETERMINE TOTAL NUMBER OF PROCESSORS*/

	// new comm 

	const int ranks[2] = {0, 1};
	MPI_Group wgroup, group;
	MPI_Comm_group(MPI_COMM_WORLD, &wgroup);
	MPI_Group_incl(wgroup, 2, ranks, &group);
	MPI_Comm comm1;
	MPI_Comm_create(MPI_COMM_WORLD, group, &comm1);

	MPI_Comm comm = MPI_COMM_WORLD;


	printf("begin, my rank:%d\n", my_rank);

	if (my_rank == 0)
	{
		double sum = 0.;
		struct Data *datas = (struct Data*)(malloc(TEST_TIME * sizeof(struct Data)));
		memset(datas, 1, TEST_TIME * sizeof(struct Data));
		for (int i = 0; i < TEST_TIME; i++)
		{
			struct Data *data = datas + i;
			// printf("Address of data of rank0: %p, index: %d\n", data, i);
			struct timeval tv;
			MPI_Recv(data, sizeof(struct Data), MPI_BYTE, 1, 1, comm, &stat);
			snprintf(data->data, sizeof(data->data), "%s, num:%d", "hello world!", i);
			gettimeofday(&tv, NULL);
			printf("time:%fus, receive:%s\n", difftimeval(&tv, &data->tv), data->data);
			sum += difftimeval(&tv, &data->tv);
			printf("Pause at 0\n");
			getchar();
		}
		free(datas);
		printf("Avg time: %lf\n", sum / TEST_TIME);
		printf("=======================\n");

		sum = 0.;
		for (int i = 0; i < TEST_TIME; i++)
		{
			struct Data *data =  (struct Data*)(malloc(sizeof(struct Data)));
			memset(data, 1, sizeof(struct Data));
			// printf("Address of data of rank0: %p, index: %d\n", data, i);
			struct timeval tv;
			MPI_Recv(data, sizeof(struct Data), MPI_BYTE, 1, 1, comm, &stat);
			snprintf(data->data, sizeof(data->data), "%s, num:%d", "hello world!", i);
			gettimeofday(&tv, NULL);
			printf("time:%fus, receive:%s\n", difftimeval(&tv, &data->tv), data->data);
			sum += difftimeval(&tv, &data->tv);
			// printf("Pause at 0\n");
			// getchar();

			free(data);
		}
		printf("Avg time: %lf\n", sum / TEST_TIME);
	}
	else if (my_rank == 1)
	{
		struct Data *datas =  (struct Data*)(malloc(TEST_TIME * sizeof(struct Data)));
		// static struct Data data_[TEST_TIME];
		// struct Data* datas = &data_;
		// printf("address of static: %p\n", datas);
		memset(datas, 1, TEST_TIME * sizeof(struct Data));
		for (int i = 0; i < TEST_TIME; i++)
		{
			struct Data *data = datas + i;
			// printf("Address of data of rank1: %p, index: %d\n", data, i);
			memset(data->data, 0, sizeof(data->data));
			snprintf(data->data, sizeof(data->data), "%s, num:%d", "hello world!", i);
			gettimeofday(&data->tv, NULL);
			MPI_Send(data, sizeof(struct Data), MPI_BYTE, 0, 1, comm);
		}
		free(datas);

		for (int i = 0; i < TEST_TIME; i++)
		{
			struct Data *data = (struct Data*)(malloc(sizeof(struct Data)));
			memset(data, 1, sizeof(struct Data));
			// printf("Address of data of rank1: %p, index: %d\n", data, i);
			memset(data->data, 0, sizeof(data->data));
			snprintf(data->data, sizeof(data->data), "%s, num:%d", "hello world!", i);
			gettimeofday(&data->tv, NULL);
			MPI_Send(data, sizeof(struct Data), MPI_BYTE, 0, 1, comm);
			free(data);
		}
	}

	if (my_rank == 0)
		printf("That is all for now!\n");


	if(MPI_GROUP_NULL!=group) MPI_Group_free(&group);
	if(MPI_COMM_NULL!=comm1) MPI_Comm_free(&comm1);

	MPI_Finalize(); /* EXIT MPI */
}