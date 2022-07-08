#include "Utils.h"
// port: 0 or 1
// pin: 0 - 31
uint32_t pselConfig(bool connect, int port, int pin)
{
    uint32_t result = 0;

    if (!connect)
    {
        result |= 0x80000000;
    }
    result |= port << 5;
    result |= pin;

    return result;
}

void reconfigureSpi()
{
    // This is the magic for reconfiguring SPI.
    // This code only works on Gen 3 devices (Argon, Boron, Xenon, B Series SoM)

    // This is what we want to remap to, but you can use different pins.
    // SPI   Pin  Hardware Pin
    // SCK   A0   P0.3
    // MOSI  A1   P0.4
    // MISO  A2   P0.28

    // The pin mapping table is handy for finding the hardware pin numbers:
    // https://docs.particle.io/reference/hardware/pin-info/?m=table&sort=num

    // You must bring up SPI1 on the original pins first, because otherwise SPI1.begin() will
    // overwrite the reconfiguration and revert it back to the old pins.
    SPI1.begin();

    // SCK and MOSI need to be configured as OUTPUT. MISO is INPUT.
    pinMode(D3, OUTPUT); // SCK
    pinMode(D2, OUTPUT); // MOSI
    pinMode(D4, INPUT);  // MISO

    // CS is configured above in the LIS3DHSPI object construction. It doesn't affect pin reconfiguration.

    // We reconfigure SPI1, which is nRF52 SPIM2. The addresses are in the nRF52 Product Specification.
    uint8_t *pBase = (uint8_t *)0x40023000;

    // ENABLE offset 0x500
    uint32_t *pENABLE = (uint32_t *)&pBase[0x500];

    // PSEL.SCK offset 0x508
    uint32_t *pPSEL_SCL = (uint32_t *)&pBase[0x508];

    // PSEL.MOSI offset 0x50c
    uint32_t *pPSEL_MOSI = (uint32_t *)&pBase[0x50c];

    // PSEL.MISO OFFSET 0x510
    uint32_t *pPSEL_MISO = (uint32_t *)&pBase[0x510];
    Log.info("SCK=%lx MOSI=%lx MISO=%lx", *pPSEL_SCL, *pPSEL_MOSI, *pPSEL_MISO);

    // Standard pin config for SPI1
    // SCK   D2   P1.01
    // MOSI  D3   P1.02
    // MISO  D4   P1.08

    // Disconnect the old pins
    *pPSEL_SCL = pselConfig(false, 1, 2);  // D2
    *pPSEL_MOSI = pselConfig(false, 1, 1); // D3
    *pPSEL_MISO = pselConfig(false, 1, 8); // D4
    Log.info("SCK=%lx MOSI=%lx MISO=%lx", *pPSEL_SCL, *pPSEL_MOSI, *pPSEL_MISO);

    // // Restore the old pins back to INPUT mode (MISO was already input)
    // pinMode(D2, INPUT); // SCK
    // pinMode(D3, INPUT); // MOSI

    // Disable SPIM
    *pENABLE = 0;

    // Reconnect to the new pins
    // *pPSEL_SCL = 0x80000021;	// D3
    // *pPSEL_MOSI = 0x80000022; 	// D2
    *pPSEL_SCL = pselConfig(true, 1, 1);  // D3
    *pPSEL_MOSI = pselConfig(true, 1, 2); // D2
    *pPSEL_MISO = pselConfig(true, 1, 8); // D4

    // Reenable SPIM
    *pENABLE = 0x7;

    Log.info("SCK=%lx MOSI=%lx MISO=%lx", *pPSEL_SCL, *pPSEL_MOSI, *pPSEL_MISO);
}
