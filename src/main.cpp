#include <iostream>
#include <nlohmann/json.hpp>

#include <fstream>
#include <string>

#include "ftxui/dom/elements.hpp"
#include "ftxui/screen/screen.hpp"
#include "ftxui/screen/string.hpp"

using json = nlohmann::json;

// Global variables
std::string notesFile = "notes.json";
json notes;

// Function to load notes from json file
void loadNotes() {
    std::ifstream notesStream(notesFile);
    if (notesStream.is_open()) {
        notesStream >> notes;
    } else {
        std::cout << "Error opening notes file." << std::endl;
    }
}

// Function to save notes to json file
void saveNotes() {
    std::ofstream notesStream(notesFile);
    if (notesStream.is_open()) {
        notesStream << notes.dump();
    } else {
        std::cout << "Error saving notes to file." << std::endl;
    }
}

// Function to create a new note
void createNote() {
    std::string title;
    std::string body;

    std::cout << "Enter note title: ";
    std::cin >> title;
    std::cout << "Enter note body: ";
    std::cin >> body;

    notes[title] = body;

    saveNotes();
}

// Function to view a specific note
void viewNote() {
    std::string title;

    std::cout << "Enter note title: ";
    std::cin >> title;

    if (notes.count(title) > 0) {
        std::cout << "Note title: " << title << std::endl;
        std::cout << "Note body: " << notes[title] << std::endl;
    } else {
        std::cout << "Note not found." << std::endl;
    }
}

// Function to view all notes
void viewAllNotes() {
    for (json::iterator it = notes.begin(); it != notes.end(); ++it) {
        std::cout << "Title: " << it.key() << std::endl;
        std::cout << "Body: " << it.value() << std::endl;
    }
}

int main() {
    loadNotes();

    while (true) {
        int choice;

        std::cout << "1. Create Note" << std::endl;
        std::cout << "2. View Note" << std::endl;
        std::cout << "3. View All Notes" << std::endl;
        std::cout << "4. Exit" << std::endl;
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                createNote();
                break;
            case 2:
                viewNote();
                break;
            case 3:
                viewAllNotes();
                break;
            case 4:
                return 0;
            default:
                std::cout << "Invalid choice." << std::endl;
                break;
        }
    }

    return 0;
}
