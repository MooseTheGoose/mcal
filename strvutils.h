#ifndef STRVUTILS_H
#define STRVUTILS_H

#include <glib.h>

char **strndupv(char **argv, gsize n);
char **strvstr(char **argv, char *str);
char **strvrstr(char **argv, char *str);

#endif
