#ifndef _ST8_OS_H
#define _ST8_OS_H

enum {
	OS_STATUS_RUNNING,
	OS_STATUS_READY,
	OS_STATUS_WAIT,
	OS_STATUS_HALT
};

typedef struct {
	void *sender;
	void *message;
} OS_message_t;

void *OS_init (void);
void *OS_createTask (void (* task_main)(void), unsigned char priority, unsigned int stack_size);


#endif