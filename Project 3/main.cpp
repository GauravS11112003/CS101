#include <iostream>
#include <string>

#include "p3.h"

using namespace std;

int main(int argc, char *argv[]) {
    bool printPrompts = true;
    if (argc == 2 && string(argv[1]) == "no") {
        printPrompts = false;
    }

    ContactList *list = new ContactList();

    string cmd, first, last, infoName, infoVal;
    while (1) {
        if (printPrompts) cout << "Enter a command or \"help\" to get a command list: ";
        cin >> cmd;
        if (cmd == "quit") {
            break;
        }
        else if (cmd == "addContact") {
            if (printPrompts) cout << "\tEnter first name: ";
            cin >> first;
            if (printPrompts) cout << "\tEnter last name: ";
            cin >> last;
            if (!list->addContact(first, last)) {
                cout << "Error: " << first << " " << last << " already exists" << endl;
            }
        }
        else if (cmd == "addInfo") {
            if (printPrompts) cout << "\tEnter first name: ";
            cin >> first;
            if (printPrompts) cout << "\tEnter last name: ";
            cin >> last;
            if (printPrompts) cout << "\tEnter info name: ";
            cin >> infoName;
            if (printPrompts) cout << "\tEnter info value: ";
            cin >> infoVal;
            if (!list->addInfo(first, last, infoName, infoVal)) {
                cout << "Error: " << first << " " << last << " does not exist" << endl;
            }
        }
        else if (cmd == "printContact") {
            if (printPrompts) cout << "\tEnter first name: ";
            cin >> first;
            if (printPrompts) cout << "\tEnter last name: ";
            cin >> last;
            if (!list->printContact(cout, first, last)) {
                cout << "Error: " << first << " " << last << " does not exist" << endl;
            }
            cout << endl;
        }
        else if (cmd == "count") {
            cout << "\nThere are " << list->getCount() << " contacts" << endl;
        }
        else if (cmd == "print") {
            list->print(cout);
            cout << endl;
        }
        else if (cmd == "addContactOrdered") {
            if (printPrompts) cout << "\tEnter first name: ";
            cin >> first;
            if (printPrompts) cout << "\tEnter last name: ";
            cin >> last;
            if (!list->addContactOrdered(first, last)) {
                cout << "Error: " << first << " " << last << " already exists" << endl;
            }
        }
        else if (cmd == "addInfoOrdered") {
            if (printPrompts) cout << "\tEnter first name: ";
            cin >> first;
            if (printPrompts) cout << "\tEnter last name: ";
            cin >> last;
            if (printPrompts) cout << "\tEnter info name: ";
            cin >> infoName;
            if (printPrompts) cout << "\tEnter info value: ";
            cin >> infoVal;
            if (!list->addInfoOrdered(first, last, infoName, infoVal)) {
                cout << "Error: " << first << " " << last << " does not exist" << endl;
            }
        }
        else if (cmd == "removeInfo") {
            if (printPrompts) cout << "\tEnter first name: ";
            cin >> first;
            if (printPrompts) cout << "\tEnter last name: ";
            cin >> last;
            if (printPrompts) cout << "\tEnter info name: ";
            cin >> infoName;
            if (!list->removeInfo(first, last, infoName)) {
                cout << "Error: " << first << " " << last << " or " << infoName << " does not exist" << endl;
            }
        }
        else if (cmd == "removeContact") {
            if (printPrompts) cout << "\tEnter first name: ";
            cin >> first;
            if (printPrompts) cout << "\tEnter last name: ";
            cin >> last;
            if (!list->removeContact(first, last)) {
                cout << "Error: " << first << " " << last << " does not exist" << endl;
            }
        }
        else if (cmd == "destroy") {
            delete list;
            list = new ContactList();
        }
        else if (cmd == "copycon" || cmd == "copy=") {
            ContactList *list2;
            if (cmd == "copycon") {
                list2 = new ContactList(*list);
            }
            else {
                list2 = new ContactList();
                *list2 = *list;
            }

            if (printPrompts) cout << "\tAdd contact to copied list:" << endl;
            if (printPrompts) cout << "\tEnter first name: ";
            cin >> first;
            if (printPrompts) cout << "\tEnter last name: ";
            cin >> last;
            if (!list2->addContact(first, last))
            {
                cout << "Error: " << first << " " << last << " already exists" << endl;
            }
            
            if (printPrompts) cout << "\tAdd info to copied list:" << endl;
            if (printPrompts) cout << "\tEnter first name: ";
            cin >> first;
            if (printPrompts) cout << "\tEnter last name: ";
            cin >> last;
            if (printPrompts) cout << "\tEnter info name: ";
            cin >> infoName;
            if (printPrompts) cout << "\tEnter info value: ";
            cin >> infoVal;
            if (!list2->addInfo(first, last, infoName, infoVal))
            {
                cout << "Error: " << first << " " << last << " or " << infoName << "does not exist" << endl;
            }

            cout << "Original list:" << endl;
            list->print(cout);
            cout << endl;

            cout << "Copied list:" << endl;
            list2->print(cout);
            cout << endl;

            delete list2;
        }
        else if (cmd == "help") {
            cout << "Valid commands:" << endl;
            cout << "***** addContact" << endl;
            cout << "***** addInfo" << endl;
            cout << "***** printContact" << endl;
            cout << "***** count" << endl;
            cout << "***** print" << endl;
            cout << "***** addContactOrdered" << endl;
            cout << "***** addInfoOrdered" << endl;
            cout << "***** removeContact" << endl;
            cout << "***** removeInfo" << endl;
            cout << "***** destroy" << endl;
            cout << "***** copycon" << endl;
            cout << "***** copy=" << endl;
            cout << "***** help" << endl;
            cout << "***** quit" << endl;
        }
        else {
            cout << "Invalid command specified - " << cmd << endl;
        }
        if (printPrompts) cout << endl;
    }
    return 0;
}
