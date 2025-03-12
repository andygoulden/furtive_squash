#ifndef __FS_FREERTOS_H
#define __FS_FREERTOS_H

#include <stdbool.h>

// Task declarations
void systemTask(void* parameters);

bool rtosStarted(void);
void start_freertos();

#endif  // __FS_FREERTOS_H
