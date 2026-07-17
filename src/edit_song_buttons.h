#include "modding.h"
#include "global.h"
#include "recomputils.h"
#include "recompconfig.h"

#include "audio_api/all.h"

#define AUGMENT(x) x * 1.5

extern OcarinaNote sOcarinaSongNotes[OCARINA_SONG_MAX][20];
PlayState* gPlay;
Gfx** gGfx;

extern s16 sOcarinaButtonStepR;
extern s16 sOcarinaButtonStepG;
extern s16 sOcarinaButtonStepB;
extern s16 sOcarinaButtonFlashTimer;
extern s16 sOcarinaButtonFlashColorIndex;
extern s16 sOcarinaButtonDropYOffset;

extern u64 gZButtonTex[];

typedef enum NoteLength_t
{
    THIRTY_SECOND_NOTE,
    SIXTEENTH_NOTE,
    EIGHTH_NOTE,
    QUARTER_NOTE,
    HALF_NOTE,
    WHOLE_NOTE
} NoteLength;

OcarinaSongButtons replaced_human_wind_fish_btns = {
    8,
    {
        OCARINA_BTN_C_UP,
        OCARINA_BTN_C_RIGHT,
        OCARINA_BTN_A,
        OCARINA_BTN_C_UP,
        OCARINA_BTN_C_UP,
        OCARINA_BTN_A,
        OCARINA_BTN_C_UP,
        OCARINA_BTN_C_UP,
    }
};

OcarinaNote replaced_human_wind_fish_notes[20] = {
    { OCARINA_PITCH_D5, 11, 80, 0, 0, 0},
    { OCARINA_PITCH_A4, 11, 80, 0, 0, 0},
    { OCARINA_PITCH_D4, 11, 80, 0, 0, 0},
    { OCARINA_PITCH_NONE, 11, 80, 0, 0, 0},
    { OCARINA_PITCH_D5, 11, 80, 0, 0, 0},
    { OCARINA_PITCH_NONE, 22, 80, 0, 0, 0},
    { OCARINA_PITCH_D5, 11, 80, 0, 0, 0},
    { OCARINA_PITCH_NONE, 11, 80, 0, 0, 0},
    { OCARINA_PITCH_D4, 11, 80, 0, 0, 0},
    { OCARINA_PITCH_NONE, 11, 80, 0, 0, 0},
    { OCARINA_PITCH_D5, 22, 80, 0, 0, 0},
    { OCARINA_PITCH_NONE, 11, 80, 0, 0, 0},
    { OCARINA_PITCH_D5, 11, 80, 0, 0, 0},
    { OCARINA_PITCH_NONE, 100, 80, 0, 0, 0}
};

OcarinaSongButtons replaced_deku_wind_fish_btns = {
    4,
    {
        OCARINA_BTN_A,
        OCARINA_BTN_A,
        OCARINA_BTN_C_UP,
        OCARINA_BTN_C_RIGHT
    }
};

OcarinaNote replaced_deku_wind_fish_notes[20] = {
    { OCARINA_PITCH_D4, 11, 80, 0, 0, 0},
    { OCARINA_PITCH_NONE, 1, 80, 0, 0, 0},
    { OCARINA_PITCH_D4, 11, 80, 0, 0, 0},
    { OCARINA_PITCH_D5, 11, 80, 0, 0, 0},
    { OCARINA_PITCH_NONE, 14, 80, 0, 0, 0},
    { OCARINA_PITCH_A4, 11, 160, 0, 0, 0},
    { OCARINA_PITCH_NONE, 100, 66, 0, 0, 0}
};

OcarinaSongButtons replaced_goron_wind_fish_btns = {
    8,
    {
        OCARINA_BTN_A,
        OCARINA_BTN_C_UP,
        OCARINA_BTN_A,
        OCARINA_BTN_A,
        OCARINA_BTN_C_UP,
        OCARINA_BTN_C_RIGHT,
        OCARINA_BTN_C_RIGHT,
        OCARINA_BTN_C_RIGHT,
    }
};

OcarinaNote replaced_goron_wind_fish_notes[20] = {
    { OCARINA_PITCH_D4, 11, 80, 0, 0, 0},
    { OCARINA_PITCH_NONE, 11, 80, 0, 0, 0},
    { OCARINA_PITCH_D5, 11, 80, 0, 0, 0},
    { OCARINA_PITCH_D4, 10, 80, 0, 0, 0},
    { OCARINA_PITCH_NONE, 1, 80, 0, 0, 0},
    { OCARINA_PITCH_D4, 10, 80, 0, 0, 0},
    { OCARINA_PITCH_D5, 11, 80, 0, 0, 0},
    { OCARINA_PITCH_NONE, 11, 80, 0, 0, 0},
    { OCARINA_PITCH_A4, 11, 160, 0, 0, 0},
    { OCARINA_PITCH_NONE, 22, 80, 0, 0, 0},
    { OCARINA_PITCH_A4, 11, 160, 0, 0, 0},
    { OCARINA_PITCH_NONE, 22, 80, 0, 0, 0},
    { OCARINA_PITCH_A4, 11, 160, 0, 0, 0},
    { OCARINA_PITCH_NONE, 100, 66, 0, 0, 0},
};