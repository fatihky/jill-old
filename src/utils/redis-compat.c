/**
 * Directly copied from redis and cutted.
 * License of this file can be found in redis' source code repository.
 * Here is the link of the license of this file: https://github.com/antirez/redis/blob/unstable/COPYING
 */
#include <stdio.h>

void _serverAssert(char *estr, char *file, int line) {
    printf ("=== ASSERTION FAILED ===");
    printf ("==> %s:%d '%s' is not true",file,line,estr);
#ifdef HAVE_BACKTRACE
    server.assert_failed = estr;
    server.assert_file = file;
    server.assert_line = line;
    printf ("(forcing SIGSEGV to print the bug report.)");
#endif
    *((char*)-1) = 'x';
}
