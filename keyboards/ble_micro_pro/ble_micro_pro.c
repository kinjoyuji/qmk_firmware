/* Copyright 2019 sekigon-gonnoc
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "ble_micro_pro.h"
#include <string.h>

void keyboard_post_init_user()
{
    debug_enable = false;
}

void create_user_file()
{
  static const char qmk_configurator[] = "<meta http-equiv=\"refresh\" content=\"0;URL=\'https://sekigon-gonnoc.github.io/qmk_configurator\'\"/>";
  BMPAPI->usb.create_file("MAP_EDITHTM", (uint8_t*)qmk_configurator, strlen(qmk_configurator));
}
// Optional override functions below.
// You can leave any or all of these undefined.
// These are only required if you want to perform custom actions.

/*

void matrix_init_kb(void) {
  // put your keyboard start-up code here
  // runs once when the firmware starts up

  matrix_init_user();
}

void matrix_scan_kb(void) {
  // put your looping keyboard code here
  // runs every cycle (a lot)

  matrix_scan_user();
}

bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
  // put your per-action keyboard code here
  // runs for every action, just before processing by the firmware

  return process_record_user(keycode, record);
}

void led_set_kb(uint8_t usb_led) {
  // put your keyboard LED indicator (ex: Caps Lock LED) toggling code here

  led_set_user(usb_led);
}

*/
bool is_mac_mode(void) {
    return keymap_config.swap_lalt_lgui == false;
}

void set_mac_mode(bool macmode) {
    if (macmode) {
        /* The result is the same as pressing the AG_NORM(=MAGIC_UNSWAP_ALT_GUI) key. */
        /* https://github.com/qmk/qmk_firmware/blob/fb4a6ad30ea7a648acd59793ed4a30c3a8d8dc32/quantum/process_keycode/process_magic.c#L123-L124 */
        keymap_config.swap_lalt_lgui = keymap_config.swap_ralt_rgui = false;
    } else {
        /* The result is the same as pressing the AG_SWAP(=MAGIC_SWAP_ALT_GUI) key. */
        /* https://github.com/qmk/qmk_firmware/blob/fb4a6ad30ea7a648acd59793ed4a30c3a8d8dc32/quantum/process_keycode/process_magic.c#L80-L81 */
        keymap_config.swap_lalt_lgui = keymap_config.swap_ralt_rgui = true;
    }
    eeconfig_update_keymap(keymap_config.raw);
}