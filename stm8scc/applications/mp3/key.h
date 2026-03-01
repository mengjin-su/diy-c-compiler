#ifndef _KEY_H
#define _KEY_H

/* --- key value definition --- */
enum {	KEY_PLAY = 1,
		KEY_UP   = 2,
		KEY_DOWN = 4};
		
#define KEY_PLAY_PIN	5	// PA.5
#define KEY_UP_PIN		2	// PC.2
#define KEY_DOWN_PIN	5	// PE.5		
		
void KEY_init (void);
void KEY_scan (void);
char KEY_get  (void);

#endif
