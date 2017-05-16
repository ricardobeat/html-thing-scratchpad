#include <Cello.h>
#include <assert.h>
#include <gumbo.h>
#include <SFML/System.h>
#include <SFML/Window.h>
#include <SFML/Graphics.h>

// static GumboNode* first_child_of_type(const GumboNode* root, int const tag) {
//     assert(root->type == GUMBO_NODE_ELEMENT);
//     assert(root->v.element.children.length >= 1);

//     const GumboVector* root_children = &root->v.element.children;
//     GumboNode* element = NULL;

//     for (int i = 0; i < root_children->length; ++i) {
//         GumboNode* child = (GumboNode*) root_children->data[i];
//         std::cout << child->v.element.tag << "\n";
//         if (child->type == GUMBO_NODE_ELEMENT && child->v.element.tag == tag) {
//             element = child;
//             break;
//         }
//     }

//     if (element != NULL)
//         return element;

//     return nullptr;
// }

int main() {

    sfVideoMode mode = {300, 300, 32};
    sfRenderWindow* window;
    sfFont* font;
    sfText* text;
    sfEvent event;

    window = sfRenderWindow_create(mode, "SFML window", sfResize | sfClose, NULL);

    if (!window) return 1;

    sfCircleShape shape(100, 64);
    shape.setFillColor(sf::Color::Green);

    font = sfFont_createFromFile("/System/Library/Fonts/Symbol.ttf");
    if (!font) return 1;
    text = sfText_create();
    sfText_setString(text, "Hello SFML");
    sfText_setFont(text, font);
    sfText_setCharacterSize(text, 50);

    GumboOutput* output = gumbo_parse("<bacon>Hello, World!</bacon>");

    GumboNode* body = first_child_of_type(output->root, GUMBO_TAG_BODY);
    assert(body != NULL);

    GumboNode* first = first_child_of_type(body, GUMBO_TAG_UNKNOWN);
    assert(first != NULL);

    // tagname = $(String, "");
    GumboStringPiece ts = *&first->v.element.original_tag;
    gumbo_tag_from_original_text(&ts);
    var tagname = $(String, ts.data);

    printf(tagname);
    // std::cout << tagname << "\n";
    // std::cout << gumbo_normalized_tagname(first->v.element.tag) << "\n";

    while (sfRenderWindow_isOpen(window)) {
        while (sfRenderWindow_pollEvent(window, &event)) {
            if (event.type == sfEvtClosed) {
                // gumbo_destroy_output(&kGumboDefaultOptions, output);
                sfRenderWindow_close(window);
            }
        }

        sfRenderWindow_drawText(window, text, NULL);

        sfRenderWindow_clear(window, sfBlack);
        sfRenderWindow_display(window);
    }

    return 0;
}