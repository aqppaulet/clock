#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "BufferPool.h"
#include "Frame.h"

using namespace std;

BufferPool::BufferPool() {
    this->numFrames = 0;
    this->capacity = 720;
}

BufferPool::BufferPool(int numFrames) {
    this->numFrames = numFrames;
    this->capacity = 720;

    for (int i = 0; i < this->numFrames; i++) {
        Frame frame(i);
        this->frames.push_back(frame);
        page_table[i] = -1;
    }
}

vector<Frame> &BufferPool::getFrames() { return frames; }

Frame &BufferPool::getFrame(int frameID) {
    for (int i = 0; i < frames.size(); i++) {
        if (frames[i].getframeID() == frameID) {
            return frames[i];
        }
    }
    throw std::runtime_error("Frame not found");
}

void BufferPool::modifyPinInExistingFrame(int pageID, char flag) {
    for (int i = 0; i < frames.size(); i++) {
        if (page_table[frames[i].getframeID()] == pageID) {
            if (flag == 'i') {
                frames[i].incrementPinCount();
            } else if (flag == 'k' && frames[i].getPinCount() > 0) {
                frames[i].decrementPinCount();
            }
        }
    }
}

void BufferPool::setPage(Frame &frame, int frameID) {
    if (frameID >= 0 && frameID < numFrames) {
        frames[frameID] = frame;
        page_table[frameID] = 1;
    } else {
        cout << "SetP: Frame ID fuera de rango" << endl;
        return;
    }
    cout << "\nPagina [" << frameID << "] cargada correctamente." << endl;
}

void BufferPool::printPage(int frameID) {
    if (frameID >= 0 && frameID < numFrames) {
        if (page_table[frameID] == 1) {
            Frame &frame = frames[frameID];
            cout << "Frame ID: " << frameID << endl;
            cout << "Dirty Flag: " << "(frame.isDirty() ? \" true \" : \" false \")" << endl;
            cout << "Pin Count: " << "frame.getPinCount()" << endl;
            cout << "Page Capacity: " << "frame.getPage().getPageCapacity()" << endl;
            cout << "Page Content: " << "frame.getPage().getPageContent() " << endl;
        } else {
            cout << "Frame ID: " << frameID << " no contiene una página." << endl;
        }
    } else {
        cout << "Frame ID fuera de rango." << endl;
    }
}

void BufferPool::addRecord(int frameID, string record) {
    if (frameID >= 0 && frameID < numFrames) {
        if (page_table[frameID] == 1) {
            Frame &frame = frames[frameID];
            // frame.getPage().addRecord(record);
            cout << "Registro agregado correctamente." << endl;
        } else {
            cout << "Frame ID: " << frameID << " no contiene una página." << endl;
        }
    } else {
        cout << "Frame ID fuera de rango." << endl;
    }
}

void BufferPool::deleteRecord(int frameID, string record) {
    if (frameID >= 0 && frameID < numFrames) {
        if (page_table[frameID] == 1) {
            Frame &frame = frames[frameID];
            // frame.getPage().deleteRecord(record);
            cout << "Registro eliminado correctamente." << endl;
        } else {
            cout << "Frame ID: " << frameID << " no contiene una página." << endl;
        }
    } else {
        cout << "Frame ID fuera de rango." << endl;
    }
}

void BufferPool::pageIsDirty(int pageID) {
    if (pageID >= 0 && pageID < numFrames) {
        if (page_table[pageID] == 1) {
            Frame &frame = frames[pageID];
            // return frame.isDirty();
            return;
        } else {
            cout << "Frame ID: " << pageID << " no contiene una página." << endl;
            return;
        }
    } else {
        cout << "Frame ID fuera de rango." << endl;
        return;
    }
}

void BufferPool::freeFrame(int frameID) {
    if (frameID >= 0 && frameID < numFrames) {
        frames[frameID] = Frame(frameID);
        page_table[frameID] = -1;
        cout << "Frame [" << frameID << "] liberado correctamente." << endl;
    } else {
        cout << "Frame ID fuera de rango." << endl;
    }
}

int BufferPool::findFreeFrame() {
    for (auto it = page_table.begin(); it != page_table.end(); ++it) {
        if (it->second == -1) return it->first;
    }

    /*Si retorna -2 es porque todos los frames ya tienen cargado una pagina*/
    return -2;
}

void BufferPool::setPageInFrame(int frameID, int pageID, Frame &frame) {
    if (frameID >= 0 && frameID < numFrames) {
        frames[frameID] = frame;
        page_table[frameID] = pageID;
    } else {
        cout << "SetP: Frame ID fuera de rango" << endl;
        return;
    }

    // printTableFrame();

    cout << "\nPagina [" << frameID << "] cargada Correctamente\n"
         << endl;
    setHistory(pageID);
}

bool BufferPool::isPageLoaded(int pageID) {
    for (const auto &pair : page_table) {
        if (pair.second == pageID) {
            return true;
        }
    }
    return false;
}

bool BufferPool::isPagePinned(int pageID)
{
    for (const auto &pair : page_table)
    {
        if (pair.second == pageID)
        {
            if (frames[pair.first].getPinCount() > 0)
            {
                return true;
            }
        }

    }
    return false;
}

int BufferPool::getFrameId(int pageID) {
    for (const auto &pair : page_table) {
        if (pair.second == pageID) {
            return pair.first;
        }
    }
    return -1;
}

void BufferPool::printTableFrame() {
    cout << setw(10) << "Frame Id" << "\t"
         << setw(10) << "Page Id" << "\t"
         << setw(10) << "Dirty Bit" << "\t"
         << setw(10) << "Pin Count" << "\t"
         << setw(10) << "Last Used" << endl;

    for (int i = 0; i < frames.size(); i++) {
        cout << setw(10) << frames[i].getframeID() << "\t"
             << setw(10) << frames[i].getPage().getPageId() << "\t"
             << setw(10) << (frames[i].isDirty() ? "Yes" : "No") << "\t"
             << setw(10) << frames[i].getPinCount() << "\t"
             << setw(10) << history[frames[i].getframeID()] << "\t" << endl;
    }
}

void BufferPool::setHistory(int pageID) {
    history[pageID] = 0;
}

void BufferPool::incrementHistory() {
    for (auto &pair : page_table) {
        if (pair.second != -1) history[pair.first]++;
    }
}

void BufferPool::LRU() {
    cout << "LRU" << endl;
    /*
    En el unordered_map history, se guardará el frameID y el tiempo en el que fue accedido. Atraves de las veces que se llame en general con +1.
    Ahora se buscara cual frame tiene mas tiempo en estar. Luego se revisara si el pin count esta en cero y ahi recien se llamara la funcion freeFrame.
    */
    int max = 0;
    int lastUsedFrame = 0;
    for (auto &pair : history) {
        if (pair.second > max) {
            max = pair.second;
            lastUsedFrame = pair.first;
        }
    }
    if (frames[lastUsedFrame].getPinCount() == 0) { //FUNCIONA CUANDO EL PINCOUNT SEA 0 DEL QUE TENGA MAS ALTO LAST COUNT
        history[lastUsedFrame] = 0;
        freeFrame(lastUsedFrame);
    } else {
        cout << "No se puede liberar el frame " << lastUsedFrame << " porque tiene un pin count mayor a 0" << endl;
    }
}
