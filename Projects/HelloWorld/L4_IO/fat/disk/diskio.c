#include "diskio.h"
#include "spi_flash.h"
#include "sd.h"
#include "c_tlm_var.h"
#include "spi_sem.h"

uint32_t mFlashWriteCount = 0;


DSTATUS disk_initialize(BYTE drv)
{
    DSTATUS status = RES_PARERR;

    #if ENABLE_TELEMETRY
    TLM_REG_VAR(tlm_component_get_by_name("debug"), mFlashWriteCount, tlm_uint);
    #endif

    spi1_lock_access();
    {
        switch(drv)
        {
            case driveNumFlashMem: status = flash_Initialize();    break;
            case driveNumSdCard:   status = sd_initialize();       break;
            default: status = RES_PARERR;    break;
        }
    }
    spi1_release_access();

    return status;
}

DSTATUS disk_status(BYTE drv)
{
    DSTATUS status = RES_PARERR;

    // No mutex needed here: MUTEX_SECTION()
    {
        switch(drv)
        {
            // Flash memory is always good to go!
            case driveNumFlashMem: status = RES_OK;        break;
            case driveNumSdCard:   status = sd_status();   break;
            default: status = RES_PARERR;    break;
        }
    }

    return status;
}

DRESULT disk_read (BYTE drv, BYTE *buff, DWORD sector, BYTE count)
{
    DSTATUS status = RES_PARERR;

    spi1_lock_access();
    {
        switch(drv)
        {
            case driveNumFlashMem:
                status = flash_ReadSector(buff, sector, count);
                break;
            case driveNumSdCard:
                status = sd_read(buff, sector, count);
                break;
            default:
                status = RES_PARERR;
                break;
        }
    }
    spi1_release_access();

    return status;
}

DRESULT disk_write(BYTE drv, const BYTE *buff, DWORD sector, BYTE count)
{
    DSTATUS status = RES_PARERR;

    spi1_lock_access();
    {
        switch(drv)
        {
            case driveNumFlashMem:
                ++mFlashWriteCount;
                status = flash_WriteSector((unsigned char*)buff, sector, count);
                break;
            case driveNumSdCard:
                status = sd_write(buff, sector, count);
                break;
            default:
                status = RES_PARERR;
                break;
        }
    }
    spi1_release_access();

    return status;
}

DRESULT disk_ioctl(BYTE drv, BYTE ctrl,void *buff)
{
    DSTATUS status = RES_PARERR;

    spi1_lock_access();
    {
        switch(drv)
        {
            case driveNumFlashMem: status = flash_ioctl(ctrl, buff);   break;
            case driveNumSdCard:   status = sd_ioctl(ctrl, buff);      break;
            default:            status = RES_PARERR; break;
        }
    }
    spi1_release_access();

    return status;
}
