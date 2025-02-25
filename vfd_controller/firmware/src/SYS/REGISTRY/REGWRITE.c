#include "REGISTRY.h"

uint8_t REGISTRY_REGWRITE(uint16_t registerAddress, uint16_t registerValue)
{
    switch(registerAddress)
    {
        case 40100:
        {
            if((SETPOINTS.ENABLE_PROGMODE == 0) && (registerValue == 1))
            {
                SETPOINTS.ENABLE = 1;
            }

            else
            {
                SETPOINTS.ENABLE = -1;
            }

            return 0;
        }
        case 40101:
        {
            if((int16_t)registerValue > 100)
            {
                SETPOINTS.TARGET_TORQUE = 100;
            }

            else if((int16_t)registerValue < -100)
            {
                SETPOINTS.TARGET_TORQUE = -100;
            }

            else
            {
                SETPOINTS.TARGET_TORQUE = (int16_t)registerValue;
            }

            return 0;
        }
        case 40102:
        {
            SETPOINTS.TARGET_FREQUENCY = (uint16_t)registerValue;

            if(SETPOINTS.ENABLE_FREQUENCY_OVERDRIVE == 1)
            {
                if(SETPOINTS.TARGET_FREQUENCY > PARAMETERS.CONTROLLER_OVERDRIVE_FREQUENCY)
                {
                    SETPOINTS.TARGET_FREQUENCY = PARAMETERS.CONTROLLER_OVERDRIVE_FREQUENCY;
                }
            }

            else
            {
                if(SETPOINTS.TARGET_FREQUENCY > PARAMETERS.CONTROLLER_NOMINAL_FREQUENCY)
                {
                    SETPOINTS.TARGET_FREQUENCY = PARAMETERS.CONTROLLER_NOMINAL_FREQUENCY;
                }
            }

            return 0;
        }
        case 40103:
        {
            if(registerValue == 1)
            {
                SETPOINTS.REVERSAL = 1;
            }

            else
            {
                SETPOINTS.REVERSAL = 0;
            }

            return 0;
        }
        case 40104:
        {
            if(registerValue == 1)
            {
                SETPOINTS.ENABLE_CURRENT_OVERDRIVE = 1;
            }

            else
            {
                SETPOINTS.ENABLE_CURRENT_OVERDRIVE = 0;
            }

            return 0;
        }
        case 40105:
        {
            if(registerValue == 1)
            {
                SETPOINTS.ENABLE_FREQUENCY_OVERDRIVE = 1;
            }

            else
            {
                SETPOINTS.ENABLE_FREQUENCY_OVERDRIVE = 0;
            }

            return 0;
        }
        case 40110:
        {
            if((SETPOINTS.ENABLE_PROGMODE == 0) && (registerValue == 1))
            {
                SETPOINTS.ENABLE = -1;
                SETPOINTS.ENABLE_PROGMODE = 1;
            }

            else if((SETPOINTS.ENABLE_PROGMODE == 1) && (registerValue != 1) && (SETPOINTS.TRIGGER_PARAMETER_SAVE == 1))
            {
                REGISTRY_PUSHEEMEM();
            }

            else
            {
                SETPOINTS.ENABLE_PROGMODE = 0;
            }

            return 0;
        }
        case 40111:
        {
            if(SETPOINTS.ENABLE_PROGMODE == 1)
            {
                if(registerValue == 1)
                {
                    SETPOINTS.TRIGGER_PARAMETER_SAVE = 1;
                }

                else
                {
                    SETPOINTS.TRIGGER_PARAMETER_SAVE = 0;
                }
            }

            return 0;
        }
        case 40120:
        {
            if(SETPOINTS.ENABLE_PROGMODE == 1)
            {
                if(registerValue == 1)
                {
                    uint16_t counter = 0;
                    uint64_t temp = 0;
                    uint64_t scaled_motorvoltage = 0;
                    uint64_t scaled_dcbusvoltage = 0;
                    uint64_t uboost_startvoltage = 0;
                    uint64_t uboost_finalvoltage = 0;

                    /* calculate u/f lookup-table base values */
                    scaled_dcbusvoltage = ((PARAMETERS.CONTROLLER_DCBUS_NOMINAL_VOLTAGE * (1.10 * 1774)) / 2048);
                    scaled_motorvoltage = ((PARAMETERS.MOTOR_NOMINAL_VOLTAGE * (1.00 * 1448)) / 1024);
                    scaled_motorvoltage = ((scaled_motorvoltage * 255) / scaled_dcbusvoltage);

                    /* calculate u/f lookup-table general curve */
                    for(counter = 0; counter < 2048; counter++)
                    {
                        temp = ((scaled_motorvoltage * counter) / (PARAMETERS.MOTOR_NOMINAL_FREQUENCY / 32));

                        if(temp > 0xFF)
                        {
                            temp = 0xFF;
                        }

                        PARAMETERS.CONTROLLER_UF_VALUE[counter] = (uint8_t)temp;
                    }

                    /* calculate u/f lookup-table voltage boost enhancement */
                    uboost_startvoltage = ((PARAMETERS.MOTOR_MINIMAL_VOLTAGE * (1.00 * 1774)) / 1024);
                    uboost_startvoltage = ((uboost_startvoltage * 255) / scaled_dcbusvoltage);
                    uboost_finalvoltage = PARAMETERS.CONTROLLER_UF_VALUE[(PARAMETERS.MOTOR_VOLTBOOST_FREQUENCY / 32)];
                    scaled_motorvoltage = uboost_finalvoltage - uboost_startvoltage;

                    for(counter = 0; counter <= (PARAMETERS.MOTOR_VOLTBOOST_FREQUENCY / 32); counter++)
                    {
                        temp = uboost_startvoltage + ((scaled_motorvoltage * counter) / (PARAMETERS.MOTOR_VOLTBOOST_FREQUENCY / 32));

                        if(temp > 0xFF)
                        {
                            temp = 0xFF;
                        }

                        PARAMETERS.CONTROLLER_UF_VALUE[counter] = (uint8_t)temp;
                    }
                }
            }

            return 0;
        }
        case 40150:
        {
            if((SETPOINTS.ENABLE_PROGMODE == 1) && (registerValue == 1))
            {
                SETPOINTS.TRIGGER_RESET = 1;
                while(1);
            }

            else
            {
                SETPOINTS.TRIGGER_RESET = 0;
            }

            return 0;
        }
        case 40300:
        {
            if((SETPOINTS.ENABLE_PROGMODE == 1) && (registerValue > 0) && (registerValue < 248))
            {
                PARAMETERS.MODBUS_ADDRESS = (uint16_t)registerValue;
            }

            return 0;
        }
        case 40301:
        {
            if(SETPOINTS.ENABLE_PROGMODE == 1)
            {
                PARAMETERS.MODBUS_BAUDRATE = (PARAMETERS.MODBUS_BAUDRATE & 0x0000FFFF) | (((uint32_t)registerValue) << 16);
            }

            return 0;
        }
        case 40302:
        {
            if(SETPOINTS.ENABLE_PROGMODE == 1)
            {
                PARAMETERS.MODBUS_BAUDRATE = (PARAMETERS.MODBUS_BAUDRATE & 0xFFFF0000) | (((uint32_t)registerValue) <<  0);
            }

            return 0;
        }
        case 40303:
        {
            if((SETPOINTS.ENABLE_PROGMODE == 1) && (registerValue < 3))
            {
                PARAMETERS.MODBUS_PARITY = (uint16_t)registerValue;
            }

            return 0;
        }
        case 40304:
        {
            if((SETPOINTS.ENABLE_PROGMODE == 1) && ((int16_t)registerValue > -2) && ((int16_t)registerValue < 2))
            {
                PARAMETERS.MODBUS_OFFSET = (int16_t)registerValue;
            }

            return 0;
        }
        case 40400:
        {
            if(SETPOINTS.ENABLE_PROGMODE == 1)
            {
                PARAMETERS.MOTOR_NOMINAL_VOLTAGE = (uint16_t)registerValue;
            }

            return 0;
        }
        case 40401:
        {
            if(SETPOINTS.ENABLE_PROGMODE == 1)
            {
                PARAMETERS.MOTOR_MINIMAL_VOLTAGE = (uint16_t)registerValue;
            }

            return 0;
        }
        case 40402:
        {
            if(SETPOINTS.ENABLE_PROGMODE == 1)
            {
                PARAMETERS.MOTOR_NOMINAL_FREQUENCY = (uint16_t)registerValue;
            }

            return 0;
        }
        case 40403:
        {
            if(SETPOINTS.ENABLE_PROGMODE == 1)
            {
                PARAMETERS.MOTOR_VOLTBOOST_FREQUENCY = (uint16_t)registerValue;
            }

            return 0;
        }
        case 40404:
        {
            if(SETPOINTS.ENABLE_PROGMODE == 1)
            {
                PARAMETERS.MOTOR_NOMINAL_FWD_CURRENT = (uint16_t)registerValue;;
            }

            return 0;
        }
        case 40405:
        {
            if(SETPOINTS.ENABLE_PROGMODE == 1)
            {
                PARAMETERS.MOTOR_NOMINAL_REV_CURRENT = (uint16_t)registerValue;
            }

            return 0;
        }
        case 40406:
        {
            if(SETPOINTS.ENABLE_PROGMODE == 1)
            {
                PARAMETERS.MOTOR_OVERDRIVE_FWD_CURRENT = (uint16_t)registerValue;
            }

            return 0;
        }
        case 40407:
        {
            if(SETPOINTS.ENABLE_PROGMODE == 1)
            {
                PARAMETERS.MOTOR_OVERDRIVE_REV_CURRENT = (uint16_t)registerValue;
            }

            return 0;
        }
        case 40500:
        {
            if(SETPOINTS.ENABLE_PROGMODE == 1)
            {
                PARAMETERS.CONTROLLER_NOMINAL_FREQUENCY = (uint16_t)registerValue;
            }

            return 0;
        }
        case 40501:
        {
            if(SETPOINTS.ENABLE_PROGMODE == 1)
            {
                PARAMETERS.CONTROLLER_OVERDRIVE_FREQUENCY = (uint16_t)registerValue;
            }

            return 0;
        }
        case 40502:
        {
            if(SETPOINTS.ENABLE_PROGMODE == 1)
            {
                PARAMETERS.CONTROLLER_PROPORTIONAL_FACTOR = (int16_t)registerValue;
            }

            return 0;
        }
        case 40503:
        {
            if(SETPOINTS.ENABLE_PROGMODE == 1)
            {
                PARAMETERS.CONTROLLER_INTEGRAL_FACTOR = (int16_t)registerValue;
            }

            return 0;
        }
        case 40504:
        {
            if(SETPOINTS.ENABLE_PROGMODE == 1)
            {
                PARAMETERS.CONTROLLER_SAMPLETIME = (PARAMETERS.CONTROLLER_SAMPLETIME & 0x0000FFFFFFFFFFFF) | (((uint64_t)registerValue) << 48);
            }

            return 0;
        }
        case 40505:
        {
            if(SETPOINTS.ENABLE_PROGMODE == 1)
            {
                PARAMETERS.CONTROLLER_SAMPLETIME = (PARAMETERS.CONTROLLER_SAMPLETIME & 0xFFFF0000FFFFFFFF) | (((uint64_t)registerValue) << 32);
            }

            return 0;
        }
        case 40506:
        {
            if(SETPOINTS.ENABLE_PROGMODE == 1)
            {
                PARAMETERS.CONTROLLER_SAMPLETIME = (PARAMETERS.CONTROLLER_SAMPLETIME & 0xFFFFFFFF0000FFFF) | (((uint64_t)registerValue) << 16);
            }

            return 0;
        }
        case 40507:
        {
            if(SETPOINTS.ENABLE_PROGMODE == 1)
            {
                PARAMETERS.CONTROLLER_SAMPLETIME = (PARAMETERS.CONTROLLER_SAMPLETIME & 0xFFFFFFFFFFFF0000) | (((uint64_t)registerValue) <<  0);
            }

            return 0;
        }
        case 40508:
        {
            if(SETPOINTS.ENABLE_PROGMODE == 1)
            {
                PARAMETERS.CONTROLLER_ENABLE_TIMEOUT = (PARAMETERS.CONTROLLER_ENABLE_TIMEOUT & 0x0000FFFFFFFFFFFF) | (((uint64_t)registerValue) << 48);
            }

            return 0;
        }
        case 40509:
        {
            if(SETPOINTS.ENABLE_PROGMODE == 1)
            {
                PARAMETERS.CONTROLLER_ENABLE_TIMEOUT = (PARAMETERS.CONTROLLER_ENABLE_TIMEOUT & 0xFFFF0000FFFFFFFF) | (((uint64_t)registerValue) << 32);
            }

            return 0;
        }
        case 40510:
        {
            if(SETPOINTS.ENABLE_PROGMODE == 1)
            {
                PARAMETERS.CONTROLLER_ENABLE_TIMEOUT = (PARAMETERS.CONTROLLER_ENABLE_TIMEOUT & 0xFFFFFFFF0000FFFF) | (((uint64_t)registerValue) << 16);
            }

            return 0;
        }
        case 40511:
        {
            if(SETPOINTS.ENABLE_PROGMODE == 1)
            {
                PARAMETERS.CONTROLLER_ENABLE_TIMEOUT = (PARAMETERS.CONTROLLER_ENABLE_TIMEOUT & 0xFFFFFFFFFFFF0000) | (((uint64_t)registerValue) <<  0);
            }

            return 0;
        }
        case 40512:
        {
            if(SETPOINTS.ENABLE_PROGMODE == 1)
            {
                PARAMETERS.CONTROLLER_OVERDRIVE_TIMEOUT = (PARAMETERS.CONTROLLER_OVERDRIVE_TIMEOUT & 0x0000FFFFFFFFFFFF) | (((uint64_t)registerValue) << 48);
            }

            return 0;
        }
        case 40513:
        {
            if(SETPOINTS.ENABLE_PROGMODE == 1)
            {
                PARAMETERS.CONTROLLER_OVERDRIVE_TIMEOUT = (PARAMETERS.CONTROLLER_OVERDRIVE_TIMEOUT & 0xFFFF0000FFFFFFFF) | (((uint64_t)registerValue) << 32);
            }

            return 0;
        }
        case 40514:
        {
            if(SETPOINTS.ENABLE_PROGMODE == 1)
            {
                PARAMETERS.CONTROLLER_OVERDRIVE_TIMEOUT = (PARAMETERS.CONTROLLER_OVERDRIVE_TIMEOUT & 0xFFFFFFFF0000FFFF) | (((uint64_t)registerValue) << 16);
            }

            return 0;
        }
        case 40515:
        {
            if(SETPOINTS.ENABLE_PROGMODE == 1)
            {
                PARAMETERS.CONTROLLER_OVERDRIVE_TIMEOUT = (PARAMETERS.CONTROLLER_OVERDRIVE_TIMEOUT & 0xFFFFFFFFFFFF0000) | (((uint64_t)registerValue) <<  0);
            }

            return 0;
        }
        case 40516:
        {
            if(SETPOINTS.ENABLE_PROGMODE == 1)
            {
                PARAMETERS.CONTROLLER_OVERDRIVE_COOLDOWN = (PARAMETERS.CONTROLLER_OVERDRIVE_COOLDOWN & 0x0000FFFFFFFFFFFF) | (((uint64_t)registerValue) << 48);
            }

            return 0;
        }
        case 40517:
        {
            if(SETPOINTS.ENABLE_PROGMODE == 1)
            {
                PARAMETERS.CONTROLLER_OVERDRIVE_COOLDOWN = (PARAMETERS.CONTROLLER_OVERDRIVE_COOLDOWN & 0xFFFF0000FFFFFFFF) | (((uint64_t)registerValue) << 32);
            }

            return 0;
        }
        case 40518:
        {
            if(SETPOINTS.ENABLE_PROGMODE == 1)
            {
                PARAMETERS.CONTROLLER_OVERDRIVE_COOLDOWN = (PARAMETERS.CONTROLLER_OVERDRIVE_COOLDOWN & 0xFFFFFFFF0000FFFF) | (((uint64_t)registerValue) << 16);
            }

            return 0;
        }
        case 40519:
        {
            if(SETPOINTS.ENABLE_PROGMODE == 1)
            {
                PARAMETERS.CONTROLLER_OVERDRIVE_COOLDOWN = (PARAMETERS.CONTROLLER_OVERDRIVE_COOLDOWN & 0xFFFFFFFFFFFF0000) | (((uint64_t)registerValue) <<  0);
            }

            return 0;
        }
        case 40520:
        {
            if(SETPOINTS.ENABLE_PROGMODE == 1)
            {
                PARAMETERS.CONTROLLER_NOMINAL_UMZ_TIMEOUT = (PARAMETERS.CONTROLLER_NOMINAL_UMZ_TIMEOUT & 0x0000FFFFFFFFFFFF) | (((uint64_t)registerValue) << 48);
            }

            return 0;
        }
        case 40521:
        {
            if(SETPOINTS.ENABLE_PROGMODE == 1)
            {
                PARAMETERS.CONTROLLER_NOMINAL_UMZ_TIMEOUT = (PARAMETERS.CONTROLLER_NOMINAL_UMZ_TIMEOUT & 0xFFFF0000FFFFFFFF) | (((uint64_t)registerValue) << 32);
            }

            return 0;
        }
        case 40522:
        {
            if(SETPOINTS.ENABLE_PROGMODE == 1)
            {
                PARAMETERS.CONTROLLER_NOMINAL_UMZ_TIMEOUT = (PARAMETERS.CONTROLLER_NOMINAL_UMZ_TIMEOUT & 0xFFFFFFFF0000FFFF) | (((uint64_t)registerValue) << 16);
            }

            return 0;
        }
        case 40523:
        {
            if(SETPOINTS.ENABLE_PROGMODE == 1)
            {
                PARAMETERS.CONTROLLER_NOMINAL_UMZ_TIMEOUT = (PARAMETERS.CONTROLLER_NOMINAL_UMZ_TIMEOUT & 0xFFFFFFFFFFFF0000) | (((uint64_t)registerValue) <<  0);
            }

            return 0;
        }
        case 40524:
        {
            if(SETPOINTS.ENABLE_PROGMODE == 1)
            {
                PARAMETERS.CONTROLLER_OVERDRIVE_UMZ_TIMEOUT = (PARAMETERS.CONTROLLER_OVERDRIVE_UMZ_TIMEOUT & 0x0000FFFFFFFFFFFF) | (((uint64_t)registerValue) << 48);
            }

            return 0;
        }
        case 40525:
        {
            if(SETPOINTS.ENABLE_PROGMODE == 1)
            {
                PARAMETERS.CONTROLLER_OVERDRIVE_UMZ_TIMEOUT = (PARAMETERS.CONTROLLER_OVERDRIVE_UMZ_TIMEOUT & 0xFFFF0000FFFFFFFF) | (((uint64_t)registerValue) << 32);
            }

            return 0;
        }
        case 40526:
        {
            if(SETPOINTS.ENABLE_PROGMODE == 1)
            {
                PARAMETERS.CONTROLLER_OVERDRIVE_UMZ_TIMEOUT = (PARAMETERS.CONTROLLER_OVERDRIVE_UMZ_TIMEOUT & 0xFFFFFFFF0000FFFF) | (((uint64_t)registerValue) << 16);
            }

            return 0;
        }
        case 40527:
        {
            if(SETPOINTS.ENABLE_PROGMODE == 1)
            {
                PARAMETERS.CONTROLLER_OVERDRIVE_UMZ_TIMEOUT = (PARAMETERS.CONTROLLER_OVERDRIVE_UMZ_TIMEOUT & 0xFFFFFFFFFFFF0000) | (((uint64_t)registerValue) <<  0);
            }

            return 0;
        }
        case 40528:
        {
            if(SETPOINTS.ENABLE_PROGMODE == 1)
            {
                PARAMETERS.CONTROLLER_DCBUS_UMZ_TIMEOUT = (PARAMETERS.CONTROLLER_DCBUS_UMZ_TIMEOUT & 0x0000FFFFFFFFFFFF) | (((uint64_t)registerValue) << 48);
            }

            return 0;
        }
        case 40529:
        {
            if(SETPOINTS.ENABLE_PROGMODE == 1)
            {
                PARAMETERS.CONTROLLER_DCBUS_UMZ_TIMEOUT = (PARAMETERS.CONTROLLER_DCBUS_UMZ_TIMEOUT & 0xFFFF0000FFFFFFFF) | (((uint64_t)registerValue) << 32);
            }

            return 0;
        }
        case 40530:
        {
            if(SETPOINTS.ENABLE_PROGMODE == 1)
            {
                PARAMETERS.CONTROLLER_DCBUS_UMZ_TIMEOUT = (PARAMETERS.CONTROLLER_DCBUS_UMZ_TIMEOUT & 0xFFFFFFFF0000FFFF) | (((uint64_t)registerValue) << 16);
            }

            return 0;
        }
        case 40531:
        {
            if(SETPOINTS.ENABLE_PROGMODE == 1)
            {
                PARAMETERS.CONTROLLER_DCBUS_UMZ_TIMEOUT = (PARAMETERS.CONTROLLER_DCBUS_UMZ_TIMEOUT & 0xFFFFFFFFFFFF0000) | (((uint64_t)registerValue) <<  0);
            }

            return 0;
        }
        case 40532:
        {
            if(SETPOINTS.ENABLE_PROGMODE == 1)
            {
                PARAMETERS.CONTROLLER_DCBUS_NOMINAL_VOLTAGE = (uint16_t)registerValue;
            }

            return 0;
        }
        case 40533:
        {
            if(SETPOINTS.ENABLE_PROGMODE == 1)
            {
                PARAMETERS.CONTROLLER_DCBUS_MINIMAL_VOLTAGE = (uint16_t)registerValue;
            }

            return 0;
        }
        case 40534:
        {
            if(SETPOINTS.ENABLE_PROGMODE == 1)
            {
                PARAMETERS.CONTROLLER_DCBUS_MAXIMAL_VOLTAGE = (uint16_t)registerValue;
            }

            return 0;
        }
        case 40535:
        {
            if(SETPOINTS.ENABLE_PROGMODE == 1)
            {
                PARAMETERS.CONTROLLER_DCBUS_VOLTAGE_OFFSET = (int16_t)registerValue;
            }

            return 0;
        }
        case 40536:
        {
            if(SETPOINTS.ENABLE_PROGMODE == 1)
            {
                PARAMETERS.CONTROLLER_DCBUS_CURRENT_OFFSET = (int16_t)registerValue;
            }

            return 0;
        }
        case 40537:
        {
            if(SETPOINTS.ENABLE_PROGMODE == 1)
            {
                PARAMETERS.CONTROLLER_DCBUS_FAN_THRESHOLD = (int16_t)registerValue;
            }

            return 0;
        }
        case 40538:
        {
            if(SETPOINTS.ENABLE_PROGMODE == 1)
            {
                PARAMETERS.CONTROLLER_DCBUS_MAXIMAL_TEMP = (int16_t)registerValue;
            }

            return 0;
        }
        case 40539:
        {
            if(SETPOINTS.ENABLE_PROGMODE == 1)
            {
                PARAMETERS.CONTROLLER_MOTOR_FAN_THRESHOLD = (int16_t)registerValue;
            }

            return 0;
        }
        case 40540:
        {
            if(SETPOINTS.ENABLE_PROGMODE == 1)
            {
                PARAMETERS.CONTROLLER_MOTOR_MAXIMAL_TEMP = (int16_t)registerValue;
            }

            return 0;
        }
        case 40541:
        {
            if((SETPOINTS.ENABLE_PROGMODE == 1) && (registerValue < 5))
            {
                PARAMETERS.CONTROLLER_PWM_FREQUENCY = (uint16_t)registerValue;
            }

            return 0;
        }
        case 40542:
        {
            if(SETPOINTS.ENABLE_PROGMODE == 1)
            {
                PARAMETERS.CONTROLLER_PWM_DEADTIME = (uint16_t)registerValue;
            }

            return 0;
        }
        case 40543:
        {
            if((SETPOINTS.ENABLE_PROGMODE == 1) && ((uint16_t)registerValue < 4))
            {
                PARAMETERS.CONTROLLER_PWM_DEADTIME_PRESCALING = (uint16_t)registerValue;
            }

            return 0;
        }
        default:
        {
            if((registerAddress >= 41000) && (registerAddress <= 43047))
            {
                PARAMETERS.CONTROLLER_UF_VALUE[registerAddress - 41000] = (uint16_t)registerValue;
                return 0;
            }

            return 1;
        }
    }

    return 1;
}
