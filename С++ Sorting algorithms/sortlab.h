#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
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
        // Constructor
        /**
         * Constructor
         * @param name string: Name of the person
         * @param street string: Name of the street
         * @param buildingNum int: Building number
         * @param flatNum int: Flat number
         * @param birthday string: Birthday date of this person
         */
        ResidentEntry(string name, string street, int buildingNum, int flatNum, string birthday);
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
void insertSort(vector<ResidentEntry>& storage);
/**
 * Sort given vector using shaker sort
 * @param storage vector with ResidentEntry objects
 */
void shakerSort(vector<ResidentEntry>& storage);
/**
 * Sort given vector using heap sort
 * @param storage vector with ResidentEntry objects
 */
void heapSort(vector<ResidentEntry>& storage);
/**
 * making heap out of a given vector with ResidentEntry objects
 * @param storage vector with ResidentEntry objects
 * @param n size of the heap
 * @param index index in vector
 */
void heapify(vector<ResidentEntry>& storage, int n, int index);