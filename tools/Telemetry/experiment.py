import re

test = """LPC: telemetry ascii
START:disk:0
END:disk
START:debug:2
mCommandCount:2:1:2:uint16:1
mDiskTlmSize:2:1:2:uint16:0
END:debug
START:you:2
x:4:1:2:uint32:25
y:4:1:4:float:2.500000
END:you
   Finished in 1883 us


LPC: telemetry ascii
START:disk:0
END:disk
START:debug:2
mCommandCount:2:1:2:uint16:1
mDiskTlmSize:2:1:2:uint16:0
END:debug
START:you:2
x:4:1:2:uint32:25
y:4:1:4:float:2.500000
END:you
   Finished in 1883 us

us

====================================================

System Reset: Booting ...
----------------------------------------------------------
System Boot @ 48.000 Mhz
 Flash : OK -- Capacity 1997 KB, Available: 1994 KB
SD Card: Error or not present.  Error #3, Mounted: No
----------------------------------------------------------
Memory Information:
Global Used   :  2036
malloc Used   :  3104
malloc Avail. :     0
System Avail. : 60396
Next Heap ptr    : 0x10000C20
Last sbrk() ptr  : 0x10000C00
Last sbrk() size : 32
Num  sbrk() calls: 34
----------------------------------------------------------
You can program a PERMANENT ID of your board.
To do this, hold SW1 and SW4 and reset the board.
CPU flash altered/programmed counts: 373/448
CPU programmed flash (min/max): 68Kb - 144Kb
Last programming took 2814 ms
----------------------------------------------------------
Calling your main()
----------------------------------------------------------
Supported Commands:
       info : Task/CPU Info.  Use 'info 200' t ...
    meminfo : See memory info
     health : Output system health
       time : 'time' to view time.  'time set  ...
        cat : Read a file.  Ex: 'cat 0:file.tx ...
         cp : Copy files from/to Flash/SD Card ...
        dcp : Copy all files of a directory to ...
         ls : Use 'ls 0:' for Flash, or 'ls 1: ...
      mkdir : Create a directory. Ex: 'mkdir t ...
         mv : Rename a file. Ex: 'rm 0:file.tx ...
         nf : Write a new file. Ex: 'nf <file. ...
         rm : Remove a file. Ex: 'rm 0:file.tx ...
        i2c : 'i2c read 0x01 0x02 <count>' : R ...
    storage : Parameters: 'format sd', 'format ...
     reboot : Reboots the system
        log : 'log <hello>': log an info messa ...
      learn : Begin to learn IR codes for numb ...
   wireless : Use 'wireless' to see the nested ...
       file : Get a file using netload.exe or  ...
      flash : 'flash <filename>' Will flash CP ...
  telemetry : Outputs registered telemetry: 't ...
 'help <command>' to get help of a command
LPC:

================================================================

LPC: telemetry ascii
START:disk:0
END:disk
START:debug:2
mCommandCount:2:1:2:uint16:1
mDiskTlmSize:2:1:2:uint16:0
END:debug
START:you:2
x:4:1:2:uint32:25
y:4:1:4:float:FFFFF
END:you
   Finished in 1883 us

LPC: telemetry ascii
START:disk:0
END:disk
START:debug:2
mCommandCount:2:1:2:uint16:1
mDiskTlmSize:2:1:2:uint16:0
END:debug
"""

p = re.compile('(?s)LPC: telemetry ascii(.*?)[\x03][\x03][\x04][\x04][ ]{3}Finished in [0-9]+ us')
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