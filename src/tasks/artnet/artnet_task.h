#include <SD.h>
#include <FreeRTOS.h>
#include <task.h>
#include <Artnet.h>
#include "../../utils/utils.h"
#include "../../rtos_config.h"

#include "../../peripherals/dmx/dmx.h"

extern ArtnetReceiver artnet;

extern void createArtnetTask();

void artnetCallback(const uint8_t *data, uint16_t size, const ArtDmxMetadata &metadata, const ArtNetRemoteInfo &remote);