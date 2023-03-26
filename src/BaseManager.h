#ifndef _BASE_MANAGER_H
#define _BASE_MANAGER_H

#include "Configuration.h"

#ifdef OLED
    #include <Adafruit_GFX.h>
#endif

class CBaseManager {

public:

#ifdef OLED
    virtual uint16_t OLED_Status(Adafruit_GFX *oled) { return 0; };
#endif
#ifdef KEYPAD
    virtual void keyEvent(key_status_t key) { };
#endif

    virtual void loop() {};
};

#endif