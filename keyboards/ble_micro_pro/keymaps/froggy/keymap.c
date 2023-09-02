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
#include QMK_KEYBOARD_H
#include "bmp.h"
#include "bmp_custom_keycode.h"
#include "keycode_str_converter.h"

// 追加
#include <string.h>
#include "keymap_jp.h"

#ifndef MAX
#define MAX(X, Y) ((X) > (Y) ? (X) : (Y))
#endif
#ifndef MIN
#define MIN(X, Y) ((X) < (Y) ? (X) : (Y))
#endif

extern keymap_config_t keymap_config;


extern uint8_t is_master;

// 遅延入力処理用定数と変数
// レイヤーキーとの同時打鍵を実現するために一時的な入力内容を静的変数に保持する
// DELAY_TIME以内であればレイヤーキーの押下が後でも反映される
#define DELAY_TIME  75 // 遅延入力有効時間 (ms)
static uint16_t key_timer; // 遅延入力用タイマー
static uint16_t tap_timer; // タップ用タイマー
static uint16_t delay_registered_code; // 遅延入力されたキーコード
static uint8_t delay_mat_row; // 遅延入力された行
static uint8_t delay_mat_col; // 遅延入力された列
static bool delay_key_stat; // 遅延入力キーがあるときにtrue
static bool delay_key_pressing; // 延入力キーが押されている間はtrue
static bool tapping_key;

// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.
// Layer names don't all need to be of the same length, obviously, and you can also skip them
// entirely and just use numbers.

// Defines the keycodes used by our macros in process_record_user
enum custom_keycodes {
    EISU = BMP_SAFE_RANGE,
    KANA,
    OPT_TAP_SP,
    DESKTOP,
    MAC,
    WIN,
};

const key_string_map_t custom_keys_user =
{
    .start_kc = EISU,
    .end_kc = WIN,
    .key_strings = "EISU\0"
    "KANA\0"
    "OPT_TAP_SP\0"
    "DESKTOP\0"
    "MAC\0"
    "WIN\0"
};

enum layers {
    _BASE = 0, _OPT = 1, _FUNC = 2, _SYM = 3, _NUM = 4
};

const uint16_t keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_BASE] =   {{
    KC_A, KC_B, KC_C, KC_D, KC_E, KC_F, KC_G, KC_H, KC_I,
        KC_J, KC_K, KC_L, KC_M, KC_N, KC_O, KC_P, KC_Q, KC_R, KC_S
    }},
  [_OPT] =   {{
    KC_A, KC_B, KC_C, KC_D, KC_E, KC_F, KC_G, KC_H, KC_I,
        KC_J, KC_K, KC_L, KC_M, KC_N, KC_O, KC_P, KC_Q, KC_R, KC_S
    }},
  [_FUNC] =   {{
    KC_A, KC_B, KC_C, KC_D, KC_E, KC_F, KC_G, KC_H, KC_I,
        KC_J, KC_K, KC_L, KC_M, KC_N, KC_O, KC_P, KC_Q, KC_R, KC_S
    }},
  [_SYM] =   {{
    KC_A, KC_B, KC_C, KC_D, KC_E, KC_F, KC_G, KC_H, KC_I,
        KC_J, KC_K, KC_L, KC_M, KC_N, KC_O, KC_P, KC_Q, KC_R, KC_S
    }},
  [_NUM] =   {{
    KC_A, KC_B, KC_C, KC_D, KC_E, KC_F, KC_G, KC_H, KC_I,
        KC_J, KC_K, KC_L, KC_M, KC_N, KC_O, KC_P, KC_Q, KC_R, KC_S
    }},

};

uint32_t keymaps_len() {
  return 19;
}
// 追加


bool find_mairix(uint16_t keycode, uint8_t *row, uint8_t *col){
  for(uint8_t i=0; i<MATRIX_ROWS; i++){
    for(uint8_t j=0; j<MATRIX_COLS; j++){
      if( pgm_read_word(&(keymaps[_BASE][i][j]))==keycode){
        *row = i;
        *col = j;
        return true;
      }
    }
  }
  return false;
}

void unregister_delay_code(void){
  if(delay_registered_code){
    unregister_code(delay_registered_code);
    if (delay_registered_code & QK_LSFT){
      unregister_code(KC_LSFT);
    }
    if (delay_registered_code & QK_LCTL){
      unregister_code(KC_LCTL);
    }
    if (delay_registered_code & QK_LALT){
      unregister_code(KC_LALT);
    }
    if (delay_registered_code & QK_LGUI){
      unregister_code(KC_LGUI);
    }
    delay_registered_code=0;
  }
}

void register_delay_code(uint8_t layer){
  if(delay_key_stat){
    unregister_delay_code();

    uint16_t code = pgm_read_word(&(keymaps[layer][delay_mat_row][delay_mat_col]));
    if (code & QK_LSFT){
      register_code(KC_LSFT);
    }
    if (code & QK_LCTL){
      register_code(KC_LCTL);
    }
    if (code & QK_LALT){
      register_code(KC_LALT);
    }
    if (code & QK_LGUI){
      register_code(KC_LGUI);
    }
    register_code(code);
    delay_registered_code = code;
    delay_key_stat = false;
    tapping_key = true;
  }
}

// ///////////////////////////////

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  bool continue_process = process_record_user_bmp(keycode, record);
  if (continue_process == false)
  {
    return false;
  }

  // 追加
  if(tap_timer&&keycode!=OPT_TAP_SP){
    tapping_key = true;
  }

  if(keycode==delay_registered_code){
      if (!record->event.pressed){
        unregister_delay_code();
      }
  }
////////

  switch (keycode) {
    // 追加
    case JP_SCLN:
    case JP_LBRC:
    case JP_LPRN:
    case JP_LT:
    case JP_LCBR:
    case JP_P:
    case JP_K:
    case JP_R:
    case JP_A:
    case JP_F:
    case KC_BSPC:
    case JP_D:
    case JP_T:
    case JP_H:
    case JP_E:
    case JP_O:
    case JP_Y:
    case JP_S:
    case JP_N:
    case JP_I:
    case JP_U:
    case LCTL(JP_Z):
    case KC_SPC:
      if (record->event.pressed) {
        register_delay_code(_BASE);
        unregister_delay_code();
        if(find_mairix(keycode, &delay_mat_row, &delay_mat_col)){
          key_timer = timer_read();
          delay_key_stat = true;
          delay_key_pressing = true;
        }
      }else{
        delay_key_pressing = false;
      }
      return false;
      break;
    case OPT_TAP_SP:
      if (record->event.pressed) {
        tapping_key = false;
        register_delay_code(_OPT);
        unregister_delay_code();
        layer_on(_OPT);
        tap_timer = timer_read();
      }else{
        layer_off(_OPT);
        if(tapping_key==false && timer_elapsed(tap_timer) < TAPPING_TERM){
          SEND_STRING(" ");
        }
        tap_timer = 0;
      }
      return false;
      break;
    default:
      break;
  }

  return true;
}
