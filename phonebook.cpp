#ifndef PHONEBOOK_H_
#define PHONEBOOK_H_

#include <string>
#include <vector>
#include "contact.h"

struct PhoneBook {
    std::vector<Contact> contacts;
    std::string filename;
};

PhoneBook CreatePhoneBook(const std::string& filename);

bool LoadPhoneBookFromFile(PhoneBook* phone_book);

bool SavePhoneBookToFile(const PhoneBook& phone_book);

bool ExportPhoneBookToFile(const PhoneBook& phone_book,
    const std::string& export_filename);

bool ImportContactsToPhoneBook(PhoneBook* phone_book,
    const std::string& import_filename);

void AddContactToPhoneBook(PhoneBook* phone_book);

void RemoveContactFromPhoneBook(PhoneBook* phone_book);

void EditContactInPhoneBook(PhoneBook* phone_book);

void SearchInPhoneBook(const PhoneBook& phone_book);

void SortPhoneBook(PhoneBook* phone_book);

void DisplayAllContacts(const PhoneBook& phone_book);

void DisplayMainMenu(const PhoneBook& phone_book);

void DisplayFileMenu();

void HandleFileMenu(PhoneBook* phone_book);

#endif  // PHONEBOOK_H_
