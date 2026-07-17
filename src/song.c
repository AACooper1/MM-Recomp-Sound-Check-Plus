#include "song.h"

void apply_tempo_to_notes(MilkBarSong* song)
{
    for (int i = 0; i < song->numNotes; i++)
    {
        song->notes[i].length *= song->tempo;
    }
}