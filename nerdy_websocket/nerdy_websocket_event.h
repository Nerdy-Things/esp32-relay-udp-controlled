#ifndef NERDY_WEBSOCKET_EVENT_EVENT_H
#define NERDY_WEBSOCKET_EVENT_EVENT_H

#include "esp_event.h"

extern esp_event_base_t NERDY_WEBSOCKET_EVENT; 

enum {
    NERDY_WEBSOCKET_EVENT_ERROR,
    NERDY_WEBSOCKET_EVENT_MESSAGE
};

#endif
