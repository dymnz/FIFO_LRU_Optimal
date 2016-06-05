#pragma once
#include <iostream>

//#define DEBUG

class LRU {
public:
	int *storage;
	int *pageRefClock;
	int frameSize;
	int clock;
	int pageFault;

	LRU(int size) {
		frameSize = size;
		storage = new int[size];
		pageRefClock = new int[size];
		clock = pageFault = 0;

		for (int i = 0; i < size; i++) {
			pageRefClock[i] = storage[i] = -1;
		}
	}

	~LRU() {
		delete storage;
		delete pageRefClock;
	}

	void get(int reference) {
		int pageIndex = isPageStored(reference);

		if (pageIndex == -1)
		{
			int oldestEntryIndex = indexForOldestEntry();
			storage[oldestEntryIndex] = reference;
			pageRefClock[oldestEntryIndex] = clock;
			pageFault++;
		}
		else {
			pageRefClock[pageIndex] = clock;
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
	
	int isPageStored(int reference) {
		for (int i = 0; i < frameSize; i++) {
			if (storage[i] == reference)
				return i;
		}
		return -1;
	}

	void print() {
		for (int i = 0; i < frameSize; i++)
			std::cout << storage[i] << " ";
		std::cout << "\n";
		for (int i = 0; i < frameSize; i++)
			std::cout << pageRefClock[i] << " ";
	}
};
