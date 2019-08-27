#include QMK_KEYBOARD_H

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* Layer 0: Default Layer
     * ,-----------------------------------------------.
     * |Esc|  Q|  W|  E|  R|  T|  Y|  U|  I|  O|  P|Bsp|
     * |-----------------------------------------------|
     * |Tab |  A|  S|  D|  F|  G|  H|  J|  K|  L| Enter|
     * |-----------------------------------------------|
     * |Shift|  Z|  X|  C|  V|  B|  N|  M|  ,| Up|Shift|
     * |-----------------------------------------------|
     * |Ctrl |Gui|Alt| FN | Space |Alt|Left| Down|Right|
     * `-----------------------------------------------'
     */
    [0] = LAYOUT_default(
        KC_ESC,     KC_Q,    KC_W,   KC_E,   KC_R,   KC_T,   KC_Y,   KC_U, KC_I,    KC_O,    KC_P,   KC_BSPC,
        KC_TAB,     KC_A,    KC_S,   KC_D,   KC_F,   KC_G,   KC_H,   KC_J, KC_K,    KC_L,    KC_ENT,
        KC_LSFT,    KC_Z,    KC_X,   KC_C,   KC_V,   KC_B,   KC_N,   KC_M, KC_COMM, KC_UP,   KC_RSFT,
        KC_LCTL, KC_LGUI, KC_LALT,           MO(1),        KC_SPC,         KC_LEFT, KC_DOWN, KC_RIGHT),
    [1] = LAYOUT_default(
        _______,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,   KC_8,    KC_9,     KC_0, KC_DEL,
        _______, RGB_TOG, RGB_MOD, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______,          _______,          _______, _______, _______)
};
