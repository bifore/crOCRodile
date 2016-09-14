#include <ntdef.h>
#include "router.h"
#include "../utils/error.h"

bool tryInit(unsigned int port) {
    return false;
}

bool initServerOnPort(unsigned int port) {
    if (port < 8000) {
        fail("Invalid port number. The port number must be greater than 8000. Aborting.");
        return false;
    }
    return tryInit(port);
}

