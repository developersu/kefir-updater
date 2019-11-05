#ifndef _DOWNLOAD_H_
#define _DOWNLOAD_H_

#define AMS_URL     "https://github.com/rashevskyv/switch/releases/latest/download/atmo.zip"
#define SXOS_URL    "https://github.com/rashevskyv/switch/releases/latest/download/sxos.zip"
#define APP_URL     "https://github.com/ITotalJustice/atmosphere-updater/releases/latest/download/atmosphere-updater.nro"

int downloadFile(const char *url, const char *output, int api_mode);

#endif