#ifndef CONTACT_H
#define CONTACT_H
#include <string>
#include <iostream>

class Contact {
public:
    Contact();
    Contact(const std::string& name, const std::string& phone,
        const std::string& email);
    std::string GetName() const;
    std::string GetPhone() const;
    std::string GetEmail() const;
    void SetName(const std::string& name);
    void SetPhone(const std::string& phone);
    void SetEmail(const std::string& email);

    void Display() const;

    bool ContainsName(const std::string& search_string) const;
    bool operator<(const Contact& other) const;

    friend std::ostream& operator<<(std::ostream& os, const Contact& contact);
    friend std::istream& operator>>(std::istream& is, Contact& contact);
private:
    std::string name_;
    std::string phone_;
    std::string email_;
};
#endif