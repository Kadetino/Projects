#include "hashlab.h"

// Resident Entry class
ResidentEntry::ResidentEntry(string _name, string _street, int _buildingNum, int _flatNum, string _birthday) {
    this->name = _name;
    this->street = _street;
    this->buildingNum = _buildingNum;
    this->flatNum = _flatNum;
    this->birthday = _birthday;
}

ResidentEntry::ResidentEntry(const ResidentEntry &other) {
    this->name = other.name;
    this->street = other.street;
    this->buildingNum = other.buildingNum;
    this->flatNum = other.flatNum;
    this->birthday = other.birthday;
}

ResidentEntry::~ResidentEntry() = default;

bool ResidentEntry::operator<(const ResidentEntry &other) {
    // Street
    switch ((this->street).compare(other.street)){
        case -1: return true;
        case 1: return false;
    }
    // Building
    if (this->buildingNum != other.buildingNum){
        return (this->buildingNum < other.buildingNum);
    }
    // Flat
    if (this->flatNum != other.flatNum){
        return (this->flatNum < other.flatNum);
    }
    // Name
    switch ((this->name).compare(other.name)){
        case -1: return true;
        case 1: return false;
    }
    // Birthday
    if (this->birthday != other.birthday){
        return (this->birthday < other.birthday);
    }
    return false;
}

bool ResidentEntry::operator<=(const ResidentEntry &other) {
    if (*this==other) return true;
    // Street
    switch ((this->street).compare(other.street)){
        case -1: return true;
        case 1: return false;
    }
    // Building
    if (this->buildingNum != other.buildingNum){
        return (this->buildingNum < other.buildingNum);
    }
    // Flat
    if (this->flatNum != other.flatNum){
        return (this->flatNum < other.flatNum);
    }
    // Name
    switch ((this->name).compare(other.name)){
        case -1: return true;
        case 1: return false;
    }
    // Birthday
    if (this->birthday != other.birthday){
        return (this->birthday < other.birthday);
    }
    return true;
}

bool ResidentEntry::operator>(const ResidentEntry &other) {
    // Street
    switch ((this->street).compare(other.street)){
        case -1: return false;
        case 1: return true;
    }
    // Building
    if (this->buildingNum != other.buildingNum){
        return (this->buildingNum > other.buildingNum);
    }
    // Flat
    if (this->flatNum != other.flatNum){
        return (this->flatNum > other.flatNum);
    }
    // Name
    switch ((this->name).compare(other.name)){
        case -1: return false;
        case 1: return true;
    }
    // Birthday
    if (this->birthday != other.birthday){
        return (this->birthday > other.birthday);
    }
    return false;
}

bool ResidentEntry::operator>=(const ResidentEntry &other) {
    if (*this==other) return true;
    // Street
    switch ((this->street).compare(other.street)){
        case -1: return false;
        case 1: return true;
    }
    // Building
    if (this->buildingNum != other.buildingNum){
        return (this->buildingNum > other.buildingNum);
    }
    // Flat
    if (this->flatNum != other.flatNum){
        return (this->flatNum > other.flatNum);
    }
    // Name
    switch ((this->name).compare(other.name)){
        case -1: return false;
        case 1: return true;
    }
    // Birthday
    if (this->birthday != other.birthday){
        return (this->birthday > other.birthday);
    }
    return false;
}

bool ResidentEntry::operator==(const ResidentEntry &other) const {
    if(this->street==other.street && this->name==other.name && this->buildingNum==other.buildingNum &&this->flatNum==other.flatNum&& this->birthday==other.birthday) return true;
    else return false;
}

// Utility
vector<ResidentEntry> extractDataFromCSV(const string& filename) {
    vector<ResidentEntry> storage;
    ifstream file(filename);
    string tempstr,tempName,tempStreet,tempBirthday;
    int tempBuilding,tempFlat;
    // Extracting data
    while (getline(file, tempstr)){
        if (tempstr=="ID;Street;BuildingNum;FlatNum;Birthday") continue;
        // Name
        auto nextSeparator = tempstr.find(';');
        tempName = tempstr.substr(0,nextSeparator);
        tempstr.erase(0,nextSeparator+1);
        // Street
        nextSeparator = tempstr.find(';');
        tempStreet = tempstr.substr(0,nextSeparator);
        tempstr.erase(0,nextSeparator+1);
        // Building number
        nextSeparator = tempstr.find(';');
        tempBuilding = stoi(tempstr.substr(0,nextSeparator));
        tempstr.erase(0,nextSeparator+1);
        // Flat number
        nextSeparator = tempstr.find(';');
        tempFlat = stoi(tempstr.substr(0,nextSeparator));
        tempstr.erase(0,nextSeparator+1);
        // Birthday
        tempBirthday = tempstr;
        // add to storage
        ResidentEntry tempEntry = ResidentEntry(tempName,tempStreet,tempBuilding,tempFlat,tempBirthday);
        storage.push_back(tempEntry);
    }
    return storage;
}

void printToCSV(const vector<ResidentEntry>& storage, const string& type) {
    //fileCreation
    string filename = "sample"+ to_string(storage.size())+type+".csv";
    ofstream file(filename);

    file <<"ID;Street;BuildingNum;FlatNum;Birthday"<< endl;
    for (const auto & i : storage){
        file<<i.name<<";";
        file<<i.street<<";";
        file<<i.buildingNum<<";";
        file<<i.flatNum<<";";
        file<<i.birthday<<"\n";
    }
}

// Task
void heapify(vector<ResidentEntry>& storage, int n, int index) {
    int largest_element_index = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    // Left > parent
    if (left < n && storage[left] > storage[largest_element_index]) {
        largest_element_index = left;
    }
    // Right > parent
    if (right < n && storage[right] > storage[largest_element_index]) {
        largest_element_index = right;
    }
    // Largest element!=root
    if (largest_element_index != index) {
        swap(storage[index], storage[largest_element_index]);
        heapify(storage, n, largest_element_index);
    }
}

void heapSort(vector<ResidentEntry>& storage) {
    // Making heap
    int n = storage.size();
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(storage, n, i);
    }
    // Sorting
    for (int i=n-1; i>=0; i--){
        swap(storage[0], storage[i]); // root==largest -> move to the end
        heapify(storage, i, 0); // go again
    }
}

int binarySearch(const vector<ResidentEntry>& storage, string key) {
    int left=0, right=storage.size()-1;
    int mid = (left+right)/2;

    while (mid!=left and mid!=right) {
        if (storage[mid].street==key) return mid;
        else if (storage[mid].street<key) {
            left=mid;
        }
        else if (storage[mid].street>key) {
            right=mid;
        }
        mid = (left+right)/2;
    }
    return -1;
}

int linearSearch(const vector<ResidentEntry>& storage, string key) {
    for (int i=0;i<storage.size();i++) {
        if (storage[i].street==key) {
            return i;
        }
    }
    return -1;
}