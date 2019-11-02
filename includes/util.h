#ifndef _UTIL_H_
#define _UTIL_H_

#define ROOT                    "/"
#define APP_PATH                "/switch/kefir-updater/"
#define AMS_OUTPUT              "/switch/kefir-updater/ams.zip"
#define APP_OUTPUT              "/switch/kefir-updater/kefir-updater.nro"
#define OLD_APP_PATH            "/switch/kefir-updater.nro"

#define YES                     10
#define NO                      20
#define ON                      1
#define OFF                     0

void writeSysVersion();                                                 // writes the sys version.
void writeAmsVersion();                                                 // writes the ams version with hash.
char *getSysVersion();                                                  // returns sys version.
char *getAmsVersion();                                                  // returns ams version.

void copyFile(char *src, char *dest);                                   // basic copy file. Use malloc if you need dynamic mem
int parseSearch(char *phare_string, char *filter, char* new_string);    // hacky way to parse a file for a string
int update_atmo(char *url, char *output, int mode);                     // update either ams or hekate
int update_sxos(char *url, char *output, int mode);                     // update either ams or hekate
void update_app();                                                      // update the app

#endif