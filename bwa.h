/* This file was automatically generated.  Do not edit (except for compile time directive)! */ 
#ifndef _BWA_H_
#define _BWA_H_
/*
 * BWA (BlockChain Web Applications): An embeddable bytecode compiler and a virtual machine for the PHP(5) programming language.
 * Copyright (C) 2020 BlockChain Web Application
 * Version 2.1.4
 */
#include <stdarg.h> /* needed for the definition of va_list */
#define BWA_VERSION "2.1.4"
#define BWA_VERSION_NUMBER 2001004
#define BWA_SIG "BWA/2.1.4"
#define BWA_COPYRIGHT "Copyright (C) BlockChain Web Application 2020."
/* Make sure we can call this stuff from C++ */
#ifdef __cplusplus
extern "C" { 
#endif
/* Forward declaration to public objects */
typedef struct bwa_io_stream bwa_io_stream;
typedef struct bwa_context bwa_context;
typedef struct bwa_value bwa_value;
typedef struct bwa_vfs bwa_vfs;
typedef struct bwa_vm bwa_vm;
typedef struct bwa bwa;
/* BWA public definitions */
#if !defined(BWA_STANDARD_DEFS)
#define BWA_STANDARD_DEFS
#if defined (_WIN32) || defined (WIN32) || defined(__MINGW32__) || defined (_MSC_VER) || defined (_WIN32_WCE)
/* Windows Systems */
#if !defined(__WINNT__)
#define __WINNT__
#endif 
#else
/*
 * By default we will assume that we are compiling on a UNIX systems.
 * Otherwise the OS_OTHER directive must be defined.
 */
#if !defined(OS_OTHER)
#if !defined(__UNIXES__)
#define __UNIXES__
#endif /* __UNIXES__ */
#else
#endif /* OS_OTHER */
#endif /* __WINNT__/__UNIXES__ */
#if defined(_MSC_VER) || defined(__BORLANDC__)
typedef signed __int64     sxi64; /* 64 bits(8 bytes) signed int64 */
typedef unsigned __int64   sxu64; /* 64 bits(8 bytes) unsigned int64 */
#else
typedef signed long long int   sxi64; /* 64 bits(8 bytes) signed int64 */
typedef unsigned long long int sxu64; /* 64 bits(8 bytes) unsigned int64 */
#endif /* _MSC_VER */
/* Signature of the consumer routine */
typedef int (*ProcConsumer)(const void *,unsigned int,void *);
/* Forward reference */
typedef struct SyMutexMethods SyMutexMethods;
typedef struct SyMemMethods SyMemMethods;
typedef struct SyString SyString;
typedef struct syiovec syiovec;
typedef struct SyMutex SyMutex;
typedef struct Sytm Sytm;
/* Scatter and gather array. */
struct syiovec
{
#if defined (__WINNT__)
	/* Same fields type and offset as WSABUF structure defined one winsock2 header */
	unsigned long nLen;
	char *pBase;
#else
	void *pBase;
	unsigned long nLen;
#endif
};
struct SyString
{
	const char *zString;  /* Raw string (may not be null terminated) */
	unsigned int nByte;   /* Raw string length */
};
/* Time structure. */
struct Sytm
{
  int tm_sec;     /* seconds (0 - 60) */
  int tm_min;     /* minutes (0 - 59) */
  int tm_hour;    /* hours (0 - 23) */
  int tm_mday;    /* day of month (1 - 31) */
  int tm_mon;     /* month of year (0 - 11) */
  int tm_year;    /* year + 1900 */
  int tm_wday;    /* day of week (Sunday = 0) */
  int tm_yday;    /* day of year (0 - 365) */
  int tm_isdst;   /* is summer time in effect? */
  char *tm_zone;  /* abbreviation of timezone name */
  long tm_gmtoff; /* offset from UTC in seconds */
};
/* Convert a tm structure (struct tm *) found in <time.h> to a Sytm structure */
#define STRUCT_TM_TO_SYTM(pTM,pSYTM) \
	(pSYTM)->tm_hour = (pTM)->tm_hour;\
	(pSYTM)->tm_min	 = (pTM)->tm_min;\
	(pSYTM)->tm_sec	 = (pTM)->tm_sec;\
	(pSYTM)->tm_mon	 = (pTM)->tm_mon;\
	(pSYTM)->tm_mday = (pTM)->tm_mday;\
	(pSYTM)->tm_year = (pTM)->tm_year + 1900;\
	(pSYTM)->tm_yday = (pTM)->tm_yday;\
	(pSYTM)->tm_wday = (pTM)->tm_wday;\
	(pSYTM)->tm_isdst = (pTM)->tm_isdst;\
	(pSYTM)->tm_gmtoff = 0;\
	(pSYTM)->tm_zone = 0;

/* Convert a SYSTEMTIME structure (LPSYSTEMTIME: Windows Systems only ) to a Sytm structure */
#define SYSTEMTIME_TO_SYTM(pSYSTIME,pSYTM) \
	 (pSYTM)->tm_hour = (pSYSTIME)->wHour;\
	 (pSYTM)->tm_min  = (pSYSTIME)->wMinute;\
	 (pSYTM)->tm_sec  = (pSYSTIME)->wSecond;\
	 (pSYTM)->tm_mon  = (pSYSTIME)->wMonth - 1;\
	 (pSYTM)->tm_mday = (pSYSTIME)->wDay;\
	 (pSYTM)->tm_year = (pSYSTIME)->wYear;\
	 (pSYTM)->tm_yday = 0;\
	 (pSYTM)->tm_wday = (pSYSTIME)->wDayOfWeek;\
	 (pSYTM)->tm_gmtoff = 0;\
	 (pSYTM)->tm_isdst = -1;\
	 (pSYTM)->tm_zone = 0;

/* Dynamic memory allocation methods. */
struct SyMemMethods 
{
	void * (*xAlloc)(unsigned int);          /* [Required:] Allocate a memory chunk */
	void * (*xRealloc)(void *,unsigned int); /* [Required:] Re-allocate a memory chunk */
	void   (*xFree)(void *);                 /* [Required:] Release a memory chunk */
	unsigned int  (*xChunkSize)(void *);     /* [Optional:] Return chunk size */
	int    (*xInit)(void *);                 /* [Optional:] Initialization callback */
	void   (*xRelease)(void *);              /* [Optional:] Release callback */
	void  *pUserData;                        /* [Optional:] First argument to xInit() and xRelease() */
};
/* Out of memory callback signature. */
typedef int (*ProcMemError)(void *);
/* Mutex methods. */
struct SyMutexMethods 
{
	int (*xGlobalInit)(void);		/* [Optional:] Global mutex initialization */
	void  (*xGlobalRelease)(void);	/* [Optional:] Global Release callback () */
	SyMutex * (*xNew)(int);	        /* [Required:] Request a new mutex */
	void  (*xRelease)(SyMutex *);	/* [Optional:] Release a mutex  */
	void  (*xEnter)(SyMutex *);	    /* [Required:] Enter mutex */
	int (*xTryEnter)(SyMutex *);    /* [Optional:] Try to enter a mutex */
	void  (*xLeave)(SyMutex *);	    /* [Required:] Leave a locked mutex */
};
#if defined (_MSC_VER) || defined (__MINGW32__) ||  defined (__GNUC__) && defined (__declspec)
#define SX_APIIMPORT	__declspec(dllimport)
#define SX_APIEXPORT	__declspec(dllexport)
#else
#define	SX_APIIMPORT
#define	SX_APIEXPORT
#endif
/* Standard return values from BWA public interfaces */
#define SXRET_OK       0      /* Not an error */	
#define SXERR_MEM      (-1)   /* Out of memory */
#define SXERR_IO       (-2)   /* IO error */
#define SXERR_EMPTY    (-3)   /* Empty field */
#define SXERR_LOCKED   (-4)   /* Locked operation */
#define SXERR_ORANGE   (-5)   /* Out of range value */
#define SXERR_NOTFOUND (-6)   /* Item not found */
#define SXERR_LIMIT    (-7)   /* Limit reached */
#define SXERR_MORE     (-8)   /* Need more input */
#define SXERR_INVALID  (-9)   /* Invalid parameter */
#define SXERR_ABORT    (-10)  /* User callback request an operation abort */
#define SXERR_EXISTS   (-11)  /* Item exists */
#define SXERR_SYNTAX   (-12)  /* Syntax error */
#define SXERR_UNKNOWN  (-13)  /* Unknown error */
#define SXERR_BUSY     (-14)  /* Busy operation */
#define SXERR_OVERFLOW (-15)  /* Stack or buffer overflow */
#define SXERR_WILLBLOCK (-16) /* Operation will block */
#define SXERR_NOTIMPLEMENTED  (-17) /* Operation not implemented */
#define SXERR_EOF      (-18) /* End of input */
#define SXERR_PERM     (-19) /* Permission error */
#define SXERR_NOOP     (-20) /* No-op */	
#define SXERR_FORMAT   (-21) /* Invalid format */
#define SXERR_NEXT     (-22) /* Not an error */
#define SXERR_OS       (-23) /* System call return an error */
#define SXERR_CORRUPT  (-24) /* Corrupted pointer */
#define SXERR_CONTINUE (-25) /* Not an error: Operation in progress */
#define SXERR_NOMATCH  (-26) /* No match */
#define SXERR_RESET    (-27) /* Operation reset */
#define SXERR_DONE     (-28) /* Not an error */
#define SXERR_SHORT    (-29) /* Buffer too short */
#define SXERR_PATH     (-30) /* Path error */
#define SXERR_TIMEOUT  (-31) /* Timeout */
#define SXERR_BIG      (-32) /* Too big for processing */
#define SXERR_RETRY    (-33) /* Retry your call */
#define SXERR_IGNORE   (-63) /* Ignore */
#endif /* BWA_PUBLIC_DEFS */
/* Standard BWA return values */
#define BWA_OK      SXRET_OK      /* Successful result */
/* beginning-of-error-codes */
#define BWA_NOMEM   SXERR_MEM     /* Out of memory */
#define BWA_ABORT   SXERR_ABORT   /* Foreign Function request operation abort/Another thread have released this instance */
#define BWA_IO_ERR  SXERR_IO      /* IO error */
#define BWA_CORRUPT SXERR_CORRUPT /* Corrupt pointer/Unknown configuration option */
#define BWA_LOOKED  SXERR_LOCKED  /* Forbidden Operation */ 
#define BWA_COMPILE_ERR (-70)     /* Compilation error */
#define BWA_VM_ERR      (-71)     /* Virtual machine error */
/* end-of-error-codes */
/*
 * If compiling for a processor that lacks floating point
 * support, substitute integer for floating-point.
 */
#ifdef BWA_OMIT_FLOATING_POINT
typedef sxi64 bwa_real;
#else
typedef double bwa_real;
#endif
typedef sxi64 bwa_int64;
#define BWA_APIEXPORT SX_APIEXPORT
/*
 * Engine Configuration Commands.
 *
 * The following set of constants are the available configuration verbs that can
 * be used by the host-application to configure the BWA engine.
 * These constants must be passed as the second argument to the [bwa_config()] 
 * interface.
 * Each options require a variable number of arguments.
 * The [bwa_config()] interface will return BWA_OK on success, any other
 * return value indicates failure.
 */
#define BWA_CONFIG_ERR_OUTPUT    1  /* TWO ARGUMENTS: int (*xConsumer)(const void *pOut,unsigned int nLen,void *pUserData),void *pUserData */
#define BWA_CONFIG_ERR_ABORT     2  /* RESERVED FOR FUTURE USE */
#define BWA_CONFIG_ERR_LOG       3  /* TWO ARGUMENTS: const char **pzBuf,int *pLen */
/*
 * Virtual Machine Configuration Commands.
 *
 * The following set of constants are the available configuration verbs that can
 * be used by the host-application to configure the BWA Virtual machine.
 * These constants must be passed as the second argument to the [bwa_vm_config()] 
 * interface.
 * Each options require a variable number of arguments.
 * The [bwa_vm_config()] interface will return BWA_OK on success, any other return
 * value indicates failure.
 * There are many options but the most importants are: BWA_VM_CONFIG_OUTPUT which install
 * a VM output consumer callback, BWA_VM_CONFIG_HTTP_REQUEST which parse and register
 * a HTTP request and BWA_VM_CONFIG_ARGV_ENTRY which populate the $argv array.
 */
#define BWA_VM_CONFIG_OUTPUT           1  /* TWO ARGUMENTS: int (*xConsumer)(const void *pOut,unsigned int nLen,void *pUserData),void *pUserData */
#define BWA_VM_CONFIG_IMPORT_PATH      3  /* ONE ARGUMENT: const char *zIncludePath */
#define BWA_VM_CONFIG_ERR_REPORT       4  /* NO ARGUMENTS: Report all run-time errors in the VM output */
#define BWA_VM_CONFIG_RECURSION_DEPTH  5  /* ONE ARGUMENT: int nMaxDepth */
#define BWA_VM_OUTPUT_LENGTH           6  /* ONE ARGUMENT: unsigned int *pLength */
#define BWA_VM_CONFIG_CREATE_SUPER     7  /* TWO ARGUMENTS: const char *zName,bwa_value *pValue */
#define BWA_VM_CONFIG_CREATE_VAR       8  /* TWO ARGUMENTS: const char *zName,bwa_value *pValue */
#define BWA_VM_CONFIG_HTTP_REQUEST     9  /* TWO ARGUMENTS: const char *zRawRequest,int nRequestLength */
#define BWA_VM_CONFIG_SERVER_ATTR     10  /* THREE ARGUMENTS: const char *zKey,const char *zValue,int nLen */
#define BWA_VM_CONFIG_ENV_ATTR        11  /* THREE ARGUMENTS: const char *zKey,const char *zValue,int nLen */
#define BWA_VM_CONFIG_SESSION_ATTR    12  /* THREE ARGUMENTS: const char *zKey,const char *zValue,int nLen */
#define BWA_VM_CONFIG_POST_ATTR       13  /* THREE ARGUMENTS: const char *zKey,const char *zValue,int nLen */
#define BWA_VM_CONFIG_GET_ATTR        14  /* THREE ARGUMENTS: const char *zKey,const char *zValue,int nLen */
#define BWA_VM_CONFIG_COOKIE_ATTR     15  /* THREE ARGUMENTS: const char *zKey,const char *zValue,int nLen */
#define BWA_VM_CONFIG_HEADER_ATTR     16  /* THREE ARGUMENTS: const char *zKey,const char *zValue,int nLen */
#define BWA_VM_CONFIG_EXEC_VALUE      17  /* ONE ARGUMENT: bwa_value **ppValue */
#define BWA_VM_CONFIG_IO_STREAM       18  /* ONE ARGUMENT: const bwa_io_stream *pStream */
#define BWA_VM_CONFIG_ARGV_ENTRY      19  /* ONE ARGUMENT: const char *zValue */
#define BWA_VM_CONFIG_EXTRACT_OUTPUT  20  /* TWO ARGUMENTS: const void **ppOut,unsigned int *pOutputLen */
#define BWA_VM_CONFIG_ERR_LOG_HANDLER 21  /* ONE ARGUMENT: void (*xErrLog)(const char *,int,const char *,const char *) */
/*
 * Global Library Configuration Commands.
 *
 * The following set of constants are the available configuration verbs that can
 * be used by the host-application to configure the whole library.
 * These constants must be passed as the first argument to the [bwa_lib_config()] 
 * interface.
 * Each options require a variable number of arguments.
 * The [bwa_lib_config()] interface will return BWA_OK on success, any other return
 * value indicates failure.
 * Notes:
 * The default configuration is recommended for most applications and so the call to
 * [bwa_lib_config()] is usually not necessary. It is provided to support rare 
 * applications with unusual needs. 
 * The [bwa_lib_config()] interface is not threadsafe. The application must insure that
 * no other [bwa_*()] interfaces are invoked by other threads while [bwa_lib_config()]
 * is running. Furthermore, [bwa_lib_config()] may only be invoked prior to library
 * initialization using [bwa_lib_init()] or [bwa_init()] or after shutdown
 * by [bwa_lib_shutdown()]. If [bwa_lib_config()] is called after [bwa_lib_init()]
 * or [bwa_init()] and before [bwa_lib_shutdown()] then it will return BWA_LOCKED.
 * Refer to the official documentation for more information on the configuration verbs
 * and their expected parameters.
 */
#define BWA_LIB_CONFIG_USER_MALLOC            1 /* ONE ARGUMENT: const SyMemMethods *pMemMethods */ 
#define BWA_LIB_CONFIG_MEM_ERR_CALLBACK       2 /* TWO ARGUMENTS: int (*xMemError)(void *),void *pUserData */
#define BWA_LIB_CONFIG_USER_MUTEX             3 /* ONE ARGUMENT: const SyMutexMethods *pMutexMethods */ 
#define BWA_LIB_CONFIG_THREAD_LEVEL_SINGLE    4 /* NO ARGUMENTS */ 
#define BWA_LIB_CONFIG_THREAD_LEVEL_MULTI     5 /* NO ARGUMENTS */ 
#define BWA_LIB_CONFIG_VFS                    6 /* ONE ARGUMENT: const bwa_vfs *pVfs */
/*
 * Compile-time flags.
 * The new compile interfaces [bwa_compile_v2()] and [bwa_compile_file()] takes
 * as their last argument zero or more combination of compile time flags.
 * These flags are used to control the behavior of the BWA compiler while
 * processing the input.
 * Refer to the official documentation for additional information.
 */
#define BWA_PHP_ONLY 0x01 /* If this flag is set then the code to compile is assumed
                           * to be plain PHP only. That is, there is no need to delimit
						   * the PHP code using the standard tags such as <?php ?> or <? ?>.
						   * Everything will pass through the BWA compiler.
						   */
#define BWA_PHP_EXPR 0x02 /* This flag is reserved for future use. */
/*
 * Call Context Error Message Serverity Level.
 *
 * The following constans are the allowed severity level that can
 * passed as the second argument to the [bwa_context_throw_error()] or
 * [bwa_context_throw_error_format()] interfaces.
 * Refer to the official documentation for additional information.
 */
#define BWA_CTX_ERR      1 /* Call context error such as unexpected number of arguments,invalid types and so on. */
#define BWA_CTX_WARNING  2 /* Call context Warning */
#define BWA_CTX_NOTICE   3 /* Call context Notice */
/* Current VFS structure version*/
#define BWA_VFS_VERSION 2 
/* 
 * BWA Virtual File System (VFS).
 *
 * An instance of the bwa_vfs object defines the interface between the BWA core
 * and the underlying operating system. The "vfs" in the name of the object stands
 * for "virtual file system". The vfs is used to implement PHP system functions
 * such as mkdir(), chdir(), stat(), get_user_name() and many more.
 * The value of the iVersion field is initially 2 but may be larger in future versions
 * of BWA.
 * Additional fields may be appended to this object when the iVersion value is increased.
 * Only a single vfs can be registered within the BWA core. Vfs registration is done
 * using the bwa_lib_config() interface with a configuration verb set to BWA_LIB_CONFIG_VFS.
 * Note that Windows and UNIX (Linux, FreeBSD, Solaris, Mac OS X, etc.) users does not have to
 * worry about registering and installing a vfs since BWA come with a built-in vfs for these
 * platforms which implement most the methods defined below.
 * Host-application running on exotic systems (ie: Other than Windows and UNIX systems) must
 * register their own vfs in order to be able to use and call PHP system function.
 * Also note that the bwa_compile_file() interface depend on the xMmap() method of the underlying
 * vfs which mean that this method must be available (Always the case using the built-in VFS)
 * in order to use this interface.
 * Developers wishing to implement the vfs methods can contact symisc systems to obtain
 * the BWA VFS C/C++ Specification manual.
 */
struct bwa_vfs
{
	const char *zName;  /* Underlying VFS name [i.e: FreeBSD/Linux/Windows...] */
	int iVersion;       /* Current VFS structure version [default 2] */
	/* Directory functions */
	int (*xChdir)(const char *);                     /* Change directory */
	int (*xChroot)(const char *);                    /* Change the root directory */
	int (*xGetcwd)(bwa_context *);                   /* Get the current working directory */
	int (*xMkdir)(const char *,int,int);             /* Make directory */
	int (*xRmdir)(const char *);                     /* Remove directory */
	int (*xIsdir)(const char *);                     /* Tells whether the filename is a directory */
	int (*xRename)(const char *,const char *);       /* Renames a file or directory */
	int (*xRealpath)(const char *,bwa_context *);    /* Return canonicalized absolute pathname*/
	/* Systems functions */
	int (*xSleep)(unsigned int);                     /* Delay execution in microseconds */
	int (*xUnlink)(const char *);                    /* Deletes a file */
	int (*xFileExists)(const char *);                /* Checks whether a file or directory exists */
	int (*xChmod)(const char *,int);                 /* Changes file mode */
	int (*xChown)(const char *,const char *);        /* Changes file owner */
	int (*xChgrp)(const char *,const char *);        /* Changes file group */
	bwa_int64 (*xFreeSpace)(const char *);           /* Available space on filesystem or disk partition */
	bwa_int64 (*xTotalSpace)(const char *);          /* Total space on filesystem or disk partition */
	bwa_int64 (*xFileSize)(const char *);            /* Gets file size */
	bwa_int64 (*xFileAtime)(const char *);           /* Gets last access time of file */
	bwa_int64 (*xFileMtime)(const char *);           /* Gets file modification time */
	bwa_int64 (*xFileCtime)(const char *);           /* Gets inode change time of file */
	int (*xStat)(const char *,bwa_value *,bwa_value *);   /* Gives information about a file */
	int (*xlStat)(const char *,bwa_value *,bwa_value *);  /* Gives information about a file */
	int (*xIsfile)(const char *);                    /* Tells whether the filename is a regular file */
	int (*xIslink)(const char *);                    /* Tells whether the filename is a symbolic link */
	int (*xReadable)(const char *);                  /* Tells whether a file exists and is readable */
	int (*xWritable)(const char *);                  /* Tells whether the filename is writable */
	int (*xExecutable)(const char *);                /* Tells whether the filename is executable */
	int (*xFiletype)(const char *,bwa_context *);    /* Gets file type [i.e: fifo,dir,file..] */
	int (*xGetenv)(const char *,bwa_context *);      /* Gets the value of an environment variable */
	int (*xSetenv)(const char *,const char *);       /* Sets the value of an environment variable */
	int (*xTouch)(const char *,bwa_int64,bwa_int64); /* Sets access and modification time of file */
	int (*xMmap)(const char *,void **,bwa_int64 *);  /* Read-only memory map of the whole file */
	void (*xUnmap)(void *,bwa_int64);                /* Unmap a memory view */
	int (*xLink)(const char *,const char *,int);     /* Create hard or symbolic link */
	int (*xUmask)(int);                              /* Change the current umask */
	void (*xTempDir)(bwa_context *);                 /* Get path of the temporary directory */
	unsigned int (*xProcessId)(void);                /* Get running process ID */
	int (*xUid)(void);                               /* user ID of the process */
	int (*xGid)(void);                               /* group ID of the process */
	void (*xUsername)(bwa_context *);                /* Running username */
	int (*xExec)(const char *,bwa_context *);        /* Execute an external program */
};
/* Current BWA IO stream structure version. */
#define BWA_IO_STREAM_VERSION 1 
/* 
 * Possible open mode flags that can be passed to the xOpen() routine
 * of the underlying IO stream device .
 * Refer to the BWA IO Stream C/C++ specification manual
 * for additional information.
 */
#define BWA_IO_OPEN_RDONLY   0x001  /* Read-only open */
#define BWA_IO_OPEN_WRONLY   0x002  /* Write-only open */
#define BWA_IO_OPEN_RDWR     0x004  /* Read-write open. */
#define BWA_IO_OPEN_CREATE   0x008  /* If the file does not exist it will be created */
#define BWA_IO_OPEN_TRUNC    0x010  /* Truncate the file to zero length */
#define BWA_IO_OPEN_APPEND   0x020  /* Append mode.The file offset is positioned at the end of the file */
#define BWA_IO_OPEN_EXCL     0x040  /* Ensure that this call creates the file,the file must not exist before */
#define BWA_IO_OPEN_BINARY   0x080  /* Simple hint: Data is binary */
#define BWA_IO_OPEN_TEMP     0x100  /* Simple hint: Temporary file */
#define BWA_IO_OPEN_TEXT     0x200  /* Simple hint: Data is textual */
/*
 * BWA IO Stream Device.
 *
 * An instance of the bwa_io_stream object defines the interface between the BWA core
 * and the underlying stream device.
 * A stream is a smart mechanism for generalizing file, network, data compression
 * and other IO operations which share a common set of functions using an abstracted
 * unified interface.
 * A stream device is additional code which tells the stream how to handle specific
 * protocols/encodings. For example, the http device knows how to translate a URL
 * into an HTTP/1.1 request for a file on a remote server.
 * BWA come with two built-in IO streams device:
 * The file:// stream which perform very efficient disk IO and the php:// stream
 * which is a special stream that allow access various I/O streams (See the PHP official
 * documentation for more information on this stream).
 * A stream is referenced as: scheme://target 
 * scheme(string) - The name of the wrapper to be used. Examples include: file,http,https,ftp,
 * ftps, compress.zlib, compress.bz2, and php. If no wrapper is specified,the function default
 * is used (typically file://). 
 * target - Depends on the device used. For filesystem related streams this is typically a path
 * and filename of the desired file.For network related streams this is typically a hostname,often
 * with a path appended. 
 * IO stream devices are registered using a call to bwa_vm_config() with a configuration verb
 * set to BWA_VM_CONFIG_IO_STREAM.
 * Currently the BWA development team is working on the implementation of the http:// and ftp://
 * IO stream protocols. These devices will be available in the next major release of the BWA engine.
 * Developers wishing to implement their own IO stream devices must understand and follow
 */
struct bwa_io_stream
{
	const char *zName;                                     /* Underlying stream name [i.e: file/http/zip/php,..] */
	int iVersion;                                          /* IO stream structure version [default 1]*/
	int  (*xOpen)(const char *,int,bwa_value *,void **);   /* Open handle*/
	int  (*xOpenDir)(const char *,bwa_value *,void **);    /* Open directory handle */
	void (*xClose)(void *);                                /* Close file handle */
	void (*xCloseDir)(void *);                             /* Close directory handle */
	bwa_int64 (*xRead)(void *,void *,bwa_int64);           /* Read from the open stream */         
	int (*xReadDir)(void *,bwa_context *);                 /* Read entry from directory handle */
	bwa_int64 (*xWrite)(void *,const void *,bwa_int64);    /* Write to the open stream */
	int (*xSeek)(void *,bwa_int64,int);                    /* Seek on the open stream */
	int (*xLock)(void *,int);                              /* Lock/Unlock the open stream */
	void (*xRewindDir)(void *);                            /* Rewind directory handle */
	bwa_int64 (*xTell)(void *);                            /* Current position of the stream  read/write pointer */
	int (*xTrunc)(void *,bwa_int64);                       /* Truncates the open stream to a given length */
	int (*xSync)(void *);                                  /* Flush open stream data */
	int (*xStat)(void *,bwa_value *,bwa_value *);          /* Stat an open stream handle */
};
/* 
 * C-API-REF: Please refer to the official documentation for interfaces
 * purpose and expected parameters. 
 */ 
/* Engine Handling Interfaces */
BWA_APIEXPORT int bwa_init(bwa **ppEngine);
BWA_APIEXPORT int bwa_config(bwa *pEngine,int nConfigOp,...);
BWA_APIEXPORT int bwa_release(bwa *pEngine);
/* Compile Interfaces */
BWA_APIEXPORT int bwa_compile(bwa *pEngine,const char *zSource,int nLen,bwa_vm **ppOutVm);
BWA_APIEXPORT int bwa_compile_v2(bwa *pEngine,const char *zSource,int nLen,bwa_vm **ppOutVm,int iFlags);
BWA_APIEXPORT int bwa_compile_file(bwa *pEngine,const char *zFilePath,bwa_vm **ppOutVm,int iFlags);
/* Virtual Machine Handling Interfaces */
BWA_APIEXPORT int bwa_vm_config(bwa_vm *pVm,int iConfigOp,...);
BWA_APIEXPORT int bwa_vm_exec(bwa_vm *pVm,int *pExitStatus);
BWA_APIEXPORT int bwa_vm_reset(bwa_vm *pVm);
BWA_APIEXPORT int bwa_vm_release(bwa_vm *pVm);
BWA_APIEXPORT int bwa_vm_dump_v2(bwa_vm *pVm,int (*xConsumer)(const void *,unsigned int,void *),void *pUserData);
/* In-process Extending Interfaces */
BWA_APIEXPORT int bwa_create_function(bwa_vm *pVm,const char *zName,int (*xFunc)(bwa_context *,int,bwa_value **),void *pUserData);
BWA_APIEXPORT int bwa_delete_function(bwa_vm *pVm,const char *zName);
BWA_APIEXPORT int bwa_create_constant(bwa_vm *pVm,const char *zName,void (*xExpand)(bwa_value *,void *),void *pUserData);
BWA_APIEXPORT int bwa_delete_constant(bwa_vm *pVm,const char *zName);
/* Foreign Function Parameter Values */
BWA_APIEXPORT int bwa_value_to_int(bwa_value *pValue);
BWA_APIEXPORT int bwa_value_to_bool(bwa_value *pValue);
BWA_APIEXPORT bwa_int64 bwa_value_to_int64(bwa_value *pValue);
BWA_APIEXPORT double bwa_value_to_double(bwa_value *pValue);
BWA_APIEXPORT const char * bwa_value_to_string(bwa_value *pValue,int *pLen);
BWA_APIEXPORT void * bwa_value_to_resource(bwa_value *pValue);
BWA_APIEXPORT int bwa_value_compare(bwa_value *pLeft,bwa_value *pRight,int bStrict);
/* Setting The Result Of A Foreign Function */
BWA_APIEXPORT int bwa_result_int(bwa_context *pCtx,int iValue);
BWA_APIEXPORT int bwa_result_int64(bwa_context *pCtx,bwa_int64 iValue);
BWA_APIEXPORT int bwa_result_bool(bwa_context *pCtx,int iBool);
BWA_APIEXPORT int bwa_result_double(bwa_context *pCtx,double Value);
BWA_APIEXPORT int bwa_result_null(bwa_context *pCtx);
BWA_APIEXPORT int bwa_result_string(bwa_context *pCtx,const char *zString,int nLen);
BWA_APIEXPORT int bwa_result_string_format(bwa_context *pCtx,const char *zFormat,...);
BWA_APIEXPORT int bwa_result_value(bwa_context *pCtx,bwa_value *pValue);
BWA_APIEXPORT int bwa_result_resource(bwa_context *pCtx,void *pUserData);
/* Call Context Handling Interfaces */
BWA_APIEXPORT int bwa_context_output(bwa_context *pCtx,const char *zString,int nLen);
BWA_APIEXPORT int bwa_context_output_format(bwa_context *pCtx,const char *zFormat,...);
BWA_APIEXPORT int bwa_context_throw_error(bwa_context *pCtx,int iErr,const char *zErr);
BWA_APIEXPORT int bwa_context_throw_error_format(bwa_context *pCtx,int iErr,const char *zFormat,...);
BWA_APIEXPORT unsigned int bwa_context_random_num(bwa_context *pCtx);
BWA_APIEXPORT int bwa_context_random_string(bwa_context *pCtx,char *zBuf,int nBuflen);
BWA_APIEXPORT void * bwa_context_user_data(bwa_context *pCtx);
BWA_APIEXPORT int    bwa_context_push_aux_data(bwa_context *pCtx,void *pUserData);
BWA_APIEXPORT void * bwa_context_peek_aux_data(bwa_context *pCtx);
BWA_APIEXPORT void * bwa_context_pop_aux_data(bwa_context *pCtx);
BWA_APIEXPORT unsigned int bwa_context_result_buf_length(bwa_context *pCtx);
BWA_APIEXPORT const char * bwa_function_name(bwa_context *pCtx);
/* Call Context Memory Management Interfaces */
BWA_APIEXPORT void * bwa_context_alloc_chunk(bwa_context *pCtx,unsigned int nByte,int ZeroChunk,int AutoRelease);
BWA_APIEXPORT void * bwa_context_realloc_chunk(bwa_context *pCtx,void *pChunk,unsigned int nByte);
BWA_APIEXPORT void bwa_context_free_chunk(bwa_context *pCtx,void *pChunk);
/* On Demand Dynamically Typed Value Object allocation interfaces */
BWA_APIEXPORT bwa_value * bwa_new_scalar(bwa_vm *pVm);
BWA_APIEXPORT bwa_value * bwa_new_array(bwa_vm *pVm);
BWA_APIEXPORT int bwa_release_value(bwa_vm *pVm,bwa_value *pValue);
BWA_APIEXPORT bwa_value * bwa_context_new_scalar(bwa_context *pCtx);
BWA_APIEXPORT bwa_value * bwa_context_new_array(bwa_context *pCtx);
BWA_APIEXPORT void bwa_context_release_value(bwa_context *pCtx,bwa_value *pValue);
/* Dynamically Typed Value Object Management Interfaces */
BWA_APIEXPORT int bwa_value_int(bwa_value *pVal,int iValue);
BWA_APIEXPORT int bwa_value_int64(bwa_value *pVal,bwa_int64 iValue);
BWA_APIEXPORT int bwa_value_bool(bwa_value *pVal,int iBool);
BWA_APIEXPORT int bwa_value_null(bwa_value *pVal);
BWA_APIEXPORT int bwa_value_double(bwa_value *pVal,double Value);
BWA_APIEXPORT int bwa_value_string(bwa_value *pVal,const char *zString,int nLen);
BWA_APIEXPORT int bwa_value_string_format(bwa_value *pVal,const char *zFormat,...);
BWA_APIEXPORT int bwa_value_reset_string_cursor(bwa_value *pVal);
BWA_APIEXPORT int bwa_value_resource(bwa_value *pVal,void *pUserData);
BWA_APIEXPORT int bwa_value_release(bwa_value *pVal);
BWA_APIEXPORT bwa_value * bwa_array_fetch(bwa_value *pArray,const char *zKey,int nByte);
BWA_APIEXPORT int bwa_array_walk(bwa_value *pArray,int (*xWalk)(bwa_value *,bwa_value *,void *),void *pUserData);
BWA_APIEXPORT int bwa_array_add_elem(bwa_value *pArray,bwa_value *pKey,bwa_value *pValue);
BWA_APIEXPORT int bwa_array_add_strkey_elem(bwa_value *pArray,const char *zKey,bwa_value *pValue);
BWA_APIEXPORT int bwa_array_add_intkey_elem(bwa_value *pArray,int iKey,bwa_value *pValue);
BWA_APIEXPORT unsigned int bwa_array_count(bwa_value *pArray);
BWA_APIEXPORT int bwa_object_walk(bwa_value *pObject,int (*xWalk)(const char *,bwa_value *,void *),void *pUserData);
BWA_APIEXPORT bwa_value * bwa_object_fetch_attr(bwa_value *pObject,const char *zAttr);
BWA_APIEXPORT const char * bwa_object_get_class_name(bwa_value *pObject,int *pLength);
BWA_APIEXPORT int bwa_value_is_int(bwa_value *pVal);
BWA_APIEXPORT int bwa_value_is_float(bwa_value *pVal);
BWA_APIEXPORT int bwa_value_is_bool(bwa_value *pVal);
BWA_APIEXPORT int bwa_value_is_string(bwa_value *pVal);
BWA_APIEXPORT int bwa_value_is_null(bwa_value *pVal);
BWA_APIEXPORT int bwa_value_is_numeric(bwa_value *pVal);
BWA_APIEXPORT int bwa_value_is_callable(bwa_value *pVal);
BWA_APIEXPORT int bwa_value_is_scalar(bwa_value *pVal);
BWA_APIEXPORT int bwa_value_is_array(bwa_value *pVal);
BWA_APIEXPORT int bwa_value_is_object(bwa_value *pVal);
BWA_APIEXPORT int bwa_value_is_resource(bwa_value *pVal);
BWA_APIEXPORT int bwa_value_is_empty(bwa_value *pVal);
/* Global Library Management Interfaces */
BWA_APIEXPORT int bwa_lib_init(void);
BWA_APIEXPORT int bwa_lib_config(int nConfigOp,...);
BWA_APIEXPORT int bwa_lib_shutdown(void);
BWA_APIEXPORT int bwa_lib_is_threadsafe(void);
BWA_APIEXPORT const char * bwa_lib_version(void);
BWA_APIEXPORT const char * bwa_lib_signature(void);
BWA_APIEXPORT const char * bwa_lib_copyright(void);
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* _BWA_H_ */
