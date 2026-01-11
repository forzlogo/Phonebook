#include "file_operations.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include "contact.h"

std::vector<Contact> ReadContactsFromFile(const std::string& filename,
    int* loaded_count,
    int* error_count) {
    std::vector<Contact> contacts;
    *loaded_count = 0;
    *error_count = 0;

    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cout << "Файл '" << filename << "' не найден.\n";
        return contacts;
    }

    std::cout << "Чтение файла: " << filename << "\n";

    std::string line;
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

        std::stringstream ss(line);
        std::string name, phone, email;

        if (!std::getline(ss, name, ',') ||
            !std::getline(ss, phone, ',') ||
            !std::getline(ss, email)) {

            std::cout << "Строка " << line_number
                << ": некорректный формат\n";
            (*error_count)++;
            continue;
        }

        if (!name.empty() && name.front() == '"' && name.back() == '"') {
            name = name.substr(1, name.length() - 2);
        }

        if (name.empty()) {
            std::cout << "Строка " << line_number
                << ": пустое имя\n";
            (*error_count)++;
            continue;
        }

        contacts.push_back(CreateContact(name, phone, email));
        (*loaded_count)++;

        if ((*loaded_count) % 10 == 0) {
            std::cout << "Загружено " << *loaded_count << " контактов...\n";
        }
    }

    if (file.bad()) {
        std::cout << "Ошибка ввода/вывода при чтении файла\n";
        (*error_count)++;
    }

    file.close();

    return contacts;
}

bool WriteContactsToFile(const std::string& filename,
    const std::vector<Contact>& contacts) {
    std::ofstream file(filename);

    if (!file.is_open()) {
        std::cout << "Не удалось открыть файл для записи: "
            << filename << "\n";
        return false;
    }

    file << "# Телефонный справочник\n";
    file << "# Формат: Имя,Телефон,Email\n";
    file << "# Создано: " << __DATE__ << " " << __TIME__ << "\n\n";

    int saved_count = 0;
    for (const auto& contact : contacts) {
        file << contact.name << "," << contact.phone << "," << contact.email << "\n";
        saved_count++;

        if (saved_count % 20 == 0) {
            std::cout << "Сохранено " << saved_count << " контактов...\n";
        }
    }

    file.close();

    std::cout << "Успешно сохранено " << saved_count
        << " контактов в файл: " << filename << "\n";

    return true;
}
