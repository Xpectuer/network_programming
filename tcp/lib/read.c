#include "common.h"
// read n bytes from socketfd
size_t readn(int fd, void* buffer, size_t size) {
    char * buffer_ptr = buffer;
    int length = size;
    // read n bytes from socketfd

    while(length > 0) {
        int result = read(fd, buffer_ptr, length);
        // error
        if(length < 0) {
            // non-blocking error
            if(errno == EINTR) continue;
            else return EXIT_FAILURE;
        }
        if(result == 0) {
            // EOF
            break;
        }

        length -= result;
        buffer_ptr += result;
    }
    // actual num of bytes read
    return (size - length);
}

