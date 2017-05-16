#include <assert.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <gumbo.h>
#include <Yoga.h>
#include <YGNodeList.h>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

using std::cout;
using std::string;
using std::vector;
using std::tuple;
using std::make_tuple;

// #define puts(...) cout << __VA_ARGS__ << "\n"

vector<GumboNode> boxes;

static GumboNode* first_child_of_type(const GumboNode* root, int const tag) {
    assert(root->type == GUMBO_NODE_ELEMENT);
    assert(root->v.element.children.length >= 1);

    const GumboVector* root_children = &root->v.element.children;
    GumboNode* element = NULL;

    for (int i = 0; i < root_children->length; ++i) {
        GumboNode* child = (GumboNode*) root_children->data[i];
        if (child->type == GUMBO_NODE_ELEMENT && child->v.element.tag == tag) {
            element = child;
            break;
        }
    }

    if (element != NULL)
        return element;

    return nullptr;
}

static GumboNode* first_node_of_type(GumboNode* root, int const node_type) {
    assert(root->type == GUMBO_NODE_ELEMENT);
    assert(root->v.element.children.length >= 1);

    const GumboVector* root_children = &root->v.element.children;
    GumboNode* element = NULL;

    for (int i = 0; i < root_children->length; ++i) {
        GumboNode* child = (GumboNode*) root_children->data[i];
        if (child->type == node_type) {
            element = child;
            break;
        }
    }

    if (element != NULL)
        return element;

    return nullptr;
}

static const char* get_attribute(GumboNode* node, const char* attr_name) {
  GumboAttribute* attr;
  if (node->type == GUMBO_NODE_ELEMENT &&
        (attr = gumbo_get_attribute(&node->v.element.attributes, attr_name))) {
    return attr->value;
  }
  return nullptr;
}

static int get_attribute_as_int(GumboNode* node, const char* attr_name) {
    auto value = get_attribute(node, attr_name);
    if (value) {
        return std::stoi(value);
    } else {
        return 0;
    }
}

static sf::Color get_color(GumboNode* node) {
    auto val = get_attribute(node, "color");
    if (val) {
        auto value = string(val);
        if (value.compare("black") == 0)   return sf::Color::Black;
        if (value.compare("white") == 0)   return sf::Color::White;
        if (value.compare("red") == 0)     return sf::Color::Red;
        if (value.compare("green") == 0)   return sf::Color::Green;
        if (value.compare("blue") == 0)    return sf::Color::Blue;
        if (value.compare("yellow") == 0)  return sf::Color::Yellow;
        if (value.compare("magenta") == 0) return sf::Color::Magenta;
        if (value.compare("cyan") == 0)    return sf::Color::Cyan;
    }
    return sf::Color::Cyan;
}

static string get_tagname(GumboNode *node) {
    string tagname = "";
    GumboStringPiece ts = node->v.element.original_tag;
    gumbo_tag_from_original_text(&ts);
    return string(ts.data, ts.length);
}

static const char* get_text(GumboNode *node) {
    GumboNode* text = first_node_of_type(node, GUMBO_NODE_TEXT);
    return text->v.text.text;
}

string read_file(string filename) {
    std::ifstream in(filename, std::ios::in);
    std::stringstream s;
    s << in.rdbuf();
    return s.str();
}

tuple<int, int> get_viewport_size() {
    GLint dims[4] = {0};
    glGetIntegerv(GL_VIEWPORT, dims);
    int w = (int) dims[2];
    int h = (int) dims[3];
    return make_tuple(w, h);
}

void RenderThread(sf::RenderWindow *window) {
    window->clear();
    window->resetGLStates();
    sf::View view = window->getView();
    sf::Vector2u window_size = window->getSize();

    // auto handle = window->getSystemHandle();

    // sf::IntRect vp = window->getViewport(view);
    // puts(window_size.x);
    int window_width;
    int window_height;
    std::tie(window_width, window_height) = get_viewport_size();

    // int window_width = (int*) fbWidth;
    // int window_height = (int*) fbHeight;

    // cout << window_width;

    view.setSize(window_width, window_height);

    YGNodeRef root = YGNodeNew();
    YGNodeStyleSetWidth(root, window_width);
    YGNodeStyleSetHeight(root, window_height);
    YGNodeStyleSetFlexDirection(root, YGFlexDirectionRow);
    YGNodeStyleSetPadding(root, YGEdgeAll, 10);
    YGNodeStyleSetFlexWrap(root, YGWrapWrap);
    YGNodeStyleSetJustifyContent(root, YGJustifyCenter);

    for (int i = 0; i < boxes.size(); ++i) {
        GumboNode child = boxes[i];
        if (child.type == GUMBO_NODE_ELEMENT) {
            int width   = 100;
            int height  = 100;
            int padding = 0;
            int margin  = 10;
            // layout
            YGNodeRef ynode = YGNodeNew();
            YGNodeStyleSetWidth(ynode, width);
            YGNodeStyleSetHeight(ynode, height);
            YGNodeStyleSetAlignSelf(ynode, YGAlignCenter);
            YGNodeStyleSetMargin(ynode, YGEdgeAll, margin);
            YGNodeInsertChild(root, ynode, i);
        }
    }

    YGNodeCalculateLayout(root, window_width, window_height, YGDirectionLTR);

    const int childCount = YGNodeGetChildCount(root);
    for (int i = 0; i < childCount; i++) {
        const YGNodeRef child = YGNodeGetChild(root, i);
        // drawing
        sf::RectangleShape box(sf::Vector2f(0, 0));
        box.setFillColor(sf::Color::Cyan);
        float left = YGNodeLayoutGetLeft(child);
        float top = YGNodeLayoutGetTop(child);
        float w = YGNodeLayoutGetWidth(child);
        float h = YGNodeLayoutGetHeight(child);
        box.setPosition(left, top);
        box.setSize(sf::Vector2f(w, h));
        window->draw(box);
    }
    
    window->display();
}

int main(int argc, char* argv[]) {
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(640, 480), "HTML thing", sf::Style::Default, settings);
    // window.setFramerateLimit(60);
    window.setVerticalSyncEnabled(true);
    sf::View view(window.getDefaultView());

    sf::Font font;
    if (!font.loadFromFile("PanicSans.ttf"))
        return EXIT_FAILURE;
    sf::Text text("Hello world", font, 50);

    assert(argv[1]);

    cout << "Loading: " << argv[1] << "\n";
    string coisa = read_file(argv[1]);
    const char* source = coisa.c_str();

    GumboOutput* output = gumbo_parse(source);
    GumboNode* body = first_child_of_type(output->root, GUMBO_TAG_BODY);
    assert(body != NULL);

    const GumboVector* elements = &body->v.element.children;

    for (int i = 0; i < elements->length; ++i) {
        GumboNode* child = (GumboNode*) elements->data[i];
        if (child->type == GUMBO_NODE_ELEMENT) {
            boxes.push_back(*child);
        }
    }

    // string tagname = "";
    // GumboStringPiece ts = *&first->v.element.original_tag;
    // gumbo_tag_from_original_text(&ts);
    // tagname = string(ts.data, ts.length);

    // cout << tagname << "\n";
    // cout << gumbo_normalized_tagname(first->v.element.tag) << "\n";

    // sf::Thread render(&RenderThread, &window);
    // render.launch();

    window.display(); // stops screen from flickering on load

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                boxes.clear();
                gumbo_destroy_output(&kGumboDefaultOptions, output);
            } else
            if (event.type == sf::Event::Resized) {
                sf::Vector2u window_size = window.getSize();
                window.setView(view = sf::View(sf::FloatRect(0.f, 0.f, window_size.x, window_size.y)));
            }
        }
        window.clear();
        sf::View view = window.getView();
        sf::Vector2u window_size = window.getSize();

        int window_width;
        int window_height;
        std::tie(window_width, window_height) = get_viewport_size();

        // view.setSize(window_width, window_height);

        YGNodeRef root = YGNodeNew();
        YGNodeStyleSetWidth(root, window_width);
        YGNodeStyleSetHeight(root, window_height);
        YGNodeStyleSetFlexDirection(root, YGFlexDirectionRow);
        YGNodeStyleSetPadding(root, YGEdgeAll, 10);
        YGNodeStyleSetFlexWrap(root, YGWrapWrap);
        YGNodeStyleSetJustifyContent(root, YGJustifyCenter);

        // layout
        for (int i = 0; i < boxes.size(); ++i) {
            GumboNode child = boxes[i];
            if (child.type == GUMBO_NODE_ELEMENT) {
                auto style_w = get_attribute_as_int(&child, "width");
                auto style_h = get_attribute_as_int(&child, "height");
                int width   = style_w ? style_w : 100;
                int height  = style_h ? style_h : 100;
                int padding = 0;
                int margin  = 10;
                YGNodeRef ynode = YGNodeNew();
                YGNodeStyleSetWidth(ynode, width);
                YGNodeStyleSetHeight(ynode, height);
                YGNodeStyleSetAlignSelf(ynode, YGAlignCenter);
                YGNodeStyleSetMargin(ynode, YGEdgeAll, margin);
                YGNodeInsertChild(root, ynode, i);
            }
        }

        YGNodeCalculateLayout(root, window_width, window_height, YGDirectionLTR);

        // drawing
        const int childCount = YGNodeGetChildCount(root);
        for (int i = 0; i < childCount; i++) {
            const YGNodeRef child = YGNodeGetChild(root, i);
            auto childbox = &boxes[i];
            sf::RectangleShape box(sf::Vector2f(0, 0));
            box.setFillColor(get_color(childbox));
            float left = YGNodeLayoutGetLeft(child);
            float top = YGNodeLayoutGetTop(child);
            float w = YGNodeLayoutGetWidth(child);
            float h = YGNodeLayoutGetHeight(child);
            box.setPosition(left, top);
            box.setSize(sf::Vector2f(w, h));
            window.draw(box);
            sf::Text text(get_text(childbox), font, 28);
            text.setFillColor(sf::Color::Black);
            text.setPosition(left, top);
            window.draw(text);
        }
        
        window.display();
    }

    // render.wait();

    return 0;
}