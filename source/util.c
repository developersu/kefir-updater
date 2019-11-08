#include <stdio.h>
#include <string.h>
#include <switch.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

#pragma once

#include "util.h"
#include "menu.h"
#include "unzip.h"
#include "download.h"
#include "reboot_payload.h"
// #include "../types.h"
// #include "../kernel/event.h"
// #include "../services/acc.h"
// #include "../sf/service.h"

#define TEMP_FILE               "/switch/atmosphere-updater/temp"
#define FILTER_STRING           "browser_download_url\":\""

char g_sysVersion[50];
// char g_amsVersion[50];


char *getSysVersion()
{
    return g_sysVersion;
}

// char *getAmsVersion()
// {
    // return g_amsVersion;
// }

void writeSysVersion()
{
	Result ret = 0;
	SetSysFirmwareVersion ver;

	if (R_FAILED(ret = setsysGetFirmwareVersion(&ver))) 
    {
		printf("GetFirmwareVersion() failed: 0x%x.\n\n", ret);
		return;
	}

    char sysVersionBuffer[20];
	snprintf(sysVersionBuffer, 20, "%u.%u.%u", ver.major, ver.minor, ver.micro);
    snprintf(g_sysVersion, sizeof(g_sysVersion), "Версия системного ПО: %s", sysVersionBuffer);
}

/*void writeAmsVersion()
{
	Result ret = 0;
	u64 ver;
    u64 fullHash;
    SplConfigItem SplConfigItem_ExosphereVersion = (SplConfigItem)65000;
    SplConfigItem SplConfigItem_ExosphereVerHash = (SplConfigItem)65003;

	if (R_FAILED(ret = splGetConfig(SplConfigItem_ExosphereVersion, &ver))) 
    {
		printf("SplConfigItem_ExosphereVersion() failed: 0x%x.\n\n", ret);
		return;
	}

    if (R_FAILED(ret = splGetConfig(SplConfigItem_ExosphereVerHash, &fullHash))) 
    {
		printf("SplConfigItem_ExosphereVerHash() failed: 0x%x.\n\n", ret);
		return;
	}

    // write only the first 8 char of the hash.
    char shortHash[8];
	snprintf(shortHash, sizeof(shortHash), "%lx", fullHash);

    // write ams version number + hash.
    char amsVersionNum[25];
	snprintf(amsVersionNum, sizeof(amsVersionNum), "%lu.%lu.%lu (%s)", (ver >> 32) & 0xFF,  (ver >> 24) & 0xFF, (ver >> 16) & 0xFF, shortHash);

    // write string + ams version to global variable.
    snprintf(g_amsVersion, sizeof(g_amsVersion), "Atmosphere Ver: %s", amsVersionNum);
}*/

void copyFile(char *src, char *dest)
{
    FILE *srcfile = fopen(src, "rb");
    FILE *newfile = fopen(dest, "wb");

    if (srcfile && newfile)
    {
        char buffer[10000]; // 10kb per write, which is fast
        size_t bytes;       // size of the file to write (10kb or filesize max)

        while (0 < (bytes = fread(buffer, 1, sizeof(buffer), srcfile)))
        {
            fwrite(buffer, 1, bytes, newfile);
        }
    }
    fclose(srcfile);
    fclose(newfile);
}
// Check if it's dir on 'path' 
int is_dir(const char* path){
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISDIR(path_stat.st_mode);
}

// Remove entry
int remove_entry(const char* dir_name){
    if (is_dir(dir_name)!=0)
        return remove(dir_name);

    DIR *dir;
    dir = opendir(dir_name);

    size_t dSz = strlen(dir_name);
    struct dirent *s_dirent;

    char* full_name;

    while ((s_dirent = readdir(dir)) != NULL){
        if ((strcmp(s_dirent->d_name, ".") == 0) || (strcmp(s_dirent->d_name, "..") == 0))
            continue;
        full_name = malloc(dSz + strlen(s_dirent->d_name) + 2); // '/'+'\0'

        strcpy(full_name, dir_name);
        strcat(full_name, "/");
        strcat(full_name, s_dirent->d_name);    

        if (is_dir(full_name)){
            remove_entry(full_name);
        }
        remove(full_name);      // NOTE: Not validating returning value; could be -1; TODO: Add validation/notification/log-reporting

        free(full_name);
    }


    closedir(dir);
    remove(dir_name);           // NOTE: Not validating returning value; could be -1; TODO: Add validation/notification/log-reporting
    
    return 0;
}

int update_atmo(char *url, char *output, int mode)
{
    remove_old();
    errorBox(400, 250, "      Update complete!\nRestart app to take effect");

    /*if (!downloadFile(AMS_URL, output, OFF))
    {
        unzip(output, mode);  
            if (yesNoBox1(390, 250, "Reboot to Payload?") == YES)
                reboot_payload("/atmosphere/reboot_payload.bin");
        return 0;

    }*/

    return 1;
    
}

int update_sxos(char *url, char *output, int mode)
{   

    if (!downloadFile(SXOS_URL, output, OFF))
    {
        unzip(output, mode);

            if (yesNoBox1(390, 250, "Reboot to Payload?") == YES)
                reboot_payload("/atmosphere/reboot_payload.bin");
        return 0;

    }

    return 1;
}



void remove_old()
{
    remove_entry("/atmosphere/titles/4200000000000010");
    remove_entry("/atmosphere/titles/00FF0012656180FF");
    /*FS_RemoveDir(fs, "/atmosphere/titles/420000000000000E");
    FS_RemoveDir(fs, "/atmosphere/titles/010000000000100B");
    FS_RemoveDir(fs, "/atmosphere/titles/01FF415446660000");
    FS_RemoveDir(fs, "/atmosphere/titles/0100000000000352");
    FS_RemoveDir(fs, "/atmosphere/titles/00FF747765616BFF");
    remove("/atmosphere/kips/fs_mitm.kip");
    remove("/atmosphere/kips/ldn_mitm.kip");
    remove("/atmosphere/kips/loader.kip");
    remove("/atmosphere/kips/pm.kip");
    remove("/atmosphere/kips/sm.kip");
    remove("/atmosphere/kips/ams_mitm.kip");
    remove("/atmosphere/flags/hbl_cal_read.flag");*/
}

/*
void update_app()
{
    // download new nro as a tempfile.
    if (!downloadFile(APP_URL, TEMP_FILE, OFF))
    {
        // remove current nro file.
        remove(APP_OUTPUT);
        // remove nro from /switch/.
        remove(OLD_APP_PATH);
        // rename the downloaded temp_file with the correct nro name.
        rename(TEMP_FILE, APP_OUTPUT);
        // using errorBox as a message window on this occasion. 
        errorBox(400, 250, "      Update complete!\nRestart app to take effect");
    }
}
*/
