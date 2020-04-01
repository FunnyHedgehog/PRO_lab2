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
	int numberFromConsole;
	MPI_Status status;
	cout << "Enter number for process 1: "<< endl;
	cin >> numberFromConsole;
	MPI_Send(&numberFromConsole, 1, MPI_INT, SECOND_RANK, TAG, MPI_COMM_WORLD);
	numberFromConsole *= 2;
	MPI_Send(&numberFromConsole, 1, MPI_INT, THIRD_RANK, TAG, MPI_COMM_WORLD);
	MPI_Send(&numberFromConsole, 1, MPI_INT, FOURTH_RANK, TAG, MPI_COMM_WORLD);
	
	//second task
	int buf_size = sizeof(TEXT); 
	vector<char> buff(BUFFER_SIZE + MPI_BSEND_OVERHEAD);
	char* buf = buff.data();
	MPI_Buffer_attach(buf, BUFFER_SIZE + MPI_BSEND_OVERHEAD);
	buf = (char*)TEXT;
	for (int i = 1; i <= 3; i++) {
		MPI_Bsend(&buf, BUFFER_SIZE, MPI_CHAR, i, TAG, MPI_COMM_WORLD);
	}
	cout << "Process number 0 send mesagges " << buf << endl;
	MPI_Buffer_detach(&buf, &buf_size);
	free(buf);
	
	//third task
	double number = 2.0;
	number *= 3;
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
void runProcess(int rank) {
	MPI_Status status;
	recieveNumberFromProcess(&status, rank);
	recieveStringFromProcess(&status, rank);
	recieveDoubleNumberFromProcess(&status, rank);

}
int main(int argc, char * argv[]) {
	MPI_Init(&argc, &argv); 
	int rank, size;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	if (rank == FIRST_RANK) {
		firstProcess();
	}
	else {
		runProcess(rank);
	}
	MPI_Finalize();
	return 0;
}