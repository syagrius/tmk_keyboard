/*
Copyright 2016 Jun Wako <wakojun@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "unimap_trans.h"
#include "keymap_common.h"

// https://github.com/qmk/qmk_firmware/blob/master/keyboards/atreus/keymaps/khitsule/keymap.c
// 
#define tap_mod_macro(record, mod, macro) ( ((record)->event.pressed) ? \
     ( ((record)->tap.count <= 0 || (record)->tap.interrupted) ? MACRO(D(mod), END) : MACRO_NONE ) : \
     ( ((record)->tap.count > 1 && !((record)->tap.interrupted)) ? (macro) : MACRO(U(mod), END) ) )


enum macro_id {
    SAY_HELLO,
    ALT_TAB
};

#define AC_FN0 ACTION_LAYER_MOMENTARY(1)
#define AC_HELLO ACTION_MACRO_TAP(SAY_HELLO)
#define AC_ALTTAB ACTION_MACRO(ALT_TAB)


#ifdef KEYMAP_SECTION_ENABLE
const action_t actionmaps[][UNIMAP_ROWS][UNIMAP_COLS] __attribute__ ((section (".keymap.keymaps"))) = {
#else
const action_t actionmaps[][UNIMAP_ROWS][UNIMAP_COLS] PROGMEM = {
#endif
    UNIMAP(
              F1,  F2,  F3,  F4, F5, F6, F7, F8, F9, F10, F11, F12,
    ESC,      F21, F22,  F13,  F14,  F15,  F16,  F17,  F18,  F19,  F20, F21, F22,   PSCR,SLCK,PAUS,         VOLD,VOLU,MUTE,
    GRV, 1,   2,   3,   4,   5,   6,   7,   8,   9,   0,   MINS,EQL, JYEN,BSPC,     INS, HOME,PGUP,    NLCK,PSLS,PAST,PMNS,
    TAB, Q,   W,   E,   R,   T,   Y,   U,   I,   O,   P,   LBRC,RBRC,     BSLS,     DEL, END, PGDN,    P7,  P8,  P9,  PPLS,
    CAPS,A,   S,   D,   F,   G,   H,   J,   K,   L,   SCLN,QUOT,     NUHS, FN0,                        P4,  P5,  P6,  PCMM, // FN0<->ENT
    LSFT,NUBS,Z,   X,   C,   V,   B,   N,   M,   COMM,DOT, SLSH,     RO,  RSFT,          UP,           P1,  P2,  P3,  PENT,
    LCTL,LGUI,LALT,MHEN,          SPC,           HENK,KANA,RALT,RGUI,FN0, RCTL,     LEFT,DOWN,RGHT,    HELLO,  PDOT,  PEQL  // HELLO prepended to AC_HELLO // original sequence was: P0, PDOT, PEQL
    ),
    UNIMAP(
              TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,
    GRV,      TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,          TRNS,TRNS,BTLD,         TRNS,TRNS,TRNS,
    ESC, F1,  F2,  F3,  F4,  F5,  F6,  F7,  F8,  F9,  F10, F11, F12, INS, DEL,      TRNS,TRNS,TRNS,    TRNS,TRNS,TRNS,TRNS,
    TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,PSCR,SLCK,PAUS,UP,  INS,      TRNS,     TRNS,TRNS,TRNS,    F17 ,F18, F19,TRNS,
    TRNS,VOLD,VOLU,MUTE,TRNS,TRNS,TRNS,TRNS,HOME,PGUP,LEFT,RGHT,     TRNS,TRNS,                        F24 ,F15, F16 ,TRNS,
    TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,END, PGDN,DOWN,     TRNS,TRNS,          PGUP,         F21 ,F22 ,F23 ,TRNS,
    TRNS,TRNS,TRNS,TRNS,          TRNS,          TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,     HOME,PGDN,END,     F20,  PDOT,  PEQL     // original sequence: P0, PDOT, PEQL
    ),
};

#ifdef KEYMAP_SECTION_ENABLE
const action_t fn_actions[] __attribute__ ((section (".keymap.fn_actions"))) = { // syagrius
#else
const action_t fn_actions[] PROGMEM = {
#endif
     [0] = ACTION_MACRO_TAP(SAY_HELLO), // will print 'hello' for example
     [1] = ACTION_MACRO(ALT_TAB)
};

// Syagrius
const macro_t *action_get_macro(keyrecord_t *record, uint8_t id, uint8_t opt)
{
    switch (id) {
        case SAY_HELLO:
        xprintf("count %d interrupt: %d\n",record->tap.count,record->tap.interrupted);
        if (record->event.pressed) {            
            // Si on reste appuyé on passe ce-dessous
            if ((record->tap.count <= 0) || (record->tap.interrupted))
                return(MACRO( I(0), T(P3), END ));
            
            // tap detected
            if ((record->tap.count > 2) && !(record->tap.interrupted))
                return(MACRO( I(0), T(BSPC), T(BSPC), T(H), T(E), T(L), T(L), W(255), T(O), END ));
        } 
        else // !event.pressed
        {
            //if((record->tap.count <= 0) && record->tap.interrupted)
            // Detection d'un relachement après longue pression, on fait rien
            if(record->tap.count <= 0) 
                break;
                //return(MACRO( I(200), T(P4), END ));

            // Ici on est sur la remontée de touche lorsqu'elle est appuyée une seule fois
            if(record->tap.count == 1) // si on a pas fait le TAP => envoi pas la touche
              return(MACRO( I(0), T(P0), END ));
            
            if(record->tap.count == 2) // si on a pas fait le TAP => envoi pas la touche au second tap
              return(MACRO( I(0), T(P0), END ));
        }
		break;
		
        case 2:
        if (record->event.pressed) {
        	if (record->tap.count >= 3)
				return(MACRO( I(0), T(H), T(E), T(L), T(L), W(255), T(O), END ));
	       
		   	if ((record->tap.count > 0) && record->tap.interrupted) 
       		   	return(MACRO(I(0), T(H), END));
       		return(MACRO_NONE);
		}  
		else
        { // not event.pressed
        	 return(MACRO_NONE);
        }
        
/*            return (record->event.pressed ?
                    MACRO( I(0), T(H), T(E), T(L), T(L), W(255), T(O), END ) :
                    MACRO_NONE );
                    */
        case 1:
            return (record->event.pressed ?
                    MACRO( D(LALT), D(TAB), END ) :
                    MACRO( U(TAB), END ));
    }
    return MACRO_NONE;
}
