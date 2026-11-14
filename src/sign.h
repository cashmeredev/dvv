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
