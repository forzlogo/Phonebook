#include "phonebook.h"
#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <windows.h>

PhoneBook::PhoneBook(const std::string& filename) : filename_(filename) {
    LoadFromFile();
}

void PhoneBook::AddContact() {
    std::string name, phone, email;

    std::cout << "\n=== ДОБАВЛЕНИЕ КОНТАКТА ===\n";

    std::cout << "Имя: ";
    std::getline(std::cin, name);

    std::cout << "Телефон: ";
    std::getline(std::cin, phone);

    while (!IsValidPhone(phone)) {
        std::cout << "Неверный телефон. Еще раз: ";
        std::getline(std::cin, phone);
    }

    std::cout << "Email: ";
    std::getline(std::cin, email);

    while (!IsValidEmail(email)) {
        std::cout << "Неверный email. Еще раз: ";
        std::getline(std::cin, email);
    }

    contacts_.emplace_back(name, phone, email);
    SaveToFile();
    std::cout << "Контакт добавлен!\n";
}

void PhoneBook::RemoveContact() {
    if (contacts_.empty()) {
        std::cout << "Справочник пуст!\n";
        return;
    }

    DisplayAll();
    std::cout << "Номер для удаления (1-" << contacts_.size() << "): ";

    int index;
    std::cin >> index;

    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(1000, '\n');
        std::cout << "Неверный ввод!\n";
        return;
    }

    if (index >= 1 && index <= static_cast<int>(contacts_.size())) {
        contacts_.erase(contacts_.begin() + index - 1);
        SaveToFile();
        std::cout << "Контакт удален!\n";
    }
    else {
        std::cout << "Неверный номер!\n";
    }
    std::cin.ignore(1000, '\n');
}

void PhoneBook::EditContact() {
    if (contacts_.empty()) {
        std::cout << "Справочник пуст!\n";
        return;
    }

    DisplayAll();
    std::cout << "Номер для редактирования (1-" << contacts_.size() << "): ";

    int index;
    std::cin >> index;

    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(1000, '\n');
        std::cout << "Неверный ввод!\n";
        return;
    }

    if (index >= 1 && index <= static_cast<int>(contacts_.size())) {
        Contact& contact = contacts_[index - 1];
        std::string name, phone, email;

        std::cin.ignore(1000, '\n');

        std::cout << "Текущее имя: " << contact.GetName() << "\n";
        std::cout << "Новое имя (Enter - оставить): ";
        std::getline(std::cin, name);
        if (!name.empty()) contact.SetName(name);

        std::cout << "Текущий телефон: " << contact.GetPhone() << "\n";
        std::cout << "Новый телефон: ";
        std::getline(std::cin, phone);
        while (!phone.empty() && !IsValidPhone(phone)) {
            std::cout << "Неверный телефон. Еще раз: ";
            std::getline(std::cin, phone);
        }
        if (!phone.empty()) contact.SetPhone(phone);

        std::cout << "Текущий email: " << contact.GetEmail() << "\n";
        std::cout << "Новый email: ";
        std::getline(std::cin, email);
        while (!email.empty() && !IsValidEmail(email)) {
            std::cout << "Неверный email. Еще раз: ";
            std::getline(std::cin, email);
        }
        if (!email.empty()) contact.SetEmail(email);

        SaveToFile();
        std::cout << "Контакт изменен!\n";
    }
    else {
        std::cout << "Неверный номер!\n";
    }
}

void PhoneBook::SearchByName() const {
    if (contacts_.empty()) {
        std::cout << "Справочник пуст!\n";
        return;
    }

    std::string search_string;
    std::cout << "Поиск: ";
    std::getline(std::cin, search_string);

    bool found = false;
    std::cout << "\n=== РЕЗУЛЬТАТЫ ===\n";

    for (size_t i = 0; i < contacts_.size(); ++i) {
        if (contacts_[i].ContainsName(search_string)) {
            std::cout << i + 1 << ". ";
            contacts_[i].Display();
            found = true;
        }
    }

    if (!found) {
        std::cout << "Не найдено!\n";
    }
}

void PhoneBook::SortByName() {
    std::sort(contacts_.begin(), contacts_.end());
    SaveToFile();
    std::cout << "Отсортировано!\n";
}

void PhoneBook::DisplayAll() const {
    if (contacts_.empty()) {
        std::cout << "Справочник пуст!\n";
        return;
    }

    std::cout << "\n=== ВСЕ КОНТАКТЫ (" << contacts_.size() << ") ===\n";
    for (size_t i = 0; i < contacts_.size(); ++i) {
        std::cout << i + 1 << ". ";
        contacts_[i].Display();
    }
}

int PhoneBook::GetContactCount() const {
    return static_cast<int>(contacts_.size());
}

bool PhoneBook::SaveToFile() const {
    std::ofstream file(filename_);
    if (!file.is_open()) {
        return false;
    }

    for (const auto& contact : contacts_) {
        file << contact << "\n";
    }

    file.close();
    return true;
}

bool PhoneBook::LoadFromFile() {
    std::ifstream file(filename_);
    if (!file.is_open()) {
        return false;
    }

    contacts_.clear();
    std::string line;

    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string name, phone, email;

        if (std::getline(ss, name, ',') &&
            std::getline(ss, phone, ',') &&
            std::getline(ss, email)) {
            contacts_.emplace_back(name, phone, email);
        }
    }

    file.close();
    return true;
}

bool PhoneBook::ExportToFile(const std::string& export_filename) const {
    std::ofstream file(export_filename);
    if (!file.is_open()) {
        std::cout << "Ошибка открытия файла!\n";
        return false;
    }

    for (const auto& contact : contacts_) {
        file << contact << "\n";
    }

    file.close();
    std::cout << "Экспортировано: " << contacts_.size() << " контактов\n";
    return true;
}

bool PhoneBook::ImportFromFile(const std::string& import_filename) {
    std::ifstream file(import_filename);
    if (!file.is_open()) {
        std::cout << "Ошибка открытия файла!\n";
        return false;
    }

    std::string line;
    int imported_count = 0;

    while (std::getline(file, line)) {
        if (line.empty()) continue;

        size_t start = line.find_first_not_of(" \t");
        size_t end = line.find_last_not_of(" \t");
        if (start != std::string::npos && end != std::string::npos) {
            line = line.substr(start, end - start + 1);
        }

        size_t first_comma = line.find(',');
        size_t second_comma = line.find(',', first_comma + 1);

        if (first_comma != std::string::npos && second_comma != std::string::npos) {
            std::string name = line.substr(0, first_comma);
            std::string phone = line.substr(first_comma + 1, second_comma - first_comma - 1);
            std::string email = line.substr(second_comma + 1);

            name.erase(0, name.find_first_not_of(" \t"));
            name.erase(name.find_last_not_of(" \t") + 1);
            phone.erase(0, phone.find_first_not_of(" \t"));
            phone.erase(phone.find_last_not_of(" \t") + 1);
            email.erase(0, email.find_first_not_of(" \t"));
            email.erase(email.find_last_not_of(" \t") + 1);

            contacts_.emplace_back(name, phone, email);
            imported_count++;
        }
    }

    file.close();
    SaveToFile();
    std::cout << "Импортировано: " << imported_count << " контактов\n";
    return true;
}

void PhoneBook::SetFilename(const std::string& filename) {
    filename_ = filename;
    LoadFromFile();
}

std::string PhoneBook::GetFilename() const {
    return filename_;
}

bool PhoneBook::IsValidEmail(const std::string& email) const {
    if (email.empty()) return true;

    size_t at_pos = email.find('@');
    if (at_pos == std::string::npos || at_pos == 0 ||
        at_pos == email.length() - 1) {
        return false;
    }

    size_t dot_pos = email.find('.', at_pos);
    if (dot_pos == std::string::npos || dot_pos == at_pos + 1 ||
        dot_pos == email.length() - 1) {
        return false;
    }

    return true;
}

bool PhoneBook::IsValidPhone(const std::string& phone) const {
    if (phone.empty()) return true;

    int digit_count = 0;
    for (char c : phone) {
        if (std::isdigit(static_cast<unsigned char>(c))) {
            ++digit_count;
        }
    }
    return digit_count >= 7;
}