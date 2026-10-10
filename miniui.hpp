#ifndef MINIUI_HPP
#define MINIUI_HPP

#include <map>
#include <string>
#include <utility>
#include <vector>

namespace miniui {

struct Node {
    std::string type;
    std::string text;
    std::map<std::string, std::string> props;
    std::vector<Node> children;
};

inline Node text(std::string value) {
    return Node{"text", std::move(value), {}, {}};
}

inline Node h(std::string type,
              std::vector<Node> children = {},
              std::map<std::string, std::string> props = {}) {
    return Node{std::move(type), "", std::move(props), std::move(children)};
}

inline Node label(std::string value) {
    return h("label", {text(std::move(value))});
}

inline std::string prop(const Node& node, const std::string& key,
                        const std::string& fallback = "") {
    const auto it = node.props.find(key);
    return it == node.props.end() ? fallback : it->second;
}

inline void render_into(const Node& node, std::string& out, int depth = 0) {
    const std::string indent(static_cast<std::size_t>(depth * 2), ' ');
    if (node.type == "text") {
        out += indent + node.text + "\n";
        return;
    }
    if (node.type == "title") {
        out += indent + "=== " + prop(node, "value", "MiniUI") + " ===\n";
    } else if (node.type == "button") {
        out += indent + "[ " + prop(node, "label", "button") + " ]\n";
    } else if (node.type == "panel") {
        out += indent + "----------------------------------------\n";
        for (const auto& child : node.children) render_into(child, out, depth + 1);
        out += indent + "----------------------------------------\n";
        return;
    } else if (node.type == "item") {
        const std::string mark = prop(node, "done") == "true" ? "[x] " : "[ ] ";
        out += indent + mark + prop(node, "value") + "\n";
    } else if (node.type == "spacer") {
        out += "\n";
    }
    for (const auto& child : node.children) render_into(child, out, depth + 1);
}

inline std::string render(const Node& root) {
    std::string output;
    render_into(root, output);
    return output;
}

} // namespace miniui
#endif
