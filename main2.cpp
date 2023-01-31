#include <iostream>
#include <nlohmann/json.hpp>

#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <functional>

#include "ftxui/dom/elements.hpp"
#include "ftxui/screen/screen.hpp"
#include "ftxui/screen/string.hpp"
#include "ftxui/component/captured_mouse.hpp"
#include "ftxui/component/screen_interactive.hpp"

#include "ftxui/component/component.hpp"
#include "ftxui/component/component_base.hpp"


using json = nlohmann::json;
using namespace ftxui;


// be able to categorize contacts
enum tags
{
    red = 0,
    orange,
    yellow,
    green,
    blue,
    purple,
};


Component Wrap(std::string name, Component component) {
  return Renderer(component, [name, component] {
    return hbox({
               text(name) | size(WIDTH, EQUAL, 8),
               separator(),
               component->Render() | xflex,
           }) |
           xflex;
  });
}

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
    
    auto screen = ScreenInteractive::FitComponent();
    std::string content ="";
    std::string placeholder = "placeholder";
    component input = Input(&content, &placeholder);
    screen.Loop(input);

    Element document = 
        window(text("Contact"),
            text("the elemnt") | border | flex,
        );



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
    
    auto layout = Container::Vertical({
      menu,
      toggle,
      checkboxes,
      radiobox,
      input,
      sliders,
      button,
    });

    auto component = Renderer(layout, [&] {
    return vbox({
               menu->Render(),
               separator(),
               toggle->Render(),
               separator(),
               checkboxes->Render(),
               separator(),
               radiobox->Render(),
               separator(),
               input->Render(),
               separator(),
               sliders->Render(),
               separator(),
               button->Render(),
           }) |
           xflex | size(WIDTH, GREATER_THAN, 40) | border;
    });

    screen.Loop(component);

    return 0;
}
