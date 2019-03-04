#pragma once
#include "job.h"

class MemPartition {
	static int idCounter;
	int id;
	int size;
	int waste;
	job job_;
	bool status; //true if job is in, false if job is not, default is false

public:
	MemPartition();
	MemPartition(int);

	int getId();
	int getSize();
	int getWaste();
	job getJob();
	void setJob(job);
	bool getStatus();

};

MemPartition::MemPartition() {

}

MemPartition::MemPartition(int size)
	:size(size), id(idCounter), status(false)
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

int MemPartition::getWaste()
{
	return waste;
}

job MemPartition::getJob()
{
	return job_;
}

void MemPartition::setJob(job job_)
{
	//Set job's partition ID
	job_.setPartitionID(id);

	//Flip status
	status = true;

	//Update waste
	waste = size - job_.getSize();

	this->job_ = job_;
}

bool MemPartition::getStatus()
{
	return status;
}

int MemPartition::idCounter = 1;
