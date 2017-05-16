#include <assert.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <gumbo.h>
#include <Yoga.h>
#include <YGNodeList.h>
// #include <SDL2/SDL.h>
#include <SDL2/SDL_gpu.h>
#include <stdio.h>
#include <vector>

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

    return element == NULL ? nullptr : element;
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

    return element == NULL ? nullptr : element;
}

static const char* get_attribute(GumboNode* node, const char* attr_name) {
  GumboAttribute* attr;
  if (node->type == GUMBO_NODE_ELEMENT) {
      attr = gumbo_get_attribute(&node->v.element.attributes, attr_name);
  }
  return attr ? attr->value : nullptr;
}

static int get_attribute_as_int(GumboNode* node, const char* attr_name) {
    auto value = get_attribute(node, attr_name);
    return value ? std::stoi(value) : 0;
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

int main(int argc, char* argv[]) {

    int SCREEN_WIDTH = 640;
    int SCREEN_HEIGHT = 480;

    SDL_Window *window;
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow(
        "An SDL2 window",                  // window title
        SDL_WINDOWPOS_UNDEFINED,           // initial x position
        SDL_WINDOWPOS_UNDEFINED,           // initial y position
        SCREEN_WIDTH,                      // width, in pixels
        SCREEN_HEIGHT,                     // height, in pixels
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_BORDERLESS
    );
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

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

    SDL_Event e;
    bool quit = false;
    while (!quit){
        while (SDL_PollEvent(&e)){
            if (e.type == SDL_QUIT){
                quit = true;
            }
            if (e.type == SDL_KEYDOWN){
                quit = true;
            }
            if (e.type == SDL_MOUSEBUTTONDOWN){
                quit = true;
            }
        }
        //Render the scene
        SDL_RenderClear(renderer);
        SDL_Rect fillRect = { SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
        SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF); 
        SDL_RenderFillRect(renderer, &fillRect);
        SDL_RenderPresent(renderer);
    }
    
    SDL_DestroyWindow(window);

    return 0;
}