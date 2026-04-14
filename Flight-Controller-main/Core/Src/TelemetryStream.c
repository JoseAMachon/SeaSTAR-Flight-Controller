
#include "main.h"
#include "TelemetryStream.h"
#include "UartProtocol.h"
#include "PacketIDs.h"


static environmental_packet_t environmental_pkt;

static positional_packet_t orientation_pkt;

void TelemetryStream_Init(void)
{
    environmental_pkt.temp= 22500;      
    environmental_pkt.depth = 1000;     
    environmental_pkt.pressure = 101325; 

    orientation_pkt.yaw = 0;
    orientation_pkt.pitch = 0;
    orientation_pkt.roll = 0;
}

void TelemetryStream_SendEnvironmental(void)
{
 //   Protocol_SendPacket(sizeof(environmental_pkt), ID_ENVIRONMENTAL_TELEMETRY, &environmental_pkt);


     environmental_pkt.temp  += 10;   
    if (environmental_pkt.temp > 23000) {
        environmental_pkt.temp = 22000;
    }

     environmental_pkt.depth += 25;    
    if (environmental_pkt.depth > 3000) {
        environmental_pkt.depth = 1000;
    }

    environmental_pkt.pressure += 100;
    if (environmental_pkt.pressure > 102500) {
        environmental_pkt.pressure = 101000;
    }
}

void TelemetryStream_SendOrientation(void)
{
   // Protocol_SendPacket(sizeof(orientation_pkt), ID_POSITIONAL_TELEMETRY, &orientation_pkt);


    orientation_pkt.yaw += 500;    
    if (orientation_pkt.yaw > 180000) {
        orientation_pkt.yaw = -180000;
    }

    orientation_pkt.pitch += 100;  
    if (orientation_pkt.pitch > 10000) {
        orientation_pkt.pitch = -10000;
    }

    orientation_pkt.roll -= 150;  
    if (orientation_pkt.roll < -10000) {
        orientation_pkt.roll = 10000;
    }
}