#ifndef PAGE_H
#define PAGE_H

class Page
{

  private:
    int owner;
    int id;
    int relId;

  public:
    unsigned int timestamp;
    bool useBit;

    Page(int own, int identifier, int rel){
      owner = own;
      id = identifier;
      relId = rel;
      timestamp = 0;
      useBit = true;
    }

    int getOwner(){
      return owner;
    }

    int getID(){
      return id;
    }

    int getRelativeID(){
      return relId;
    }

    void swap(Page pageToSwap){
      owner = pageToSwap.owner;
      id = pageToSwap.id;
      relId = pageToSwap.relId;
      useBit = pageToSwap.useBit;
    }
};

#endif
