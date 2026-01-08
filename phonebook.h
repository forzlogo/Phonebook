#ifndef PHONEBOOK_H
#define PHONEBOOK_H
#include <string>
#include <vector>
#include "contact.h"

class PhoneBook {
public:
    explicit PhoneBook(const std::string& filename = "contacts.txt");

    void AddContact();
    void RemoveContact();
    void EditContact();
    void SearchByName() const;
    void SortByName();
    void DisplayAll() const;
    int GetContactCount() const;

    bool SaveToFile() const;
    bool LoadFromFile();
    bool ExportToFile(const std::string& export_filename) const;
    bool ImportFromFile(const std::string& import_filename);

    void SetFilename(const std::string& filename);
    std::string GetFilename() const;
private:
    bool IsValidEmail(const std::string& email) const;
    bool IsValidPhone(const std::string& phone) const;

    std::vector<Contact> contacts_;
    std::string filename_;
};
#endif