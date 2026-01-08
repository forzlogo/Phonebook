#include "phonebook.h"
#include <iostream>
#include <limits>
#include <windows.h>

void DisplayMainMenu(const PhoneBook& phone_book) {
    std::cout << "\n=== TEЛEФOHНЫЙ CПPABOЧHИK ===\n";
    std::cout << "Файл: " << phone_book.GetFilename() << "\n";
    std::cout << "Koнтaктoв: " << phone_book.GetContactCount() << "\n";
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
    std::cout << "2. Экспорт в другой файл\n";
    std::cout << "3. Импорт из файла\n";
    std::cout << "4. Сменить файл по умолчанию\n";
    std::cout << "5. Вернуться в главное меню\n";
    std::cout << "Выберите действие: ";
}

void HandleFileMenu(PhoneBook& phone_book) {
    int choice = 0;

    while (choice != 5) {
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
            if (phone_book.SaveToFile()) {
                std::cout << "Файл сохранен: " << phone_book.GetFilename() << "\n";
            }
            break;

        case 2: {
            std::string filename;
            std::cout << "Имя файла для экспорта: ";
            std::getline(std::cin, filename);
            phone_book.ExportToFile(filename);
            break;
        }

        case 3: {
            std::string filename;
            std::cout << "Имя файла для импорта: ";
            std::getline(std::cin, filename);
            phone_book.ImportFromFile(filename);
            break;
        }

        case 4: {
            std::string filename;
            std::cout << "Текущий файл: " << phone_book.GetFilename() << "\n";
            std::cout << "Новое имя файла: ";
            std::getline(std::cin, filename);
            phone_book.SetFilename(filename);
            std::cout << "Файл изменен\n";
            break;
        }

        case 5:
            std::cout << "Возврат в меню...\n";
            break;

        default:
            std::cout << "Неверный выбор!\n";
        }

        if (choice != 5) {
            std::cout << "\nНажмите Enter...";
            std::cin.get();
        }
    }
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    std::cout << "=== TEЛEФOHНЫЙ CПPABOЧHИK ===\n";
    PhoneBook phone_book;

    int choice = 0;

    while (choice != 8) {
        system("cls");
        DisplayMainMenu(phone_book);
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
            phone_book.AddContact();
            break;

        case 2:
            phone_book.RemoveContact();
            break;

        case 3:
            phone_book.EditContact();
            break;

        case 4:
            phone_book.SearchByName();
            break;

        case 5:
            phone_book.SortByName();
            break;

        case 6:
            phone_book.DisplayAll();
            break;

        case 7:
            HandleFileMenu(phone_book);
            continue;

        case 8:
            std::cout << "Выход...\n";
            return 0;

        default:
            std::cout << "Неверный выбор!\n";
        }

        std::cout << "\nНажмите Enter...";
        std::cin.get();
    }

    return 0;
}