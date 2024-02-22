/**
 * File: easy_mode_ending_msg.c
 * Description: Displays the ending message when you beat Easy Mode
 * Associated objects: 0x2140
 *
 * Mapped by the TLB? = Yes
 */

#include "objects/menu/easy_mode_ending_msg.h"
#include "sound.h"
#include "cv64.h"
#include "atari.h"
#include "system_work.h"

/*
 *   You have completed
 *   the game in EASY mode.
 *   If you want to face
 *   the challenge
 *       of more stages,
 *   try NORMAL mode.
 */
u16 cv64_ovl_easyending_msg[] = {
#include "objects/menu/easy_mode_ending.msg"
};

cv64_ovl_easyending_func_t cv64_ovl_easyending_funcs[] = {
    cv64_ovl_easyending_init,
    cv64_ovl_easyending_loop,
    cv64_ovl_easyending_destroy
};

void cv64_ovl_easyending_entrypoint(cv64_ovl_easyending_t* self) {
    ENTER(self, cv64_ovl_easyending_funcs);
}

void cv64_ovl_easyending_init(cv64_ovl_easyending_t* self) {
    mfds_state* new_textbox;
    u32 message_ptr;

    (*play_sound)(SD_00A);
    new_textbox = (*textbox_create)(
        self, common_camera_HUD, (OPEN_TEXTBOX | FAST_TEXT_TRANSITION)
    );
    self->ending_textbox = new_textbox;
    (*textbox_setPos)(new_textbox, 30, 110, 1);
    (*textbox_setDimensions)(new_textbox, 6, 250, 0, 0);
    new_textbox->palette = TEXT_COLOR_WHITE;
    new_textbox->display_time = 0;
    message_ptr = (*text_getMessageFromPool)(cv64_ovl_easyending_msg, 0);
    (*textbox_setMessagePtr)(
        new_textbox,
        GET_UNMAPPED_ADDRESS(NI_OVL_EASY_MODE_ENDING_MSG, message_ptr),
        NULL,
        0
    );
    (*textbox_enableLens)(
        new_textbox,
        (WINDOW_FLAG_40000 | WINDOW_FLAG_20 | WINDOW_FLAG_10 | WINDOW_FLAG_4 |
         WINDOW_FLAG_1),
        30
    );
    (*object_curLevel_goToNextFuncAndClearTimer)(
        self->header.current_function, &self->header.functionInfo_ID
    );
}

void func_0F00018C() {}

void cv64_ovl_easyending_loop(cv64_ovl_easyending_t* self) {
    s16 i, j;
    u32* textbox_flags = &self->ending_textbox->flags;

    self->active_time++;
    if ((self->active_time > 120) &&
        (sys.controllers[0].buttons_pressed & A_BUTTON)) {
        *textbox_flags |= CLOSE_TEXTBOX;
        sys.SaveStruct_gameplay.money = 0;
        sys.SaveStruct_gameplay.time_saved_counter = 0;
        sys.SaveStruct_gameplay.death_counter = 0;
        sys.SaveStruct_gameplay.field55_0xbc = 0;
        sys.SaveStruct_gameplay.field59_0xc0 = 0;
        sys.SaveStruct_gameplay.field63_0xc4 = 0;
        sys.SaveStruct_gameplay.field67_0xc8 = 0;
        sys.SaveStruct_gameplay.field69_0xca = 0;
        sys.SaveStruct_gameplay.field71_0xcc = 0;
        sys.SaveStruct_gameplay.field75_0xd0 = 0;
        sys.SaveStruct_gameplay.field77_0xd2 = 0;
        sys.SaveStruct_gameplay.field79_0xd4 = 0;
        sys.SaveStruct_gameplay.field83_0xd8 = 0;
        sys.SaveStruct_gameplay.money_spent_on_Renon = 0;
        sys.SaveStruct_gameplay.player_status = 0;
        sys.SaveStruct_gameplay.life = 100;
        sys.SaveStruct_gameplay.field13_0x5c = 100;
        sys.SaveStruct_gameplay.subweapon = SUBWEAPON_NONE;
        sys.current_PowerUp_level = 0;
        sys.SaveStruct_gameplay.health_depletion_rate_while_poisoned = 0;
        sys.SaveStruct_gameplay.current_hour_VAMP = 0;
        sys.SaveStruct_gameplay.week = 0;
        sys.SaveStruct_gameplay.day = 0;
        sys.SaveStruct_gameplay.hour = 0;
        sys.SaveStruct_gameplay.minute = 0;
        sys.SaveStruct_gameplay.seconds = 0;
        sys.SaveStruct_gameplay.milliseconds = 0;

        // clang-format off

        // Clear event flags
        for (i = 0; i < NUM_EVENT_FLAGS; i++) sys.SaveStruct_gameplay.event_flags[i] = 0;
        // Remove inventory items
        for (j = 0; j < SIZE_ITEMS_ARRAY; j++) sys.SaveStruct_gameplay.items.array[j] = 0;

        // clang-format on

        (*atari_work_table_init)();
        self->active_time = 0;
        (*object_curLevel_goToNextFuncAndClearTimer)(
            self->header.current_function, &self->header.functionInfo_ID
        );
    }
}

void cv64_ovl_easyending_destroy(cv64_ovl_easyending_t* self) {
    u32 textbox_flags;

    self->active_time++;
    if (self->active_time > 30) {
        textbox_flags = self->ending_textbox->flags;
        if (!(textbox_flags & MFDS_FLAG_20000000) &&
            !(textbox_flags & CLOSE_LENS) &&
            !(textbox_flags & MFDS_FLAG_2000000)) {
            if (!(textbox_flags & TEXTBOX_IS_ACTIVE)) {
                self->ending_textbox->flags = textbox_flags | CLOSE_TEXTBOX;
            }
            self->header.destroy(self);
        }
    }
}
