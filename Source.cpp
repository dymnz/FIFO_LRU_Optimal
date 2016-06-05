#include <iostream>
#include <iomanip>  
#include <ctime>
#include "FIFO.h"
#include "LRU.h"
#include "Optimal.h"

#define MIN_FRAME 1
#define MAX_FRAME 7
#define TYPE 10
#define REF_START 0
#define PAGE_NUM 200
#define TEST_TIME 100
#define VERBOSE false
#define WITH_OPTIMAL true

#if WITH_OPTIMAL
	int pfOptimal[MAX_FRAME - MIN_FRAME + 1] = { 0 };
#endif

int referenceString[PAGE_NUM];
int pfLRU[MAX_FRAME - MIN_FRAME + 1] = { 0 }, pfFIFO[MAX_FRAME - MIN_FRAME + 1] = { 0 };

int main(int argc, char* argv[])
{
	srand(time(NULL));
	for (int t = 0; t < TEST_TIME; t++) {
		for (int i = 0; i < PAGE_NUM; i++) {
			referenceString[i] = rand() % TYPE;
		}

		for (int i = MIN_FRAME; i <= MAX_FRAME; i++)
		{
			FIFO fifo(i);
			for (int r = 0; r < PAGE_NUM; r++)
				fifo.get(referenceString[r]);
			pfFIFO[i - MIN_FRAME] += fifo.pageFault;
#if VERBOSE
			std::cout << "FIFO with " << i << " frames has " << fifo.pageFault << " page faults \n";
#endif 
		}
		for (int i = MIN_FRAME; i <= MAX_FRAME; i++)
		{
			LRU lru(i);
			for (int r = 0; r < PAGE_NUM; r++)
				lru.get(referenceString[r]);
			pfLRU[i - MIN_FRAME] += lru.pageFault;
#if VERBOSE
			std::cout << "LRU with " << i << " frames has " << lru.pageFault << " page faults \n";
#endif
		}

#if WITH_OPTIMAL
		for (int i = MIN_FRAME; i <= MAX_FRAME; i++)
		{
			Optimal optimal(i, PAGE_NUM, TYPE, REF_START);
			optimal.loadReferenceString(referenceString);
			for (int r = 0; r < PAGE_NUM; r++)
				optimal.tick();
			pfOptimal[i - MIN_FRAME] += optimal.pageFault;
#if VERBOSE
			std::cout << "Optimal with " << i << " frames has " << optimal.pageFault << " page faults \n";
#endif
		}
#endif
	}
	std::cout << "Total page faults: \n";
	std::cout << "For\t\t";
	for (int i = MIN_FRAME; i <= MAX_FRAME; i++) {
		std::cout.width(10); std::cout << i;
	}
	std::cout << " frames\n";

	std::cout << "FIFO:\t\t";

	for (int i = MIN_FRAME; i <= MAX_FRAME; i++) {
		std::cout.width(10); std::cout << std::right << pfFIFO[i - MIN_FRAME];
	}
	std::cout << "\nLRU:\t\t";
	for (int i = MIN_FRAME; i <= MAX_FRAME; i++) {
		std::cout.width(10); std::cout << std::right << pfLRU[i - MIN_FRAME];
	}

#if WITH_OPTIMAL
	std::cout << "\nOptimal:\t";
	for (int i = MIN_FRAME; i <= MAX_FRAME; i++) {
		std::cout.width(10); std::cout << std::right << pfOptimal[i - MIN_FRAME];
	}
#endif
	
	char shit;
	std::cin >> shit;
	return 0;
}

