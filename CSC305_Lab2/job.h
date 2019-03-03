#pragma once

using namespace std;

class job {
	static int idCounter; //Used to keep count of unique ids to assign automatically
	int id;
	int size;
	bool status; //true if 'RUN', false if 'WAIT'; default is false
	int partitionId;

public:
	job();
	job(int, int);

	int getId();
	int getSize();
	bool getStatus();
	void setStatus(bool);
	int getPartitionID();
	void setPartitionID(int);

};

job::job() {

}

job::job(int size, int status) 
	:status(status), id(idCounter)
{
	idCounter++;
}

int job::getId()
{
	return id;
}

int job::getSize()
{
	return size;
}

bool job::getStatus()
{
	return status;
}

void job::setStatus(bool status)
{
	this->status = status;
}

int job::getPartitionID()
{
	return partitionId;
}

void job::setPartitionID(int partitionId)
{
	this->partitionId = partitionId;
}

int job::idCounter = 1;