#include "C:/Users/User/Desktop/HIS/include/Guest.h"

#include <sstream>

Guest::Guest(const std::string& name, const std::string& phone,
    const std::string& email, const std::string& notes)
    : name(name), phone(phone), email(email), notes(notes) {}

bool Guest::hasName() const {
    return !name.empty();
}

bool Guest::hasPhone() const {
    return !phone.empty();
}

bool Guest::hasEmail() const {
    return !email.empty();
}

const std::string& Guest::getName() const {
    return name;
}

const std::string& Guest::getPhone() const {
    return phone;
}

const std::string& Guest::getEmail() const {
    return email;
}

const std::string& Guest::getNotes() const {
    return notes;
}

void Guest::setName(const std::string& newName) {
    name = newName;
}

void Guest::setPhone(const std::string& newPhone) {
    phone = newPhone;
}

void Guest::setEmail(const std::string& newEmail) {
    email = newEmail;
}

void Guest::setNotes(const std::string& newNotes) {
    notes = newNotes;
}

std::string Guest::toShortString() const {
    if (!name.empty()) {
        return name;
    }
    std::ostringstream ss;
    ss << "<Unnamed>";
    return ss.str();
}

std::string Guest::toContactString() const {
    std::ostringstream ss;
    if (!name.empty()) {
        ss << name;
    }
    if (!phone.empty()) {
        ss << " | Phone: " << phone;
    }
    if (!email.empty()) {
        ss << " | Email: " << email;
    }
    return ss.str();
}

bool Guest::operator==(const Guest& other) const {
    return name == other.name
        && phone == other.phone
        && email == other.email
        && notes == other.notes;
}

bool Guest::operator!=(const Guest& other) const {
    return !(*this == other);
}


