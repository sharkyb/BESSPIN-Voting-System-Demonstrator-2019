#ifndef __SECURE_LOG_T__
#define __SECURE_LOG_T__

#include "../crypto/crypto_t.h"
#include "../crypto/base64.h"
#include "log_t.h"
#define SECURE_LOG_ENTRY_LENGTH (LOG_ENTRY_LENGTH + SHA256_DIGEST_LENGTH_BYTES)

#define SHA256_BASE_64_DIGEST_LENGTH_BYTES 44
#define SHA256_BASE_64_DIGEST_BUFFER_LENGTH_BYTES (SHA256_BASE_64_DIGEST_LENGTH_BYTES + 2)

typedef uint8_t sha256_base64_digest[SHA256_BASE_64_DIGEST_BUFFER_LENGTH_BYTES];
#define BASE64_SECURE_BLOCK_LOG_ENTRY_LENGTH (LOG_ENTRY_LENGTH + SHA256_BASE_64_DIGEST_LENGTH_BYTES)

typedef struct secure_log_entries
{
    log_entry the_entry;
    sha256_digest the_digest;
} secure_log_entry;

typedef enum
{
    no_integrity,
    hashchain_sha2_256,
    hashchain_sha3_256
} integrity;
typedef enum
{
    no_provenance
} provenance;
typedef enum
{
    no_confidentiality,
    all_plaintext,
    all_ciphertext
} confidentiality;
typedef enum
{
    no_authentication,
    aes_cbc,
    hmac_sha2_256,
    rsa_1025
} authentication;
typedef enum
{
    no_access_control,
    userid
} access_control;
typedef enum
{
    no_non_repudiation
} non_repudiation;
typedef struct secure_log_security_policies
{
    integrity I;
    provenance P;
    confidentiality C;
    authentication A;
    access_control ACL;
    non_repudiation NR;
} secure_log_security_policy;

/*dragan added*/
typedef struct base64_secure_log_entries
{
    log_entry the_entry;
    sha256_base64_digest the_digest;
} base64_secure_log_entry;

#endif
