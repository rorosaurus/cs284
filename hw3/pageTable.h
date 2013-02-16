#ifndef PAGETABLE_H
#define PAGETABLE_H

#include <vector>
#include "page.h"

using namespace std;

class PageTable
{

  private:
    vector<Page> pages;
    int memoryNeeded;
    int pageSize;
    
  public:
    PageTable(int memNeeded, int startLoc, int pSize, int owner){
      memoryNeeded = memNeeded;
      pageSize = pSize;
      
      int finalLoc = startLoc + memoryNeeded;
      int relLoc = 0;
      for(int i=startLoc; i<finalLoc; i++){
        Page currentPage(owner, i, relLoc);
        pages.push_back(currentPage);
        relLoc++;
      }
    }
    
    Page getPage(int location){
      return pages[location];
    }
    
    vector<Page> getPages(){
      return pages;
    }

    int getNumberOfPages(){
      return pages.size();
    }
};

#endif
