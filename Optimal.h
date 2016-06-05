#pragma once
#include <iostream>

//#define DEBUG

class Optimal {
	public:
		int *storage;
		int *pageRefStep;
		int *referenceString;
		int frameSize, referenceStringSize, referenceStringTypes, referenceStartingIndex;
		int pageFault, clock, storedPage;

		Optimal(int frameSize, int referenceStringSize, int referenceStringTypes, int referenceStartingIndex) {
			this->frameSize = frameSize;
			this->referenceStringSize = referenceStringSize;
			this->referenceStringTypes = referenceStringTypes;
			this->referenceStartingIndex = referenceStartingIndex;

			storage = new int[frameSize];
			pageRefStep = new int[referenceStringTypes];
			referenceString = new int[referenceStringSize];

			pageFault = clock = storedPage = 0;

			for (int i = 0; i < frameSize; i++) {
				storage[i] = -1;
			}
		}

		void loadReferenceString(int refStr[]) {
			for (int i = 0; i < referenceStringSize; i++)
				referenceString[i] = refStr[i];
		}

		~Optimal() {
			delete storage;
			delete pageRefStep;
			delete referenceString;
		}

		void tick() {
			int reference = referenceString[clock];
			if (!isPageStored(reference))
			{
				
				pageFault++;
				if(clock+1 < referenceStringSize)
				{ 
					int index = indexOfOptimalReplacement();
					storage[index] = reference;
					storedPage++;
				}
			}

			clock++;
		}

	private:
		int indexOfOptimalReplacement() {
			if (storedPage < frameSize) {
				return storedPage;
			}

			for (int i = 0; i < referenceStringTypes; i++) {
				pageRefStep[i] = referenceStringSize + 1;
			}

			for (int i = clock+1; i < referenceStringSize; i++) {
				if(pageRefStep[referenceString[i] - referenceStartingIndex] == referenceStringSize + 1)
					pageRefStep[referenceString[i] - referenceStartingIndex] = i;
			}
#ifdef DEBUG
			std::cout << "pageRefStep: ";
			for (int i = 0; i < referenceStringTypes; i++) {
				std::cout << pageRefStep[i] << ",";
			}
			std::cout << "\nstorage:";
			for (int i = 0; i < frameSize; i++) {
				std::cout << storage[i] << ",";
			}
			std::cout << "\n";
#endif // DEBUG

			int optimalPage, optimalStep;
			optimalPage = optimalStep = -1;
			for (int i = 0; i < referenceStringTypes; i++) {
				if(!isPageStored(i + referenceStartingIndex))
					continue;

				if (optimalStep < pageRefStep[i]) {
					optimalStep = pageRefStep[i];
					optimalPage = i;
				}
			}
			//std::cout <<"\n"<< optimalPage + referenceStartingIndex << " " << clock << " \n";
			for (int i = 0; i < frameSize; i++) {
			//	std::cout << storage[i] << " " << optimalPage + referenceStartingIndex << " " << clock << " \n";
				if (storage[i] == optimalPage + referenceStartingIndex)
					return i;
			}
			char shit;
			std::cout << "error\n";
			std::cin >> shit;
			exit(69);
		}

		bool isPageStored(int reference) {
			for (int i = 0; i < frameSize; i++) {
				if (storage[i] == reference)
					return true;
			}
			return false;
		}

	};
