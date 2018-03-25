#ifndef _PNG_H
#define _PNG_H

#include <DisplayCore.h>

extern "C" {
#include "utility/upng.h"
}

class PNG : public Image {
    private:
        const uint8_t *_data;
        size_t _length;

    public:
        PNG(const uint8_t *data, size_t len) : _data(data), _length(len) {}

        void draw(DisplayCore *dev, int x, int y);
        void draw(DisplayCore *dev, int x, int y, color_t t) { draw(dev, x, y); }
        void drawTransformed(DisplayCore *dev, int x, int y, int xf) { draw(dev, x, y); }
        void drawTransformed(DisplayCore *dev, int x, int y, int xf, color_t t) { draw(dev, x, y); }

};

#endif
