#ifndef PHONEBOOK_FILE_OPERATIONS_H_
#define PHONEBOOK_FILE_OPERATIONS_H_

#include <string>
#include <vector>
#include "contact.h"

std::vector<Contact> ReadContactsFromFile(const std::string& filename,
    int* loaded_count,
    int* error_count);

bool WriteContactsToFile(const std::string& filename,
    const std::vector<Contact>& contacts);

#endif
