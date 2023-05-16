#include <iostream>
#include <list>
#include <vector>
#include <cstdlib>
#include <algorithm>
using namespace std;

/**
 * Abstract observer class
 */
class abstractObserver {
public:
    // Constructor
    virtual ~abstractObserver()= default;

    // Methods
    virtual void update_state(const std::string &message_from_subject) = 0;

    //Variables
    string observer_name_;
    int current_bid;
};

/**
 * Abstract subject class
 */
class abstractSubject {
    public:
        // Destructor
        virtual ~abstractSubject()= default;

        // Methods
        virtual void add_new_observer(abstractObserver *observer) = 0;
        virtual void remove_observer(abstractObserver *observer) = 0;

        virtual void notify_observers() = 0;
};

/**
 * Subject class
 */
class Subject: public abstractSubject {
    private:
        list<abstractObserver *> list_of_all_connected_observers_;
        string current_state_;
        vector<int> observers_response_bid;
        int highest_bid;

    public:
        // Constructor
        /**
         * Subject constructor
         */
        Subject() {
            observers_response_bid.clear();
            cout << "Auction is now in session!\n";
        }

        // Destructor
        /**
         * Subject destructor
         */
        ~Subject() override {
            cout << "Auction session has been closed. ;(\n";
        }

        // Edit list_of_all_connected_observers_
        /**
         * Add new observer to current subject
         * @param observer new observer
         */
        void add_new_observer(abstractObserver *observer) override {
            list_of_all_connected_observers_.push_back(observer);
        }

        /**
         * Remove observer to current subject
         * @param observer observer from list_of_all_connected_observers_
         */
        void remove_observer(abstractObserver *observer) override {
            list_of_all_connected_observers_.remove(observer);
        }

        // Notify everyone involved
        /**
         * Notify all observers of occurred changes
         */
        void notify_observers() override {
            auto iterator = list_of_all_connected_observers_.begin();
            observer_count();
            while (iterator != list_of_all_connected_observers_.end()) {
                (*iterator)->update_state(current_state_);
                ++iterator;
            }
        }

        // Change current state
        /**
         * Change current state of subject
         * @param new_state new state
         */
        void change_state(string new_state = "None") {
            this->current_state_ = new_state;
            cout << "New auction item: \"" << new_state << "\"\n\n";
            observers_response_bid.clear();
            notify_observers();
        }

        /**
         * response from observer
         * @param their_bid bid of an observer
         */
        void add_new_response_bid(int their_bid) {
            observers_response_bid.push_back(their_bid);
        }

        // Display number of followers
        /**
         * Display how many observers are following this subject
         */
        void observer_count() {
            cout << "Current amount of bidders: " << list_of_all_connected_observers_.size() << endl;
        }

        /**
         * Find the highest bidder among all observers and announce him as a winner
         */
        void announce_winner() {
            string winner = "Nobody";
            this->highest_bid = *(max_element(this->observers_response_bid.begin(), this->observers_response_bid.end()));
            auto iterator = list_of_all_connected_observers_.begin();
            while (iterator != list_of_all_connected_observers_.end()) {
                if ((*iterator)->current_bid == this->highest_bid){
                    winner = (*iterator)->observer_name_;
                    break;
                }
                ++iterator;
            }
            cout << this->current_state_ << " is sold for " << this->highest_bid << "$ to "<< winner <<"!\n";
            this->current_state_ = "None";
            this->observers_response_bid.clear();
        }
};

/**
 * Observer class
 */
class Observer: public abstractObserver {
    private:
        string current_state_from_subject_;
        Subject &subject_;


    public:
        // Constructor
        /**
         * Constructor for observer
         * @param subject
         * @param name
         */
        Observer(Subject &subject, string name): subject_(subject) {
            this->observer_name_ = name;
            this->subject_.add_new_observer(this);
            this->current_bid = 0;
            cout << "Observer \"" << observer_name_ << "\" has joined the auction.\n";
        }

        // Destructor
        /**
         * Destructor for observer
         */
        ~Observer() override {
            cout << "Observer \"" << this->observer_name_ << "\" has left...\n";
        }

        /**
         * update current state from subject
         * @param current_state
         */
        void update_state(const string &current_state) override {
            current_state_from_subject_ = current_state;
            current_bid = rand() % 1000;
            this->subject_.add_new_response_bid(current_bid);
            display_new_state();
        }

        /**
         * unfollow subject
         */
        void remove_observer_from_subject() {
            subject_.remove_observer(this);
            cout << "Observer \"" << observer_name_ << "\" no longer follows the auction.\n";
        }

        /**
         * message-notification about change in status
         */
        void display_new_state() {
            cout << "Observer \"" << this->observer_name_ << "\": I bid " << current_bid << "$ for \"" << this->current_state_from_subject_ << "\"\n";
        }
};