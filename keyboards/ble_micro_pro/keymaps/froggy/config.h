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

#pragma once

// place overrides here

#ifndef CONFIG_USER_H
#define CONFIG_USER_H

#undef TAPPING_TERM
#define TAPPING_TERM 200
#define ONESHOT_TAP_TOGGLE 5 /* Tapping this number of times holds the key until tapped this number of times again. */
#define ONESHOT_TIMEOUT 5000 /* Time (in ms) before the one shot key is released */

// #undef OLED_FONT_H
// #define OLED_FONT_H "helixfont.h"

// // If you need more program area, try select and reduce rgblight modes to use.

// // Selection of RGBLIGHT MODE to use.
// #ifdef RGBLIGHT_ENABLE
//    #define RGBLIGHT_DEFAULT_MODE	RGBLIGHT_MODE_STATIC_LIGHT	//EEPROM のクリア時に使用するデフォルト モード
//    #define RGBLIGHT_DEFAULT_HUE	0 //（赤）	EEPROM のクリア時に使用するデフォルトの色相
//    #define RGBLIGHT_DEFAULT_SAT	(255)	//EEPROM のクリア時に使用するデフォルトの飽和度
//    #define RGBLIGHT_DEFAULT_VAL	RGBLIGHT_LIMIT_VAL//	EEPROM のクリア時に使用するデフォルト値 (明るさ)
//    #define RGBLIGHT_DEFAULT_SPD	0//EEPROM のクリア時に使用するデフォルトの速度
// #endif

#endif /* CONFIG_USER_H */