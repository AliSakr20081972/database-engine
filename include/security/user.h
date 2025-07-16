#ifndef SECURITY_USER_H
#define SECURITY_USER_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define USER_NAME_MAX 32

typedef struct User {
    char username[USER_NAME_MAX];
    uint64_t password_hash;
} User;

void user_create(User *user, const char *username, const char *password);
bool user_authenticate(const User *user, const char *username, const char *password);

#ifdef __cplusplus
}
#endif

#endif /* SECURITY_USER_H */
