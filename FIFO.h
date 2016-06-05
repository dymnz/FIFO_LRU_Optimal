#pragma once
#include <iostream>

class FIFO {
public:
	int *storage;
	int *pageRefClock;
	int frameSize;
	int clock;
	int pageFault;

	FIFO(int size) {
		frameSize = size;
		storage =  new int[size];
		pageRefClock = new int[size];
		clock = pageFault = 0;

		for (int i = 0; i < size; i++) {
			pageRefClock[i] = storage[i] = -1;
		}
	}

	~FIFO() {
		delete storage;
		delete pageRefClock;
	}

	void get(int reference) {
		if (!isPageStored(reference))
		{
			storage[indexForOldestEntry()] = reference;
			pageRefClock[indexForOldestEntry()] = clock;
			pageFault++;
		}
		clock++;

	}

private:
	int indexForOldestEntry() {
		int oldestEntryIndex = 0;
		int oldestEntryClock = 20160603;

		for (int i = 0; i < frameSize; i++) {
			if (pageRefClock[i] == -1)
				return i;
			if (pageRefClock[i] < oldestEntryClock) {
				oldestEntryIndex = i;
				oldestEntryClock = pageRefClock[i];
			}
		}
		return oldestEntryIndex;
	}
	
	bool isPageStored(int reference) {
		for (int i = 0; i < frameSize; i++) {
			if (storage[i] == reference)
				return true;
		}
		return false;
	}

	void print() {
		for (int i = 0; i < frameSize; i++)
			std::cout << storage[i] << " ";
		std::cout << "\n";
		for (int i = 0; i < frameSize; i++)
			std::cout << pageRefClock[i] << " ";
	}
};
