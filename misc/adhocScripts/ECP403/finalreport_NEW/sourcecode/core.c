
#include "core.h"
#include <myFat_diskio.h>
#include "global_config.h"

#include <string.h> // for use of memset
#include <ctype.h> // for use of toupper

#include <stdlib.h> // for malloc/free

WYT_FileSystem_t FILE_SYSTEM;

// **************************************
// FILESYSTEM OPS
// **************************************

/* TODO */
/* many of these fields will be hard coded for our purposes */

/* parse boot sector, more specifically the BPB for volume info */
/* other parts of boot sector are code */
uint8_t WYT_fs_init(
	void
){
	
	if (WYT_disk_initialize() & STA_NOINIT){
			_Error_Handler(__FILE__, __LINE__);
			return ERROR_CODE;
	}
	
	/* 
	filesystem struct contents:
	uint8_t		fs_type;
	uint8_t		fs_flags;
	uint8_t		fs_sectorsPerCluster;

	uint16_t	fs_bytesPerSector;
	uint16_t	fs_reservedSectors;

	uint32_t	fs_size;
	uint32_t 	fs_nClusters;

	uint32_t	fs_firstCluster;

	WYT_File_t*	fs_activeFile;	// may be useful in future
	WYT_File_t* fs_activeDirectory;
	
	*/
	
	/* initialize disk/device (only support one)*/
		/* query for volume info */
	/* basically get and fill boot sector struct */

	uint8_t* BPB_BUFF = (uint8_t *) malloc(BPBSIZE);


	/* 	
	BPB struct contents --
	
	uint8_t 		BS_jmpBoot[3];
	uint8_t 		BS_OEMName[8];
	
	uint16_t 	    BPB_BytsPerSec;
	uint8_t			BPB_SecPerClus;
	uint16_t		BPB_RsvdSecCnt;
	uint8_t			BPB_NumFATs;
	uint16_t		BPB_RootEntCnt;
	uint16_t		BPB_TotSec16;
	uint8_t			BPB_Media;
	uint16_t		BPB_FATSz16;
	uint16_t		BPB_SecPerTrk;
	uint16_t		BPB_NumHeads;
	uint8_t 		BPB_HiddSec;
	uint8_t 		BPB_TotSec32;
 
	uint8_t		extended_sector[54];
	*/
	
	uint32_t bsect = 0;


	/* note that a call to read() may cause a hardfault if done incorrectly */
	// 12-18: fails here
	if (WYT_disk_read(BPB_BUFF, bsect, 0, sizeof (BPB_BUFF)) != OK){
		_Error_Handler(__FILE__, __LINE__);
		return ERROR_CODE;
	}
	
	/* zero or NULL all of FS struct memory? */
	/* zeroing for now */
	memset(&FILE_SYSTEM,0,sizeof(WYT_FileSystem_t));
	
	/* this is most easily figured out from cluster size -- you need the type to access that type's EBPB, in turn allowing access to the type signature. Go figure. */
	/* ...but for now we're only supporting FAT16, so this can get hardcoded*/
	FILE_SYSTEM.fs_type = 16;
	
	// allocate buffer for EBPB16
	// 12-17: BPB contents not set properly. Step through in debug mode.
	WYT_BPB_base_t BPBbase;
	memcpy(&BPBbase, &BPB_BUFF, sizeof(BPB_BUFF));

	/* flags should be ...what? */
	/* don't know for now, just say 0*/
	FILE_SYSTEM.fs_flags = 0x00;
	
	/* this can be 1,2,4...128 */
	FILE_SYSTEM.fs_sectorsPerCluster = BPBbase.BPB_SecPerClus;
	
	FILE_SYSTEM.fs_bytesPerSector = BPBbase.BPB_BytsPerSec;
	
	FILE_SYSTEM.fs_reservedSectors = BPBbase.BPB_RsvdSecCnt;
	
	
	
	/* number of FATs in volume; almost always 2 */
	FILE_SYSTEM.fs_size = BPBbase.BPB_NumFATs;
	
	
	FILE_SYSTEM.fs_nClusters = (BPBbase.BPB_TotSec16) / (BPBbase.BPB_SecPerClus);
	
	free(BPB_BUFF);

	return 0;
}

// **************************************
// DIRECTORY OPS
// **************************************

/* FUTURE */
/* not implemented */
uint8_t WYT_dir_remove(
	WYT_File_t*  targetDirectory
){
	return ERROR_CODE;
}

/* FUTURE */
/* not implemented */
/* should this or should this not be separate from fil_open()? */
/* both apply to dirents but keeping them separate allows more clear handling of attributes/ flags and special cases */
uint8_t WYT_dir_open(
	const char* targetDirectory,
	uint8_t flags
){
	/* recursively follow path */
	/* check if root dir */
	/* make sure it's a dir */
	/* rewind dir */
	return ERROR_CODE;
}

/* FUTURE */
/* not implemented */
/* can be done by dir/file_open with create flag... keep this?*/
uint8_t WYT_dir_create(
	WYT_File_t* targetDirectory
){
	return ERROR_CODE;
}

/* FUTURE */
/* not implemented */
/* (see WYT_fil_read() instead ) */
/* Read a directory entry (which may itself be a directory)
		- would it be better to have separate functions for specific applications? 
			- we'll see */
/* as of present, thinking actual dirent struct more useful than file struct */
/* references: 

http://elm-chan.org/fsw/ff/doc/readdir.html 
https://elixir.bootlin.com/linux/latest/source/fs/readdir.c

https://code.woboq.org/userspace/glibc/dirent/
	https://code.woboq.org/userspace/glibc/sysdeps/mach/hurd/readdir.c.html
	https://code.woboq.org/userspace/glibc/sysdeps/posix/readdir.c.html

*/
uint8_t WYT_dir_readdir(
	WYT_File_t* targetDirectory
){
	uint8_t status = ERROR_CODE;
	
	/* maybe do some name formatting */
	/* read all of directory sectors ( while sector defined) */
	
	/* check that dirent is valid */
		/* invalid ex: deleted, empty, volume id */
		
	/* check /set the dirent type (file, dir, symlink, etc.?) */
	/* shift index by DWORD (32b) */
	/* read 32b */
	/* check if last dir in table */
	/* valid dirent? */
	
	/* move to next dirent */
	
	
	
	return status;
}

// **************************************
// DIRENTRY AND FILE OPS
// **************************************

// todo: accept input flags for write() to create
uint8_t WYT_fil_create(
	WYT_File_t* targetFile
	,const char* targetFilePath	/* the actual path of the file to open */

){

	// CHECKME: actually figure out what this should be
	uint8_t createflags = 0xFF;

	return WYT_fil_open(targetFile, targetFilePath, createflags);
	
}

/* TODO */
/* open file -- also serves to validate file path by recursive search */
/* also may create file if it doesn't exist, but this will probably be more explicitly handled in a separate function */
// TODO: add flags (readonly, etc.)

uint8_t WYT_fil_open(
	WYT_File_t* targetFile /* the struct pointer to be associated with the file upon successful opening */
	,const char* targetFilePath	/* the actual path of the file to open */
	,uint8_t openFlags
){
		
		uint8_t openStatus = ERROR_CODE;

		WYT_File_t* currentWorkingDirectory = FILE_SYSTEM.fs_activeDirectory;

		WYT_DirEntry_t* direntryBuffer = malloc(sizeof(WYT_DirEntry_t));
	
		/* check every dirent in this directory */
		/* while dirents remain */
		/* this seems doable only with read() to both change dirs and set as open */
		/* note that read operates on the currently open file and so this is not recursively defined */

		memset(direntryBuffer, '0', sizeof(WYT_DirEntry_t));

		while(WYT_fil_read(currentWorkingDirectory,direntryBuffer,sizeof(WYT_DirEntry_t))){
			/* check cwd for file (stringwise?) */
			/* currently returns 0 for match */
			if ((WYT_dir_nameCompare(targetFilePath,
					(char*) (targetFile->fil_activeDirentry)->dirent_name))
					== strnlen(targetFilePath,FILENAME_MAX)){

				openStatus = FILNAME_MATCH;
				targetFile->fil_flags = openFlags;
				break;
			}
			else {
				// if no match
				// change to next dirent
				// todo
				 WYT_fil_lseek(currentWorkingDirectory,
						 -sizeof(WYT_DirEntry_t),
						 SEEK_CUR);
			}


		}
		if (openStatus != FILNAME_MATCH){

			return ERROR_CODE;
		}

		/* check flags */
		
		/*  set object attributes for file/dir */
		free(direntryBuffer);
		return openStatus;
}

/* TODO */
/* write data to currently opened file */
/* we should be abstracted from disk I/O layer; this is not strictly part of the library. just CALL the physical disk read/write routines */
uint8_t WYT_fil_write(
	WYT_File_t* targetFile
	,const void* inputBuffer
	,uint32_t inputSize
){
	uint32_t startSector, newSector;
	uint16_t totalBytesWritten;
	uint16_t currentCycleBytes;
	uint16_t byteInSector;

	DRESULT dr;

	/* check if file is empty; if so first cluster & sector must be initialized */

	/* need to find starting sector for this file */
	startSector = (targetFile->fil_currentCluster)
	+
	((targetFile->fil_pointer) / (FILE_SYSTEM.fs_bytesPerSector)); // convert byte offset to sector offset and use to increment given cluster base

	byteInSector = (targetFile->fil_pointer) / (FILE_SYSTEM.fs_bytesPerSector);

	/* get current sector and current offset from volume info */
	uint32_t remaining_bytes = ((targetFile->fil_activeDirentry)->dirent_size) - targetFile->fil_pointer;

	/* if we want to write more than we have room for, truncate to only space available */
	if (inputSize>remaining_bytes) inputSize=remaining_bytes;

	// while remaining bytes > 0
	// ...but we want to do this in terms of chunks for efficiency ( so i isn't incremented by 1 but rather a variable amount or chunk size
	for (totalBytesWritten=0; totalBytesWritten<inputSize; ){

		// bytes per sector - offset = bytes left in current sector
		// desired buffer size minus already written ... i.e. bytes left to write for this buffer
		// min between these so we don't write over any bounds
		currentCycleBytes = MIN((FILE_SYSTEM.fs_bytesPerSector) - byteInSector,
				inputSize - totalBytesWritten);

		if (currentCycleBytes < FILE_SYSTEM.fs_bytesPerSector){

		}
		else{

		}

		/* todo: check, either before or after reading, whether the current byte is on a sector boundary*/
		/* after reading and incrementing file pointer (byte) we should be on a sector boundary; i.e. have read the whole sector */


		/* todo: check if top of file */
		/* todo: check if cluster boundary */
		/* base case: NOT a boundary */

		dr = WYT_disk_write(inputBuffer, targetFile->fil_currentSector, currentCycleBytes);
				if (dr) return ERROR_CODE;

		// todo: more stuff relating to physical read?
				// maybe set flags

		/* increment file pointer by amount read this cycle */
		targetFile->fil_pointer += currentCycleBytes;
		totalBytesWritten += currentCycleBytes;

		// we want to be writing a complete sector for every pass through this loop... then if at the end we aren't on a sector boundary something is wrong
		if (((targetFile->fil_pointer) % (FILE_SYSTEM.fs_bytesPerSector)) != 0){
			// if we haven't, then the sector must have ended and so we don't need to move to a new one or check boundaries, we can just exit the read loop
			break;
		}

		newSector++;

	}

	return ERROR_CODE;
}


/* TODO */
/* note: a FILE must be open to be read from; a DIRECTORY doesn't (?)*/
/* unintuitively it seems this must be implemented before open functions */
/* separately implement diskio read funcs (SPI poll)? */
/* we should be abstracted from disk I/O layer; this is not strictly part of the library. just CALL the physical disk read/write routines */

uint8_t WYT_fil_read(
	WYT_File_t* targetFile,
	void* outputBuffer,
	uint8_t outputSize
){
	
	uint32_t startSector, newSector;
	uint16_t totalBytesRead;
	uint16_t currentCycleBytes;
	uint16_t byteInSector;
	
	DRESULT dr;
	
	/* make sure file is open in right mode */
	if (targetFile->fil_flags){
		return ERROR_CODE;
	}
	
	/* need to find starting sector for this file */
	startSector = (targetFile->fil_currentCluster) 
	+ 
	((targetFile->fil_pointer) / (FILE_SYSTEM.fs_bytesPerSector)); // convert byte offset to sector offset and use to increment given cluster base
	
	byteInSector = (targetFile->fil_pointer) / (FILE_SYSTEM.fs_bytesPerSector);

	/* get current sector and current offset from volume info */
	uint32_t remaining_bytes = ((targetFile->fil_activeDirentry)->dirent_size) - targetFile->fil_pointer;
	
	/* if we want to write more than we have room for, truncate to only space available */
	if (outputSize>remaining_bytes) outputSize=remaining_bytes;
	
	// while remaining bytes > 0
	// ...but we want to do this in terms of chunks for efficiency ( so i isn't incremented by 1 but rather a variable amount or chunk size
	for (totalBytesRead=0; totalBytesRead<outputSize; ){

		// bytes per sector - offset = bytes left in current sector
		// desired buffer size minus already written ... i.e. bytes left to write for this buffer
		// min between these so we don't write over any bounds
		currentCycleBytes = MIN((FILE_SYSTEM.fs_bytesPerSector) - byteInSector,
				outputSize - totalBytesRead);


		/* todo: check, either before or after reading, whether the current byte is on a sector boundary*/
		/* after reading and incrementing file pointer (byte) we should be on a sector boundary; i.e. have read the whole sector */


		/* todo: check if top of file */
		/* todo: check if cluster boundary */
		/* base case: NOT a boundary */

		dr = WYT_disk_read(outputBuffer, targetFile->fil_currentSector, targetFile->fil_pointer % 512, currentCycleBytes);
				if (dr) return ERROR_CODE;

		// todo: more stuff relating to physical read?
				// maybe set flags

		/* increment file pointer by amount read this cycle */
		targetFile->fil_pointer += currentCycleBytes;
		totalBytesRead += currentCycleBytes;

		// we want to be reading a complete sector for every pass through this loop... then if at the end we aren't on a sector boundary something is wrong
		if (((targetFile->fil_pointer) % (FILE_SYSTEM.fs_bytesPerSector)) != 0){
			// if we haven't, then the sector must have ended and so we don't need to move to a new one or check boundaries, we can just exit the read loop
			break;
		}

		newSector++;

		// todo:

	}
	
	return ERROR_CODE;

}

/* FUTURE */
/* not implemented */
/* free() and update flags */
uint8_t WYT_fil_close(
	WYT_File_t* targetFile
){
	/* FUTURE: flags... e.g. set as invalid write location? */

	return ERROR_CODE;
}



// offset pointer to currently open file by DWORD
/* references:
https://code.woboq.org/userspace/glibc/io/lseek.c.html https://code.woboq.org/userspace/glibc/sysdeps/unix/sysv/linux/lseek.c.html*/

/* CHECKME : is this really necessary for a 1-directory FS? */
/*
SEEK_SET
The offset is set to offset bytes.
SEEK_CUR
The offset is set to its current location plus offset bytes.
SEEK_END
The offset is set to the size of the file plus offset bytes.
*/
uint8_t WYT_fil_lseek(
	WYT_File_t* inputFile,
	uint32_t offset,
	uint8_t whence
){
	
  uint32_t fileSize = ((inputFile->fil_activeDirentry)->dirent_size);
	
  if (!inputFile)
    {
      return ERROR_CODE;
    }

  switch (whence)
    {
    case SEEK_SET:
		break;	// offset is unchanged from input
    case SEEK_CUR:
		offset = (inputFile->fil_pointer) + offset;
		break;
    case SEEK_END:
		offset =  fileSize + offset;
      break;
	  
    default:
      return ERROR_CODE;
    }
	
  // handle case where a positive offset was added beyond file size
    if(offset>fileSize){
	  offset = fileSize;
  }
	// possibly extra handling here
	
  return ERROR_CODE;
}

/* not implemented */
uint8_t WYT_fil_rm(
	WYT_File_t* targetFile
){
	return ERROR_CODE;
}
// **************************************
// HELPER FUNCTIONS
// **************************************
/* may or may not  want these */

/* not implemented; returns FAT16 for now */
uint8_t WYT_FAT_type(
		WYT_BPB_base_t* BPB)
{
	// FUTURE: implement cluster count separately in case another function needs it
	return FAT16;
	
}


// note: this must be called multiple times to search a whole directory
// like str(n)cmp but for partial matches and with string formatting

// returns # of matching chars
// else: some errorcode
uint8_t WYT_dir_nameCompare(const char* path, const char* dirent)
{
	uint8_t i;
	
	char Filename[FILENAME_MAX];
	
	// 'zero' memory for string buffer to compare
	memset (Filename, ' ', FILENAME_MAX);
	
	// sanitize input string
	for (i=0; i<FILENAME_MAX; i++){
		if ((*path == '.')
			&& (i==(FILENAME_MAX-3))){ // allow 3 char extension
			path++;
		}
		
		// todo: more formatting?

		// UPPERCASE for all chars in name
		Filename[i] = toupper(*path);
		path++;
	}
	
	// after this loop Filename[] is essentially a cleaned up version of *path.

	//FUTURE: multi directory support:
		// split and pass each substring to below loop
		// compare to cwd following every subdir
		// if match, set match segment as new cwd and repeat
	
	// for DR purposes, only root dir
	for (i=0;i<FILENAME_MAX; i++){
		dirent++;
		// char does not match; exit loop
		if (Filename[i] != *dirent){
			break;
		}
	}
	
	return i;
	
}

