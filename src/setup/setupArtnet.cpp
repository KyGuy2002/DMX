#include "setupArtnet.h"

#include <Arduino.h>
#include <Artnet.h>

void setupArtnet(ArtnetReceiver &artnet) {

    artnet.setArtPollReplyConfigLongName("ProjectDMX LumaKit - Lighting Swiss Army Knife");
    artnet.setArtPollReplyConfigShortName("ProjectDMX LumaKit");

    artnet.begin();

}