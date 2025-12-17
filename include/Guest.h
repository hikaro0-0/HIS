#pragma once

#include <string>

class Guest {
public:
    Guest(const std::string& name = "", const std::string& phone = "",
        const std::string& email = "", const std::string& notes = "");

    bool hasName() const;
    bool hasPhone() const;
    bool hasEmail() const;

    const std::string& getName() const;
    const std::string& getPhone() const;
    const std::string& getEmail() const;
    const std::string& getNotes() const;

    void setName(const std::string& name);
    void setPhone(const std::string& phone);
    void setEmail(const std::string& email);
    void setNotes(const std::string& notes);

    std::string toShortString() const;
    std::string toContactString() const;

    bool operator==(const Guest& other) const;
    bool operator!=(const Guest& other) const;

private:
    std::string name;
    std::string phone;
    std::string email;
    std::string notes;
};


