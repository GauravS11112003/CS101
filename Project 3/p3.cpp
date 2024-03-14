#include <iostream>
#include <string>

#include "p3.h"

using namespace std;

Info::Info() {
    this->name = "No Name Set";
    this->value = "No Value Set";
    this->next = nullptr;
}

Info::Info(std::string name, std::string value, Info *next) {
    this->name = name;
    this->value = value;
    this->next = next;
}

Contact::Contact() {
    this->first = "No First Set";
    this->last = "No Last Set";
    this->next = nullptr;
    this->headInfoList = nullptr;
}

Contact::Contact(std::string first, std::string last, Contact *next) {
    this->first = first;
    this->last = last;
    this->next = next;
    this->headInfoList = nullptr;
}

ContactList::ContactList() {
    this->headContactList = nullptr;
    this->count = 0;
}

int ContactList::getCount() {
    return this->count;
}

// print the specified contact and its information
// 1. return false and print nothing if the contact is not in the list
// 2. otherwise return true and print the contact
bool ContactList::printContact(std::ostream &os, std::string first, std::string last) {
    Contact *temp = headContactList;
    while (temp != nullptr) {
        if (temp->first == first && temp->last == last) {
            os << "Contact Name: " << temp->first << " " << temp->last << std::endl;
            Info *infoTemp = temp->headInfoList;
            while (infoTemp != nullptr) {
                os << "    " << infoTemp->name << " | " << infoTemp->value << std::endl;
                infoTemp = infoTemp->next;
            }
            return true;
        }
        temp = temp->next;
    }
    return false; // Contact not found
}

// print all contacts and their information
// print nothing if the list is empty
void ContactList::print(std::ostream &os) {
    if (headContactList == nullptr) {
        return; // List is empty
    }
    Contact *temp = headContactList;
    while (temp != nullptr) {
        os << "Contact Name: " << temp->first << " " << temp->last << std::endl;
        Info *infoTemp = temp->headInfoList;
        while (infoTemp != nullptr) {
            os << "    " << infoTemp->name << " | " << infoTemp->value << std::endl;
            infoTemp = infoTemp->next;
        }
        temp = temp->next;
    }
}
// add a contact to the back of the list
// 1. return false and do nothing if the contact is already in the list
// 2. otherwise return true and add the contact to the back of the list
// - do not forget to update count
bool ContactList::addContact(std::string first, std::string last) {
       // Check if the contact already exists in the list
    Contact *temp = this->headContactList;
    while (temp != nullptr) {
        if (temp->first == first && temp->last == last) {
            return false; // Contact already exists
        }
        temp = temp->next;
    }

    // If the contact does not exist, add it to the end of the list
    Contact *newContact = new Contact(first, last, nullptr);
    if (this->headContactList == nullptr) {
        // If the list is empty, make the new contact the head of the list
        this->headContactList = newContact;
    } else {
        // If the list is not empty, traverse to the end and add the new contact
        temp = this->headContactList;
        while (temp->next != nullptr) {
            temp = temp->next;
        }
        temp->next = newContact;
    }

    // Increase the count of contacts
    this->count++;

    return true;
}

// add info to the back of a contact's info list
// 1. return false and do nothing if the contact is not in the list
// 2. if the infoName is already in the info list, update the infoValue and return true
// 3. otherwise add the info to the back of the contact's list and return true
bool ContactList::addInfo(std::string first, std::string last, std::string infoName, std::string infoVal) {
    Contact *contact = headContactList;
    while (contact != nullptr) {
        if (contact->first == first && contact->last == last) {
            Info *info = contact->headInfoList;
            Info *prevInfo = nullptr;
            while (info != nullptr) {
                if (info->name == infoName) {
                    info->value = infoVal; // Update existing info value
                    return true;
                }
                prevInfo = info;
                info = info->next;
            }
            // If infoName not found, add new info
            Info *newInfo = new Info(infoName, infoVal);
            if (prevInfo == nullptr) {
                contact->headInfoList = newInfo; // First info for this contact
            } else {
                prevInfo->next = newInfo; // Add newInfo at the end of the list
            }
            return true;
        }
        contact = contact->next;
    }
    return false; // Contact not found
}


Contact* findContactPrev(Contact **head, std::string first, std::string last) {
    Contact *prev = nullptr, *cur = *head;
    while (cur != nullptr && (cur->last < last || (cur->last == last && cur->first < first))) {
        prev = cur;
        cur = cur->next;
    }
    // Return nullptr if contact not found, else the previous node
    return (cur != nullptr && cur->first == first && cur->last == last) ? prev : nullptr;
}

void insertContactOrdered(Contact **head, Contact *newContact) {
    if (*head == nullptr || (*head)->last > newContact->last || ((*head)->last == newContact->last && (*head)->first > newContact->first)) {
        newContact->next = *head;
        *head = newContact;
    } else {
        Contact *cur = *head;
        while (cur->next != nullptr && (cur->next->last < newContact->last || (cur->next->last == newContact->last && cur->next->first < newContact->first))) {
            cur = cur->next;
        }
        newContact->next = cur->next;
        cur->next = newContact;
    }
}

Info* findInfoPrev(Info **head, std::string name) {
    Info *prev = nullptr, *cur = *head;
    while (cur != nullptr && cur->name < name) {
        prev = cur;
        cur = cur->next;
    }
    // Return nullptr if info not found, else the previous node
    return (cur != nullptr && cur->name == name) ? prev : nullptr;
}

void insertInfoOrdered(Info **head, Info *newInfo) {
    if (*head == nullptr || (*head)->name > newInfo->name) {
        newInfo->next = *head;
        *head = newInfo;
    } else {
        Info *cur = *head;
        while (cur->next != nullptr && cur->next->name < newInfo->name) {
            cur = cur->next;
        }
        newInfo->next = cur->next;
        cur->next = newInfo;
    }
}

// add a contact to the list in ascending order by last name
//     if last names are equal, then order by first name ascending
// 1. return false and do nothing if the contact is already in the list
// 2. otherwise return true and add the contact to the list
// - do not forget to update count
// - compare strings with the built-in comparison operators: <, >, ==, etc.
// - a compare method/function is recommended
bool ContactList::addContactOrdered(std::string first, std::string last) {
    // Check for existing contact
    Contact* prev = nullptr;
    for (Contact* cur = headContactList; cur != nullptr; prev = cur, cur = cur->next) {
        if (cur->first == first && cur->last == last) {
            // Contact already exists
            return false;
        }
        if ((cur->last > last) || (cur->last == last && cur->first > first)) {
            break;
        }
    }

    // Create new contact
    Contact* newContact = new Contact(first, last);

    if (prev == nullptr) {
        // Insert at the beginning
        newContact->next = headContactList;
        headContactList = newContact;
    } else {
        // Insert after prev
        newContact->next = prev->next;
        prev->next = newContact;
    }

    count++; // Update count
    return true;
}

// add info to a contact's info list in ascending order by infoName
// 1. return false and do nothing if the contact is not in the list
// 2. if the infoName is already in the info list, update the infoValue and return true
// 3. otherwise add the info to the contact's list and return true
bool ContactList::addInfoOrdered(std::string first, std::string last, std::string infoName, std::string infoVal) {
    // Find the contact
    Contact* target = nullptr;
    for (Contact* cur = headContactList; cur != nullptr; cur = cur->next) {
        if (cur->first == first && cur->last == last) {
            target = cur;
            break;
        }
    }

    if (target == nullptr) {
        // Contact not found
        return false;
    }

    // Check for existing info
    Info* prevInfo = nullptr;
    for (Info* cur = target->headInfoList; cur != nullptr; prevInfo = cur, cur = cur->next) {
        if (cur->name == infoName) {
            // Info exists, update value
            cur->value = infoVal;
            return true;
        }
        if (cur->name > infoName) {
            break;
        }
    }

    // Create new info
    Info* newInfo = new Info(infoName, infoVal);

    if (prevInfo == nullptr) {
        // Insert at the beginning
        newInfo->next = target->headInfoList;
        target->headInfoList = newInfo;
    } else {
        // Insert after prevInfo
        newInfo->next = prevInfo->next;
        prevInfo->next = newInfo;
    }

    return true;
}

// remove the contact and its info from the list
// 1. return false and do nothing if the contact is not in the list
// 2. otherwise return true and remove the contact and its info
// - do not forget to update count
bool ContactList::removeContact(std::string first, std::string last) {
  // Check if the list is empty
    if (this->headContactList == nullptr) {
        return false; // List is empty, contact not found
    }

    Contact *temp = this->headContactList;
    Contact *prev = nullptr;

    // Traverse the list to find the contact
    while (temp != nullptr) {
        if (temp->first == first && temp->last == last) {
            // Contact found, remove it from the list
            if (prev == nullptr) {
                // The contact to remove is the head of the list
                this->headContactList = temp->next;
            } else {
                // The contact to remove is in the middle or end of the list
                prev->next = temp->next;
            }

            // Delete the contact's info list
            Info *infoTemp = temp->headInfoList;
            while (infoTemp != nullptr) {
                Info *nextInfo = infoTemp->next;
                delete infoTemp;
                infoTemp = nextInfo;
            }

            // Delete the contact
            delete temp;

            // Decrease the count of contacts
            this->count--;

            return true;
        }

        prev = temp;
        temp = temp->next;
    }

    // Contact not found
    return false;
}

// remove the info from a contact's info list
// 1. return false and do nothing if the contact is not in the list
// 2. return false and do nothing if the info is not in the contact's info list
// 3. otherwise return true and remove the info from the contact's list
bool ContactList::removeInfo(std::string first, std::string last, std::string infoName) {
    // Find the target contact
    Contact *target = headContactList;
    while (target != nullptr && !(target->first == first && target->last == last)) {
        target = target->next;
    }
    
    if (target == nullptr) {
        // Contact not found
        return false;
    }
    
    Info *prevInfo = nullptr, *curInfo = target->headInfoList;
    // Find the info to remove
    while (curInfo != nullptr) {
        if (curInfo->name == infoName) {
            break;
        }
        prevInfo = curInfo;
        curInfo = curInfo->next;
    }
    
    if (curInfo == nullptr) {
        // Info not found
        return false;
    }
    
    // Remove info from the list
    if (prevInfo == nullptr) {
        target->headInfoList = curInfo->next;
    } else {
        prevInfo->next = curInfo->next;
    }
    
    delete curInfo; // Delete the info node
    return true;
}

// destroy the list by removing all contacts and their infos
ContactList::~ContactList() {
    Contact *cur = headContactList, *nextContact;
    while (cur != nullptr) {
        nextContact = cur->next;
        Info *curInfo = cur->headInfoList, *nextInfo;
        while (curInfo != nullptr) {
            nextInfo = curInfo->next;
            delete curInfo; // Free each Info node
            curInfo = nextInfo;
        }
        delete cur; // Free each Contact node
        cur = nextContact;
    }
}
// deep copy the source list
// - do not forget to update count
ContactList::ContactList(const ContactList &src) {
    headContactList = nullptr;
    count = src.count;
    Contact **lastPtr = &headContactList;
    for (Contact *srcCur = src.headContactList; srcCur != nullptr; srcCur = srcCur->next) {
        *lastPtr = new Contact(srcCur->first, srcCur->last);
        Info **lastInfoPtr = &((*lastPtr)->headInfoList);
        for (Info *srcInfo = srcCur->headInfoList; srcInfo != nullptr; srcInfo = srcInfo->next) {
            *lastInfoPtr = new Info(srcInfo->name, srcInfo->value);
            lastInfoPtr = &((*lastInfoPtr)->next);
        }
        lastPtr = &((*lastPtr)->next);
    }
}
// remove all contacts and their info then deep copy the source list
// - do not forget to update count
const ContactList &ContactList::operator=(const ContactList &src) {
    if (this != &src) { // Protect against self-assignment
        // Clear current list
        this->~ContactList(); // Reuse destructor to free current memory

        // Deep copy from src
        headContactList = nullptr;
        count = src.count;
        Contact **lastPtr = &headContactList;
        for (Contact *srcCur = src.headContactList; srcCur != nullptr; srcCur = srcCur->next) {
            *lastPtr = new Contact(srcCur->first, srcCur->last);
            Info **lastInfoPtr = &((*lastPtr)->headInfoList);
            for (Info *srcInfo = srcCur->headInfoList; srcInfo != nullptr; srcInfo = srcInfo->next) {
                *lastInfoPtr = new Info(srcInfo->name, srcInfo->value);
                lastInfoPtr = &((*lastInfoPtr)->next);
            }
            lastPtr = &((*lastPtr)->next);
        }
    }
    return *this;
}
