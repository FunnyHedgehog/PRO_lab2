#define _CRT_SECURE_NO_WARNINGS
#include <mpi.h>
#include <iostream>
#include <stdio.h>
#include <time.h>
#include <Windows.h>
#define MAX_MESSAGE_SIZE 100
#define BUFFER_SIZE 100

using namespace std;


void firstProcess(int *buffer_size, int *message_size) {
	int TAG = 0;
	int number_from_console;
	MPI_Status status;
	cout << "Enter number for process 1: "<< endl;
	cin >> number_from_console;

	MPI_Send(&number_from_console, 1, MPI_INT, 1, TAG, MPI_COMM_WORLD);//відсилає другому
	number_from_console *= 2;
	MPI_Send(&number_from_console, 1, MPI_INT, 2, TAG, MPI_COMM_WORLD);
	MPI_Send(&number_from_console, 1, MPI_INT, 3, TAG, MPI_COMM_WORLD);

	const char * my_message = "A life is a moment";//task2
	*buffer_size = sizeof(my_message);
	int buf_size = *buffer_size;
	char *buf = (char*)malloc(buf_size + MPI_BSEND_OVERHEAD);

	MPI_Buffer_attach(buf, buf_size + MPI_BSEND_OVERHEAD);
	buf = (char*)my_message;
	MPI_Bsend(&buf, buf_size, MPI_CHAR, 1, TAG, MPI_COMM_WORLD);
	cout << "PROCESS #1 SEND MESSAGE " << buf << endl;
	MPI_Bsend(&buf, buf_size, MPI_CHAR, 2, TAG, MPI_COMM_WORLD);
	cout << "PROCESS #1 SEND MESSAGE " << buf << endl;
	MPI_Bsend(&buf, buf_size, MPI_CHAR, 3, TAG, MPI_COMM_WORLD);
	cout << "PROCESS #1 SEND MESSAGE " << buf << endl;
	MPI_Buffer_detach(&buf, &buf_size);
	free(buf);

	double number = 2.0;
	number *= 3;
	cout << number << endl;
	MPI_Ssend(&number, 1, MPI_DOUBLE, 1, TAG,MPI_COMM_WORLD);
	MPI_Ssend(&number, 1, MPI_DOUBLE, 2, TAG, MPI_COMM_WORLD);
	MPI_Ssend(&number, 1, MPI_DOUBLE, 3, TAG, MPI_COMM_WORLD);

}
void secondProcess(int *buffer_size, int *message_size) {
	int TAG = 0;
	int number_from_first_process;
	MPI_Status status;
	MPI_Recv(&number_from_first_process, 1, MPI_INT, 0, TAG, MPI_COMM_WORLD, &status);
	cout << "PROCESS #2 RECEIVE NUMBER FROM PROCESS #1: " << number_from_first_process << endl;

	int buf_size = *buffer_size;
	char * buf = (char*)malloc(buf_size);

	MPI_Recv(&buf, buf_size, MPI_CHAR, 0, TAG, MPI_COMM_WORLD, &status);//???
	cout << "PROCESS #2 RECEIVE BUFFER FROM PROCESS #1: " << buf << endl;

	double number;
	MPI_Recv(&number, 1, MPI_DOUBLE, 0, TAG,	MPI_COMM_WORLD, &status);

}
void thirthProcess(int *buffer_size, int *message_size, char * word) {

	int TAG = 0;
	int number_from_first_process;
	MPI_Status status;
	MPI_Recv(&number_from_first_process, 1, MPI_INT, 0, TAG, MPI_COMM_WORLD, &status);
	cout << "PROCESS #3 RECEIVE NUMBER FROM PROCESS #1: " << number_from_first_process << endl;

	int buf_size = *buffer_size;
	char * buf = (char*)malloc(buf_size);

	MPI_Recv(&buf, buf_size, MPI_CHAR, 0, TAG, MPI_COMM_WORLD, &status);//???
	cout << "PROCESS #3 RECEIVE BUFFER FROM PROCESS #1: " << buf << endl;

	double number;
	MPI_Recv(&number, 1, MPI_DOUBLE, 0, TAG, MPI_COMM_WORLD, &status);
	cout << "PROCESS #3 RECEIVE Number FROM PROCESS #1: " << number << endl;

}
void fourthProcess(int *buffer_size, int *message_size, char * word) {
	int TAG = 0;
	int number_from_first_process;
	MPI_Status status;
	MPI_Recv(&number_from_first_process, 1, MPI_INT, 0, TAG, MPI_COMM_WORLD, &status);
	cout << "PROCESS #4 RECEIVE NUMBER FROM PROCESS #1: " << number_from_first_process << endl;

	int buf_size = *buffer_size;
	char * buf = (char*)malloc(buf_size);

	MPI_Recv(&buf, buf_size, MPI_CHAR, 0, TAG, MPI_COMM_WORLD, &status);//???
	cout << "PROCESS #4 RECEIVE BUFFER FROM PROCESS #1: " << buf << endl;

	double number;
	MPI_Recv(&number, 1, MPI_DOUBLE, 0, TAG, MPI_COMM_WORLD, &status);
}

int main(int argc, char * argv[]) {
	int message_size = 4, buffer_size = sizeof("A life is a moment ");
	MPI_Init(&argc, &argv); int rank, size;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	switch (rank)
	{
	case 0:
		firstProcess(&buffer_size, &message_size);
		break;
	case 1:
		secondProcess(&buffer_size, &message_size);
		break;

	case 2:
		thirthProcess(&buffer_size, &message_size, argv[1]);
		break;

	case 3:
		fourthProcess(&buffer_size, &message_size, argv[1]);
		break;

	}
	MPI_Finalize();
	return 0;
}