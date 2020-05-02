#include <PNG.h>

static uint32_t upng_get_pixel(upng_t *upng, int x, int y) {
    const uint8_t *buf = upng_get_buffer(upng);
    int w = upng_get_width(upng);
    int h = upng_get_height(upng);
    uint32_t off = y * w + x;
    if (off >= w * h) {
        return 0;
    }
    switch (upng_get_format(upng)) {
        case UPNG_RGB8: {
            uint8_t r = buf[off * 3];
            uint8_t g = buf[off * 3 + 1];
            uint8_t b = buf[off * 3 + 2];
            return (r << 16) | (g << 8) | b;
        }
        break;

        case UPNG_RGBA8: {
            uint8_t r = buf[off * 4];
            uint8_t g = buf[off * 4 + 1];
            uint8_t b = buf[off * 4 + 2];
            uint8_t a = buf[off * 4 + 2];
            return (a << 24) | (r << 16) | (g << 8) | b;
        }
        break;

        case UPNG_LUMINANCE8: {
            uint8_t b = buf[off];
            return (b << 16) | (b << 8) | b;
        }
        break;

        case UPNG_LUMINANCE_ALPHA8: {
            uint8_t b = buf[off * 2];
            uint8_t a = buf[off * 2 + 1];
            return (a << 24) | (b << 16) | (b << 8) | b;
        }
        break;

    }
}

void PNG::draw(Cariad *dev, int x, int y) {
    upng_t *upng;
    upng = upng_new_from_bytes(_data, _length);
    if (upng != NULL) {
        upng_decode(upng);
        if (upng_get_error(upng) == UPNG_EOK) {
            int w = upng_get_width(upng);
            int h = upng_get_height(upng);

            switch (upng_get_format(upng)) {
                case UPNG_RGB8: 
                case UPNG_LUMINANCE8:

                    dev->openWindow(x, y, w, h);

                    for (int yy = 0; yy < h; yy++) {
                        for (int xx = 0; xx < w; xx++) {
                            dev->windowData(rgb(upng_get_pixel(upng, xx, yy)));
                        }
                    }
                    dev->closeWindow();
                    break;

                case UPNG_RGBA8: 
                case UPNG_LUMINANCE_ALPHA8: 
                    for (int yy = 0; yy < h; yy++) {
                        for (int xx = 0; xx < w; xx++) {
                            uint32_t col = upng_get_pixel(upng, xx, yy);
                            uint8_t a = (col >> 23) & 0xFE; 
                            color_t c = rgb(col & 0xFFFFFF);
                            color_t b = dev->colorAt(x + xx, y + yy);
                            dev->setPixel(x + xx, y + yy, mix(b, c, a));
                        }
                    }
                break;
            }
        } else {
            dev->printf("PNG Error: %d\r\n", upng_get_error(upng));
        }
        upng_free(upng);
    } else {
        dev->println("Unable to create PNG");
    }
}
