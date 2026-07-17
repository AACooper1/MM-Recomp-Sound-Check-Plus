#ifndef SONG_H
#define SONG_H

#include "modding.h"
#include "global.h"
#include "recomputils.h"
#include "recompconfig.h"

#include "musicdefs.h"

typedef struct MilkBarSong_t {
    int id;
    const char* name;
    u8 tempo;

    OcarinaNote* notes;
    u8 numNotes;
} MilkBarSong;

MilkBarSong gMilkBarSongs[64];

void apply_tempo_to_notes(MilkBarSong* song);

#endif