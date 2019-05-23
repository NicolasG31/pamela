/*
** folder_mng.c for pamela in /mnt/c/Users/Nicolas/Desktop/pamela
** 
** Made by DESKTOP-JH20AP8
** Login   <nicolas.guillon@epitech.eu>
** 
** Started on  Sun Nov 26 22:08:02 2017 DESKTOP-JH20AP8
** Last update Sun Nov 26 22:08:04 2017 DESKTOP-JH20AP8
*/

#include "pamela.h"

//Executes a shell command
int cmd_shell(const char *tmp, ...)
{
  va_list ap;
  char cmd[512];

  va_start(ap, tmp);
  if (vsprintf(cmd, tmp, ap) < 0)
    return (-1);
  if (system(cmd) < 0)
    return (-1);
  va_end(ap);
  return (0);
}

//Get home for dir to username
const char *get_homedir(const char *user)
{
  char *homedir;

  if ((homedir = (getpwnam(user))->pw_dir) == NULL)
    return (NULL);
  return (homedir);
}

//Creates container
int create_container(const char *path, const char *user, const char *pass)
{
  if (cmd_shell("dd if=/dev/urandom bs=1M count=50 of=%s", path) != 0 ||
      encrypt_container(user, pass) != 0 ||
      open_encrypted(user, pass) != 0 ||
      cmd_shell("mkfs.ext4 /dev/mapper/%s_container", user) != 0 ||
      mkdir_and_mount(user) != 0)
    return (-1);
  return (0);
}

int mkdir_and_mount(const char *user)
{
  if (cmd_shell("mkdir %s/secure_data-rw", get_homedir(user)) != 0 ||
      cmd_shell("mount /dev/mapper/%s_container %s/secure_data-rw", user, get_homedir(user)) != 0 ||
      cmd_shell("chown %s %s/secure_data-rw", user, get_homedir(user)) != 0)
    return (-1);
  return (0);
}

//Closes container
int close_container(const char *user)
{
  if (cmd_shell("umount -f %s/secure_data-rw", get_homedir(user)) != 0 ||
      cmd_shell("cryptsetup luksClose %s_container", user) != 0 ||
      cmd_shell("rm -rf %s/secure_data-rw", get_homedir(user)) != 0)
    return (-1);
  return (0);
}

//Opens container
int open_container(const char *user, const char *pass)
{
  char path[512];

  if (sprintf(path, "%s/.encrypted_data", get_homedir(user)) < 0)
    return (-1);
  else if (access(path, F_OK) == -1)
    return (create_container(path, user, pass));
  else if (open_encrypted(user, pass) != 0)
    return (-1);
  if (mkdir_and_mount(user) == -1)
    return (-1);
  return (0);
}
