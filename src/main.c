#include <zephyr/kernel.h>
#include <lvgl.h>
#include "baumer.h"

static lv_obj_t *label;
static lv_obj_t *img;

static const uint32_t colors[] = {
    0xFF0000, // red 
    0x00FF00, // green
    0x0000FF, // blue 
    0xFFFFFF, // white 
    0x000000, // black 
    0xFFFF00, // yellow 
    0x00FFFF, // cyan 
    0xFF00FF, // magenta 
};

static const char *words[] = {
    "I",
    "want",
    "to",
    "work",
    "@",
    "",
    "",
    ""
};

static void color_timer_cb(lv_timer_t *timer)
{
    static uint32_t idx = 0;

    lv_obj_t *screen = lv_timer_get_user_data(timer);

    lv_obj_set_style_bg_color(
        screen,
        lv_color_hex(colors[idx]),
        LV_PART_MAIN);

    idx = (idx + 1) % (sizeof(colors) / sizeof(colors[0]));
}

static void word_timer_cb(lv_timer_t *timer)
{
    static size_t idx = 0;

    lv_label_set_text(label, words[idx]);

    if (idx > 4) {
        lv_obj_add_flag(label, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(img, LV_OBJ_FLAG_HIDDEN);
    } else {
        lv_obj_add_flag(img, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(label, LV_OBJ_FLAG_HIDDEN);
    }

    idx++;
    if (idx >= ARRAY_SIZE(words)) {
        idx = 0;
    }
}

int main(void)
{
    printk("I am alive!\n");

    lv_init();
    printk("lv_init done.\n");

    const struct device *display = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));
    if (!device_is_ready(display)) {
        printk("Display not ready\n");
        return -1;
    }

    printk("Display ready\n");

    lv_obj_t *screen = lv_scr_act();

    img = lv_image_create(screen);
    lv_image_set_src(img, &baumer);
    lv_obj_center(img);

    label = lv_label_create(screen);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_22, 0);
    lv_obj_set_style_text_color(label, lv_color_hex(0xFFFFFF), 0);
    lv_obj_center(label);
    lv_label_set_text(label, words[0]);


    bool color_test = false;
    if (color_test) {
        lv_obj_add_flag(label, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(img, LV_OBJ_FLAG_HIDDEN);
        lv_timer_create(color_timer_cb, 500, screen);
    } else {
        lv_timer_create(word_timer_cb, 500, label);
    }

    lv_obj_add_flag(img, LV_OBJ_FLAG_HIDDEN);

    lv_obj_set_style_bg_color(
        screen,
        lv_color_hex(0x000000),
        LV_PART_MAIN);

    while (1) {
        lv_timer_handler();
        k_msleep(1);
    }
    return 0;
}
