#pragma once

#include <lvgl.h>
#include "util.h"

struct layer_status_state {
    uint8_t index;
};

uint8_t zmk_display_visible_layer_index(void);
bool zmk_display_win_mode_active(void);

void draw_layer_status(lv_obj_t *canvas, const struct status_state *state);
void draw_os_mode_status(lv_obj_t *canvas, const struct status_state *state);
