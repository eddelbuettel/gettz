// -*- mode: C; c-indent-level: 4; c-basic-offset: 4; indent-tabs-mode: nil; 
//
//  gettz -- Retrieve Timezone Information from System
//
//  Copyright (C) 2016  Dirk Eddelbuettel <edd@debian.org>
//
//  This file is part of gettz
//
//  gettz is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 2 of the License, or
//  (at your option) any later version.
//
//  gettz is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with gettz.  If not, see <http://www.gnu.org/licenses/>.

//  Using two functions written by Duane McCully and posted on StackOverflow 
//  on 2015-11-15 -- see http://stackoverflow.com/a/33881726/143305 
//
//  SO content is licensed CC-BY-SA (prior to Feb 2016, and MIT since) 
//
//  Picked up by Dirk Eddelbuettel and marginally rearranged: 
//   - commented out main()
//   - added R accessor
//   - reindented and reformatted
//   - several small changes to satisfy `g++ -Wall -pedantic`
//
//  The additional work is licensed under GPL (>= 2) just like R itself.
//
//  Note that R also a function `Sys.timezone()` but we have encountered cases
//  where `Sys.timezone()` did//not* retrieve an answer whereas this function did.
//  Apparently, R's function follows the (soft)link from '/etc/localtime' whereas
//  this function reads '/etc/timezone' (on Debian/Ubuntu) or '/etc/sysconfig/clock'
//  (on RedHat/CentOS/Fedora) or '/etc/TIMEZONE' (on Solaris).  
//
//  OS X returns NULL. It is unclear if there is an API
//
//  Windows does not apply, but could be integrated by, say, looking at the registry
//
//  Copyright Dirk Eddelbuettel, September 2016, and licensed GPL-2 or later

#include <string.h>
#include <strings.h>
#include <stdio.h>

#include <R.h>
#include <Rinternals.h>

char *findDefaultTZ(char *tz, size_t tzSize);
char *getValue(const char *filename, const char *tag, char *value, size_t valueSize);

extern "C" SEXP gettz_impl(void) {
    char v[128];                // we can do this here as Rf_mkChar() copies again 
    char *res = findDefaultTZ(v, 128);

    if (res == NULL) {
        return R_NilValue;
    } else {
        SEXP val = PROTECT(Rf_allocVector(STRSXP, 1));
        SET_STRING_ELT(val, 0, Rf_mkChar(res));
        UNPROTECT(1);
        return val;
    }
}

// int main(int argc, char **argv) {
//     char tz[128];

//     if (findDefaultTZ(tz, sizeof(tz)))
//         printf("Default timezone is %s.\n", tz);
//     else
//         printf("Unable to determine default timezone.\n");
//     return 0;
// }

char *findDefaultTZ(char *tz, size_t tzSize) {
    char *ret = NULL;
    // If there is an /etc/timezone file, then we expect it to contain
    // nothing except the timezone. 
    FILE *fd = fopen("/etc/timezone", "r"); 	// Debian and Ubuntu 
    if (fd) {
        char buffer[128];
        // There should only be one line, in this case. 
        while (fgets(buffer, sizeof(buffer), fd)) {
            char *lasts = buffer;
            // We don't want a line feed on the end. 
            char *tag = strtok_r(lasts, " \t\n", &lasts);
            // Idiot check. 
            if (tag && strlen(tag) > 0 && tag[0] != '#') {
                strncpy(tz, tag, tzSize);
                ret = tz;
            }
        }
        fclose(fd);
    }
    else if (getValue("/etc/sysconfig/clock", "ZONE", tz, tzSize)) 	// RH/FC/CentOS
        ret = tz;
    else if (getValue("/etc/TIMEZONE", "TZ", tz, tzSize))     		// Solaris
        ret = tz;
    return ret;
}

// Look for tag=someValue within filename.  When found, return someValue
// in the provided value parameter up to valueSize in length.  If someValue
// is enclosed in quotes, remove them. 
char *getValue(const char *filename, const char *tag, char *value, size_t valueSize) {
    char buffer[128], *lasts;
    int foundTag = 0;

    FILE *fd = fopen(filename, "r");
    if (fd) {
        // Process the file, line by line.
        while (fgets(buffer, sizeof(buffer), fd)) {
            lasts = buffer;
            // Look for lines with tag=value. 
            char *token = strtok_r(lasts, "=", &lasts);
            // Is this the tag we are looking for? 
            if (token && !strcmp(token, tag)) {
                // Parse out the value. 
                char *zone = strtok_r(lasts, " \t\n", &lasts);
                // If everything looks good, copy it to our return var. 
                if (zone && strlen(zone) > 0) {
                    size_t i = 0;
                    int j = 0;
                    char quote = 0x00;
                    // Rather than just simple copy, remove quotes while we copy. 
                    for (i = 0; i < strlen(zone) && i < valueSize - 1; i++) {
                        // Start quote. 
                        if (quote == 0x00 && zone[i] == '"')
                            quote = zone[i];
                        // End quote. 
                        else if (quote != 0x00 && quote == zone[i])
                            quote = 0x00;
                        // Copy bytes. 
                        else {
                            value[j] = zone[i];
                            j++;
                        }
                    }
                    value[j] = 0x00;
                    foundTag = 1;
                }
                break;
            }
        }
        fclose(fd);
    }
    if (foundTag)
        return value;
    return NULL;
}
