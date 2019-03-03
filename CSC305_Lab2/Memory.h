#pragma once
#include "MemPartition.h"


class Memory {
	MemPartition* partitions;
	int maximumPartitions;
	int totalMemorySize;
	int lastSetJobPartitionIndex; //For next fit
	int initiatedPartitionCounter;

public:
	Memory();
	Memory(int, int totalMemorysSize);

	void setPartitionSizeAt(int index, int size);

	void setPartitionJob(char, job);
	MemPartition getPartitionAt(int);

	int firstFit(job);
	//int nextFit(int, job);
	//int bestFit(int, job);
	//int worstFit(int, job);
};

Memory::Memory() {
}

Memory::Memory(int maximumPartitions, int totalMemorySize)
	:maximumPartitions(maximumPartitions), totalMemorySize(totalMemorySize)
{
	partitions = new MemPartition[maximumPartitions];

}

void Memory::setPartitionSizeAt(int index, int size)
{
	partitions[index] = MemPartition(size);
}

void Memory::setPartitionJob(char algorithm, job _job)
{

	int partitionIndex;

	switch (algorithm) {
	case 'F':
		partitionIndex = firstFit(_job);
		break;
		/*
	case 'N':
		nextFit(size, _job);
		break;
	case 'B':
		bestFit(size, _job);
		break;
	case 'W':
		worstFit(size, _job);
		break;

		*/
	}

	partitions[partitionIndex].setJob(_job);
}

MemPartition Memory::getPartitionAt(int partitionIndex) {
	return partitions[partitionIndex];
}

int Memory::firstFit(job job_)
{
	//"Infinite" loop that stops when partition found or no partitions fit
	int i = 0;
	while (true) {
		if (partitions[i].getSize() >= job_.getSize())
			return i;
		else if (i == maximumPartitions - 1)
			return -1;
		else
			i++;
	}
}

