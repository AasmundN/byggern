#ifndef __TC__
#define __TC__

void TC_init(unsigned long period);

void TC_set_cb(void (*cb)());

#endif // __TC__
