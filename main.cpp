#include <iostream>
#include <limits>
#include <cstdlib>
#include "phonebook.h"

#ifdef _WIN32
#include <windows.h>
#endif

int main() {
#ifdef _WIN32
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
#endif

    std::cout << "=== ТЕЛЕФОННЫЙ СПРАВОЧНИК ===\n";
    PhoneBook phone_book = CreatePhoneBook("contacts.txt");
    LoadPhoneBookFromFile(&phone_book);

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
            AddContactToPhoneBook(&phone_book);
            break;

        case 2:
            RemoveContactFromPhoneBook(&phone_book);
            break;

        case 3:
            EditContactInPhoneBook(&phone_book);
            break;

        case 4:
            SearchInPhoneBook(phone_book);
            break;

        case 5:
            SortPhoneBook(&phone_book);
            break;

        case 6:
            DisplayAllContacts(phone_book);
            break;

        case 7:
            HandleFileMenu(&phone_book);
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
