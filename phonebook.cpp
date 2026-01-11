#include "phonebook.h"
#include "validation.h"
#include "file_operations.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <cstdlib>
#include <cstring>

// Функция для проверки расширения .txt
bool IsTxtFile(const std::string& filename) {
    if (filename.length() < 4) return false;
    return filename.substr(filename.length() - 4) == ".txt";
}

// Функция для добавления расширения .txt если его нет
std::string EnsureTxtExtension(const std::string& filename) {
    if (IsTxtFile(filename)) {
        return filename;
    }
    return filename + ".txt";
}

PhoneBook CreatePhoneBook(const std::string& filename) {
    PhoneBook phone_book;
    phone_book.filename = EnsureTxtExtension(filename);
    return phone_book;
}

bool LoadPhoneBookFromFile(PhoneBook* phone_book) {
    int loaded_count = 0, error_count = 0;
    phone_book->contacts = ReadContactsFromFile(phone_book->filename, &loaded_count, &error_count);
    return loaded_count > 0 || error_count == 0;
}

bool SavePhoneBookToFile(const PhoneBook& phone_book) {
    return WriteContactsToFile(phone_book.filename, phone_book.contacts);
}

bool ExportPhoneBookToFile(const PhoneBook& phone_book,
    const std::string& export_filename) {

    std::string filename = EnsureTxtExtension(export_filename);

    // Проверяем, не пытаемся ли экспортировать в тот же файл
    if (filename == phone_book.filename) {
        std::cout << "Нельзя экспортировать в текущий файл. Используйте 'Сохранить в текущий файл'.\n";
        return false;
    }

    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cout << "Ошибка открытия файла " << filename << "!\n";
        return false;
    }

    for (const auto& contact : phone_book.contacts) {
        file << contact.name << "," << contact.phone << "," << contact.email << "\n";
    }

    file.close();
    std::cout << "Экспортировано " << phone_book.contacts.size()
        << " контактов в файл: " << filename << "\n";
    return true;
}

bool ImportContactsToPhoneBook(PhoneBook* phone_book,
    const std::string& import_filename) {

    std::string filename = EnsureTxtExtension(import_filename);

    // Проверяем, не пытаемся ли импортировать из того же файла
    if (filename == phone_book->filename) {
        std::cout << "Нельзя импортировать из текущего файла.\n";
        return false;
    }

    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Ошибка открытия файла " << filename << "!\n";
        return false;
    }

    std::string line;
    int imported_count = 0;
    int line_number = 0;

    while (std::getline(file, line)) {
        line_number++;

        if (line.empty() || line[0] == '#') {
            continue;
        }

        size_t start = line.find_first_not_of(" \t");
        size_t end = line.find_last_not_of(" \t");
        if (start == std::string::npos) {
            continue;
        }

        line = line.substr(start, end - start + 1);

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

            phone_book->contacts.push_back(CreateContact(name, phone, email));
            imported_count++;
        }
        else {
            std::cout << "Строка " << line_number
                << " в файле " << filename
                << ": некорректный формат, пропущена\n";
        }
    }

    file.close();

    if (imported_count > 0) {
        SavePhoneBookToFile(*phone_book);
        std::cout << "Импортировано " << imported_count
            << " контактов из файла: " << filename << "\n";
        return true;
    }
    else {
        std::cout << "Не удалось импортировать контакты из файла: " << filename << "\n";
        return false;
    }
}

void AddContactToPhoneBook(PhoneBook* phone_book) {
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

    phone_book->contacts.push_back(CreateContact(name, phone, email));
    SavePhoneBookToFile(*phone_book);
    std::cout << "Контакт добавлен!\n";
}

void RemoveContactFromPhoneBook(PhoneBook* phone_book) {
    if (phone_book->contacts.empty()) {
        std::cout << "Справочник пуст!\n";
        return;
    }

    DisplayAllContacts(*phone_book);
    std::cout << "Номер для удаления (1-" << phone_book->contacts.size() << "): ";

    int index;
    std::cin >> index;

    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(1000, '\n');
        std::cout << "Неверный ввод!\n";
        return;
    }

    if (index >= 1 && index <= static_cast<int>(phone_book->contacts.size())) {
        phone_book->contacts.erase(phone_book->contacts.begin() + index - 1);
        SavePhoneBookToFile(*phone_book);
        std::cout << "Контакт удален!\n";
    }
    else {
        std::cout << "Неверный номер!\n";
    }
    std::cin.ignore(1000, '\n');
}

void EditContactInPhoneBook(PhoneBook* phone_book) {
    if (phone_book->contacts.empty()) {
        std::cout << "Справочник пуст!\n";
        return;
    }

    DisplayAllContacts(*phone_book);
    std::cout << "Номер для редактирования (1-" << phone_book->contacts.size() << "): ";

    int index;
    std::cin >> index;

    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(1000, '\n');
        std::cout << "Неверный ввод!\n";
        return;
    }

    if (index >= 1 && index <= static_cast<int>(phone_book->contacts.size())) {
        Contact& contact = phone_book->contacts[index - 1];
        std::string name, phone, email;

        std::cin.ignore(1000, '\n');

        std::cout << "Текущее имя: " << contact.name << "\n";
        std::cout << "Новое имя (Enter - оставить): ";
        std::getline(std::cin, name);
        if (!name.empty()) contact.name = name;

        std::cout << "Текущий телефон: " << contact.phone << "\n";
        std::cout << "Новый телефон: ";
        std::getline(std::cin, phone);
        while (!phone.empty() && !IsValidPhone(phone)) {
            std::cout << "Неверный телефон. Еще раз: ";
            std::getline(std::cin, phone);
        }
        if (!phone.empty()) contact.phone = phone;

        std::cout << "Текущий email: " << contact.email << "\n";
        std::cout << "Новый email: ";
        std::getline(std::cin, email);
        while (!email.empty() && !IsValidEmail(email)) {
            std::cout << "Неверный email. Еще раз: ";
            std::getline(std::cin, email);
        }
        if (!email.empty()) contact.email = email;

        SavePhoneBookToFile(*phone_book);
        std::cout << "Контакт изменен!\n";
    }
    else {
        std::cout << "Неверный номер!\n";
    }
}

void SearchInPhoneBook(const PhoneBook& phone_book) {
    if (phone_book.contacts.empty()) {
        std::cout << "Справочник пуст!\n";
        return;
    }

    std::string search_string;
    std::cout << "Поиск: ";
    std::getline(std::cin, search_string);

    bool found = false;
    std::cout << "\n=== РЕЗУЛЬТАТЫ ===\n";

    for (size_t i = 0; i < phone_book.contacts.size(); ++i) {
        if (ContactContainsName(phone_book.contacts[i], search_string)) {
            std::cout << i + 1 << ". ";
            DisplayContact(phone_book.contacts[i]);
            found = true;
        }
    }

    if (!found) {
        std::cout << "Не найдено!\n";
    }
}

void SortPhoneBook(PhoneBook* phone_book) {
    std::sort(phone_book->contacts.begin(), phone_book->contacts.end(), CompareContactsByName);
    SavePhoneBookToFile(*phone_book);
    std::cout << "Отсортировано!\n";
}

void DisplayAllContacts(const PhoneBook& phone_book) {
    if (phone_book.contacts.empty()) {
        std::cout << "Справочник пуст!\n";
        return;
    }

    std::cout << "\n=== ВСЕ КОНТАКТЫ (" << phone_book.contacts.size() << ") ===\n";
    for (size_t i = 0; i < phone_book.contacts.size(); ++i) {
        std::cout << i + 1 << ". ";
        DisplayContact(phone_book.contacts[i]);
    }
}

void DisplayMainMenu(const PhoneBook& phone_book) {
    std::cout << "\n=== ТЕЛЕФОННЫЙ СПРАВОЧНИК ===\n";
    std::cout << "Файл: " << phone_book.filename << "\n";
    std::cout << "Контактов: " << phone_book.contacts.size() << "\n";
    std::cout << "===============================\n";
    std::cout << "1. Добавить контакт\n";
    std::cout << "2. Удалить контакт\n";
    std::cout << "3. Редактировать контакт\n";
    std::cout << "4. Поиск по имени\n";
    std::cout << "5. Сортировать по имени\n";
    std::cout << "6. Показать все контакты\n";
    std::cout << "7. Работа с файлами\n";
    std::cout << "8. Выход\n";
    std::cout << "Выберите действие: ";
}

void DisplayFileMenu() {
    std::cout << "\n=== РАБОТА С ФАЙЛАМИ ===\n";
    std::cout << "1. Сохранить в текущий файл\n";
    std::cout << "2. Экспорт в файл\n";
    std::cout << "3. Импорт из файла\n";
    std::cout << "4. Вернуться в главное меню\n";
    std::cout << "Выберите действие: ";
}

void HandleFileMenu(PhoneBook* phone_book) {
    int choice = 0;

    while (choice != 4) {
        system("cls");
        DisplayFileMenu();
        std::cin >> choice;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            std::cout << "Неверный ввод!\n";
            continue;
        }

        std::cin.ignore(1000, '\n');

        switch (choice) {
        case 1:
            if (SavePhoneBookToFile(*phone_book)) {
                std::cout << "Файл сохранен: " << phone_book->filename << "\n";
            }
            break;

        case 2: {
            std::string filename;
            std::cout << "Имя файла для экспорта: ";
            std::getline(std::cin, filename);
            if (!filename.empty()) {
                ExportPhoneBookToFile(*phone_book, filename);
            }
            else {
                std::cout << "Имя файла не может быть пустым!\n";
            }
            break;
        }

        case 3: {
            std::string filename;
            std::cout << "Имя файла для импорта: ";
            std::getline(std::cin, filename);
            if (!filename.empty()) {
                ImportContactsToPhoneBook(phone_book, filename);
            }
            else {
                std::cout << "Имя файла не может быть пустым!\n";
            }
            break;
        }

        case 4:
            std::cout << "Возврат в меню...\n";
            break;

        default:
            std::cout << "Неверный выбор!\n";
        }

        if (choice != 4) {
            std::cout << "\nНажмите Enter...";
            std::cin.get();
        }
    }
}
