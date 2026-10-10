# MiniUI C++ — Build Your Own UI Framework

A small C++17 proof-of-concept for Task 2. It demonstrates a component tree, a renderer, state updates, event/command handling, and a to-do/counter demo. It uses the terminal renderer option explicitly allowed by the task, so it needs no web server or third-party libraries.

## Build and run

Linux/macOS with g++:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp -o miniui_demo
./miniui_demo
```

Windows with MinGW:

```powershell
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp -o miniui_demo.exe
.\miniui_demo.exe
```

## Try it

- `+`, `-`, `r`: increment, decrement, or reset the counter
- `a`: add a to-do item
- `t`: toggle a to-do item's completed state
- `d`: delete a to-do item
- `h`: help
- `q`: quit

## Framework API

`miniui.hpp` defines a `Node` component tree, `h(type, children, props)` to describe components, and `render(root)` to render a tree to terminal text. `main.cpp` builds the UI from `AppState`; commands mutate the state and the app rebuilds and re-renders the tree. This illustrates the core UI loop:

1. Describe UI as nested components.
2. Render component tree.
3. Handle user input as an event.
4. Update application state.
5. Rebuild and render the UI again.

## Scope

This is an educational terminal UI framework, not a browser framework. The task explicitly permits a terminal UI instead of HTML. State exists in memory for the current run and is reset when the program exits. No external libraries are required.
