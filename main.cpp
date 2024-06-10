#include <iostream>

#include "BufferManager.h"
#include "Page.h"
#include "Clock.h"


/* Erick Malcoaccha*/
int main() {
    BufferManager BfManger(4);
     initializeClock(4);

    while (true) {
        int instruction;
        int cont = 0;//Para preguntar una sola vez el metodode reemplazo
        cout << "================================================================"
             << endl;
        cout << "1. Load page " << endl;
        cout << "2. UpdatePage " << endl;
        cout << "3. Kill Process " << endl;
        cout << "4. Save page " << endl;
        cout << "5. Print Table Frame" << endl;
        cout << "6. Exit" << endl;
        cout << "================================================================"
             << endl;
        cout << "Option: ";
        cin >> instruction;

        if (instruction == 6) {
            break;
        } else if (instruction == 1) {

            cout << "Choose LRU (U)/ Clock(C) "<< endl;
            char rplMode;
            cout << "rplMode: ";
            cin >> rplMode;
            cont++;

            cout << "En que modo desea cargar" << endl;
            cout << "(L) Lectura" << endl;
            cout << "(W) Escritura" << endl;
            char mode;
            cout << "Mode: ";
            cin >> mode;
            int pageid;
            cout << "Page ID: ";
            cin >> pageid;
            string blockPath;
            cout << "Block Path: ";
            cin >> blockPath;
            BfManger.loadPageFromDisk(blockPath, pageid, mode, rplMode);

        } else if (instruction == 2) {
            // BfManger.updatePage();
        } else if (instruction == 3) {
            cout << "Indica el proceso que deseas terminar" << endl;
            int pageid;
            cout << "Page ID: ";
            cin >> pageid;
            BfManger.killProcess(pageid);
        } else if (instruction == 4) {
            cout << "Indica la pagina que deseas guardar" << endl;
            /* Aqui deberia retornarme un error si la pagina tiene procesos sin
             * terminar*/
            int pageid;
            cout << "Page ID: ";
            cin >> pageid;
        } else if (instruction == 5) {
            BfManger.printTableFrame();
        }
    }
    return 0;
}
