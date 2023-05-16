#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <unordered_set>


using namespace std;
/**
 * Class dedicated to store data about resident of local district
 */
class ResidentEntry{
public:
    /**
    * < operator for ResidentEntry objects
    * @param other ResidentEntry: compared ResidentEntry
    * @return true or false
    */
    bool operator<(const ResidentEntry& other);
    /**
     * > operator for ResidentEntry objects
     * @param other ResidentEntry: compared ResidentEntry
     * @return true or false
     */
    bool operator>(const ResidentEntry& other);
    /**
     * >= operator for ResidentEntry objects
     * @param other ResidentEntry: compared ResidentEntry
     * @return true or false
     */
    bool operator>=(const ResidentEntry& other);
    /**
     * <= operator for ResidentEntry objects
     * @param other ResidentEntry: compared ResidentEntry
     * @return true or false
     */
    bool operator<=(const ResidentEntry& other);
    /**
     * == operator for ResidentEntry objects
     * @param other ResidentEntry: compared ResidentEntry
     * @return true or false
     */
    bool operator==(const ResidentEntry& other) const;
    string name, street, birthday;
    uint32_t buildingNum, flatNum;
    int64_t hash;
    // Constructor
    /**
     * Constructor
     * @param name string: Name of the person
     * @param street string: Name of the street
     * @param buildingNum uint32_t: Building number
     * @param flatNum uint32_t: Flat number
     * @param birthday string: Birthday date of this person
     * @param hash int64_t: Hash made from street
     */
    ResidentEntry(string name, string street, int buildingNum, int flatNum, string birthday, int64_t hash);
    // Copy
    /**
     * Copy constructor
     * @param other ResidentEntry from which properties will be copied
     */
    ResidentEntry(const ResidentEntry& other);
    // Destructor
    /**
     * Destructor
     */
    virtual ~ResidentEntry();
};

// Utility

/**
 * Utility: Reading .csv file and putting extracted data into a vector filled with ResidentEntry objects
 * @param filename Name of the file that needs to be read
 * @return vector with ResidentEntry objects
 */
vector<ResidentEntry> extractDataFromCSV(const string& filename);
/**
 * Print vector with ResidentEntry objects into .csv
 * @param storage vector with ResidentEntry objects
 * @param type string that will be appended after name of the file
 */
void printToCSV(const vector<ResidentEntry>& storage, const string& type);

// Task
/**
 * Sort given vector using insert sort
 * @param storage vector with ResidentEntry objects that needs to be sorted
 */
void heapSort(vector<ResidentEntry>& storage);
/**
 * making heap out of a given vector with ResidentEntry objects
 * @param storage vector with ResidentEntry objects
 * @param n size of the heap
 * @param index index in vector
 */
void heapify(vector<ResidentEntry>& storage, int n, int index);
/**
 * Binary search performed on vector<ResidentEntry>. Keys = ResidentEntry.street
 * @param storage vector with ResidentEntry objects
 * @param key Target for which search is performed
 * @return int position in vector, where target was found
 */
int binarySearch(const vector<ResidentEntry>& storage, string key);
/**
 * Linear search performed on vector<ResidentEntry>. Keys = ResidentEntry.street
 * @param storage vector with ResidentEntry objects
 * @param key Target for which search is performed
 * @return int position in vector, where target was found
 */
int linearSearch(const vector<ResidentEntry>& storage, string key);
/**
 * Calculate hash
 * @param s string which will be used as source input for hash function
 * @param p number for generating hash
 * @param good_hash bool flag to indicate if "good" or "bad" hash function should be used
 * @return hash value
 */
int64_t calculate_hash(string &s, int p, bool good_hash);



//struct ResidentHash {
//public:
//    size_t operator()(const ResidentEntry & object) const {
//        return object.hash;
//    }
//};
//struct ResidentEqual {
//public:
//    bool operator()(const ResidentEntry & object1, const ResidentEntry & object2) const {
//        if (object1 == object2)
//            return true;
//        else
//            return false;
//    }
//};

class myHashTable {
    private:
        vector<pair<int64_t,vector<int>>> hashStorage;
        vector<ResidentEntry> objectStorage;
        /**
        * Create custom-hashtable
        * @param storage vector with ResidentEntry objects
        * @return int position in storage vector, where target was found
        */
    vector<pair<int64_t,vector<int>>> create_hash_table (vector<ResidentEntry> &storage);

    public:

        /**
         * Constructor
         * @param objects which vector<ResidentEntry> will be used to create hash table
         */
        explicit myHashTable(vector<ResidentEntry> objects);
        // Destructor
        /**
         * Destructor
         */
        virtual ~myHashTable();
        /**
        * Custom search using ResidentEntry.hash field. Keys = ResidentEntry.street
        * @param key Target for which search is performed
        * @return int index in storage, where target was found
        */
        int custom_hash_search(string key);
        /**
         * Show and return element with given index
         * @param index
         */
        ResidentEntry show_element(int index);
        /**
        * Print hash table to standard output. debug
        * @param fancy bool flag for one of the two print options
        */
        void debug_print_hash_table(bool fancy);
};