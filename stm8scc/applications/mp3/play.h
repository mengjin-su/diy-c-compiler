#ifndef _PLAY_H
#define _PLAY_H

#include "fat.h"

enum {
	PLAY_RUN,
	PLAY_DONE,
	PLAY_PAUSE,
	PLAY_INIT,
	PLAY_READY,
	PLAY_ERR = -1
};

typedef struct {

	unsigned long secRemain;	// remaining sectors count
    unsigned char inPlay;		// signal: 0 = not in play
    FileDesc_t   *fileDesc;		// file descriptor

} play_ctrl_t;

extern _bank0_ char play_state;
extern _bank0_ play_ctrl_t PLAY_ctrlBlk;

void PLAY_init (FileDesc_t *file_desc);
char PLAY_music (void);
void PLAY_fade_out (void);

#endif
