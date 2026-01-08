#include "contact.h"
#include <algorithm>
#include <cctype>
#include <windows.h>

Contact::Contact() : name_(""), phone_(""), email_("") {}

Contact::Contact(const std::string& name, const std::string& phone,
    const std::string& email)
    : name_(name), phone_(phone), email_(email) {
}

std::string Contact::GetName() const { return name_; }
std::string Contact::GetPhone() const { return phone_; }
std::string Contact::GetEmail() const { return email_; }

void Contact::SetName(const std::string& name) { name_ = name; }
void Contact::SetPhone(const std::string& phone) { phone_ = phone; }
void Contact::SetEmail(const std::string& email) { email_ = email; }

void Contact::Display() const {
    std::cout << "Имя: " << name_
        << "\nТелефон: " << phone_
        << "\nEmail: " << email_
        << "\n-------------------------\n";
}

bool Contact::ContainsName(const std::string& search_string) const {
    std::string lower_name = name_;
    std::string lower_search = search_string;

    for (char& c : lower_name) {
        if (c >= 'A' && c <= 'Z') c = c - 'A' + 'a';
    }
    for (char& c : lower_search) {
        if (c >= 'A' && c <= 'Z') c = c - 'A' + 'a';
    }

    return lower_name.find(lower_search) != std::string::npos;
}

bool Contact::operator<(const Contact& other) const {
    return name_ < other.name_;
}

std::ostream& operator<<(std::ostream& os, const Contact& contact) {
    os << contact.name_ << "," << contact.phone_ << "," << contact.email_;
    return os;
}

std::istream& operator>>(std::istream& is, Contact& contact) {
    std::getline(is, contact.name_, ',');
    std::getline(is, contact.phone_, ',');
    std::getline(is, contact.email_);
    return is;
}