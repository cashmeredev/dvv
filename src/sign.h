#include "monocypher-ed25519.h"
#include <stdint.h>
#include <sys/random.h>

static bool generate_keys(uint8_t secret_key[64], uint8_t public_key[32])
{
  uint8_t seed[32];

  if (getrandom(seed, sizeof(seed), 0) != (ssize_t)sizeof(seed))
    {
      return false;
    }
  crypto_ed25519_key_pair(secret_key, public_key, seed);
  return true;
}

/* signs the url of one project */
/* static void _sign_project(const uint8_t secret_key[64], const char *project_url, */
/*                          uint8_t signature[64]) */
/* { */
/*   crypto_ed25519_sign(signature, secret_key, */
/*                       (const uint8_t *)project_url, strlen(project_url)); */
/* } */

/* /\* checks the signature found in a project *\/ */
/* static bool verify_project(const uint8_t public_key[32], const char *project_url, */
/*                            const uint8_t signature[64]) */
/* { */
/*   return crypto_ed25519_check(signature, public_key, */
/*                               (const uint8_t *)project_url, strlen(project_url)) == 0; */
/* } */
