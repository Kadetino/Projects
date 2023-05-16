#include "hashlab.h"
#include <chrono>
#include <map>
int main(){
    string key = "Stroginsky Boulevard";

    vector<string> filenames {"sample100.csv","sample1000.csv","sample5000.csv","sample10000.csv","sample20000.csv","sample30000.csv","sample50000.csv"};
    for (string & filename : filenames){
        vector<ResidentEntry> storage = extractDataFromCSV(filename);
        string sortedFilename = "sample"+ to_string(storage.size())+"Heap.csv";
        cout << "\n--------" + to_string(storage.size()) + " elements--------\n";
        cout << "Heap sort + binary search: ";
        chrono::steady_clock::time_point begin = chrono::steady_clock::now();
        heapSort(storage);
        int temp = binarySearch(storage,key)+1;
        chrono::steady_clock::time_point end = chrono::steady_clock::now();
        auto elapsedTime = chrono::duration_cast<chrono::nanoseconds>(end - begin).count();
        cout << "Time elapsed: " << elapsedTime << " [nanoseconds]";
        cout << " Line: " << temp << endl;

        cout << "Binary search for sorted entries: ";
        storage = extractDataFromCSV(sortedFilename);
        begin = chrono::steady_clock::now();
        temp = binarySearch(storage,key)+1;
        end = chrono::steady_clock::now();
        elapsedTime = chrono::duration_cast<chrono::nanoseconds>(end - begin).count();
        cout << "Time elapsed: " << elapsedTime << " [nanoseconds]";
        cout << " Line: " << temp << endl;

        cout << "Linear search: ";
        storage = extractDataFromCSV(filename);
        begin = chrono::steady_clock::now();
        temp = linearSearch(storage,key)+1;
        end = chrono::steady_clock::now();
        elapsedTime = chrono::duration_cast<chrono::nanoseconds>(end - begin).count();
        cout << "Time elapsed: " << elapsedTime << " [nanoseconds]";
        cout << " Line: " << temp << endl;

        cout << "Multimap search: ";
        multimap<string, ResidentEntry> mapStorage;
        for (auto & i : storage) {
            mapStorage.insert({i.street,i});
        }
        begin = chrono::steady_clock::now();
        auto tempMap = mapStorage.find(key);
        end = chrono::steady_clock::now();
        elapsedTime = chrono::duration_cast<chrono::nanoseconds>(end - begin).count();
        cout << "Time elapsed: " << elapsedTime << " [nanoseconds]";
        cout << " | Found: " << tempMap->second.name << " from " << tempMap->first << endl;
    }
    return 0;
}
