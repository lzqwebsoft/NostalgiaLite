#include <string>
#include <jni.h>
#include "Emulator.h"
#include "android_log.h"

#ifndef BRIDGE_H_
#define BRIDGE_H_

namespace emudroid {
    class Bridge {
    public:
        Bridge(Emulator *emu);
    };
}

int Android_OpenContentUriFd(const char *fname, const char *mode);

inline FILE *fopen_fd(const char *fname, const char *mode) {
    const char *startWith = "content://";
    FILE *fp;
    if (strncmp(fname, startWith, 10) == 0) {
        LOGD("Open file is start with 'content://', is android uri file.");
        // fname is uri path
        int fd = Android_OpenContentUriFd(fname, mode);
        fp = fdopen(fd, mode);
    } else {
        fp = fopen(fname, mode);
    }
    return fp;
}

#define fopen fopen_fd
#endif
