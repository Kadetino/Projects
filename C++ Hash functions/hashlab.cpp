#include "hashlab.h"

// Resident Entry class
ResidentEntry::ResidentEntry(string _name, string _street, int _buildingNum, int _flatNum, string _birthday, int64_t _hash) {
    this->name = _name;
    this->street = _street;
    this->buildingNum = _buildingNum;
    this->flatNum = _flatNum;
    this->birthday = _birthday;
    this->hash = _hash;
}

ResidentEntry::ResidentEntry(const ResidentEntry &other) {
    this->name = other.name;
    this->street = other.street;
    this->buildingNum = other.buildingNum;
    this->flatNum = other.flatNum;
    this->birthday = other.birthday;
    this->hash = other.hash;
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
            ResidentEntry tempEntry = ResidentEntry(tempName,tempStreet,tempBuilding,tempFlat,tempBirthday, calculate_hash(tempStreet,1444,
                                                                                                                       true));
        storage.push_back(tempEntry);
    }
    return storage;
}

void printToCSV(const vector<ResidentEntry>& storage, const string& type) {
    //fileCreation
    string filename = "sample"+ to_string(storage.size())+type+".csv";
    ofstream file(filename);

    file <<"ID;Street;BuildingNum;FlatNum;Birthday;hash"<< endl;
    for (const auto & i : storage){
        file<<i.name<<";";
        file<<i.street<<";";
        file<<i.buildingNum<<";";
        file<<i.flatNum<<";";
        file<<i.birthday<<";";
        file<<i.hash<<"\n";
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

//-----------------------------------------------------------------------
//Hash lab

int64_t calculate_hash(string &s, int p, bool good_hash) {
    int64_t hash = 0, p_pow = 1;
    // "Good" function
    if (good_hash) {
        for (size_t i = 0; i < s.length(); ++i) {
            hash += (s[i] - 'a' + 1) * p_pow;
            p_pow *= p;
        }
    }
    // "Bad" function
    else {
        if (s.length() >= 2) {
            hash = (s[0] - 'a' + 1) + (s[s.length() - 1] - 'a' + 1);
        }
    }
    return hash;
}

myHashTable::myHashTable(vector<ResidentEntry> objects) {
    this->hashStorage = create_hash_table(objects);
    this->objectStorage = objects;
}

int myHashTable::custom_hash_search(string key) {
    int64_t search_hash = calculate_hash(key, 1444, true);

    for (int i=0; i<this->hashStorage.size();i++) {
        if (this->hashStorage[i].first == search_hash) {
            if (this->hashStorage[i].second.size() == 1) {
                return this->hashStorage[i].second[0];
            }
            else {
                for (int j = 0; j < this->hashStorage[i].second.size(); j++) {
                    if (this->objectStorage[this->hashStorage[i].second[j]].street == key) {
                        return this->hashStorage[i].second[j];
                    }
                }
            }
            return -1;
        }
    }
    return -1;
}

ResidentEntry myHashTable::show_element(int index) {
    ResidentEntry target = this->objectStorage[index];
    cout << target.name << " from " << target.street << "; born - " << target.birthday << endl;
    return target;
}

void myHashTable::debug_print_hash_table(bool fancy) {
    cout << "---------\n\nDEBUG: hash indicesStorage for " << to_string(this->objectStorage.size()) << " elements\n";
    if (fancy) {
        for (int i=0;i < this->hashStorage.size();i++) {
            cout << "Unique hash #" << i << ": "<< this->hashStorage[i].first << " | Elements: ";
            vector<int> temp = this->hashStorage[i].second;
            for (int k : temp) {
                cout << k << "; ";
            }
            cout << endl;
        }
    }
    else {
        for (int i=0;i < this->hashStorage.size();i++) {
            cout << "Unique hash #" << i << ": "<< this->hashStorage[i].first << " | Size: " << this->hashStorage[i].second.size() << endl;
        }
        cout << endl << "----------" << endl;
    }
}

vector<pair<int64_t, vector<int>>> myHashTable::create_hash_table(vector<ResidentEntry> &storage) {
    // Place to store hashes and indices in storage
    vector <pair<int64_t, int>> hashes (storage.size());
    // Extract hashes
    for (int i=0; i<storage.size();i++){
        hashes[i] = make_pair(storage[i].hash, i);
    }
    // Sort and form indicesStorage
    sort (hashes.begin(), hashes.end());
    vector<pair<int64_t,vector<int>>> hash_table;
    // First elements to get everything going
    int64_t temp_pair_hash = hashes[0].first;

    vector<int> temp_vector_objects;
    temp_vector_objects.push_back(hashes[0].second);
    // Form indicesStorage
    for (int i=1; i < storage.size(); i++) {
        if (hashes[i].first==temp_pair_hash){
            temp_vector_objects.push_back(hashes[i].second);
        }
        else {
            hash_table.emplace_back (temp_pair_hash, temp_vector_objects);
            temp_pair_hash = hashes[i].first;
            temp_vector_objects.clear();
            temp_vector_objects.push_back(hashes[i].second);
        }
    }
    // Insert last element and return
    hash_table.emplace_back (temp_pair_hash, temp_vector_objects);

    return hash_table;
}

myHashTable::~myHashTable() = default;
