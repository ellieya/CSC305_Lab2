
/*
Any search functions should be adjusted to run until initiatedPartitionCounter, not maximum...
NEED OVERLOADED ASSIGNMENT
*/

#pragma once
#include <cassert>
#include "MemPartition.h"


class Memory {
	MemPartition* partitions;
	int maximumPartitions;
	int totalMemorySize;
	int lastSetJobPartitionIndex = 0; //For next fit
	int initiatedPartitionCounter = 0;
	int unallocSize;

public:
	Memory();
	Memory(int, int totalMemorysSize);

	void setPartitionSizeAt(int index, int size);

	bool setPartitionJob(char, job);
	//POST: Returns true if job is successful. Otherwise, false.
	MemPartition getPartitionAt(int);
	int getTotalMemorySize();
	int getInitiatedPartitionCounter();
	int getUnallocSize();


	int firstFit(job);
	int nextFit(job);
	//int bestFit(int, job);
	//int worstFit(int, job);

	Memory& operator =(const Memory&);
};

Memory::Memory() {
}

Memory::Memory(int maximumPartitions, int totalMemorySize)
	:maximumPartitions(maximumPartitions), totalMemorySize(totalMemorySize)
{
	partitions = new MemPartition[maximumPartitions];
	unallocSize = totalMemorySize;
}

void Memory::setPartitionSizeAt(int index, int size)
{
	partitions[index] = MemPartition(size);
	initiatedPartitionCounter++;

	//Update unallocSize
	unallocSize -= size;

	//Assert no more than maximumPartitions
	assert(initiatedPartitionCounter <= maximumPartitions);
}

bool Memory::setPartitionJob(char algorithm, job _job)
{

	int partitionIndex;

	switch (algorithm) {
	case 'F':
		partitionIndex = firstFit(_job);
		break;
	case 'N':
		partitionIndex = nextFit(_job);
		break;
		/*
	case 'B':
		bestFit(size, _job);
		break;
	case 'W':
		worstFit(size, _job);
		break;

		*/
		//NO MATCH CASE
	}

	if (partitionIndex != -1) {
		_job.setStatus(true);
		partitions[partitionIndex].setJob(_job);
		return true;
	}

	return false;
}

MemPartition Memory::getPartitionAt(int partitionIndex) {
	return partitions[partitionIndex];
}

int Memory::getTotalMemorySize()
{
	return totalMemorySize;
}

inline int Memory::getInitiatedPartitionCounter()
{
	return initiatedPartitionCounter;
}

inline int Memory::getUnallocSize()
{
	return unallocSize;
}

int Memory::firstFit(job job_)
{
	//"Infinite" loop that stops when partition found or no available partitions fit
	int i = 0;
	while (true) {
		//"If partition is free, and if job can fit into partition..."
		if ((!partitions[i].getStatus()) && (partitions[i].getSize() >= job_.getSize()))
			return i;
		else if (i == initiatedPartitionCounter - 1)
			return -1;
		else
			i++;
	}
}


int Memory::nextFit(job job_)
{
	//"Infinite" loop that stops when partition found or no available partitions fit
	int initial = lastSetJobPartitionIndex;
	int i = lastSetJobPartitionIndex;
	while (true) {
		//"If partition is free, and if job can fit into partition..."
		if ((!partitions[i].getStatus()) && (partitions[i].getSize() >= job_.getSize())) {
			lastSetJobPartitionIndex = i;
			return i;
		}
		else if (i == (initiatedPartitionCounter - 1 + lastSetJobPartitionIndex) % initiatedPartitionCounter)
			return -1;
		else {
			i++;
			i %= initiatedPartitionCounter;
		}
	}
}

Memory & Memory::operator=(const Memory & incomingValue)
{
	maximumPartitions = incomingValue.maximumPartitions;
	totalMemorySize = incomingValue.totalMemorySize;
	lastSetJobPartitionIndex = incomingValue.lastSetJobPartitionIndex;
	initiatedPartitionCounter = incomingValue.initiatedPartitionCounter;

	partitions = new MemPartition[maximumPartitions];
	for (int i = 0; i < maximumPartitions; i++) {
		partitions[i] = incomingValue.partitions[i];
	}

	return *this;
}

