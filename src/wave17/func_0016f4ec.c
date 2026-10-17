#include <stddef.h>
#include <string.h>

#include "game/layout.h"
#include "game/screen.h"

struct LayoutDimensionNode {
    unsigned reserved00;
    struct LayoutListNode link;
    unsigned char reserved0c[0x3c];
    float width;
    float height;
};

struct LayoutDimensionList {
    unsigned char reserved00[0x10];
    struct LayoutListNode root;
};

struct LayoutContainer {
    unsigned char reserved00[0x10];
    struct LayoutDimensionList *dimensions;
};

void func_0016f4ec(const struct DisplayData *object, float *width, float *height)
{
    const struct LayoutDimensionList *list = object->parts[0]->container->dimensions;
    const struct LayoutListNode *root = &list->root;
    const struct LayoutDimensionNode *node = 0;
    if (root->next)
        node = (const struct LayoutDimensionNode *)
            ((const char *)root->previous - offsetof(struct LayoutDimensionNode, link));
    if (node) {
        float size[2];
        memcpy(size, &node->width, sizeof size);
        memcpy(width, &size[0], sizeof *width);
        memcpy(height, &size[1], sizeof *height);
    } else {
        *width = 0.0f;
        *height = 0.0f;
    }
}
