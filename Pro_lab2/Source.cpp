#define _CRT_SECURE_NO_WARNINGS
#include <mpi.h>
#include <iostream>
#include <vector>

#define TEXT "A life is a moment "
#define BUFFER_SIZE sizeof(TEXT)
#define TAG 0
#define FIRST_RANK 0
#define SECOND_RANK 1
#define THIRD_RANK 2
#define FOURTH_RANK 3

using namespace std;

void firstProcess() {
	
	//first task
	int number_from_console;
	MPI_Status status;
	cout << "Enter number for process 1: "<< endl;
	cin >> number_from_console;
	MPI_Send(&number_from_console, 1, MPI_INT, SECOND_RANK, TAG, MPI_COMM_WORLD);//відсилає другому
	number_from_console *= 2;
	MPI_Send(&number_from_console, 1, MPI_INT, THIRD_RANK, TAG, MPI_COMM_WORLD);
	MPI_Send(&number_from_console, 1, MPI_INT, FOURTH_RANK, TAG, MPI_COMM_WORLD);
	
	//second task
	int buf_size = sizeof(TEXT); 
	vector<char> buff(BUFFER_SIZE + MPI_BSEND_OVERHEAD);
	char* buf = buff.data();
	MPI_Buffer_attach(buf, BUFFER_SIZE + MPI_BSEND_OVERHEAD);
	buf = (char*)TEXT;
	for (int i = 1; i <= 3; i++) {
		MPI_Bsend(&buf, BUFFER_SIZE, MPI_CHAR, i, TAG, MPI_COMM_WORLD);
	}
	cout << "First process send mesagges " << buf << endl;
	MPI_Buffer_detach(&buf, &buf_size);
	free(buf);
	
	//third task
	double number = 2.0;
	number *= 3;
	cout << number << endl;
	for (int i = 1; i <= 3; i++) {
		MPI_Ssend(&number, 1, MPI_DOUBLE, i, TAG, MPI_COMM_WORLD);
	}

}
int recieveNumberFromProcess(MPI_Status *status, int numberOfProcess) {
	int recievedNumber;
	MPI_Recv(&recievedNumber, 1, MPI_INT, FIRST_RANK, TAG, MPI_COMM_WORLD, status);
	cout << "Process number " << numberOfProcess << " receive number from process with rank 0: " << recievedNumber << endl;
	return recievedNumber;
}
char* recieveStringFromProcess(MPI_Status *status, int numberOfProcess) {
	vector<char> buff(BUFFER_SIZE + MPI_BSEND_OVERHEAD);
	char* buf = buff.data();

	MPI_Recv(&buf, BUFFER_SIZE, MPI_CHAR, FIRST_RANK, TAG, MPI_COMM_WORLD, status);
	cout << "Process number " << numberOfProcess << " receive message from process with rank 0: " << buf << endl;
	return buf;
}
double recieveDoubleNumberFromProcess(MPI_Status *status, int numberOfProcess) {
	double number;
	MPI_Recv(&number, 1, MPI_DOUBLE, FIRST_RANK, TAG, MPI_COMM_WORLD, status);
	if (numberOfProcess % 2 != 0) {
		cout << "Process number " << numberOfProcess << " receive double number from process with rank 0: " << number << endl;
	}
	return number;
}
void secondProcess(){
	MPI_Status status;
	recieveNumberFromProcess(&status, SECOND_RANK);
	recieveStringFromProcess(&status, SECOND_RANK);
	recieveDoubleNumberFromProcess(&status, SECOND_RANK);
}
void thirdProcess(){
	MPI_Status status;
	recieveNumberFromProcess(&status, THIRD_RANK);
	recieveStringFromProcess(&status, THIRD_RANK);
	recieveDoubleNumberFromProcess(&status, THIRD_RANK);
}
void fourthProcess(){
	MPI_Status status;
	recieveNumberFromProcess(&status, FOURTH_RANK);
	recieveStringFromProcess(&status, FOURTH_RANK);
	recieveDoubleNumberFromProcess(&status, FOURTH_RANK);
}
//void runProcess(int rank) {
//	MPI_Status status;
//	recieveNumberFromProcess(&status, rank);
//	recieveStringFromProcess(&status, rank);
//	recieveDoubleNumberFromProcess(&status, rank);
//
//}
int main(int argc, char * argv[]) {
	MPI_Init(&argc, &argv); int rank, size;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	//MPI_Status status;
	switch (rank)
	{
	case FIRST_RANK:
		firstProcess();
		//runProcess(FIRST_RANK);
		break;
	case SECOND_RANK:
		secondProcess();
		//runProcess(SECOND_RANK);
		break;
	case THIRD_RANK:
		thirdProcess();
		//runProcess(THIRD_RANK);
		break;
	case FOURTH_RANK:
		fourthProcess();
		//runProcess(FOURTH_RANK);
		break;
	}
	MPI_Finalize();
	return 0;
}