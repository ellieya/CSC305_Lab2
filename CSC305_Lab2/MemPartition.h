#pragma once
#include "job.h"

class MemPartition {
	static int idCounter;
	int id;
	int size;
	job job_;

public:
	MemPartition();
	MemPartition(int);

	int getId();
	int getSize();
	job getJob();
	void setJob(job);

};

MemPartition::MemPartition() {

}

MemPartition::MemPartition(int size)
	:size(size), id(idCounter)
{
	idCounter++;
}

int MemPartition::getId()
{
	return id;
}

int MemPartition::getSize()
{
	return size;
}

job MemPartition::getJob()
{
	return job_;
}

void MemPartition::setJob(job job_)
{
	this->job_ = job_;
}

int MemPartition::idCounter = 0;
