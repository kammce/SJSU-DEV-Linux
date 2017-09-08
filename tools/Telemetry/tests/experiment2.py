import re

test = """ets Jun  8 2016 00:22:57

rst:0x1 (POWERON_RESET),boot:0x13 (SPI_FAST_FLASH_BOOT)
ets Jun  8 2016 00:22:57

rst:0x10 (RTCWDT_RTC_RESET),boot:0x13 (SPI_FAST_FLASH_BOOT)
configsip: 0, SPIWP:0xee
clk_drv:0x00,q_drv:0x00,d_drv:0x00,cs0_drv:0x00,hd_drv:0x00,wp_drv:0x00
mode:DIO, clock div:2
load:0x3fff0008,len:8
load:0x3fff0010,len:4440
load:0x40078000,len:11080
load:0x40080000,len:252
entry 0x40080034
[0;32mI (47) boot: ESP-IDF v2.0-rc1-1131-gd84df2b-dirty 2nd stage bootloader[0m
[0;32mI (47) boot: compile time 12:10:14[0m
[0;32mI (57) boot: Enabling RNG early entropy source...[0m
[0;32mI (106) boot: SPI Speed      : 40MHz[0m
[0;32mI (145) boot: SPI Mode       : DIO[0m
[0;32mI (183) boot: SPI Flash Size : 4MB[0m
[0;32mI (221) boot: Partition Table:[0m
[0;32mI (255) boot: ## Label            Usage          Type ST Offset   Length[0m
[0;32mI (323) boot:  0 nvs              WiFi data        01 02 00009000 00006000[0m
[0;32mI (393) boot:  1 phy_init         RF data          01 01 0000f000 00001000[0m
[0;32mI (463) boot:  2 factory          factory app      00 00 00010000 00100000[0m
[0;32mI (533) boot: End of partition table[0m
[0;32mI (572) boot: Disabling RNG early entropy source...[0m
[0;32mI (623) boot: Loading app partition at offset 00010000[0m
[0;32mI (1907) boot: segment 0: paddr=0x00010018 vaddr=0x00000000 size=0x0ffe8 ( 65512) [0m
[0;32mI (1907) boot: segment 1: paddr=0x00020008 vaddr=0x3f400010 size=0x0e7a8 ( 59304) map[0m
[0;32mI (1958) boot: segment 2: paddr=0x0002e7b8 vaddr=0x3ffb0000 size=0x02344 (  9028) load[0m
[0;32mI (2048) boot: segment 3: paddr=0x00030b04 vaddr=0x40080000 size=0x00400 (  1024) load[0m
[0;32mI (2118) boot: segment 4: paddr=0x00030f0c vaddr=0x40080400 size=0x12568 ( 75112) load[0m
[0;32mI (2288) boot: segment 5: paddr=0x0004347c vaddr=0x400c0000 size=0x00000 (     0) load[0m
[0;32mI (2289) boot: segment 6: paddr=0x00043484 vaddr=0x00000000 size=0x0cb84 ( 52100) [0m
[0;32mI (2351) boot: segment 7: paddr=0x00050010 vaddr=0x400d0018 size=0x4c278 (311928) map[0m
[0;32mI (2430) cpu_start: Pro cpu up.[0m
[0;32mI (2465) cpu_start: Single core mode[0m
[0;32mI (2505) heap_alloc_caps: Initializing. RAM available for dynamic allocation:[0m
[0;32mI (2576) heap_alloc_caps: At 3FFAE2A0 len 00001D60 (7 KiB): DRAM[0m
[0;32mI (2638) heap_alloc_caps: At 3FFB7258 len 00028DA8 (163 KiB): DRAM[0m
[0;32mI (2701) heap_alloc_caps: At 3FFE0440 len 00003BC0 (14 KiB): D/IRAM[0m
[0;32mI (2765) heap_alloc_caps: At 3FFE4350 len 0001BCB0 (111 KiB): D/IRAM[0m
[0;32mI (2830) heap_alloc_caps: At 40092968 len 0000D698 (53 KiB): IRAM[0m
[0;32mI (2893) cpu_start: Pro cpu start user code[0m
[0;32mI (3051) cpu_start: Starting scheduler on PRO CPU.[0m
I (3104) wifi: wifi firmware version: 72ddf26
I (3104) wifi: config NVS flash: enabled
I (3104) wifi: config nano formating: disabled
[0;32mI (3106) system_api: Base MAC address is not set, read default base MAC address from BLK0 of EFUSE[0m
[0;32mI (3115) system_api: Base MAC address is not set, read default base MAC address from BLK0 of EFUSE[0m
I (3130) wifi: Init dynamic tx buffer num: 32
I (3130) wifi: Init dynamic rx buffer num: 32
I (3134) wifi: wifi driver task: 3ffbbeac, prio:23, stack:4096
I (3139) wifi: Init static rx buffer num: 10
I (3143) wifi: Init dynamic rx buffer num: 32
I (3147) wifi: Init rx ampdu len mblock:7
I (3151) wifi: Init lldesc rx ampdu entry mblock:4
I (3155) wifi: wifi power manager task: 0x3ffc1284 prio: 21 stack: 2560
I (3820) wifi: wifi timer task: 3ffc2314, prio:22, stack:3584
[0;32mI (3839) phy: phy_version: 355.1, 59464c5, Jun 14 2017, 20:25:06, 0, 0[0m
I (3840) wifi: mode : sta (30:ae:a4:02:f4:5c) + softAP (30:ae:a4:02:f4:5d)
0 :: buffer ==telemetry ascii
==


0 :: buffer ==telemetry ascii
==
LPC: telemetry ascii
START:http_server:3
heap:4:1:2:uint32:231555
tasks:4:1:2:uint32:0
err:1:1:2:uint8:0
END:http_server
   Finished in 0 us

0 :: buffer ==telemetry ascii
==
LPC: telemetry ascii
START:http_server:3
heap:4:1:2:uint32:231666
tasks:4:1:2:uint32:0
err:1:1:2:uint8:0
END:http_server
   Finished in 0 us

0 :: buffer ==telemetry ascii
==
LPC: telemetry ascii
START:http_server:3
heap:4:1:2:uint32:231777
tasks:4:1:2:uint32:0
err:1:1:2:uint8:0
END:http_server
   Finished in 0 us
"""

p = re.compile('(?s)LPC: telemetry ascii(.*?)[\x03][\x03][\x04][\x04][ ]{3}Finished in [0-9]+ us\n')
p2 = re.compile('(?s)LPC: telemetry ascii(.*)')

arr = p.findall(test)
clean = p.sub('', test)
clean2 = p2.sub('', clean)

print(arr)
print(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>")
print(clean)
print(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>")
print(clean2)
print(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>")
print(arr[-1])