#include "Header.h"

const int MAXIMUM_PARTITIONS = 5;

int main();

void takeInput(ArrayQueue<job>&, Memory&);
//PRE: ArrayQueue and Memory objects must be defined and sent in as arguments
//POST: Update data inside ArrayQueue and Memory objects from file

void calculate(char algorithm, ArrayQueue<job>&, ArrayQueue<job>&, Memory&, int&);

void display(Memory main, Memory first, Memory best, Memory next, Memory worst, int, int, int, int);

int main() {

	ArrayQueue<job> jobsQueue;
	ArrayQueue<job> jobsWaitQueue;
	Memory mainMemory;

	takeInput(jobsQueue, mainMemory);

	Memory firstFitMemory;
	firstFitMemory = mainMemory; //Overloaded assignment operator can only be used after initialization
	ArrayQueue<job> firstFitJobsQueue = jobsQueue;
	ArrayQueue<job> firstFitJobsWaitQueue = jobsWaitQueue;
	int firstFitJobsWaitCount;

	Memory bestFitMemory;
	bestFitMemory = mainMemory; //Overloaded assignment operator can only be used after initialization
	ArrayQueue<job> bestFitJobsQueue = jobsQueue;
	ArrayQueue<job> bestFitJobsWaitQueue = jobsWaitQueue;
	int bestFitJobsWaitCount;

	Memory nextFitMemory;
	nextFitMemory = mainMemory; //Overloaded assignment operator can only be used after initialization
	ArrayQueue<job> nextFitJobsQueue = jobsQueue;
	ArrayQueue<job> nextFitJobsWaitQueue = jobsWaitQueue;
	int nextFitJobsWaitCount;

	Memory worstFitMemory;
	worstFitMemory = mainMemory; //Overloaded assignment operator can only be used after initialization
	ArrayQueue<job> worstFitJobsQueue = jobsQueue;
	ArrayQueue<job> worstFitJobsWaitQueue = jobsWaitQueue;
	int worstFitJobsWaitCount;

	calculate('F', firstFitJobsQueue, firstFitJobsWaitQueue, firstFitMemory, firstFitJobsWaitCount);
	calculate('B', bestFitJobsQueue, bestFitJobsWaitQueue, bestFitMemory, bestFitJobsWaitCount);
	calculate('N', nextFitJobsQueue, nextFitJobsWaitQueue, nextFitMemory, nextFitJobsWaitCount);
	calculate('W', worstFitJobsQueue, worstFitJobsWaitQueue, worstFitMemory, worstFitJobsWaitCount);

	display(mainMemory, firstFitMemory, bestFitMemory, nextFitMemory, worstFitMemory, firstFitJobsWaitCount, nextFitJobsWaitCount, bestFitJobsWaitCount, worstFitJobsWaitCount);

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

	/* For debugging purposes...
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

	/* For debugging purposes...
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
	
	
	/* For debugging purposes...
	//Check memory partition jobs data...
	cout << "ID | SIZE | WASTE | STATUS | PARTITION_ID" << endl;
	for (int i = 0; i < targetMemory.getInitiatedPartitionCounter(); i++) {
		//if (targetMemory.getPartitionAt(i).getStatus()) {
			cout << targetMemory.getPartitionAt(i).getJob().getId() << " "
			<< targetMemory.getPartitionAt(i).getJob().getSize() << " "
			<< targetMemory.getPartitionAt(i).getWaste() << " "
			<< targetMemory.getPartitionAt(i).getJob().getStatus() << " "
			<< targetMemory.getPartitionAt(i).getJob().getPartitionID() << " " << endl;
			//}
	}
	*/
	

	//Calculate jobs on wait
	jobsWaitCount = 0;
	while (!targetJobsWaitQueue.isEmpty()) {
		targetJobsWaitQueue.dequeue();
		jobsWaitCount++;
	}
}

void display(Memory mainMemory, Memory firstFitMemory, Memory bestFitMemory, Memory nextFitMemory, Memory worstFitMemory, int firstFitJobsWaitCount, int bestFitJobsWaitCount, int nextFitJobsWaitCount, int worstFitJobsWaitCount) {

	cout << "\n\t\t" << "First Fit\t" << "Best Fit\t" << "Next Fit\t" << "Worst Fit" << endl;

	for (int i = 0; i < mainMemory.getInitiatedPartitionCounter(); i++) {
		cout << "P#" << i + 1 << "\t\t";

		//Print first fit
		if (firstFitMemory.getPartitionAt(i).getStatus())
			cout << "J#" << firstFitMemory.getPartitionAt(i).getJob().getId() << "/" << firstFitMemory.getPartitionAt(i).getWaste();
		else
			cout << "-";

		cout << "\t\t";

		//Print best fit
		if (bestFitMemory.getPartitionAt(i).getStatus())
			cout << "J#" << bestFitMemory.getPartitionAt(i).getJob().getId() << "/" << bestFitMemory.getPartitionAt(i).getWaste();
		else
			cout << "-";
		cout << "\t\t";

		//Print next fit
		if (nextFitMemory.getPartitionAt(i).getStatus())
			cout << "J#" << nextFitMemory.getPartitionAt(i).getJob().getId() << "/" << nextFitMemory.getPartitionAt(i).getWaste();
		else
			cout << "-";
		cout << "\t\t";

		//Print worst fit
		if (worstFitMemory.getPartitionAt(i).getStatus())
			cout << "J#" << worstFitMemory.getPartitionAt(i).getJob().getId() << "/" << worstFitMemory.getPartitionAt(i).getWaste();
		else
			cout << "-";
		cout << "\t\t";

		cout << endl;
	}

	cout << "\n";

	//Print waste, not counting unallocated partition
	cout << "WASTE\t\t" << firstFitMemory.getTotalWaste('N') << "\t\t" << bestFitMemory.getTotalWaste('N') << "\t\t" << nextFitMemory.getTotalWaste('N') << "\t\t" << worstFitMemory.getTotalWaste('N') << endl;

	//Print waste, counting unallocated partition
	cout << "WASTE+UNALLOC\t" << firstFitMemory.getTotalWaste('U') << "\t\t" << bestFitMemory.getTotalWaste('U') << "\t\t" << nextFitMemory.getTotalWaste('U') << "\t\t" << worstFitMemory.getTotalWaste('U') << endl;

	//Print waiting jobs
	cout << "WAITING JOBS\t" << firstFitJobsWaitCount <<"\t\t" << bestFitJobsWaitCount << "\t\t" << nextFitJobsWaitCount << "\t\t" << worstFitJobsWaitCount << endl;

}