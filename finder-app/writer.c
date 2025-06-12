#include <stdio.h>
#include <string.h>
#include <syslog.h>

int main(int argc, char* argv[]) {
    openlog("writer", LOG_PID, LOG_USER);

    if (argc != 3) {
        syslog(LOG_ERR, "Usage: ./writer <filename> <message>");
        closelog();
        return 1;
    }

    char* writefile = argv[1];
    char* writestr = argv[2];

    int writestr_len = strlen(writestr);

    FILE* file = fopen(writefile, "w+");

    if (!file) {
        syslog(LOG_ERR, "Error opening file %s for writing", writefile);
        closelog();
        return 1;
    }

    size_t ret = fwrite(writestr, sizeof(char), writestr_len, file);

    if (ret < writestr_len) {
        syslog(LOG_ERR, "Error writing to file");
        closelog();
        fclose(file);
        return 1;
    }

    syslog(LOG_DEBUG, "Writing %s to %s", writestr, writefile);

    fclose(file);
    closelog();

    return 0;
}