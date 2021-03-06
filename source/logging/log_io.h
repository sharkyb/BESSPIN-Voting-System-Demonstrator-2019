#ifndef __LOG_IO_H__
#define __LOG_IO_H__

#include "secure_log_t.h"
#include "log_fs.h"
#include "log_net.h"
#include <string.h>

typedef Log_Handle *log_io_stream;
typedef Log_Handle *log_file;

//
// ACSL Predicates supporting contracts for Log_IO
//

/*@
  axiomatic log_io_axioms {
  logic
    size_t File_Num_Entries{L}(Log_Handle *f) reads *f, log_fs; // abstract
  }
*/

/*@
  predicate
    File_Exists{L}(char *name) = Log_FS_Exists (name);

  predicate
    File_Is_Open{L}(Log_Handle *f) = Log_FS_File_Is_Open (f);

  predicate
    valid_secure_log_entry(secure_log_entry sle)=
      \valid_read((uint8_t*)sle.the_entry[0 .. LOG_ENTRY_LENGTH - 1]) &&
      \valid_read((uint8_t*)sle.the_digest[0 .. SHA256_DIGEST_LENGTH_BYTES - 1]);

  predicate
    Log_IO_Initialized{L} = Log_FS_Initialized{L} &&
                            Log_Net_Initialized{L};
  global invariant log_file_is_not_empty:
   \forall log_file f; File_Num_Entries(f) > 0 ;

  global invariant log_io_streams_is_not_empty:
   \forall log_io_stream lis; File_Num_Entries(lis) > 0 ;
*/

/* Mounts the FileSystem and any other initialization necessary.  */
/* This must be called EXACTLY ONCE by a main program before any  */
/* other Log_IO operations can be performed.                      */
/*@ assigns log_fs \from \nothing;
    ensures Log_IO_Initialized;
 */
Log_FS_Result Log_IO_Initialize(void);

/*@ requires Log_IO_Initialized;
    requires valid_string(name);
    assigns \result \from *name, log_fs;
    ensures \result <==> File_Exists (name);
    ensures \result == true || \result == false;
 */
bool Log_IO_File_Exists(const char *name);

/* Create new and empty log file. Any existing file with same name is destroyed. */
/*@ requires Log_IO_Initialized;
    requires valid_string(name);
    requires \valid(stream);
    requires \separated(stream, name);
    assigns log_fs \from log_fs, name;
    assigns *stream \from log_fs, name, endpoint;

    behavior success:
      ensures \result == LOG_FS_OK;
      ensures \valid (stream);
      ensures File_Is_Open (stream);

    behavior failure:
      ensures \result == LOG_FS_ERROR;
      ensures !File_Is_Open (stream);

    complete behaviors;
    disjoint behaviors;
 */
Log_FS_Result Log_IO_Create_New(Log_Handle *stream, // OUT
                                const char *name,   // IN
                                const http_endpoint endpoint);  // IN

/*@ requires Log_IO_Initialized;
    requires \valid(stream);
    requires valid_string(name);
    requires \separated(stream, name);
    assigns *stream \from log_fs, name, endpoint;
    assigns \result \from log_fs, name, endpoint;
    behavior success:
      ensures \result == LOG_FS_OK;
      ensures \valid (stream);
      ensures File_Is_Open (stream);
      // ensures stream is open for read AND write AND append modes

    behavior failure:
      ensures \result == LOG_FS_ERROR;
      ensures !File_Is_Open (stream);

    complete behaviors;
    disjoint behaviors;
 */
Log_FS_Result Log_IO_Open(Log_Handle *stream, // OUT
                          const char *name,   // IN
                          const http_endpoint endpoint);  // IN


/*@ requires Log_IO_Initialized;
    requires \valid(stream);
    assigns log_fs \from log_fs, stream;
    ensures !File_Is_Open (stream);
 */
Log_FS_Result Log_IO_Close(Log_Handle *stream); // IN


/* Forces any internal buffers out to disk. Call this after Write */
/*@ requires Log_IO_Initialized;
    requires \valid(stream);
    requires File_Is_Open (stream);
    assigns log_fs \from log_fs, stream;
    ensures File_Is_Open (stream);
 */
Log_FS_Result Log_IO_Sync(Log_Handle *stream); // IN


/*@ requires Log_IO_Initialized;
    requires \valid(stream);
    requires valid_string(stream->remote_file_name);
    requires File_Is_Open (stream);
    assigns log_fs \from log_fs, stream, the_entry;
 */
Log_FS_Result Log_IO_Write_Base64_Entry(Log_Handle *stream,          // IN
                                 secure_log_entry the_entry); // IN

/*@ requires Log_IO_Initialized;
    requires \valid(stream);
    requires File_Is_Open (stream);
    assigns \result \from *stream, log_fs;
    ensures \result == File_Num_Entries (stream);
 */
size_t Log_IO_Num_Base64_Entries(Log_Handle *stream);

/*@ requires Log_IO_Initialized;
    requires \valid(stream);
    requires File_Is_Open (stream);
    requires n >= 0;
    requires n <  File_Num_Entries (stream);
    assigns \result \from *stream, n, log_fs;
 */
secure_log_entry Log_IO_Read_Base64_Entry(Log_Handle *stream, // IN
                                   size_t n);          // IN
/*@ requires Log_IO_Initialized;
    requires \valid(stream);
    requires File_Is_Open (stream);
    assigns \result \from *stream, log_fs;
 */
secure_log_entry Log_IO_Read_Last_Base64_Entry(Log_Handle *stream);


#endif /* __LOG_IO_H__ */
