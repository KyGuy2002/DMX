#include "setupArtnet.h"

#include <Arduino.h>
#include <Artnet.h>

void setupArtnet(ArtnetReceiver &artnet) {

    artnet.setArtPollReplyConfigLongName("Project DMX Helper");
    artnet.setArtPollReplyConfigShortName("Project DMX Helper");

    artnet.begin();

}