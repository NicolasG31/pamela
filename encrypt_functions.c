/*
** encrypt_functions.c for pamela in /mnt/c/Users/Nicolas/Desktop/pamela
** 
** Made by DESKTOP-JH20AP8
** Login   <nicolas.guillon@epitech.eu>
** 
** Started on  Sun Nov 26 22:07:47 2017 DESKTOP-JH20AP8
** Last update Sun Nov 26 22:07:49 2017 DESKTOP-JH20AP8
*/

#include "pamela.h"

//Open secure volume user
int open_encrypted(const char *user, const char *pass)
{
  char path[512];
  struct crypt_device *crypt;
  char device_name[512];

  if ((sprintf(path, "%s/.encrypted_data", get_homedir(user)) < 0) ||
      (sprintf(device_name, "%s_container", user) < 0) ||
      (crypt_init(&crypt, path) < 0) ||
      (crypt_load(crypt, CRYPT_LUKS1, NULL) < 0) ||
      (crypt_activate_by_passphrase(crypt, device_name, CRYPT_ANY_SLOT, pass, strlen(pass), 0) < 0))
    return (-1);
  crypt_free(crypt);
  return (0);
}

//Change key to secure container
int change_key(const char *user, const char *oldPw, const char *pw)
{
  char path[512];
  struct crypt_device *crypt;
  uid_t uid;

  uid = getuid()
  if ((setuid(0) != 0) ||
      (sprintf(path, "%s/.encrypted_data", get_homedir(user)) < 0) ||
      (crypt_init(&crypt, path) < 0) ||
      (crypt_load(crypt, CRYPT_LUKS1, NULL) < 0) ||
      (crypt_keyslot_add_by_passphrase(crypt, CRYPT_ANY_SLOT, oldPw, strlen(oldPw), pw, strlen(pw)) < 0))
    return (-1);
  crypt_free(crypt);
  if (setuid(uid) != 0)
    return (-1);
  return (0);
}

//Format secure volume
int encrypt_container(const char *user, const char *pass)
{
  char path[512];
  struct crypt_params_luks1 params;
  struct crypt_device *crypt;

  if ((sprintf(path, "%s/.encrypted_data", get_homedir(user)) < 0) ||
      (crypt_init(&crypt, path) < 0))
    return (-1);
  params.data_alignment = 0;
  params.hash = "sha1";
  params.data_device = NULL;
  if ((crypt_format(crypt, CRYPT_LUKS1, "aes", "xts-plain64", NULL, NULL, 256 / 8, &params) < 0) ||
      (crypt_keyslot_add_by_volume_key(crypt, CRYPT_ANY_SLOT, NULL, 0, pass, strlen(pass)) < 0))
    return (-1);
  return (0);
}
