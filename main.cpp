#include "miniui.hpp"

#include <algorithm>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

struct Todo {
    std::string title;
    bool done = false;
};

struct AppState {
    int counter = 0;
    std::vector<Todo> todos{{"Read the task brief", false}, {"Build a small UI framework", true}};
};

miniui::Node build_app(const AppState& state) {
    using miniui::h;
    using miniui::text;
    using miniui::Node;

    std::vector<Node> children;
    // Fixed: Passed attributes as the second argument
    children.push_back(h("title", {{"value", "MiniUI C++ — Demo App"}}));
    children.push_back(text("A tiny component-tree UI framework rendered in the terminal."));
    children.push_back(h("spacer"));

    children.push_back(h("panel", {
        h("title", {{"value", "Counter"}}),
        text("Current value: " + std::to_string(state.counter)),
        text("Commands: +  increment | -  decrement | r  reset")
    }));
    children.push_back(h("spacer"));

    std::vector<Node> todo_nodes;
    todo_nodes.push_back(h("title", {{"value", "To-do list"}}));
    if (state.todos.empty()) {
        todo_nodes.push_back(text("No tasks yet. Use 'a' to add one."));
    } else {
        for (std::size_t i = 0; i < state.todos.size(); ++i) {
            // Fixed: Passed item attributes as the second argument
            todo_nodes.push_back(h("item", {
                {"value", std::to_string(i + 1) + ". " + state.todos[i].title},
                {"done", state.todos[i].done ? "true" : "false"}
            }));
        }
    }
    todo_nodes.push_back(text("Commands: a  add | t  toggle task | d  delete task"));
    children.push_back(h("panel", std::move(todo_nodes)));
    children.push_back(h("spacer"));
    children.push_back(text("Global commands: h  help | q  quit"));
    return h("app", std::move(children));
}

void print_help() {
    std::cout << "\nCommands\n"
              << "  +  increase counter\n"
              << "  -  decrease counter\n"
              << "  r  reset counter\n"
              << "  a  add a to-do item\n"
              << "  t  toggle a to-do item (asks for its number)\n"
              << "  d  delete a to-do item (asks for its number)\n"
              << "  h  show this help\n"
              << "  q  quit\n\n";
}

bool read_index(std::size_t& index, const std::string& prompt, std::size_t max) {
    std::cout << prompt;
    long long entered = 0;
    if (!(std::cin >> entered)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input.\n";
        return false;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    if (entered < 1 || static_cast<unsigned long long>(entered) > max) {
        std::cout << "Please enter a number from 1 to " << max << ".\n";
        return false;
    }
    index = static_cast<std::size_t>(entered - 1);
    return true;
}

int main() {
    AppState state;
    std::cout << "MiniUI C++ framework demo\nType 'h' for commands.\n";
    std::string command;
    while (true) {
        std::cout << "\n" << miniui::render(build_app(state));
        std::cout << "> ";
        if (!std::getline(std::cin, command)) break;
        if (command == "q") break;
        if (command == "+") ++state.counter;
        else if (command == "-") --state.counter;
        else if (command == "r") state.counter = 0;
        else if (command == "a") {
            std::cout << "New task: ";
            std::string title;
            std::getline(std::cin, title);
            if (title.empty()) std::cout << "Task was empty; nothing added.\n";
            else if (title.size() > 120) std::cout << "Keep task titles to 120 characters or fewer.\n";
            else state.todos.push_back({title, false});
        } else if (command == "t") {
            std::size_t index = 0;
            if (!state.todos.empty() && read_index(index, "Task number to toggle: ", state.todos.size()))
                state.todos[index].done = !state.todos[index].done;
            else if (state.todos.empty()) std::cout << "There are no tasks to toggle.\n";
        } else if (command == "d") {
            std::size_t index = 0;
            if (!state.todos.empty() && read_index(index, "Task number to delete: ", state.todos.size()))
                state.todos.erase(state.todos.begin() + static_cast<std::ptrdiff_t>(index));
            else if (state.todos.empty()) std::cout << "There are no tasks to delete.\n";
        } else if (command == "h") print_help();
        else std::cout << "Unknown command. Type 'h' for help.\n";
    }
    std::cout << "Goodbye!\n";
    return 0;
}
