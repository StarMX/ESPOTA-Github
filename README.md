# ESP32&ESP8266 OTA GitHub

## Examples

```
#define REPOSITORY "StarMX/ESP-01"
#include "otagithub.h"



if (espota::github.Update())
{
    ESP_LOGI("ota", espota::github.version.tag);
    //check version your code
    //if (??? != espota::github.version.tag)

    //upgrade example 1
    //espota::github.Upgrade();

    //upgrade example 2
    #ifdef ESP32
    espota::github.Upgrade("firmware-esp32.bin");
    #endif
    #ifdef ESP8266
    espota::github.Upgrade("firmware-esp8266.bin");
    #endif
}
```