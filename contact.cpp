#include "contact.h"
#include <algorithm>
#include <cctype>
#include <iostream>

Contact CreateContact(const std::string& name,
    const std::string& phone,
    const std::string& email) {
    Contact contact;
    contact.name = name;
    contact.phone = phone;
    contact.email = email;
    return contact;
}

void DisplayContact(const Contact& contact) {
    std::cout << "Имя: " << contact.name
        << "\nТелефон: " << contact.phone
        << "\nEmail: " << contact.email
        << "\n-------------------------\n";
}

bool CompareContactsByName(const Contact& a, const Contact& b) {
    return a.name < b.name;
}

bool ContactContainsName(const Contact& contact,
    const std::string& search_string) {
    std::string lower_name = contact.name;
    std::string lower_search = search_string;

    for (char& c : lower_name) {
        if (c >= 'A' && c <= 'Z') c = c - 'A' + 'a';
    }
    for (char& c : lower_search) {
        if (c >= 'A' && c <= 'Z') c = c - 'A' + 'a';
    }

    return lower_name.find(lower_search) != std::string::npos;
}
