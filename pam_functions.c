/*
** pam_functions.c for pamela in /mnt/c/Users/Nicolas/Desktop/pamela
** 
** Made by DESKTOP-JH20AP8
** Login   <nicolas.guillon@epitech.eu>
** 
** Started on  Sun Nov 26 22:08:31 2017 DESKTOP-JH20AP8
** Last update Sun Nov 26 22:09:11 2017 DESKTOP-JH20AP8
*/

#include "pamela.h"

//Account
PAM_EXTERN int pam_sm_acct_mgmt(pam_handle_t *pamh,
                                int flags,
                                int argc,
                                const char **argv)
{
  return PAM_SUCCESS;
}

//Change password
PAM_EXTERN int pam_sm_setcred(pam_handle_t *pamh,
                              int flags,
                              int argc,
                              const char **argv)
{
  return PAM_SUCCESS;
}

//Function called by pam_set_data
void my_cleanup(pam_handle_t *pamh,
                void *data,
                int error_status)
{
  if (data)
  {
    memset(data, 0, strlen(data));
    free(data);
  }
}

//Pam function to login
PAM_EXTERN int pam_sm_authenticate(pam_handle_t *pamh,
                                   int flags,
                                   int argc,
                                   const char **argv)
{
  int ret;
  const char *login;
  const char *pw;

  if (((ret = pam_get_user(pamh, &login, "login: ")) != PAM_SUCCESS) ||
      ((ret = pam_get_item(pamh, PAM_AUTHTOK, (const void **)&pw)) != PAM_SUCCESS) ||
      ((ret = pam_set_data(pamh, "pam_user_pass", strdup(pw), &my_cleanup)) != PAM_SUCCESS))
    return (ret);

  printf("Pamela : Logged in\n");
  return PAM_SUCCESS;
}

//Pam function to open session
PAM_EXTERN int pam_sm_open_session(pam_handle_t *pamh,
                                   int flags,
                                   int argc,
                                   const char **argv)
{
  int ret;
  const char *login;
  const char *pw;

  printf("Pamela : Creating a container, please wait.\n");
  if (((ret = pam_get_user(pamh, &login, "login: ")) != PAM_SUCCESS) ||
      ((ret = pam_get_data(pamh, "pam_user_pass", (const void **)&pw)) != PAM_SUCCESS))
    return (ret);
  if (open_container(login, pw) != 0)
    return PAM_SESSION_ERR;
  printf("Pamela : Created a container in your home folder\n");
  return PAM_SUCCESS;
}

//Pam function to close session
PAM_EXTERN int pam_sm_close_session(pam_handle_t *pamh,
                                    int flags,
                                    int argc,
                                    const char **argv)
{
  int ret;
  const char *login;

  if ((ret = pam_get_user(pamh, &login, "login: ")) != PAM_SUCCESS)
    return (ret);
  else if (close_container(login) != 0)
    return PAM_SESSION_ERR;
  printf("Pamela : Your container is encrypted.\n");
  return PAM_SUCCESS;
}

//Token change
PAM_EXTERN int pam_sm_chauthtok(pam_handle_t *pamh,
                                int flags,
                                int argc,
                                const char **argv)
{
  int ret;
  const char *login;
  const char *pw;
  const char *oldPw;

  if (((ret = pam_get_user(pamh, &login, "login: ")) != PAM_SUCCESS) ||
      ((ret = pam_get_item(pamh, PAM_AUTHTOK, (const void **)&pw)) != PAM_SUCCESS))
    return (ret);
  if (pw)
  {
    printf("Pamela : Changing container key\n");
    if ((ret = pam_get_item(pamh, PAM_OLDAUTHTOK, (const void **)&oldPw)) != PAM_SUCCESS)
      return (ret);
    else if (change_key(login, oldPw, pw) != 0)
      return PAM_SYSTEM_ERR;
    printf("Pamela : Changed container key\n");
  }
  return PAM_SUCCESS;
}
