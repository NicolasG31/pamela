##
## Makefile for pamela in /mnt/c/Users/Nicolas/Desktop/pamela
## 
## Made by DESKTOP-JH20AP8
## Login   <nicolas.guillon@epitech.eu>
## 
## Started on  Sun Nov 26 22:08:15 2017 DESKTOP-JH20AP8
## Last update Sun Nov 26 22:08:16 2017 DESKTOP-JH20AP8
##

CC	=	gcc

CP	=	cp

MKDIR	=	mkdir -p

RM	=	rm -rf

NAME	=	pamela.so

SRCS	=	pam_functions.c	\
		folder_mng.c	\
		encrypt_functions.c 

OBJS	=	$(SRCS:.c=.o)

LDFLAGS	=	-lcryptsetup

CFLAGS	=	-fPIC -fno-stack-protector -I ./

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(OBJS) -shared -rdynamic $(LDFLAGS) -o $(NAME)

install:
	make
	@$(MKDIR) /lib/security
	@$(CP) $(NAME) /lib/security/$(NAME)
	@echo "auth optional pamela.so" >> /etc/pam.d/common-auth
	@echo "session optional pamela.so" >> /etc/pam.d/common-session
	@echo "account optional pamela.so" >> /etc/pam.d/common-account
	@echo "password optional pamela.so" >> /etc/pam.d/common-password
	@printf "Installed Pamela\n"

uninstall:
	make clean
	@$(RM) /lib/security
	@head -n -1 /etc/pam.d/common-auth > /tmp/common-auth && mv /tmp/common-auth /etc/pam.d/common-auth
	@head -n -1 /etc/pam.d/common-session > /tmp/common-session && mv /tmp/common-session /etc/pam.d/common-session
	@head -n -1 /etc/pam.d/common-account > /tmp/common-account && mv /tmp/common-account /etc/pam.d/common-account
	@head -n -1 /etc/pam.d/common-password > /tmp/common-password && mv /tmp/common-password /etc/pam.d/common-password
	@printf "Uninstalled Pamela\n"

check:
ifeq ("$(wildcard /lib/security)", "")
	@printf "Pamela is not installed\n"
else
	@printf "Pamela is installed\n"
endif

test:

clean:
	$(RM) $(OBJS)
	$(RM) $(NAME)

re: clean all
