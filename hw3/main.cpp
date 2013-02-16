/*
* Rory Hayes
* main.cpp
* HW3
*/

#include <iostream>
#include <fstream>
#include <cmath>
#include <stdlib.h>
#include <vector>
#include "page.h"
#include "pageTable.h"

using namespace std;

// Prototypes
bool validateArgs();
void createPageTables();
void createMainMemory();

// Globals
char* programListFilename;
char* programTraceFilename;
int pageSize;
string praSelected;
string prePaging;
int mainMemorySize = 512;
int numOfPagesInMainMem;
vector<PageTable> pageTables;
vector<Page> mainMemory;

int main (int argc, char * const argv[]) {

    // Only proceed if we have the proper number of arguments
    if (argc == 6){
	
    // Store args
		programListFilename = argv[1];
    programTraceFilename = argv[2];
    pageSize = atoi(argv[3]);
		praSelected = argv[4];
		prePaging = argv[5];
		
		// Validate the arguments
		if(validateArgs()){
			createPageTables();
			createMainMemory();
		}
	}
	
	// Output error otherwise
	else{
		cout << "Improper number of arguments supplied.\n";
	}
	
    return 0;
}

void createMainMemory(){
  numOfPagesInMainMem = mainMemorySize/pageSize;
  int currentIndex = 0;
  for(int i=0;i<numOfPagesInMainMem;i++){
    //mainMemory.push_back(pageTables[i%pageTables.size()].getPage(currentIndex));
    //if(i%10==9)currentIndex++;
    
    mainMemory.push_back(pageTables[i%pageTables.size()].getPage(i/pageTables.size()));
    //cout << mainMemory[i].getID();
  }
  cout << "filled\n";
  for(int i=0;i<numOfPagesInMainMem;i++){
    for(int j=0;j<numOfPagesInMainMem;j++){
      if(i != j && mainMemory[i].getID() == mainMemory[j].getID()){
        cout << "GAHHHHHHHHHHHH " << mainMemory[i].getID() << "\n";
      }
    }
  }
  
}

void createPageTables(){
	// Create and open a filestream
  ifstream programfs;
  programfs.open(programListFilename);
  
  int startingLoc = 0;
  
  while(!programfs.eof()){
	  int programNumber;
	  int programWords;
	  programfs >> programNumber;
	  programfs >> programWords;
	  int neededMem = (programWords+pageSize)/pageSize;
	  PageTable currentPageTable(neededMem, startingLoc, pageSize, programNumber);
	  pageTables.push_back(currentPageTable);
	  startingLoc += neededMem;
	}
	
	/*for(int i=0;i<pageTables.size();i++){
	  cout << pageTables[i].getPages().back().getID() << "\n";
  }*/
}

bool validateArgs(){
	bool valid = true;
	
	// Create and open filestreams for the first two args
    ifstream programListStream;
    programListStream.open(programListFilename);
    if (programListStream.fail()){
        cout << "Could not open " << programListFilename << "\n";
		valid = false;
    }
	
	// Create and open filestreams for the first two args
	ifstream programTraceStream;
	programTraceStream.open(programTraceFilename);
	if (programTraceStream.fail()){
        cout << "Could not open " << programTraceFilename << "\n";
		valid = false;
    }

	// Ensure it's a positive power of 2
	double power = log10(pageSize)/log10(2);
	double intpart;
	double fractpart = modf(power, &intpart);
	if (fractpart != 0.0 || power < 1){
		cout << "Bad page size!\n";
		valid = false;
	}
	
	// Ensure it is one of the specified commands
	if(praSelected != "clock" && praSelected != "lru" && praSelected != "fifo"){
		cout << "Invalid Page Replacement Algorigthm!\n";
		valid = false;
	}
	
	// Validate 0 or 1
	if(prePaging != "0" && prePaging != "1"){
		cout << "Invalid prepaging / demand paging value!\n";
		valid = false;
	}
		
	return valid;
}
