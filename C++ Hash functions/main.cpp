#include "hashlab.h"
#include <chrono>
#include <map>
int main(){
    string key = "Stroginsky Boulevard";

    vector<string> filenames {"sample100.csv","sample1000.csv","sample5000.csv","sample10000.csv","sample20000.csv","sample30000.csv","sample50000.csv"};
    for (string & filename : filenames){
        //Heap sort + binary search:
        vector<ResidentEntry> storage = extractDataFromCSV(filename);
        cout << "\n--------" + to_string(storage.size()) + " elements--------\n";
        cout << "Heap sort + binary search: ";
        chrono::steady_clock::time_point begin = chrono::steady_clock::now();
        heapSort(storage);
        int temp = binarySearch(storage,key)+1;
        chrono::steady_clock::time_point end = chrono::steady_clock::now();
        auto elapsedTime = chrono::duration_cast<chrono::nanoseconds>(end - begin).count();
        cout << "Time elapsed: " << elapsedTime << " [nanoseconds]";
        cout << " Line: " << temp << endl;

        //Binary search
        cout << "Binary search for sorted entries: ";
        storage = extractDataFromCSV(filename);
        heapSort(storage);
        begin = chrono::steady_clock::now();
        temp = binarySearch(storage,key)+1;
        end = chrono::steady_clock::now();
        elapsedTime = chrono::duration_cast<chrono::nanoseconds>(end - begin).count();
        cout << "Time elapsed: " << elapsedTime << " [nanoseconds]";
        cout << " Line: " << temp << endl;

        //Linear
        cout << "Linear search: ";
        storage = extractDataFromCSV(filename);
        begin = chrono::steady_clock::now();
        temp = linearSearch(storage,key)+1;
        end = chrono::steady_clock::now();
        elapsedTime = chrono::duration_cast<chrono::nanoseconds>(end - begin).count();
        cout << "Time elapsed: " << elapsedTime << " [nanoseconds]";
        cout << " Line: " << temp << endl;

        // Multimap
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

        //Custom map - lab3
        cout << "Custom hash search: ";
        myHashTable tempHashLab = myHashTable(extractDataFromCSV(filename));
//            unordered_set<ResidentEntry, ResidentHash, ResidentEqual> set_of_residents;
//        ResidentEntry tempEntry = ResidentEntry("tempName","tempStreet",0,0,"tempBirthday", calculate_hash(key, 1444, true));
//            for (auto & i : storage)
//                set_of_residents.insert(i);
        begin = chrono::steady_clock::now();
//           set_of_residents.find(tempEntry);
        auto yep = tempHashLab.custom_hash_search(key);
        end = chrono::steady_clock::now();
        elapsedTime = chrono::duration_cast<chrono::nanoseconds>(end - begin).count();
        cout << "Time elapsed: " << elapsedTime << " [nanoseconds]";
        cout << " Line: " << yep+1;
        if (yep != -1) {
            cout << " | Found: ";
            tempHashLab.show_element(yep);
        }
        else cout << endl;
        //printToCSV(storage,"hash");

    }
    return 0;
}
