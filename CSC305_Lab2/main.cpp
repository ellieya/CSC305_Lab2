#include "Header.h"

int main() {

	const int MAXIMUM_PARTITIONS = 5;

	//ArrayQueue<job> jobsList;
	Memory mainMemory;

	int intHolder = -1;

	/*
	SECTION #1: TAKE INPUT; change this to file input later
	*/
	//Input total memory size
	cout << "Input total memory size: ";
	cin >> intHolder;
	mainMemory = Memory(MAXIMUM_PARTITIONS, intHolder);

	//Input partition size & give ID to each of them
	cout << "Input partition sizes (up to 5, type in '0' when done): ";

	int i = 0;
	while (intHolder != 0) {
		cin >> intHolder;
		//check intHolder valid (not neg, not 0, etc.)
		if (intHolder > 0) {
			//if valid, intHolder into new partition
			mainMemory.setPartitionSizeAt(i, intHolder);
			i++;

			/*
			Exceptions to handle:
			Size total > memory size

			ALso, change it so that mainMemory partition array size is based on # inputted
			Additionally, inside memory, check if partition amt exceed maximum
			Also, the constructor should only take maximum and dynamically reallocate array based on new size..

			Maybe we can have program shout when total # is > maximum partitions.
			Either way, we need a total # initated thus far variable in Memory.
			*/
		}
	}

	for (int i = 0; i < MAXIMUM_PARTITIONS; i++)
		cout << mainMemory.getPartitionAt(i).getId() << " " << mainMemory.getPartitionAt(i).getSize() << endl;



	//Input job list


	/*
	SECTION #2: CALCULATE
	*/
	//Calculate initial memory allocation
	//Calculate memory waste


	/*
	SECTION #3: DISPLAY
	*/
	//Display initial memory allocation
	//Display memory waste
	//DIsplay jobs that could not be allocated and are on WAIT

	system("pause");
	return 0;
}