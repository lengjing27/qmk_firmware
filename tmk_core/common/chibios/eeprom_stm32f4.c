/**
 * @file eeprom_stm32f4.c
 * @author astro
 */

#include <string.h>
#include "eeprom_stm32.h"
#include "hal.h"

#define EE_SECTOR_SIZE 0x4000
#define EE_SECTOR_START 0x0800C000
#define EE_SECTOR_ID 3
#define EE_MAX_BYTES 1024 //compatible with avr which has 1024 bytes eeprom
#define IS_VALID_ADDRESS(ADDRESS) (((ADDRESS) >= 0) && ((ADDRESS) < EE_MAX_BYTES))
#define INDEX_TO_ADDRESS(ADDRESS) (EE_SECTOR_START + ADDRESS*4)

#define FLASH_PSIZE_BYTE 0
#define FLASH_PSIZE_HFWORD FLASH_CR_PSIZE_0
#define FLASH_PSIZE_WORD FLASH_CR_PSIZE_1
#define FLASH_CR_SNB_Pos 3
#define FLASH_KEY1_F4 0x45670123
#define FLASH_KEY2_F4 0xCDEF89AB
#define FLASH_EMPTY_VALUE 0xFFFFFFFF

uint8_t DataBuf[EE_MAX_BYTES];

typedef union {
    struct {
        uint16_t addr;
        uint16_t val;
    } flash;
    uint32_t value;
} __attribute__((packed)) flash_data_t;

static FLASH_Status FLASH_StatusF4(void)
{
    if (FLASH->SR & FLASH_SR_BSY) return FLASH_BUSY;

    if ((FLASH->SR & (FLASH_SR_PGSERR | FLASH_SR_PGPERR | FLASH_SR_PGAERR)) != 0) return FLASH_ERROR_PG;

    if ((FLASH->SR & FLASH_SR_WRPERR) != 0) return FLASH_ERROR_WRP;

    return FLASH_COMPLETE;
}

static void FLASH_DelayF4(void)
{
    __IO uint32_t i = 0;
    for (i = 0xFF; i != 0; i--) {
    }
}

static FLASH_Status FLASH_WaitF4(void)
{
    /* Check for the Flash Status */
    FLASH_Status status = FLASH_StatusF4();
    /* Wait for a Flash operation to complete or a TIMEOUT to occur */
    while( status == FLASH_BUSY) {
        FLASH_DelayF4();
        status = FLASH_StatusF4();
    }

    return status;
}

static void FLASH_UnlockF4(void) {
    FLASH->KEYR = FLASH_KEY1_F4;
    FLASH->KEYR = FLASH_KEY2_F4;
}

static FLASH_Status FLASH_EraseSectorF4(uint32_t sector) {
    FLASH_Status status = FLASH_WaitF4();
    if (status == FLASH_COMPLETE) {
        /* if the previous operation is completed, proceed to erase the page */
        FLASH->CR |= FLASH_CR_SER | (sector << FLASH_CR_SNB_Pos);
        FLASH->CR |= FLASH_CR_STRT;

        /* Wait for last operation to be completed */
        status = FLASH_WaitF4();
        /* clear the erase bit */
        FLASH->CR &= ~FLASH_CR_SNB;
        FLASH->CR &= ~FLASH_CR_SER;
        if (status != FLASH_COMPLETE) {
            // clear the error bit
            if (FLASH->SR & FLASH_SR_PGAERR) {
                FLASH->SR |= FLASH_SR_PGAERR;
            }
            if (FLASH->SR & FLASH_SR_PGPERR) {
                FLASH->SR |= FLASH_SR_PGPERR;
            }
            if (FLASH->SR & FLASH_SR_PGSERR) {
                FLASH->SR |= FLASH_SR_PGSERR;
            }
            if (FLASH->SR & FLASH_SR_WRPERR) {
                FLASH->SR |= FLASH_SR_WRPERR;
            }
        }
    }
    /* Return the Erase Status */
    return status;
}

static FLASH_Status FLASH_ProgramWordF4(uint32_t address, uint32_t data)
{
    FLASH_Status status;
    if ((address % sizeof(uint32_t) != 0) || !IS_VALID_ADDRESS(address)) {
        // address not aligned
        return FLASH_BAD_ADDRESS;
    }
    status = FLASH_WaitF4();

    /* Wait for last operation to be completed */
    if (status == FLASH_COMPLETE) {
        // set size to word
        FLASH->CR &= ~FLASH_CR_PSIZE;
        FLASH->CR |= FLASH_PSIZE_WORD;
        FLASH->CR |= FLASH_CR_PG;

        *(__IO uint32_t *)address = data;
        /* Wait for last operation to be completed */
        status = FLASH_WaitF4();
        /* clear the pg bit */
        FLASH->CR &= ~FLASH_CR_PG;
        if (status != FLASH_COMPLETE) {
            // clear the error bit
            if (FLASH->SR & FLASH_SR_PGAERR) {
                FLASH->SR |= FLASH_SR_PGAERR;
            }
            if (FLASH->SR & FLASH_SR_PGPERR) {
                FLASH->SR |= FLASH_SR_PGPERR;
            }
            if (FLASH->SR & FLASH_SR_PGSERR) {
                FLASH->SR |= FLASH_SR_PGSERR;
            }
            if (FLASH->SR & FLASH_SR_WRPERR) {
                FLASH->SR |= FLASH_SR_WRPERR;
            }
        }
    }
    return status;
}

static uint32_t FLASH_FindValidAddress(void)
{
    uint32_t begin = EE_SECTOR_START;
    uint32_t end = begin + EE_SECTOR_SIZE;
    while( begin != end) {
        uint32_t data = *(__IO uint32_t*)(begin);
        if (data == FLASH_EMPTY_VALUE) {
            return begin;
        }
        begin += 4;
    }
    return FLASH_EMPTY_VALUE;
}

static void FLASH_Backup(void)
{
    uint32_t begin = EE_SECTOR_START;
    uint32_t end = begin + EE_SECTOR_SIZE;
    memset(&DataBuf[0], 0xFF, sizeof(DataBuf));
    while( begin != end) {
        flash_data_t flash_data = *(__IO flash_data_t*)(begin);
        if (IS_VALID_ADDRESS(flash_data.flash.addr)){
            DataBuf[flash_data.flash.addr] = flash_data.flash.val;
        }
        begin += 4;
    }
}

static uint16_t FLASH_Tranfer(void)
{
    for (uint8_t i = 0; i < EE_MAX_BYTES; i++) {
        if (DataBuf[i] != 0xFF) {
            flash_data_t flash_data = {.flash.addr=i, .flash.addr=DataBuf[i]};
            FLASH_ProgramWordF4(INDEX_TO_ADDRESS(i), flash_data.value);
        }
    }
    return FLASH_COMPLETE;
}

/*******************************************************************
 * stm32 eeprom interface
 *******************************************************************/
uint16_t EEPROM_Init(void)
{
    FLASH_UnlockF4();
    return FEE_DENSITY_BYTES;
}

void EEPROM_Erase(void)
{
    FLASH_EraseSectorF4(EE_SECTOR_ID);
}

uint16_t EEPROM_WriteDataByte(uint16_t Address, uint8_t DataByte)
{
    flash_data_t flash_data = {.flash.addr=Address, .flash.val = DataByte};
    uint32_t addr = FLASH_FindValidAddress();
    if (addr == FLASH_EMPTY_VALUE) {
        FLASH_Backup();
        DataBuf[Address] = DataByte;
        FLASH_EraseSectorF4(EE_SECTOR_ID);
        return FLASH_Tranfer();
    }

    return FLASH_ProgramWordF4(addr, flash_data.value);
}

uint8_t  EEPROM_ReadDataByte(uint16_t Address)
{
    uint32_t begin = EE_SECTOR_START;
    uint32_t end = begin + EE_SECTOR_SIZE-4;
    while( end != begin - 4) {
        flash_data_t data = *(__IO flash_data_t*)(end);
        if ( data.flash.addr == Address) {
            return (uint8_t)data.flash.val;
        }
        end -= 4;
    }
    return 0xFF;
}
#define EE_EMU
#ifdef EE_EMU
#    define EEPROM_SIZE 128
static uint8_t buffer[EEPROM_SIZE];

uint8_t eeprom_read_byte(const uint8_t *addr) {
    uint32_t offset = (uint32_t)addr;
    return buffer[offset];
}

void eeprom_write_byte(uint8_t *addr, uint8_t value) {
    uint32_t offset = (uint32_t)addr;
    buffer[offset]  = value;
}

uint16_t eeprom_read_word(const uint16_t *addr) {
    const uint8_t *p = (const uint8_t *)addr;
    return eeprom_read_byte(p) | (eeprom_read_byte(p + 1) << 8);
}

uint32_t eeprom_read_dword(const uint32_t *addr) {
    const uint8_t *p = (const uint8_t *)addr;
    return eeprom_read_byte(p) | (eeprom_read_byte(p + 1) << 8) | (eeprom_read_byte(p + 2) << 16) | (eeprom_read_byte(p + 3) << 24);
}

void eeprom_read_block(void *buf, const void *addr, uint32_t len) {
    const uint8_t *p    = (const uint8_t *)addr;
    uint8_t *      dest = (uint8_t *)buf;
    while (len--) {
        *dest++ = eeprom_read_byte(p++);
    }
}

void eeprom_write_word(uint16_t *addr, uint16_t value) {
    uint8_t *p = (uint8_t *)addr;
    eeprom_write_byte(p++, value);
    eeprom_write_byte(p, value >> 8);
}

void eeprom_write_dword(uint32_t *addr, uint32_t value) {
    uint8_t *p = (uint8_t *)addr;
    eeprom_write_byte(p++, value);
    eeprom_write_byte(p++, value >> 8);
    eeprom_write_byte(p++, value >> 16);
    eeprom_write_byte(p, value >> 24);
}

void eeprom_write_block(const void *buf, void *addr, uint32_t len) {
    uint8_t *      p   = (uint8_t *)addr;
    const uint8_t *src = (const uint8_t *)buf;
    while (len--) {
        eeprom_write_byte(p++, *src++);
    }
}
#else
/*****************************************************************************
 *  Wrap library in AVR style functions.
 *******************************************************************************/

uint8_t eeprom_read_byte(const uint8_t *Address) {
    return EEPROM_ReadDataByte((uint16_t)((uint32_t)Address));
}

void eeprom_write_byte(uint8_t *Address, uint8_t Value) {
    uint8_t data = eeprom_read_byte(Address);
    if (data != Value) {
        EEPROM_WriteDataByte((uint16_t)((uint32_t)Address), Value);
    }
}


uint16_t eeprom_read_word(const uint16_t *Address) {
    const uint8_t *p = (const uint8_t *)Address;
    return eeprom_read_byte(p) | (eeprom_read_byte(p + 1) << 8);
}

void eeprom_write_word(uint16_t *Address, uint16_t Value) {
    uint8_t *p = (uint8_t *)Address;
    eeprom_write_byte(p, (uint8_t)Value);
    eeprom_write_byte(p + 1, (uint8_t)(Value >> 8));
}


uint32_t eeprom_read_dword(const uint32_t *Address) {
    const uint8_t *p = (const uint8_t *)Address;
    return eeprom_read_byte(p) | (eeprom_read_byte(p + 1) << 8) | (eeprom_read_byte(p + 2) << 16) | (eeprom_read_byte(p + 3) << 24);
}

void eeprom_write_dword(uint32_t *Address, uint32_t Value) {
    uint8_t *p = (uint8_t *)Address;
    eeprom_write_byte(p, (uint8_t)Value);
    eeprom_write_byte(p + 1, (uint8_t)(Value >> 8));
    eeprom_write_byte(p + 2, (uint8_t)(Value >> 16));
    eeprom_write_byte(p + 3, (uint8_t)(Value >> 24));
}


void eeprom_read_block(void *buf, const void *addr, uint32_t len) {
    const uint8_t *p    = (const uint8_t *)addr;
    uint8_t *      dest = (uint8_t *)buf;
    while (len--) {
        *dest++ = eeprom_read_byte(p++);
    }
}

void eeprom_write_block(const void *buf, void *addr, uint32_t len) {
    uint8_t *      p   = (uint8_t *)addr;
    const uint8_t *src = (const uint8_t *)buf;
    while (len--) {
        eeprom_write_byte(p++, *src++);
    }
}


#endif
void eeprom_update_byte(uint8_t *Address, uint8_t Value) {
    eeprom_write_byte(Address, Value);
}
void eeprom_update_word(uint16_t *Address, uint16_t Value) {
    eeprom_write_word(Address, Value);
}
void eeprom_update_dword(uint32_t *Address, uint32_t Value) {
    eeprom_write_dword(Address, Value);
}
void eeprom_update_block(const void *buf, void *addr, uint32_t len) {
    eeprom_write_block(buf, addr, len);
}
