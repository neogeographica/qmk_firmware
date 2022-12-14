#include QMK_KEYBOARD_H
#include "version.h"

bool symbols_lock;
bool mousenum_lock;
bool gui_chord;
bool shooter_mode;
bool shooter_left_shift_down;
bool shooter_right_shift_down;
bool shooter_grave_down;
uint16_t lthumb_keycode;
bool sys_chord;
bool sys_chord_flash;

enum custom_keycodes {
    JKC_SYM = SAFE_RANGE,
    JKC_MN_L,
    JKC_MN_R,
    JKC_GUI_L,
    JKC_GUI_R,
    JKC_CW,
    JKC_LTHM,
    JKC_SYS,
    JKC_SYSFL
};

enum layers{
  _MAIN = 0,
  _SYMBOLS,
  _MOUSENUM
};

// Notes about LEDs:

// LED 1 is closest to the USB cables, and separated a bit from the other two.
// That one will be used for the "shooter mode" indicator.

// LED 2 is in the middle and LED 3 is rightmost. I ended up using the
// rightmost LED for _SYMBOLS lock and the middle LED for _MOUSENUM lock.

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* Notes about "shooter mode":
 *
 * This is a mode to make the lefthand keyboard a bit nicer for FPS games.
 *
 * Shooter mode can be entered as long as the mouse/numpad layer is not
 * currently active, by pressing both GUI keys simultaneously. It can be
 * exited in the same way.
 *
 * While shooter mode is enabled:
 * - The two keys in the NUMPAD positions on the keymap will instead act as
 *   left and right shift keys.
 * - The key in the LShift position will instead act as a spacebar.
 * - The key in the CAPSWD position will instead act as the backtick/tilde key.
 * The layer keymap descriptions below will not mention this; they describe
 * the keymap as it is when shooter mode is disabled.
 *
 * An implication of this arrangement is that the symbols/special layer can
 * still be activated, locked, and deactivated while in shooter mode, but the
 * mouse/numpad layer cannot.
 */

/* Keymap _MAIN: default/base layer
 *
 * - alphanum keys in "normal" QWERTY positions, plus some other symbol keys
 * - ergodox-ish thumb key placement for shift, enter, space, backspace
 * - common thumb/pinky modifier keys where I like them
 * - clusters of cursor-movement keys
 * - keys to activate or lock other layers
 * - volume control
 * - screenlock combo for Windows and (some) Linux
 *
 * ,--------------------------------------------------.           ,--------------------------------------------------.
 * | `/Esc  |   1  |   2  |   3  |   4  |   5  |   -  |           |   =  |   6  |   7  |   8  |   9  |   0  |   F11  |
 * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
 * | Tab    |   Q  |   W  |   E  |   R  |   T  |  SYS |           |   [  |   Y  |   U  |   I  |   O  |   P  |    \   |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * | LCtrl  |   A  |   S  |   D  |   F  |   G  |------|           |------|   H  |   J  |   K  |   L  |   ;  |    '   |
 * |--------+------+------+------+------+------|  SYS |           |   ]  |------+------+------+------+------+--------|
 * | NUMPAD |   Z  |   X  |   C  |   V  |   B  |      |           |      |   N  |   M  |   ,  |   .  |   /  | NUMPAD |
 * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   |CAPSWD| PgUp | PgDn | LAlt |  SYM |                                       |  SYM | Left | Down |  Up  | Right|
 *   `----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,-------------.
 *                                        | WLOCK| Mute |       | Vol- | Vol+ |
 *                                 ,------|------|------|       |------+------+------.
 *                                 |      |      | Home |       |  End |      |      |
 *                                 |LShift| Bksp |------|       |------| Enter| Space|
 *                                 |      |      | LGui |       | RGui |      |      |
 *                                 `--------------------'       `--------------------'
 * 
 * The upper-left key is normally Escape. Use GUI+key to generate a backtick.
 * Use Shift+key to generate a tilde.
 *
 * SYM key activates the symbols/special layer. Normally is only active while
 * held; press both SYM keys together to lock the layer. Press one SYM key
 * again to unlock the layer. When the symbols/special layer is locked, LED 3
 * will be lit.
 *
 * NUMPAD key activates the mouse/numpad layer. Normally is only active while
 * held; press both NUMPAD keys together to lock the layer. Press one NUMPAD
 * key again to unlock the layer. When the mouse/numpad layer is locked, LED
 * 2 will be lit.
 *
 * CAPSWD activates "caps word" mode; letters will be capitalized and hyphens
 * changed to underlines until the space key is pressed.
 *
 * WLOCK sends a macro (LGui-L) to lock the screen. This screen lock macro
 * works for Windows and various Linux setups; for macOS see the corresponding
 * key on the symbols/special layer.
 *
 * Pressing the two SYS keys together will normally type out information about
 * the build and the currently active layers. If SYSFL is already held however
 * (by chording the SYM and NUMPAD keys), then pressing the two SYS keys will
 * reset the keyboard for flashing. When SYSFL is held, LED 1 will be lit.
 */
    [_MAIN] = LAYOUT_ergodox(
        // left hand
        QK_GESC,  KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_MINS,
        KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     JKC_SYS,
        KC_LCTL,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,
        JKC_MN_L, KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     JKC_SYS,
        JKC_CW,   KC_PGUP,  KC_PGDN,  KC_LALT,  JKC_SYM,
                                                          LGUI(KC_L), KC_MUTE,
                                                                    KC_HOME,
                                                JKC_LTHM, KC_BSPC,  JKC_GUI_L,
        // right hand
        KC_EQL,   KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_F11,
        KC_LBRC,  KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_BSLS,
                  KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,
        KC_RBRC,  KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,  JKC_MN_R,
                            JKC_SYM,  KC_LEFT,  KC_DOWN,  KC_UP,    KC_RGHT,
        KC_VOLD,  KC_VOLU,
        KC_END,
        JKC_GUI_R, KC_ENT,  KC_SPC
    ),

/* Keymap _SYMBOLS: symbols/special layer (mutually exclusive with _MOUSENUM)
 *
 * - function keys
 * - power/sleep/pause
 * - access to symbols and arrow keys on the main three rows
 * - media player
 * - screen brightness
 * - less-used "entry state" keys (caps/del/ins)
 * - screenlock combo for macOS
 *
 * ,--------------------------------------------------.           ,--------------------------------------------------.
 * | Power  |  F1  |  F2  |  F3  |  F4  |  F5  | Sleep|           | Pause|  F6  |  F7  |  F8  |  F9  |  F10 |   F12  |
 * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
 * |        | Space|  Up  | Enter|   _  |   ~  |      |           |      |   "  |   +  |   |  |   {  |   }  |        |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |        | Left | Down | Right|   -  |   `  |------|           |------|   '  |   =  |   \  |   [  |   ]  |        |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |  SYSFL |   !  |   @  |   #  |   $  |   %  |      |           |      |   ^  |   &  |   *  |   (  |   )  |  SYSFL |
 * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   |      |      |      |      |      |                                       |      |  <<  | Stop | Play |  >>  |
 *   `----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,-------------.
 *                                        | MLOCK|      |       | Brt- | Brt+ |
 *                                 ,------|------|------|       |------+------+------.
 *                                 |      |      |  Ins |       | Prev |      |      |
 *                                 | Caps |  Del |------|       |------| Play | Next |
 *                                 |      |      |      |       |      |      |      |
 *                                 `--------------------'       `--------------------'
 * 
 * MLOCK sends a macro (LCtl-LGui-Q) to lock the screen. This screen lock macro
 * works for macOS; for Windows and various Linux setups see the corresponding
 * key on the base layer.
 *
 * SYSFL unlocks the keyboard-flashing combo; see base layer comments for more.
 */
    [_SYMBOLS] = LAYOUT_ergodox(
        // left hand
        KC_PWR,   KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_SLEP,
        KC_TRNS,  KC_SPC,   KC_UP,    KC_ENT,   KC_UNDS,  KC_TILD,  KC_TRNS,
        KC_TRNS,  KC_LEFT,  KC_DOWN,  KC_RGHT,  KC_MINS,  KC_GRV,
        JKC_SYSFL,KC_EXLM,  KC_AT,    KC_HASH,  KC_DLR,   KC_PERC,  KC_TRNS,
        KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,
                                                          LCTL(LGUI(KC_Q)), KC_TRNS,
                                                                    KC_INS,
                                                KC_CAPS,  KC_DEL,   KC_TRNS,
        // right hand
        KC_PAUS,  KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F12,
        KC_TRNS,  KC_DQUO,  KC_PLUS,  KC_PIPE,  KC_LCBR,  KC_RCBR,  KC_TRNS,
                  KC_QUOT,  KC_EQL,   KC_BSLS,  KC_LBRC,  KC_RBRC,  KC_TRNS,
        KC_TRNS,  KC_CIRC,  KC_AMPR,  KC_ASTR,  KC_LPRN,  KC_RPRN,  JKC_SYSFL,
                            KC_TRNS,  KC_MRWD,  KC_MSTP,  KC_MPLY,  KC_MFFD,
        KC_BRID,  KC_BRIU,
        KC_MPRV,
        KC_TRNS,  KC_MPLY,  KC_MNXT
    ),

/* Keymap _MOUSENUM: mouse/numpad layer (mutually exclusive with _SYMBOLS)
 *
 * - left hand keys for mouse movement, wheel, and buttons
 * - right hand keys for numpad
 *
 * ,--------------------------------------------------.           ,--------------------------------------------------.
 * |        | Null | Null | Null | Null | Null | Null |           | Null | Null |NumLck|  KP/ |  KP* |  KP- |  Null  |
 * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
 * |        |LClick| MsUp |RClick| WhUp | Null |      |           | Null | Null |  KP7 |  KP8 |  KP9 |  KP+ |  Null  |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |        |MsLeft|MsDown|MsRght|WhDown| Null |------|           |------| Null |  KP4 |  KP5 |  KP6 |  KP, |  Null  |
 * |--------+------+------+------+------+------|      |           | Null |------+------+------+------+------+--------|
 * |        | Btn3 | Btn4 | Btn5 | Null | Null |      |           |      | Null |  KP1 |  KP2 |  KP3 |  KP= |        |
 * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   |      |      |      |      | SYSFL|                                       | SYSFL|  KP0 |  KP. | KPEnt| Null |
 *   `----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,-------------.
 *                                        |      |      |       |      |      |
 *                                 ,------|------|------|       |------+------+------.
 *                                 |      |      |      |       |      |      |      |
 *                                 |      |      |------|       |------|      |  KP0 |
 *                                 |      |      |      |       |      |      |      |
 *                                 `--------------------'       `--------------------'
 *
 * SYSFL unlocks the keyboard-flashing combo; see base layer comments for more.
 */
    [_MOUSENUM] = LAYOUT_ergodox(
        // left hand
        KC_TRNS,  KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,
        KC_TRNS,  KC_BTN1,  KC_MS_U,  KC_BTN2,  KC_WH_U,  KC_NO,    KC_TRNS,
        KC_TRNS,  KC_MS_L,  KC_MS_D,  KC_MS_R,  KC_WH_D,  KC_NO,
        KC_TRNS,  KC_BTN3,  KC_BTN4,  KC_BTN5,  KC_NO,    KC_NO,    KC_TRNS,
        KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  JKC_SYSFL,
                                                          KC_TRNS,  KC_TRNS,
                                                                    KC_TRNS,
                                                KC_TRNS,  KC_TRNS,  KC_TRNS,
        // right hand
        KC_NO,    KC_NO,    KC_NUM,   KC_PSLS,  KC_PAST,  KC_PMNS,  KC_NO,
        KC_NO,    KC_NO,    KC_P7,    KC_P8,    KC_P9,    KC_PPLS,  KC_NO,
                  KC_NO,    KC_P4,    KC_P5,    KC_P6,    KC_PCMM,  KC_NO,
        KC_NO,    KC_NO,    KC_P1,    KC_P2,    KC_P3,    KC_PEQL,  KC_TRNS,
                            JKC_SYSFL,KC_P0,    KC_PDOT,  KC_PENT,  KC_NO,
        KC_TRNS,  KC_TRNS,
        KC_TRNS,
        KC_TRNS,  KC_TRNS,  KC_P0)
};

// Make double-sure our state and the layer state agree that there are no
// layers active on startup. Also behave as if none of the JKC_SYS keys are
// currently depressed.
void matrix_init_user(void) {
    symbols_lock = false;
    mousenum_lock = false;
    gui_chord = false;
    shooter_mode = false;
    shooter_left_shift_down = false;
    shooter_right_shift_down = false;
    shooter_grave_down = false;
    lthumb_keycode = KC_LSFT;
    sys_chord = false;
    sys_chord_flash = false;
    layer_clear();
}

// Our hook for special actions on key events. Currently this handles the
// enable and lock behaviors for _SYMBOLS and _MOUSENUM, and the
// JKC_SYS/JKC_SYSFL keys.
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {

        // This key normally acts as left shift, but in "shooter mode" it is
        // a spacebar.
        case JKC_LTHM:
            if (record->event.pressed) {
                // Key Down
                if (!shooter_mode) {
                    lthumb_keycode = KC_LSFT;
                } else {
                    lthumb_keycode = KC_SPC;
                }
                register_code(lthumb_keycode);
            } else {
                unregister_code(lthumb_keycode);
            }
            return false; // Skip all further processing of this key

        // This key normally activates Caps Word, but in "shooter mode" it is
        // the backtick/tilde key.
        case JKC_CW:
            if (record->event.pressed) {
                // Key Down
                if (!shooter_mode) {
                    caps_word_on();
                } else {
                    register_code(KC_GRV);
                    shooter_grave_down = true;
                }
            } else {
                // Key Up
                if (shooter_grave_down) {
                    // Key was pressed as a backtick/tilde key, so should
                    // release that now.
                    unregister_code(KC_GRV);
                    shooter_grave_down = false;
                }
            }
            return false; // Skip all further processing of this key

        // For _SYMBOLS and _MOUSENUM control keys: It is assumed that there
        // will be two of these (one each for left and right hand boards).
        // Depressing a key will do a layer enable while the key is held.
        // Simultaneously depressing both of the same kind of key will lock
        // the layer on. If the layer is locked on, tap the same kind of key
        // again to unlock.
        case JKC_SYM:
            if (record->event.pressed) {
                // Key Down
                if (IS_LAYER_OFF(_SYMBOLS)) {
                    // _SYMBOLS isn't on yet, so turn it on.
                    layer_on(_SYMBOLS);
                } else {
                    // _SYMBOLS is already on. If symbols_lock is not true yet,
                    // this is because we are pushing both control keys, so
                    // set symbols_lock to true. Otherwise this means we are
                    // pushing a control key AGAIN after locking, so unlock.
                    symbols_lock = !symbols_lock;
                    if (symbols_lock) {
                        ergodox_right_led_3_on();
                    } else {
                        ergodox_right_led_3_off();
                    }
                }
            } else {
                // Key Up
                if (!symbols_lock) {
                    // If not locked, clear _SYMBOLS.
                    layer_off(_SYMBOLS);
                }
            }
            return false; // Skip all further processing of this key

        // _MOUSENUM keys have an additional consideration; in "shooter mode"
        // they simply act as left/right shift keys.
        case JKC_MN_L:
        case JKC_MN_R:
            if (record->event.pressed) {
                // Key Down
                if (shooter_mode) {
                    // Treat as shift key.
                    if (keycode == JKC_MN_L) {
                        register_code(KC_LSFT);
                        shooter_left_shift_down = true;
                    } else {
                        register_code(KC_RSFT);
                        shooter_right_shift_down = true;
                    }
                } else if (IS_LAYER_OFF(_MOUSENUM)) {
                    // _MOUSENUM isn't on yet, so turn it on.
                    layer_on(_MOUSENUM);
                } else {
                    // _MOUSENUM is already on; we are locking or unlocking it.
                    mousenum_lock = !mousenum_lock;
                    if (mousenum_lock) {
                        ergodox_right_led_2_on();
                    } else {
                        ergodox_right_led_2_off();
                    }
                }
            } else {
                // Key Up
                if (keycode == JKC_MN_L) {
                    if (shooter_left_shift_down) {
                        // Key was pressed as a shift key, so should un-shift now.
                        unregister_code(KC_LSFT);
                        shooter_left_shift_down = false;
                    } else if (!mousenum_lock) {
                        // If not locked, clear _MOUSENUM.
                        layer_off(_MOUSENUM);
                    }
                }
                else {
                    // Same as above for other key.
                    if (shooter_right_shift_down) {
                        unregister_code(KC_RSFT);
                        shooter_right_shift_down = false;
                    } else if (!mousenum_lock) {
                        layer_off(_MOUSENUM);
                    }
                }
            }
            return false; // Skip all further processing of this key

        // The GUI keys typically just emit left/right GUI key events.
        // However if both keys are pressed, and we're not on the mouse/numpad
        // layer, then additionally toggle "shooter mode".
        case JKC_GUI_L:
        case JKC_GUI_R:
            if (record->event.pressed) {
                // Key Down
                if (gui_chord) {
                    // The other GUI key is already pressed. If we're not on
                    // the mouse/numpad layer, toggle shooter mode.
                    if (IS_LAYER_OFF(_MOUSENUM)) {
                        shooter_mode = !shooter_mode;
                        if (shooter_mode) {
                            ergodox_right_led_1_on();
                        } else {
                            ergodox_right_led_1_off();
                        }
                    }
                } else {
                    gui_chord = true;
                }
                if (keycode == JKC_GUI_L) {
                    register_code(KC_LGUI);
                } else {
                    register_code(KC_RGUI);
                }
            } else {
                // Key Up
                gui_chord = false;
                if (keycode == JKC_GUI_L) {
                    unregister_code(KC_LGUI);
                } else {
                    unregister_code(KC_RGUI);
                }
            }
            return false; // Skip all further processing of this key

        // There should be two keys with this keycode assigned. If both are
        // depressed together, normally this will cause a string macro to be
        // emitted that prints out keyboard info. If a JKC_SYSFL
        // key is also being held down, then reset the keyboard for flashing.
        case JKC_SYS:
            if (record->event.pressed) {
                // Key Down
                if (sys_chord) {
                    // The other JKC_SYS key is already pressed.
                    if (sys_chord_flash) {
                        // A FLASH-enable key is pressed too, so let's flash.
                        reset_keyboard();
                    } else {
                        // Just do the normal sys chord behavior: dump info.
                        send_string_with_delay_P(PSTR(QMK_KEYBOARD "/" QMK_KEYMAP " @ " QMK_VERSION " locked layers/modes: "), 10);
                        if (symbols_lock || mousenum_lock || shooter_mode) {
                            if (symbols_lock) {
                                send_string_with_delay_P(PSTR("SYMBOLS "), 10);
                            }
                            if (mousenum_lock) {
                                send_string_with_delay_P(PSTR("MOUSE/NUMPAD "), 10);
                            }
                            if (shooter_mode) {
                                send_string_with_delay_P(PSTR("SHOOTER"), 10);
                            }
                        } else {
                            send_string_with_delay_P(PSTR("none"), 10);
                        }
                    }
                } else {
                    sys_chord = true;
                }
            } else {
                // Key Up
                sys_chord = false;
            }
            return false; // Skip all further processing of this key

        // This keycode is entirely used just to require another keypress
        // before enabling the flash behavior of the JKC_SYS keys. Since this
        // key itself is not on a base layer, that means that flash-enable
        // is a four key combo.
        case JKC_SYSFL:
            sys_chord_flash = record->event.pressed;
            return false; // Skip all further processing of this key

        default:
            return true; // Process all other keycodes normally
    }
}
