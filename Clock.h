#ifndef CLOCK_H
#define CLOCK_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include "BufferPool.h"


using namespace std;
struct ClockFrame {
    int pageID;
    bool referenceBit;

    ClockFrame(int id) : pageID(id), referenceBit(true) {}
};
static vector<ClockFrame> clockFrames;
static int clockPointer = 0;
static int tam = 0;


void initializeClock(int size_);
int addPageToClock(int pageID, BufferPool& bpool);
void removePageFromClock();
int findVictimPage(BufferPool& bpool);
void printClock();
int pagePos(int pageID);
void refTrue(int pos);
#endif

