#include "sortlab.h"
#include <chrono>

int main(){
    vector<string> filenames {"sample10.csv","sample100.csv","sample1000.csv","sample5000.csv","sample10000.csv","sample20000.csv","sample30000.csv","sample50000.csv"};

    cout << "--------Insert sort--------\n";
    for (string & filename : filenames){
        vector<ResidentEntry> storage = extractDataFromCSV(filename);
        //if (storage.size()>1000) break;
        string statusMessage = "Time elapsed: " + to_string(storage.size()) + " elements = ";
        chrono::steady_clock::time_point begin = chrono::steady_clock::now();
        insertSort(storage);
        chrono::steady_clock::time_point end = chrono::steady_clock::now();
        //printToCSV(storage,"Insert");
        cout << statusMessage << chrono::duration_cast<chrono::microseconds>(end - begin).count() << "[microseconds]" << endl;
    }

    cout << "\n--------Shaker sort--------\n";
    for (string & filename : filenames){
        vector<ResidentEntry> storage = extractDataFromCSV(filename);
        //if (storage.size()>1000) break;
        string statusMessage = "Time elapsed: " + to_string(storage.size()) + " elements = ";

        chrono::steady_clock::time_point begin = chrono::steady_clock::now();
        shakerSort(storage);
        chrono::steady_clock::time_point end = chrono::steady_clock::now();
        //printToCSV(storage,"Shaker");
        cout << statusMessage << chrono::duration_cast<chrono::microseconds>(end - begin).count() << "[microseconds]"<< endl;
    }

    cout << "\n--------Heap sort--------\n";
    for (string & filename : filenames){
        vector<ResidentEntry> storage = extractDataFromCSV(filename);
        string statusMessage = "Time elapsed: " + to_string(storage.size()) + " elements = ";

        chrono::steady_clock::time_point begin = chrono::steady_clock::now();
        heapSort(storage);
        chrono::steady_clock::time_point end = chrono::steady_clock::now();
        //printToCSV(storage,"Heap");
        cout << statusMessage << chrono::duration_cast<chrono::microseconds>(end - begin).count() << "[microseconds]"<< endl;
    }
    return 0;
}
