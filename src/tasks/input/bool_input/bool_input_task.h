#include "../../../peripherals/ethernet/ethernet.h"
#include "pins.h"
#include <Ethernet3.h>
#include <queue.h>

extern void createBoolInputTask();


void boolInputTask(void *pvParameters);