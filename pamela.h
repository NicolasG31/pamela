/*
** pamela.h for pamela in /mnt/c/Users/Nicolas/Desktop/pamela
** 
** Made by DESKTOP-JH20AP8
** Login   <nicolas.guillon@epitech.eu>
** 
** Started on  Sun Nov 26 22:07:17 2017 DESKTOP-JH20AP8
** Last update Sun Nov 26 22:07:21 2017 DESKTOP-JH20AP8
*/

#ifndef PAMELA_H_
#define PAMELA_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <security/pam_appl.h>
#include <security/pam_modules.h>
#include <security/pam_ext.h>
#include <libcryptsetup.h>
#include <pwd.h>

const char 		*get_homedir(const char *user);
int	        	open_container(const char *user, const char *pass);
int             mkdir_and_mount(const char *user);
int		        close_container(const char *user);
int	        	open_encrypted(const char *user, const char *pass);
int		        change_key(const char *user, const char *oldPw, const char *pw);
int 	        encrypt_container(const char *user, const char *pass);

#endif
