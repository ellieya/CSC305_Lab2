
/*
Any search functions should be adjusted to run until initiatedPartitionCounter, not maximum...

Need a function to calculate total memory waste

unAlloc + iteration through all to .getWaste

*/

#pragma once
#include <cassert>
#include "MemPartition.h"


class Memory {
	MemPartition* partitions;
	int maximumPartitions,
		totalMemorySize,
		lastSetJobPartitionIndex = 0, //For next fit
		initiatedPartitionCounter = 0,
		unallocSize;

	int firstFit(job);
	int bestFit(job);
	int nextFit(job);
	int worstFit(job);
	int calculateTotalWaste();
	int calculateTotalWasteWithUnalloc();

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
	int getTotalWaste(char);
	//If char argument is U, returns totalWaste of waste on used partitions + ununused partitions + unallocated memory
	//If char argument is anything else, returns only waste on used partitions

	Memory& operator = (const Memory&);
};

Memory::Memory() {
	//Intentionally blank, should not have any data if not initialized with other constructor
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
		
	case 'B':
		partitionIndex = bestFit(_job);
		break;
	case 'W':
		partitionIndex = worstFit(_job);
		break;
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

int Memory::getInitiatedPartitionCounter()
{
	return initiatedPartitionCounter;
}

int Memory::getUnallocSize()
{
	return unallocSize;
}

inline int Memory::getTotalWaste(char key)
{
	switch (key) {
	case 'U':
		return calculateTotalWasteWithUnalloc() + unallocSize;
		break;
	default:
		return calculateTotalWaste();
	}
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

int Memory::bestFit(job job_) {
	int indexHolder = -1,
		calculationHolder, //Used to hold calculation so as to avoid doing same calculation twice
		currentLowestCalcHolder = 32767; //Same as above

	//Iterate through entire array to find lowest possible waste
	for (int i = 0; i < initiatedPartitionCounter; i++) {
		calculationHolder = partitions[i].getSize() - job_.getSize();
		
		//If calculation is negative, that means it doesn't fit, skip this partition...
		if (calculationHolder >= 0) {
			//Otherwise, compare to current lowest, but only if it's not taken, not using <= b/c first to match gets priority
			if ((!partitions[i].getStatus()) && (calculationHolder < currentLowestCalcHolder)) {
				currentLowestCalcHolder = calculationHolder;
				indexHolder = i;
			}
		}
	}
	return indexHolder;
}

int Memory::worstFit(job job_) {
	int indexHolder = -1,
		calculationHolder, //Used to hold calculation so as to avoid doing same calculation twice
		currentHighestCalcHolder = -1; //Same as above, set to -1 so that 0 is allowed...

	//Iterate through entire array to find lowest possible waste
	for (int i = 0; i < initiatedPartitionCounter; i++) {
		calculationHolder = partitions[i].getSize() - job_.getSize();

		//If calculation is negative, that means it doesn't fit, skip this partition...
		if (calculationHolder >= 0) {
			//Otherwise, compare to current highest, but only if it's not taken, not using >= b/c first to match gets priority
			if ((!partitions[i].getStatus()) && (calculationHolder > currentHighestCalcHolder)) {
				currentHighestCalcHolder = calculationHolder;
				indexHolder = i;
			}
		}
	}

	return indexHolder;

}

int Memory::calculateTotalWaste()
{
	int sum = 0;
	for (int i = 0; i < initiatedPartitionCounter; i++) {
		if (partitions[i].getStatus())
			sum += partitions[i].getWaste();
	}

	return sum;
}

int Memory::calculateTotalWasteWithUnalloc()
{
	int sum = 0;
	for (int i = 0; i < initiatedPartitionCounter; i++) {
		sum += partitions[i].getWaste();
	}

	return sum;
}

Memory& Memory::operator = (const Memory & incomingValue)
{
	MemPartition* holder;

	maximumPartitions = incomingValue.maximumPartitions;
	totalMemorySize = incomingValue.totalMemorySize;
	lastSetJobPartitionIndex = incomingValue.lastSetJobPartitionIndex;
	initiatedPartitionCounter = incomingValue.initiatedPartitionCounter;

	holder = new MemPartition[maximumPartitions];
	for (int i = 0; i < maximumPartitions; i++) {
		holder[i] = incomingValue.partitions[i];
	}

	partitions = holder;

	return *this;
}

