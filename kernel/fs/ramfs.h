#pragma once

#include "../limine.h"
#include "../kernel.h"

/*
 | offset |  size  | Description
 |   0    |   100  |  File name
 |   100  |   8    |  File mode
 |   108  |   8    |  Owner's numeric user ID
 |   116  |   8    |  Group's numeric user ID
 |   124  |   12   |  File size in bytes (octal base)
 |   136  |   12   |  Last modification time in numeric Unix time format (octal)
 |   148  |   8    |  Checksum for header record
 |   156  |   1    |  Type flag
 |   157  |   100  |  Name of linked file
 |   257  |   6    |  UStar indicator "ustar" then NUL
 |   263  |   2    |  UStar version "00"
 |   265  |   32   |  Owner user name
 |   297  |   32   |  Owner group name
 |   329  |   8    |  Device major number
 |   337  |   8    |  Device minor number
 |   345  |   155  |  Filename prefix
*/

/* Typeflag field definitions */
#define REGTYPE 	'0'	/* Regular file. */
#define LNKTYPE		'1'	/* Link. */
#define SYMTYPE		'2'	/* Symbolic link. */
#define CHRTYPE		'3'	/* Character special. */
#define BLKTYPE		'4'	/* Block special. */
#define DIRTYPE		'5'	/* Directory. */
#define FIFOTYPE	'6'	/* FIFO special. */
#define CONTTYPE	'7'	/* Reserved. */
/* Mode field bit definitions (octal) */
#define	TSUID		04000	/* Set UID on execution. */
#define	TSGID		02000	/* Set GID on execution. */
#define	TSVTX		01000	/* On directories, restricted deletion flag. */
#define	TUREAD		00400	/* Read by owner. */
#define	TUWRITE		00200	/* Write by owner. */
#define	TUEXEC		00100	/* Execute/search by owner. */
#define	TGREAD		00040	/* Read by group. */
#define	TGWRITE		00020	/* Write by group. */
#define	TGEXEC		00010	/* Execute/search by group. */
#define	TOREAD		00004	/* Read by other. */
#define	TOWRITE		00002	/* Write by other. */
#define	TOEXEC		00001	/* Execute/search by other. */

struct tar_header
{
    char filename[100];
    char mode[8];
    char uid[8];
    char gid[8];
    char size[12];
    char mtime[12];
    char chksum[8];
    char typeflag[1];
};


extern struct tar_header *headers[32];
unsigned int parse();
unsigned int getsize(const char *in);
char *readFile(char* fileToReadName);
void ls();