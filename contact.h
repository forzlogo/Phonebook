#ifndef PHONEBOOK_CONTACT_H_
#define PHONEBOOK_CONTACT_H_

#include <string>

struct Contact {
    std::string name;
    std::string phone;
    std::string email;
};

Contact CreateContact(const std::string& name,
    const std::string& phone,
    const std::string& email);

void DisplayContact(const Contact& contact);

bool CompareContactsByName(const Contact& a, const Contact& b);

bool ContactContainsName(const Contact& contact,
    const std::string& search_string);

#endif
