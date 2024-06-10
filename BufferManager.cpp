#include "BufferManager.h"

#include "Frame.h"
#include "Page.h"
#include "Clock.h"


// configura LRU (Least Recently Used) con el número total de frames.

BufferManager::BufferManager(int numFrames)
{
    this->numFrames = numFrames;
    this->bpool = BufferPool(numFrames);
}

/*

Carga una página desde el disco al buffer pool.
Si todos los frames están ocupados, selecciona un frame para reemplazar usando el algoritmo LRU
guarda la página reemplazada en el disco y luego carga la nueva página en ese frame.
*/

void BufferManager::loadPageFromDisk(string blockPath, int pageID, char _mode, char _rplMode)
{
    bool mode = _mode == 'W' ? true : false;
    //bool rplMode = _rplMode == 'U' ? true : false;

    if (bpool.isPageLoaded(pageID))
    {
        cout << "La pagina ya esta cargada" << "\n";
        if (_mode == 'w')   // Sirve para poner el Dirty
        {
            bpool.getFrame(bpool.getFrameId(pageID)).setDirtyFlag(true);

        }
       /* else
        {
            bpool.getFrame(bpool.getFrameId(pageID)).setDirtyFlag(true);
        }*/
        bpool.modifyPinInExistingFrame(pageID, 'i');

        if (_rplMode == 'U')
            bpool.incrementHistory();
        /*
        if (_rplMode == 'C') {
            cout << " ESTA BIEN EL IF";
            int pos = pagePos(pageID);
            clockFrames[pos].referenceBit = true;
        }


        cout << "ESTA BIEN EL IF";
*/
        if (_rplMode == 'C')
        {
            cout << "ESTA BIEN EL IF\n";
            int pos = pagePos(pageID);
            if (pos != -1)
            {
             refTrue(pos);
            }
            else
            {
                cout << "Page not found in clock frames.\n";
            }
        }

        bpool.printTableFrame();
        printClock();

        return;
    }

    if (_rplMode == 'U')
    {
        bpool.LRU();

        Page tempPage;
        tempPage.setName(blockPath);
        tempPage.setPageId(pageID);

        int idFree = bpool.findFreeFrame();
        Frame tempFrame(idFree);
        tempFrame.setDirtyFlag(mode);
        tempFrame.setPage(tempPage);
        tempFrame.incrementPinCount();

        bpool.setPageInFrame(idFree, pageID, tempFrame);
        bpool.incrementHistory();
        bpool.printTableFrame();
    }
    else if (_rplMode == 'C')
    {
        // Replace LRU with Clock
        /* int victimFrame = findVictimPage();
         clockFrames[victimFrame].pageID = pageID;
         clockFrames[victimFrame].referenceBit = true;
         clockPointer = (victimFrame + 1) % size;*/

        int victimFrame = addPageToClock(pageID, bpool);

        Page tempPage;
        tempPage.setName(blockPath);
        tempPage.setPageId(pageID);

        Frame tempFrame(victimFrame);
        tempFrame.setDirtyFlag(mode);
        tempFrame.setPage(tempPage);
        tempFrame.incrementPinCount();

        bpool.setPageInFrame(victimFrame, pageID, tempFrame);
        // bpool.incrementHistory();
        bpool.printTableFrame();
        printClock();
    }
}

void BufferManager::killProcess(int pageID)
{
    bpool.modifyPinInExistingFrame(pageID, 'k');
    // bpool.incrementHistory();
    bpool.printTableFrame();
}

void BufferManager::printTableFrame()
{
    bpool.printTableFrame();
}
