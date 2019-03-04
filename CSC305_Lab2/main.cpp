/*
Exceptions to handle:
// Also, the constructor should only take maximum and dynamically reallocate array based on new size... (eh...)
*/

#include "Header.h"

const int MAXIMUM_PARTITIONS = 5;

int main();

void takeInput(ArrayQueue<job>&, Memory&);
//PRE: ArrayQueue and Memory objects must be defined and sent in as arguments
//POST: Update data inside ArrayQueue and Memory objects from file

void calculate(char, ArrayQueue<job>&, ArrayQueue<job>&, Memory&, int&);

void display(Memory mainMemory, Memory firstFitMemory, Memory);

int main() {

	ArrayQueue<job> jobsQueue;
	ArrayQueue<job> jobsWaitQueue;
	Memory mainMemory;

	takeInput(jobsQueue, mainMemory);

	Memory firstFitMemory = mainMemory;
	ArrayQueue<job> firstFitJobsQueue = jobsQueue;
	ArrayQueue<job> firstFitJobsWaitQueue = jobsWaitQueue;
	int firstFitJobsWaitCount;

	Memory nextFitMemory = mainMemory;
	ArrayQueue<job> nextFitJobsQueue = jobsQueue;
	ArrayQueue<job> nextFitJobsWaitQueue = jobsWaitQueue;
	int nextFitJobsWaitCount;

	Memory bestFitMemory = mainMemory;
	ArrayQueue<job> bestFitJobsQueue = jobsQueue;
	ArrayQueue<job> bestFitJobsWaitQueue = jobsWaitQueue;
	int bestFitJobsWaitCount;

	Memory worstFitMemory = mainMemory;
	ArrayQueue<job> worstFitJobsQueue = jobsQueue;
	ArrayQueue<job> worstFitJobsWaitQueue = jobsWaitQueue;
	int worstFitJobsWaitCount;

	calculate('F', firstFitJobsQueue, firstFitJobsWaitQueue, firstFitMemory, firstFitJobsWaitCount);
	calculate('N', nextFitJobsQueue, nextFitJobsWaitQueue, nextFitMemory, nextFitJobsWaitCount);
	//calculate('B', bestFitJobsQueue, bestFitJobsWaitQueue, bestFitMemory, bestFitJobsWaitCount);
	//calculate('W', worstFitJobsQueue, worstFitJobsWaitQueue, worstFitMemory, worstFitJobsWaitCount);

	/*
	SECTION #3: DISPLAY
	*/
	//Display initial memory allocation
	//Display memory waste
	//DIsplay jobs that could not be allocated and are on WAIT

	display(mainMemory, firstFitMemory, nextFitMemory);

	system("pause");
	return 0;
}

void takeInput(ArrayQueue<job>& jobsQueue, Memory& mainMemory) {
	int
		intHolder,
		sum = 0;

	job jobHolder;

	string fileName;
	ifstream fin;

/*
Section #1: Open memory/memory partitions
*/
	//Open memory sizes file
	cout << "Input memory & memory partitions file name: ";
	cin >> fileName;

	fin.open(fileName + ".txt");
	assert(!fin.fail());

	//Input total memory size

	fin >> intHolder;
	mainMemory = Memory(MAXIMUM_PARTITIONS, intHolder);

	//Input partition size & give ID to each of them

	int i = 0;
	while (!fin.eof()) {
		fin >> intHolder;
		//check intHolder valid (not neg, not 0, etc.)
		if (intHolder > 0) {
			//if valid, intHolder into new partition
			mainMemory.setPartitionSizeAt(i, intHolder);
			i++;

			sum += intHolder;

			//If size total > memory size total, abort program
			assert(sum <= mainMemory.getTotalMemorySize());
		}
	}

	/*
	//Check mainMemory data is correct...
	for (int i = 0; i < mainMemory.getInitiatedPartitionCounter(); i++)
		cout << mainMemory.getPartitionAt(i).getId() << " " << mainMemory.getPartitionAt(i).getSize() << endl;
	*/

	fin.close();


/*
Section #2: Open jobs
*/
	//Open memory sizes file
	cout << "Input jobs list file name: ";
	cin >> fileName;

	fin.open(fileName + ".txt");
	assert(!fin.fail());

	//Input job list
	while (!fin.eof()) {
		fin >> intHolder;
		jobHolder = job(intHolder);
		jobsQueue.enqueue(jobHolder);
	}

	/*
	//Check queue data is correct...
	while (!jobsQueue.isEmpty()) {
		cout << jobsQueue.peekFront().getId() << " " << jobsQueue.peekFront().getSize() << endl;
		jobsQueue.dequeue();
	}
	*/

}

void calculate(char algorithm, ArrayQueue<job>& targetJobsQueue, ArrayQueue<job>& targetJobsWaitQueue, Memory& targetMemory, int& jobsWaitCount) {

	//Calculate initial memory allocation (first fit)
	while (!targetJobsQueue.isEmpty()) {
		if (!targetMemory.setPartitionJob(algorithm, targetJobsQueue.peekFront())) {
			targetJobsWaitQueue.enqueue(targetJobsQueue.peekFront());
			targetJobsQueue.dequeue();
		}
		else {
			targetJobsQueue.dequeue();
		}
 	}
	
	/*
	//Check memory partition jobs data...
	cout << "ID | SIZE | WASTE | STATUS | PARTITION_ID" << endl;
	for (int i = 0; i < mainMemory.getInitiatedPartitionCounter(); i++) {
		if (mainMemory.getPartitionAt(i).getStatus()) {
			cout << mainMemory.getPartitionAt(i).getJob().getId() << " "
			<< mainMemory.getPartitionAt(i).getJob().getSize() << " "
			<< mainMemory.getPartitionAt(i).getWaste() << " "
			<< mainMemory.getPartitionAt(i).getJob().getStatus() << " "
			<< mainMemory.getPartitionAt(i).getJob().getPartitionID() << " " << endl;
		}
	}
	*/

	//Calculate jobs on wait
	jobsWaitCount = 0;
	while (!targetJobsWaitQueue.isEmpty()) {
		targetJobsWaitQueue.dequeue();
		jobsWaitCount++;
	}
}

void display(Memory mainMemory, Memory firstFitMemory, Memory nextFitMemory) {

	cout << "\t" << "First Fit\t" << "Next Fit\t" << "Best Fit\t" << "Worst Fit" << endl;

	for (int i = 0; i < mainMemory.getInitiatedPartitionCounter(); i++) {
		cout << "P#" << i + 1 << "\t";

		//Print first fit
		if (firstFitMemory.getPartitionAt(i).getStatus())
			cout << "J#" << firstFitMemory.getPartitionAt(i).getJob().getId() << "/" << firstFitMemory.getPartitionAt(i).getWaste();
		else
			cout << "-";
		cout << "\t";

		/*
		//Print next fit
		if (nextFitMemory.getPartitionAt(i).getStatus())
			cout << "J#" << nextFitMemory.getPartitionAt(i).getJob().getId() << "/" << nextFitMemory.getPartitionAt(i).getWaste();
		else
			cout << "-";
		cout << "\t";
		*/


		//Print best fit

		//Print worst fit

		cout << endl;
	}

}