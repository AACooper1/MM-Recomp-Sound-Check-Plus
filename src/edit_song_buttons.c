#include "edit_song_buttons.h"

RECOMP_HOOK_RETURN("ConsoleLogo_Init") void replace_human_wind_fish()
{
    gOcarinaSongButtons[OCARINA_SONG_WIND_FISH_HUMAN] = replaced_human_wind_fish_btns;
    gOcarinaSongButtons[OCARINA_SONG_WIND_FISH_DEKU] = replaced_deku_wind_fish_btns;
    gOcarinaSongButtons[OCARINA_SONG_WIND_FISH_GORON] = replaced_goron_wind_fish_btns;

    for (int i = 0; i < 20; i++)
    {
        sOcarinaSongNotes[OCARINA_SONG_WIND_FISH_HUMAN][i] = replaced_human_wind_fish_notes[i];
        sOcarinaSongNotes[OCARINA_SONG_WIND_FISH_DEKU][i] = replaced_deku_wind_fish_notes[i];
        sOcarinaSongNotes[OCARINA_SONG_WIND_FISH_GORON][i] = replaced_goron_wind_fish_notes[i];
    }

    recomp_printf("Successfully replaced Wind Fish notes!\n");

    AudioApiSequenceInfo info = { 0 };
    static unsigned char* modPath = NULL;
    if (modPath == NULL)
    {
        modPath = recomp_get_mod_file_path();
    }

    AudioApi_AddSequenceFromFs(&info, modPath, "MEGALOVANIA.zseq");
    uintptr_t megaloDevAddr = AudioApi_GetResourceDevAddr(info.resourceId);

    AudioTableEntry megalovania = {
        megaloDevAddr,
        info.filesize,
        MEDIUM_CART,
        info.cacheStrategy,
        0,
        0,
        0
    };

    recomp_printf("MEGALOVANIA is at %p.\n", megaloDevAddr);
    AudioApi_ReplaceSequence(NA_BGM_BALLAD_OF_THE_WIND_FISH, &megalovania);
    recomp_printf("Replaced!\n");
}

RECOMP_HOOK("Message_DrawOcarinaButtons") void get_playstate_and_gfx(PlayState* play, Gfx** gfxP)
{
    gPlay = play;
    gGfx = gfxP;
}

RECOMP_HOOK_RETURN("Message_DrawOcarinaButtons") void draw_the_shit()
{
    PlayState* play = gPlay;
    Gfx** gfxP = gGfx;
    MessageContext* msgCtx = &play->msgCtx;
    Gfx* gfx = *gfxP;
    u16 i;
    u16 notePosX;

    if ((play->msgCtx.msgMode >= MSGMODE_OCARINA_PLAYING) && (msgCtx->msgMode <= MSGMODE_40)) 
    {
        if ((msgCtx->ocarinaAction != OCARINA_ACTION_FREE_PLAY) && (msgCtx->ocarinaAction != OCARINA_ACTION_CHECK_NOTIME)) 
            {
            Gfx_SetupDL39_Ptr(&gfx);

            gDPSetCombineLERP(gfx++, PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIMITIVE, 0, PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIMITIVE, 0);

            if ((msgCtx->msgMode == MSGMODE_SONG_PROMPT) || (msgCtx->msgMode == MSGMODE_36)) 
            {
                if ((msgCtx->ocarinaAction != OCARINA_ACTION_PROMPT_EVAN_PART1_SECOND_HALF) && (msgCtx->ocarinaAction != OCARINA_ACTION_PROMPT_EVAN_PART2_SECOND_HALF)) 
                {
                    if ((msgCtx->ocarinaAction >= OCARINA_ACTION_PROMPT_WIND_FISH_HUMAN) && (msgCtx->ocarinaAction <= OCARINA_ACTION_PROMPT_WIND_FISH_DEKU)) 
                     {
                        sOcarinaButtonStepG = msgCtx->ocarinaAction - OCARINA_ACTION_PROMPT_WIND_FISH_HUMAN + OCARINA_SONG_WIND_FISH_HUMAN;
                    }
                    else 
                    {
                        sOcarinaButtonStepG = msgCtx->ocarinaAction - OCARINA_ACTION_PROMPT_SONATA;
                    }
                    sOcarinaButtonStepR = gOcarinaSongButtons[sOcarinaButtonStepG].numButtons;

                        
                        for (int noteposX = 100, i = 0; noteposX < gOcarinaSongButtons[play->msgCtx.ocarinaAction - OCARINA_ACTION_TIMED_PROMPT_SONATA].numButtons; noteposX += 18, i++)
                        {
                            gDPPipeSync(gfx++);
                            gDPSetPrimColor(gfx++, 0, 0, 150, 150, 150, 150);
                            gDPSetEnvColor(gfx++, 10, 10, 10, 0);

                            gDPLoadTextureBlock(
                            gfx++, gZButtonTex,
                            G_IM_FMT_IA, G_IM_SIZ_8b, 16, 16, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP,
                            G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);

                        gSPTextureRectangle(
                            gfx++, notePosX << 2,
                            msgCtx->ocarinaButtonsPosY[gOcarinaSongButtons[sOcarinaButtonStepG].buttonIndex[i]] << 2,
                            (notePosX + 16) << 2,
                            (msgCtx->ocarinaButtonsPosY[gOcarinaSongButtons[sOcarinaButtonStepG].buttonIndex[i]] + 16)
                                << 2,
                            G_TX_RENDERTILE, 0, 0, 1 << 10, 1 << 10);
                        }
                        
                    }
                }
            }

    }

    *gfxP = gfx;
}

s32 _lastInitializedSeqPlayerIndex;
char* megaloAddr;

void print_bytes(void* addr, int n)
{
    recomp_printf("Data starting from %p is:\n\n", addr);
    recomp_printf("\t\t00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F\n\n");

    uintptr_t addrInt = (uintptr_t) addr;
    recomp_printf("%08x\t", addrInt);

    for (int i = 0; i < n; i++)
    {
        if (i % 16 == 0 && i != 0) 
        {
            recomp_printf("\n%08x\t", addrInt + i);
        }
        
        recomp_printf("%02x ", *(unsigned char*)(addr + i));
    }
    recomp_printf("\n\n");
}

RECOMP_HOOK("Audio_PlayFanfareWithPlayerIOCustomPort") void print_port_io(u16 seqId, s8 ioPort, u8 ioData)
{
    recomp_printf("Playing seq %x with IO value %x on port %x.\n");
}

SequencePlayer* player_to_modify;
u8 channel_to_disable;

RECOMP_HOOK("AudioScript_SequenceChannelEnable") void get_seqplayer_to_disable(SequencePlayer* seqPlayer, u8 channelIndex, void* script)
{
    player_to_modify = seqPlayer;
    channel_to_disable = channelIndex;
}

RECOMP_HOOK_RETURN("AudioScript_SequenceChannelEnable") void stop_disabled_channels()
{
    SequencePlayer* seqPlayer = player_to_modify;

    if (seqPlayer->seqId == NA_BGM_BALLAD_OF_THE_WIND_FISH)
    {
        s8 ioPortVal = seqPlayer->seqScriptIO[4];
        if (ioPortVal & 1 << channel_to_disable)
        {
            AudioScript_SequenceChannelDisable(seqPlayer->channels[channel_to_disable]);
            recomp_printf("\nEnabled and then disabled channel %x.\n", channel_to_disable);
        }
        else
        {
            recomp_printf("\n%x & %x == %x; did not disable channel %x.\n", ioPortVal, 1 << channel_to_disable, ioPortVal & (1 << channel_to_disable), channel_to_disable);
        }
    }
    
    return;
}

RECOMP_HOOK("AudioLoad_SyncInitSeqPlayer") void before_AudioLoad_SyncInitSeqPlayer(s32 playerIndex, s32 seqId, s32 arg2)
{
    _lastInitializedSeqPlayerIndex = playerIndex;
}

// Prints live data read by seqPlayer playing Sun's Song. 
// Disabling but not deleting in case I need to use it in the future.

RECOMP_HOOK_RETURN("AudioLoad_SyncInitSeqPlayer") void after_AudioLoad_SyncInitSeqPlayer()
{
    SequencePlayer* seqPlayer = &gAudioCtx.seqPlayers[_lastInitializedSeqPlayerIndex];
    int seqId = seqPlayer->seqId;
    if (seqId == NA_BGM_BALLAD_OF_THE_WIND_FISH)
    {
        recomp_printf("Wind Fish loaded on player %x. Saving addr...\n", seqPlayer->playerIndex);
        megaloAddr = seqPlayer->seqData;
        print_bytes(megaloAddr, 0x24);
        recomp_printf("Sun's Song handled!\n");
    }
}

RECOMP_HOOK("AudioScript_ScriptReadU8") void print_the_thingy(SeqScriptState* state)
{
    if (state->pc - (u8*)megaloAddr < 0x1000 && state->pc - (u8*)megaloAddr > -0x1000)
    {
        recomp_printf("%02x ", *state->pc);
    }
}

RECOMP_HOOK("AudioScript_ScriptReadS16") void print_the_thingy2(SeqScriptState* state)
{
    if (state->pc - (u8*)megaloAddr < 0x1000 && state->pc - (u8*)megaloAddr > -0x1000)
    {
        s16 ret = *(state->pc) << 8;

        ret = *(state->pc + 1) | ret;
        recomp_printf("%04x ", ret);
    }
}

RECOMP_HOOK("AudioScript_SequencePlayerSetupChannels") void print_the_thingy3(SequencePlayer* seqPlayer, u16 channelBits)
{
    if (seqPlayer->seqId != NA_BGM_BALLAD_OF_THE_WIND_FISH) { return; }
    recomp_printf("\n(Inited channels with bits & %x!)\n", channelBits);
}