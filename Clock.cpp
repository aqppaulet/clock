#include "Clock.h"
#include "BufferPool.h"



void initializeClock(int size_) {
    tam = size_;
    clockFrames.clear();
    clockFrames.resize(tam, ClockFrame(-1));
    clockPointer = 0;
}

int addPageToClock(int pageID, BufferPool& bpool) {
    int victimPage = findVictimPage(bpool);
    clockFrames[victimPage].pageID = pageID;
    clockFrames[victimPage].referenceBit = true;
    clockPointer = (victimPage + 1) % tam;
    return victimPage;
}

void removePageFromClock() {
    // This function is not used directly but can be implemented if needed.
}

int findVictimPage(BufferPool& bpool) {
    int pageTemp;

    while (true)
    {
        pageTemp = clockFrames[clockPointer].pageID;
        cout << "pageTemp" << pageTemp<< endl;

        if (!bpool.isPagePinned(pageTemp))
        {
            cout << "NO ES PINEADO" << pageTemp<< endl;

            if (!clockFrames[clockPointer].referenceBit)
            {
                return clockPointer;
            }
            clockFrames[clockPointer].referenceBit = false;

            clockPointer = (clockPointer + 1) % tam;
        }
        else if (bpool.isPagePinned(pageTemp))
        {
            clockPointer = (clockPointer + 1) % tam;
        }
    }
}

void printClock() {
    for (int i = 0; i < tam; ++i) {
        cout << "Frame " << i << ": PageID = " << clockFrames[i].pageID
             << ", Reference Bit = " << clockFrames[i].referenceBit << endl;
    }
}


int pagePos(int pageID) {
    for (size_t i = 0; i < tam; ++i) {
        if (clockFrames[i].pageID == pageID) {
            return i;
        }
    }
    return -1; // Return -1 if the page is not found
}

void refTrue(int pos) {
    clockFrames[pos].referenceBit = true;

    }
