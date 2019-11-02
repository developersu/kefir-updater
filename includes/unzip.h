#ifndef _UNZIP_H_
#define _UNZIP_H_

#define UP_AMS          0
#define UP_SX           1
#define REBOOT_PAYLOAD  2

int unzip(const char *output, int mode);

#endif