#include "security/user.h"
#include <string.h>

/* Simple FNV-1a 64-bit hash for demonstration purposes. */
static uint64_t fnv1a_hash(const char *str)
{
    const uint64_t fnv_offset = 14695981039346656037ULL;
    const uint64_t fnv_prime = 1099511628211ULL;
    uint64_t hash = fnv_offset;
    for (const unsigned char *p = (const unsigned char*)str; *p; p++) {
        hash ^= (uint64_t)(*p);
        hash *= fnv_prime;
    }
    return hash;
}

void user_create(User *user, const char *username, const char *password)
{
    if (!user || !username || !password)
        return;
    strncpy(user->username, username, USER_NAME_MAX - 1);
    user->username[USER_NAME_MAX - 1] = '\0';
    user->password_hash = fnv1a_hash(password);
}

bool user_authenticate(const User *user, const char *username, const char *password)
{
    if (!user || !username || !password)
        return false;
    if (strncmp(user->username, username, USER_NAME_MAX) != 0)
        return false;
    return user->password_hash == fnv1a_hash(password);
}
