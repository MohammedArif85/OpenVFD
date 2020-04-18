#include "MAVG.h"

void MAVG_INIT(MAVG_s *ptr)
{
    uint16_t counter = 0;

    ptr->bufferpos = 0;
    ptr->avg = 0;
    ptr->sum = 0;

    for(counter = 0; counter < MAVG_BUFFERSIZE; counter++)
    {
        ptr->buffer[counter] = 0;
    }
}

int16_t MAVG_GET(MAVG_s *ptr)
{
    return (int16_t)(ptr->avg);
}

int16_t MAVG_UPDATE(MAVG_s *ptr, int32_t value)
{
    ptr->sum = ptr->sum - ptr->buffer[ptr->bufferpos] + value;
    ptr->avg = ptr->sum / MAVG_BUFFERSIZE;
    ptr->buffer[ptr->bufferpos] = value;
    ptr->bufferpos = (ptr->bufferpos + 1) & (MAVG_BUFFERSIZE - 1);

    return (int16_t)(ptr->avg);
}
