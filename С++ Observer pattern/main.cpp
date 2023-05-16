#include "Observer.h"

/*Вариант 18. Используя паттерн Observer, реализовать модель
электронного аукциона. Пул товаров (лотов) фиксирован, число
участников определяется на этапе выполнения, при этом оно может
изменяться в зависимости от лота.
*/


void Grand_Auction(string auction_item) {
    // Initialize
    auto *subject = new Subject;
    auto *observer1 = new Observer(*subject, "John");
    auto *observer2 = new Observer(*subject, "Mary");
    auto *observer3 = new Observer(*subject, "Francois");
    auto *observer4 = new Observer(*subject, "Pierre");
    auto *observer5 = new Observer(*subject, "Joe");;
    auto *observer6 = new Observer(*subject, "Liam");
    observer2->remove_observer_from_subject();
    cout << endl;

    // Set the item
    subject->change_state(auction_item);
    cout << endl;

    // Find the winner
    subject->announce_winner();


    // Murder (metaphorically speaking)
    cout << endl;
    delete observer6;
    delete observer5;
    delete observer4;
    delete observer3;
    delete observer2;
    delete observer1;
    delete subject;
    cout << "------------" <<endl;
}

int main() {
    Grand_Auction("Fleur de lis");
    Grand_Auction("Magic square");
    return 0;
}
