#include "../include/inv_aes.h"
#include "../include/aes.h"
#include "../include/tables.h"


inline void
MAES_inv_sub_words(uint state[])
{
    state[0] = MAES_inv_sub_word_m(state[0]);
    state[1] = MAES_inv_sub_word_m(state[1]);
    state[2] = MAES_inv_sub_word_m(state[2]);
    state[3] = MAES_inv_sub_word_m(state[3]);
    printf("inv_sub_words: %08x %08x %08x %08x\n", state[0], state[1], state[2], state[3]);
}

inline void
MAES_inv_shift_rows(uint state[])
{
    uint temp[4] = {state[0], state[1], state[2], state[3]};

    state[0] = MAES_shift_row_m(temp[0], temp[3], temp[2], temp[1]);
    state[1] = MAES_shift_row_m(temp[1], temp[0], temp[3], temp[2]);
    state[2] = MAES_shift_row_m(temp[2], temp[1], temp[0], temp[3]);
    state[3] = MAES_shift_row_m(temp[3], temp[2], temp[1], temp[0]);

    printf("inv_shift_rows: %08x %08x %08x %08x\n", state[0], state[1], state[2], state[3]);
}

void
MAES_inv_mix_column(uint state[],
                    int i)
{
    uint byte1 = state[i] >> 24,
         byte2 = (state[i] >> 16) & 0xff,
         byte3 = (state[i] >> 8) & 0xff,
         byte4 = state[i] & 0xff;

    state[i] = 0;
    state[i] |= (mul0xe[byte1] ^ mul0xb[byte2] ^ mul0xd[byte3] ^ mul0x9[byte4]) << 24;
    state[i] |= (mul0x9[byte1] ^ mul0xe[byte2] ^ mul0xb[byte3] ^ mul0xd[byte4]) << 16;
    state[i] |= (mul0xd[byte1] ^ mul0x9[byte2] ^ mul0xe[byte3] ^ mul0xb[byte4]) << 8;
    state[i] |= (mul0xb[byte1] ^ mul0xd[byte2] ^ mul0x9[byte3] ^ mul0xe[byte4]);
}

void
MAES_inv_init_round(uint state[])
{
    printf("--- first inv round ---\n");
    MAES_add_round_keys(state, &round_keys[n_round * 4]);
}

void
MAES_inv_round(uint state[],
               int  round)
{
    printf("--- %d inv round ---\n", round);
    MAES_inv_shift_rows(state);
    MAES_inv_sub_words(state);
    MAES_add_round_keys(state, &round_keys[round * 4]);
    MAES_mix_columns(state, MAES_inv_mix_column);
}

void
MAES_inv_final_round(uint state[])
{
    printf("--- final inv round ---\n");
    MAES_inv_shift_rows(state);
    MAES_inv_sub_words(state);
    MAES_add_round_keys(state, &round_keys[0]);
}
