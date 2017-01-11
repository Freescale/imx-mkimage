/*
** ###################################################################
**
**     Copyright (c) 2016 Freescale Semiconductor, Inc.
**     All rights reserved.
**
**     Redistribution and use in source and binary forms, with or without modification,
**     are permitted provided that the following conditions are met:
**
**     o Redistributions of source code must retain the above copyright notice, this list
**       of conditions and the following disclaimer.
**
**     o Redistributions in binary form must reproduce the above copyright notice, this
**       list of conditions and the following disclaimer in the documentation and/or
**       other materials provided with the distribution.
**
**     o Neither the name of Freescale Semiconductor, Inc. nor the names of its
**       contributors may be used to endorse or promote products derived from this
**       software without specific prior written permission.
**
**     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
**     ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
**     WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
**     DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
**     ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
**     (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
**     LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
**     ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
**     (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
**     SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**
**     http:                 www.freescale.com
**     mail:                 support@freescale.com
**
** ###################################################################
*/

#ifndef __DDRC_MEM_MAP_H__
#define __DDRC_MEM_MAP_H__

//JDG change base addr for DDRC
//#define DDRC_BASE_ADDR(X) 0x5c000000 + ((X & 1) * 0x20000) + ((X & 2) * 0x30000)
#define DDRC_BASE_ADDR(X) 0x5c000000 + ((X * 0x100000))

#define DDRC_MSTR(X)            REG32(DDRC_BASE_ADDR(X) + 0x00)
#define DDRC_MSTR_0             0x5c000000
#define DDRC_MSTR_1             0x5c100000
#define DDRC_STAT(X)            REG32(DDRC_BASE_ADDR(X) + 0x04)
#define DDRC_STAT_0             0x5c000004
#define DDRC_STAT_1             0x5c100004
#define DDRC_MRCTRL0(X)         REG32(DDRC_BASE_ADDR(X) + 0x10)
#define DDRC_MRCTRL0_0          0x5c000010
#define DDRC_MRCTRL0_1          0x5c100010
#define DDRC_MRCTRL1(X)         REG32(DDRC_BASE_ADDR(X) + 0x14)
#define DDRC_MRCTRL1_0          0x5c000014
#define DDRC_MRCTRL1_1          0x5c100014
#define DDRC_MRSTAT(X)          REG32(DDRC_BASE_ADDR(X) + 0x18)
#define DDRC_MRSTAT_0           0x5c000018
#define DDRC_MRSTAT_1           0x5c100018
#define DDRC_MRCTRL2(X)         REG32(DDRC_BASE_ADDR(X) + 0x1c)
#define DDRC_MRCTRL2_0          0x5c00001c
#define DDRC_MRCTRL2_1          0x5c10001c
#define DDRC_DERATEEN(X)        REG32(DDRC_BASE_ADDR(X) + 0x20)
#define DDRC_DERATEEN_0         0x5c000020
#define DDRC_DERATEEN_1         0x5c100020
#define DDRC_DERATEINT(X)       REG32(DDRC_BASE_ADDR(X) + 0x24)
#define DDRC_DERATEINT_0        0x5c000024
#define DDRC_DERATEINT_1        0x5c100024
#define DDRC_PWRCTL(X)          REG32(DDRC_BASE_ADDR(X) + 0x30)
#define DDRC_PWRCTL_0           0x5c000030
#define DDRC_PWRCTL_1           0x5c100030
#define DDRC_PWRTMG(X)          REG32(DDRC_BASE_ADDR(X) + 0x34)
#define DDRC_PWRTMG_0           0x5c000034
#define DDRC_PWRTMG_1           0x5c100034
#define DDRC_HWLPCTL(X)         REG32(DDRC_BASE_ADDR(X) + 0x38)
#define DDRC_HWLPCTL_0          0x5c000038
#define DDRC_HWLPCTL_1          0x5c100038
#define DDRC_RFSHCTL0(X)        REG32(DDRC_BASE_ADDR(X) + 0x50)
#define DDRC_RFSHCTL0_0         0x5c000050
#define DDRC_RFSHCTL0_1         0x5c100050
#define DDRC_RFSHCTL1(X)        REG32(DDRC_BASE_ADDR(X) + 0x54)
#define DDRC_RFSHCTL1_0         0x5c000054
#define DDRC_RFSHCTL1_1         0x5c100054
#define DDRC_RFSHCTL2(X)        REG32(DDRC_BASE_ADDR(X) + 0x58)
#define DDRC_RFSHCTL2_0         0x5c000058
#define DDRC_RFSHCTL2_1         0x5c100058
#define DDRC_RFSHCTL3(X)        REG32(DDRC_BASE_ADDR(X) + 0x60)
#define DDRC_RFSHCTL3_0         0x5c000060
#define DDRC_RFSHCTL3_1         0x5c100060
#define DDRC_RFSHTMG(X)         REG32(DDRC_BASE_ADDR(X) + 0x64)
#define DDRC_RFSHTMG_0          0x5c000064
#define DDRC_RFSHTMG_1          0x5c100064
#define DDRC_ECCCFG0(X)         REG32(DDRC_BASE_ADDR(X) + 0x70)
#define DDRC_ECCCFG0_0          0x5c000070
#define DDRC_ECCCFG0_1          0x5c100070
#define DDRC_ECCCFG1(X)         REG32(DDRC_BASE_ADDR(X) + 0x74)
#define DDRC_ECCCFG1_0          0x5c000074
#define DDRC_ECCCFG1_1          0x5c100074
#define DDRC_ECCSTAT(X)         REG32(DDRC_BASE_ADDR(X) + 0x78)
#define DDRC_ECCSTAT_0          0x5c000078
#define DDRC_ECCSTAT_1          0x5c100078
#define DDRC_ECCCLR(X)          REG32(DDRC_BASE_ADDR(X) + 0x7c)
#define DDRC_ECCCLR_0           0x5c00007c
#define DDRC_ECCCLR_1           0x5c10007c
#define DDRC_ECCERRCNT(X)       REG32(DDRC_BASE_ADDR(X) + 0x80)
#define DDRC_ECCERRCNT_0        0x5c000080
#define DDRC_ECCERRCNT_1        0x5c100080
#define DDRC_ECCCADDR0(X)       REG32(DDRC_BASE_ADDR(X) + 0x84)
#define DDRC_ECCCADDR0_0        0x5c000084
#define DDRC_ECCCADDR0_1        0x5c100084
#define DDRC_ECCCADDR1(X)       REG32(DDRC_BASE_ADDR(X) + 0x88)
#define DDRC_ECCCADDR1_0        0x5c000088
#define DDRC_ECCCADDR1_1        0x5c100088
#define DDRC_ECCCSYN0(X)        REG32(DDRC_BASE_ADDR(X) + 0x8c)
#define DDRC_ECCCSYN0_0         0x5c00008c
#define DDRC_ECCCSYN0_1         0x5c10008c
#define DDRC_ECCCSYN1(X)        REG32(DDRC_BASE_ADDR(X) + 0x90)
#define DDRC_ECCCSYN1_0         0x5c000090
#define DDRC_ECCCSYN1_1         0x5c100090
#define DDRC_ECCCSYN2(X)        REG32(DDRC_BASE_ADDR(X) + 0x94)
#define DDRC_ECCCSYN2_0         0x5c000094
#define DDRC_ECCCSYN2_1         0x5c100094
#define DDRC_ECCBITMASK0(X)     REG32(DDRC_BASE_ADDR(X) + 0x98)
#define DDRC_ECCBITMASK0_0      0x5c000098
#define DDRC_ECCBITMASK0_1      0x5c100098
#define DDRC_ECCBITMASK1(X)     REG32(DDRC_BASE_ADDR(X) + 0x9c)
#define DDRC_ECCBITMASK1_0      0x5c00009c
#define DDRC_ECCBITMASK1_1      0x5c10009c
#define DDRC_ECCBITMASK2(X)     REG32(DDRC_BASE_ADDR(X) + 0xa0)
#define DDRC_ECCBITMASK2_0      0x5c0000a0
#define DDRC_ECCBITMASK2_1      0x5c1000a0
#define DDRC_ECCUADDR0(X)       REG32(DDRC_BASE_ADDR(X) + 0xa4)
#define DDRC_ECCUADDR0_0        0x5c0000a4
#define DDRC_ECCUADDR0_1        0x5c1000a4
#define DDRC_ECCUADDR1(X)       REG32(DDRC_BASE_ADDR(X) + 0xa8)
#define DDRC_ECCUADDR1_0        0x5c0000a8
#define DDRC_ECCUADDR1_1        0x5c1000a8
#define DDRC_ECCUSYN0(X)        REG32(DDRC_BASE_ADDR(X) + 0xac)
#define DDRC_ECCUSYN0_0         0x5c0000ac
#define DDRC_ECCUSYN0_1         0x5c1000ac
#define DDRC_ECCUSYN1(X)        REG32(DDRC_BASE_ADDR(X) + 0xb0)
#define DDRC_ECCUSYN1_0         0x5c0000b0
#define DDRC_ECCUSYN1_1         0x5c1000b0
#define DDRC_ECCUSYN2(X)        REG32(DDRC_BASE_ADDR(X) + 0xb4)
#define DDRC_ECCUSYN2_0         0x5c0000b4
#define DDRC_ECCUSYN2_1         0x5c1000b4
#define DDRC_ECCPOISONADDR0(X)  REG32(DDRC_BASE_ADDR(X) + 0xb8)
#define DDRC_ECCPOISONADDR0_0   0x5c0000b8
#define DDRC_ECCPOISONADDR0_1   0x5c1000b8
#define DDRC_ECCPOISONADDR1(X)  REG32(DDRC_BASE_ADDR(X) + 0xbc)
#define DDRC_ECCPOISONADDR1_0   0x5c0000bc
#define DDRC_ECCPOISONADDR1_1   0x5c1000bc
#define DDRC_CRCPARCTL0(X)      REG32(DDRC_BASE_ADDR(X) + 0xc0)
#define DDRC_CRCPARCTL0_0       0x5c0000c0
#define DDRC_CRCPARCTL0_1       0x5c1000c0
#define DDRC_CRCPARCTL1(X)      REG32(DDRC_BASE_ADDR(X) + 0xc4)
#define DDRC_CRCPARCTL1_0       0x5c0000c4
#define DDRC_CRCPARCTL1_1       0x5c1000c4
#define DDRC_CRCPARCTL2(X)      REG32(DDRC_BASE_ADDR(X) + 0xc8)
#define DDRC_CRCPARCTL2_0       0x5c0000c8
#define DDRC_CRCPARCTL2_1       0x5c1000c8
#define DDRC_CRCPARSTAT(X)      REG32(DDRC_BASE_ADDR(X) + 0xcc)
#define DDRC_CRCPARSTAT_0       0x5c0000cc
#define DDRC_CRCPARSTAT_1       0x5c1000cc
#define DDRC_INIT0(X)           REG32(DDRC_BASE_ADDR(X) + 0xd0)
#define DDRC_INIT0_0            0x5c0000d0
#define DDRC_INIT0_1            0x5c1000d0
#define DDRC_INIT1(X)           REG32(DDRC_BASE_ADDR(X) + 0xd4)
#define DDRC_INIT1_0            0x5c0000d4
#define DDRC_INIT1_1            0x5c1000d4
#define DDRC_INIT2(X)           REG32(DDRC_BASE_ADDR(X) + 0xd8)
#define DDRC_INIT2_0            0x5c0000d8
#define DDRC_INIT2_1            0x5c1000d8
#define DDRC_INIT3(X)           REG32(DDRC_BASE_ADDR(X) + 0xdc)
#define DDRC_INIT3_0            0x5c0000dc
#define DDRC_INIT3_1            0x5c1000dc
#define DDRC_INIT4(X)           REG32(DDRC_BASE_ADDR(X) + 0xe0)
#define DDRC_INIT4_0            0x5c0000e0
#define DDRC_INIT4_1            0x5c1000e0
#define DDRC_INIT5(X)           REG32(DDRC_BASE_ADDR(X) + 0xe4)
#define DDRC_INIT5_0            0x5c0000e4
#define DDRC_INIT5_1            0x5c1000e4
#define DDRC_INIT6(X)           REG32(DDRC_BASE_ADDR(X) + 0xe8)
#define DDRC_INIT6_0            0x5c0000e8
#define DDRC_INIT6_1            0x5c1000e8
#define DDRC_INIT7(X)           REG32(DDRC_BASE_ADDR(X) + 0xec)
#define DDRC_INIT7_0            0x5c0000ec
#define DDRC_INIT7_1            0x5c1000ec
#define DDRC_DIMMCTL(X)         REG32(DDRC_BASE_ADDR(X) + 0xf0)
#define DDRC_DIMMCTL_0          0x5c0000f0
#define DDRC_DIMMCTL_1          0x5c1000f0
#define DDRC_RANKCTL(X)         REG32(DDRC_BASE_ADDR(X) + 0xf4)
#define DDRC_RANKCTL_0          0x5c0000f4
#define DDRC_RANKCTL_1          0x5c1000f4
#define DDRC_DRAMTMG0(X)        REG32(DDRC_BASE_ADDR(X) + 0x100)
#define DDRC_DRAMTMG0_0         0x5c000100
#define DDRC_DRAMTMG0_1         0x5c100100
#define DDRC_DRAMTMG1(X)        REG32(DDRC_BASE_ADDR(X) + 0x104)
#define DDRC_DRAMTMG1_0         0x5c000104
#define DDRC_DRAMTMG1_1         0x5c100104
#define DDRC_DRAMTMG2(X)        REG32(DDRC_BASE_ADDR(X) + 0x108)
#define DDRC_DRAMTMG2_0         0x5c000108
#define DDRC_DRAMTMG2_1         0x5c100108
#define DDRC_DRAMTMG3(X)        REG32(DDRC_BASE_ADDR(X) + 0x10c)
#define DDRC_DRAMTMG3_0         0x5c00010c
#define DDRC_DRAMTMG3_1         0x5c10010c
#define DDRC_DRAMTMG4(X)        REG32(DDRC_BASE_ADDR(X) + 0x110)
#define DDRC_DRAMTMG4_0         0x5c000110
#define DDRC_DRAMTMG4_1         0x5c100110
#define DDRC_DRAMTMG5(X)        REG32(DDRC_BASE_ADDR(X) + 0x114)
#define DDRC_DRAMTMG5_0         0x5c000114
#define DDRC_DRAMTMG5_1         0x5c100114
#define DDRC_DRAMTMG6(X)        REG32(DDRC_BASE_ADDR(X) + 0x118)
#define DDRC_DRAMTMG6_0         0x5c000118
#define DDRC_DRAMTMG6_1         0x5c100118
#define DDRC_DRAMTMG7(X)        REG32(DDRC_BASE_ADDR(X) + 0x11c)
#define DDRC_DRAMTMG7_0         0x5c00011c
#define DDRC_DRAMTMG7_1         0x5c10011c
#define DDRC_DRAMTMG8(X)        REG32(DDRC_BASE_ADDR(X) + 0x120)
#define DDRC_DRAMTMG8_0         0x5c000120
#define DDRC_DRAMTMG8_1         0x5c100120
#define DDRC_DRAMTMG9(X)        REG32(DDRC_BASE_ADDR(X) + 0x124)
#define DDRC_DRAMTMG9_0         0x5c000124
#define DDRC_DRAMTMG9_1         0x5c100124
#define DDRC_DRAMTMG10(X)       REG32(DDRC_BASE_ADDR(X) + 0x128)
#define DDRC_DRAMTMG10_0        0x5c000128
#define DDRC_DRAMTMG10_1        0x5c100128
#define DDRC_DRAMTMG11(X)       REG32(DDRC_BASE_ADDR(X) + 0x12c)
#define DDRC_DRAMTMG11_0        0x5c00012c
#define DDRC_DRAMTMG11_1        0x5c10012c
#define DDRC_DRAMTMG12(X)       REG32(DDRC_BASE_ADDR(X) + 0x130)
#define DDRC_DRAMTMG12_0        0x5c000130
#define DDRC_DRAMTMG12_1        0x5c100130
#define DDRC_DRAMTMG13(X)       REG32(DDRC_BASE_ADDR(X) + 0x134)
#define DDRC_DRAMTMG13_0        0x5c000134
#define DDRC_DRAMTMG13_1        0x5c100134
#define DDRC_DRAMTMG14(X)       REG32(DDRC_BASE_ADDR(X) + 0x138)
#define DDRC_DRAMTMG14_0        0x5c000138
#define DDRC_DRAMTMG14_1        0x5c100138
#define DDRC_DRAMTMG15(X)       REG32(DDRC_BASE_ADDR(X) + 0x13C)
#define DDRC_DRAMTMG15_0        0x5c00013c
#define DDRC_DRAMTMG15_1        0x5c10013c
#define DDRC_DRAMTMG16(X)       REG32(DDRC_BASE_ADDR(X) + 0x140)
#define DDRC_DRAMTMG16_0        0x5c000140
#define DDRC_DRAMTMG16_1        0x5c100140
//
#define DDRC_ZQCTL0(X)          REG32(DDRC_BASE_ADDR(X) + 0x180)
#define DDRC_ZQCTL0_0           0x5c000180
#define DDRC_ZQCTL0_1           0x5c100180
#define DDRC_ZQCTL1(X)          REG32(DDRC_BASE_ADDR(X) + 0x184)
#define DDRC_ZQCTL1_0           0x5c000184
#define DDRC_ZQCTL1_1           0x5c100184
#define DDRC_ZQCTL2(X)          REG32(DDRC_BASE_ADDR(X) + 0x188)
#define DDRC_ZQCTL2_0           0x5c000188
#define DDRC_ZQCTL2_1           0x5c100188
#define DDRC_ZQSTAT(X)          REG32(DDRC_BASE_ADDR(X) + 0x18c)
#define DDRC_ZQSTAT_0           0x5c00018c
#define DDRC_ZQSTAT_1           0x5c10018c
#define DDRC_DFITMG0(X)         REG32(DDRC_BASE_ADDR(X) + 0x190)
#define DDRC_DFITMG0_0          0x5c000190
#define DDRC_DFITMG0_1          0x5c100190
#define DDRC_DFITMG1(X)         REG32(DDRC_BASE_ADDR(X) + 0x194)
#define DDRC_DFITMG1_0          0x5c000194
#define DDRC_DFITMG1_1          0x5c100194
#define DDRC_DFILPCFG0(X)       REG32(DDRC_BASE_ADDR(X) + 0x198)
#define DDRC_DFILPCFG0_0        0x5c000198
#define DDRC_DFILPCFG0_1        0x5c100198
#define DDRC_DFILPCFG1(X)       REG32(DDRC_BASE_ADDR(X) + 0x19c)
#define DDRC_DFILPCFG1_0        0x5c00019c
#define DDRC_DFILPCFG1_1        0x5c10019c
#define DDRC_DFIUPD0(X)         REG32(DDRC_BASE_ADDR(X) + 0x1a0)
#define DDRC_DFIUPD0_0          0x5c0001a0
#define DDRC_DFIUPD0_1          0x5c1001a0
#define DDRC_DFIUPD1(X)         REG32(DDRC_BASE_ADDR(X) + 0x1a4)
#define DDRC_DFIUPD1_0          0x5c0001a4
#define DDRC_DFIUPD1_1          0x5c1001a4
#define DDRC_DFIUPD2(X)         REG32(DDRC_BASE_ADDR(X) + 0x1a8)
#define DDRC_DFIUPD2_0          0x5c0001a8
#define DDRC_DFIUPD2_1          0x5c1001a8
//#define DDRC_DFIUPD3(X)         REG32(DDRC_BASE_ADDR(X) + 0x1ac)     // iMX8 hasn't it
#define DDRC_DFIMISC(X)         REG32(DDRC_BASE_ADDR(X) + 0x1b0)
#define DDRC_DFIMISC_0          0x5c0001b0
#define DDRC_DFIMISC_1          0x5c1001b0
#define DDRC_DFITMG2(X)         REG32(DDRC_BASE_ADDR(X) + 0x1b4)
#define DDRC_DFITMG2_0          0x5c0001b4
#define DDRC_DFITMG2_1          0x5c1001b4
#define DDRC_DFITMG3(X)         REG32(DDRC_BASE_ADDR(X) + 0x1b8)
#define DDRC_DFITMG3_0          0x5c0001b8
#define DDRC_DFITMG3_1          0x5c1001b8
//
#define DDRC_DBICTL(X)          REG32(DDRC_BASE_ADDR(X) + 0x1c0)
#define DDRC_DBICTL_0           0x5c0001c0
#define DDRC_DBICTL_1           0x5c1001c0
#define DDRC_TRAINCTL0(X)       REG32(DDRC_BASE_ADDR(X) + 0x1d0)
#define DDRC_TRAINCTL0_0        0x5c0001d0
#define DDRC_TRAINCTL0_1        0x5c1001d0
#define DDRC_TRAINCTL1(X)       REG32(DDRC_BASE_ADDR(X) + 0x1d4)
#define DDRC_TRAINCTL1_0        0x5c0001d4
#define DDRC_TRAINCTL1_1        0x5c1001d4
#define DDRC_TRAINCTL2(X)       REG32(DDRC_BASE_ADDR(X) + 0x1d8)
#define DDRC_TRAINCTL2_0        0x5c0001d8
#define DDRC_TRAINCTL2_1        0x5c1001d8
#define DDRC_TRAINSTAT(X)       REG32(DDRC_BASE_ADDR(X) + 0x1dc)
#define DDRC_TRAINSTAT_0        0x5c0001dc
#define DDRC_TRAINSTAT_1        0x5c1001dc
#define DDRC_ADDRMAP0(X)        REG32(DDRC_BASE_ADDR(X) + 0x200)
#define DDRC_ADDRMAP0_0         0x5c000200
#define DDRC_ADDRMAP0_1         0x5c100200
#define DDRC_ADDRMAP1(X)        REG32(DDRC_BASE_ADDR(X) + 0x204)
#define DDRC_ADDRMAP1_0         0x5c000204
#define DDRC_ADDRMAP1_1         0x5c100204
#define DDRC_ADDRMAP2(X)        REG32(DDRC_BASE_ADDR(X) + 0x208)
#define DDRC_ADDRMAP2_0         0x5c000208
#define DDRC_ADDRMAP2_1         0x5c100208
#define DDRC_ADDRMAP3(X)        REG32(DDRC_BASE_ADDR(X) + 0x20c)
#define DDRC_ADDRMAP3_0         0x5c00020c
#define DDRC_ADDRMAP3_1         0x5c10020c
#define DDRC_ADDRMAP4(X)        REG32(DDRC_BASE_ADDR(X) + 0x210)
#define DDRC_ADDRMAP4_0         0x5c000210
#define DDRC_ADDRMAP4_1         0x5c100210
#define DDRC_ADDRMAP5(X)        REG32(DDRC_BASE_ADDR(X) + 0x214)
#define DDRC_ADDRMAP5_0         0x5c000214
#define DDRC_ADDRMAP5_1         0x5c100214
#define DDRC_ADDRMAP6(X)        REG32(DDRC_BASE_ADDR(X) + 0x218)
#define DDRC_ADDRMAP6_0         0x5c000218
#define DDRC_ADDRMAP6_1         0x5c100218
#define DDRC_ADDRMAP7(X)        REG32(DDRC_BASE_ADDR(X) + 0x21c)
#define DDRC_ADDRMAP7_0         0x5c00021c
#define DDRC_ADDRMAP7_1         0x5c10021c
#define DDRC_ADDRMAP8(X)        REG32(DDRC_BASE_ADDR(X) + 0x220)
#define DDRC_ADDRMAP8_0         0x5c000220
#define DDRC_ADDRMAP8_1         0x5c100220
#define DDRC_ADDRMAP9(X)        REG32(DDRC_BASE_ADDR(X) + 0x224)
#define DDRC_ADDRMAP9_0         0x5c000224
#define DDRC_ADDRMAP9_1         0x5c100224
#define DDRC_ADDRMAP10(X)       REG32(DDRC_BASE_ADDR(X) + 0x228)
#define DDRC_ADDRMAP10_0        0x5c000228
#define DDRC_ADDRMAP10_1        0x5c100228
#define DDRC_ADDRMAP11(X)       REG32(DDRC_BASE_ADDR(X) + 0x22c)
#define DDRC_ADDRMAP11_0        0x5c00022c
#define DDRC_ADDRMAP11_1        0x5c10022c
//
#define DDRC_ODTCFG(X)          REG32(DDRC_BASE_ADDR(X) + 0x240)
#define DDRC_ODTCFG_0           0x5c000240
#define DDRC_ODTCFG_1           0x5c100240
#define DDRC_ODTMAP(X)          REG32(DDRC_BASE_ADDR(X) + 0x244)
#define DDRC_ODTMAP_0           0x5c000244
#define DDRC_ODTMAP_1           0x5c100244
#define DDRC_SCHED(X)           REG32(DDRC_BASE_ADDR(X) + 0x250)
#define DDRC_SCHED_0            0x5c000250
#define DDRC_SCHED_1            0x5c100250
#define DDRC_SCHED1(X)          REG32(DDRC_BASE_ADDR(X) + 0x254)
#define DDRC_SCHED1_0           0x5c000254
#define DDRC_SCHED1_1           0x5c100254
#define DDRC_PERFHPR1(X)        REG32(DDRC_BASE_ADDR(X) + 0x25c)
#define DDRC_PERFHPR1_0         0x5c00025c
#define DDRC_PERFHPR1_1         0x5c10025c
#define DDRC_PERFLPR1(X)        REG32(DDRC_BASE_ADDR(X) + 0x264)
#define DDRC_PERFLPR1_0         0x5c000264
#define DDRC_PERFLPR1_1         0x5c100264
#define DDRC_PERFWR1(X)         REG32(DDRC_BASE_ADDR(X) + 0x26c)
#define DDRC_PERFWR1_0          0x5c00026c
#define DDRC_PERFWR1_1          0x5c10026c
#define DDRC_PERFVPR1(X)        REG32(DDRC_BASE_ADDR(X) + 0x274)
#define DDRC_PERFVPR1_0         0x5c000274
#define DDRC_PERFVPR1_1         0x5c100274
//
#define DDRC_PERFVPW1(X)        REG32(DDRC_BASE_ADDR(X) + 0x278)
#define DDRC_PERFVPW1_0         0x5c000278
#define DDRC_PERFVPW1_1         0x5c100278
//
#define DDRC_DQMAP0(X)          REG32(DDRC_BASE_ADDR(X) + 0x280)
#define DDRC_DQMAP0_0           0x5c000280
#define DDRC_DQMAP0_1           0x5c100280
#define DDRC_DQMAP1(X)          REG32(DDRC_BASE_ADDR(X) + 0x284)
#define DDRC_DQMAP1_0           0x5c000284
#define DDRC_DQMAP1_1           0x5c100284
#define DDRC_DQMAP2(X)          REG32(DDRC_BASE_ADDR(X) + 0x288)
#define DDRC_DQMAP2_0           0x5c000288
#define DDRC_DQMAP2_1           0x5c100288
#define DDRC_DQMAP3(X)          REG32(DDRC_BASE_ADDR(X) + 0x28c)
#define DDRC_DQMAP3_0           0x5c00028c
#define DDRC_DQMAP3_1           0x5c10028c
#define DDRC_DQMAP4(X)          REG32(DDRC_BASE_ADDR(X) + 0x290)
#define DDRC_DQMAP4_0           0x5c000290
#define DDRC_DQMAP4_1           0x5c100290
#define DDRC_DQMAP5(X)          REG32(DDRC_BASE_ADDR(X) + 0x294)
#define DDRC_DQMAP5_0           0x5c000294
#define DDRC_DQMAP5_1           0x5c100294
#define DDRC_DBG0(X)            REG32(DDRC_BASE_ADDR(X) + 0x300)
#define DDRC_DBG0_0             0x5c000300
#define DDRC_DBG0_1             0x5c100300
#define DDRC_DBG1(X)            REG32(DDRC_BASE_ADDR(X) + 0x304)
#define DDRC_DBG1_0             0x5c000304
#define DDRC_DBG1_1             0x5c100304
#define DDRC_DBGCAM(X)          REG32(DDRC_BASE_ADDR(X) + 0x308)
#define DDRC_DBGCAM_0           0x5c000308
#define DDRC_DBGCAM_1           0x5c100308
#define DDRC_DBGCMD(X)          REG32(DDRC_BASE_ADDR(X) + 0x30c)
#define DDRC_DBGCMD_0           0x5c00030c
#define DDRC_DBGCMD_1           0x5c10030c
#define DDRC_DBGSTAT(X)         REG32(DDRC_BASE_ADDR(X) + 0x310)
#define DDRC_DBGSTAT_0          0x5c000310
#define DDRC_DBGSTAT_1          0x5c100310
//
#define DDRC_SWCTL(X)           REG32(DDRC_BASE_ADDR(X) + 0x320)
#define DDRC_SWCTL_0            0x5c000320
#define DDRC_SWCTL_1            0x5c100320
#define DDRC_SWSTAT(X)          REG32(DDRC_BASE_ADDR(X) + 0x324)
#define DDRC_SWSTAT_0           0x5c000324
#define DDRC_SWSTAT_1           0x5c100324
#define DDRC_OCPARCFG0(X)       REG32(DDRC_BASE_ADDR(X) + 0x330)
#define DDRC_OCPARCFG0_0        0x5c000330
#define DDRC_OCPARCFG0_1        0x5c100330
#define DDRC_OCPARCFG1(X)       REG32(DDRC_BASE_ADDR(X) + 0x334)
#define DDRC_OCPARCFG1_0        0x5c000334
#define DDRC_OCPARCFG1_1        0x5c100334
#define DDRC_OCPARCFG2(X)       REG32(DDRC_BASE_ADDR(X) + 0x338)
#define DDRC_OCPARCFG2_0        0x5c000338
#define DDRC_OCPARCFG2_1        0x5c100338
#define DDRC_OCPARCFG3(X)       REG32(DDRC_BASE_ADDR(X) + 0x33c)
#define DDRC_OCPARCFG3_0        0x5c00033c
#define DDRC_OCPARCFG3_1        0x5c10033c
#define DDRC_OCPARSTAT0(X)      REG32(DDRC_BASE_ADDR(X) + 0x340)
#define DDRC_OCPARSTAT0_0       0x5c000340
#define DDRC_OCPARSTAT0_1       0x5c100340
#define DDRC_OCPARSTAT1(X)      REG32(DDRC_BASE_ADDR(X) + 0x344)
#define DDRC_OCPARSTAT1_0       0x5c000344
#define DDRC_OCPARSTAT1_1       0x5c100344
#define DDRC_OCPARWLOG0(X)      REG32(DDRC_BASE_ADDR(X) + 0x348)
#define DDRC_OCPARWLOG0_0       0x5c000348
#define DDRC_OCPARWLOG0_1       0x5c100348
#define DDRC_OCPARWLOG1(X)      REG32(DDRC_BASE_ADDR(X) + 0x34c)
#define DDRC_OCPARWLOG1_0       0x5c00034c
#define DDRC_OCPARWLOG1_1       0x5c10034c
#define DDRC_OCPARWLOG2(X)      REG32(DDRC_BASE_ADDR(X) + 0x350)
#define DDRC_OCPARWLOG2_0       0x5c000350
#define DDRC_OCPARWLOG2_1       0x5c100350
#define DDRC_OCPARAWLOG0(X)     REG32(DDRC_BASE_ADDR(X) + 0x354)
#define DDRC_OCPARAWLOG0_0      0x5c000354
#define DDRC_OCPARAWLOG0_1      0x5c100354
#define DDRC_OCPARAWLOG1(X)     REG32(DDRC_BASE_ADDR(X) + 0x358)
#define DDRC_OCPARAWLOG1_0      0x5c000358
#define DDRC_OCPARAWLOG1_1      0x5c100358
#define DDRC_OCPARRLOG0(X)      REG32(DDRC_BASE_ADDR(X) + 0x35c)
#define DDRC_OCPARRLOG0_0       0x5c00035c
#define DDRC_OCPARRLOG0_1       0x5c10035c
#define DDRC_OCPARRLOG1(X)      REG32(DDRC_BASE_ADDR(X) + 0x360)
#define DDRC_OCPARRLOG1_0       0x5c000360
#define DDRC_OCPARRLOG1_1       0x5c100360
#define DDRC_OCPARARLOG0(X)     REG32(DDRC_BASE_ADDR(X) + 0x364)
#define DDRC_OCPARARLOG0_0      0x5c000364
#define DDRC_OCPARARLOG0_1      0x5c100364
#define DDRC_OCPARARLOG1(X)     REG32(DDRC_BASE_ADDR(X) + 0x368)
#define DDRC_OCPARARLOG1_0      0x5c000368
#define DDRC_OCPARARLOG1_1      0x5c100368

#define DDRC_PSTAT(X)           REG32(DDRC_BASE_ADDR(X) + 0x3fc)
#define DDRC_PSTAT_0            0x5c0003fc
#define DDRC_PSTAT_1            0x5c1003fc
#define DDRC_PCCFG(X)           REG32(DDRC_BASE_ADDR(X) + 0x400)
#define DDRC_PCCFG_0            0x5c000400
#define DDRC_PCCFG_1            0x5c100400
#define DDRC_PCFGR_0(X)         REG32(DDRC_BASE_ADDR(X) + 0x404)
#define DDRC_PCFGR_0_0          0x5c000404
#define DDRC_PCFGR_0_1          0x5c100404
#define DDRC_PCFGR_1(X)         REG32(DDRC_BASE_ADDR(X) + 1*0xb0+0x404)
#define DDRC_PCFGR_1_0          0x5c0004b4
#define DDRC_PCFGR_1_1          0x5c1004b4
#define DDRC_PCFGR_2(X)         REG32(DDRC_BASE_ADDR(X) + 2*0xb0+0x404)
#define DDRC_PCFGR_2_0          0x5c000564
#define DDRC_PCFGR_2_1          0x5c100564
#define DDRC_PCFGR_3(X)         REG32(DDRC_BASE_ADDR(X) + 3*0xb0+0x404)
#define DDRC_PCFGR_3_0          0x5c000614
#define DDRC_PCFGR_3_1          0x5c100614
#define DDRC_PCFGW_0(X)         REG32(DDRC_BASE_ADDR(X) + 0x408)
#define DDRC_PCFGW_0_0          0x5c000408
#define DDRC_PCFGW_0_1          0x5c100408
#define DDRC_PCFGW_1(X)         REG32(DDRC_BASE_ADDR(X) + 1*0xb0+0x408)
#define DDRC_PCFGW_1_0          0x5c0004b8
#define DDRC_PCFGW_1_1          0x5c1004b8
#define DDRC_PCFGW_2(X)         REG32(DDRC_BASE_ADDR(X) + 2*0xb0+0x408)
#define DDRC_PCFGW_2_0          0x5c000568
#define DDRC_PCFGW_2_1          0x5c100568
#define DDRC_PCFGW_3(X)         REG32(DDRC_BASE_ADDR(X) + 3*0xb0+0x408)
#define DDRC_PCFGW_3_0          0x5c000618
#define DDRC_PCFGW_3_1          0x5c100618
#define DDRC_PCFGC_0(X)         REG32(DDRC_BASE_ADDR(X) + 0x40c)
#define DDRC_PCFGC_0_0          0x5c00040c
#define DDRC_PCFGC_0_1          0x5c10040c
#define DDRC_PCFGIDMASKCH(X)    REG32(DDRC_BASE_ADDR(X) + 0x410)
#define DDRC_PCFGIDMASKCH_0     0x5c000410
#define DDRC_PCFGIDMASKCH_1     0x5c100410
#define DDRC_PCFGIDVALUECH(X)   REG32(DDRC_BASE_ADDR(X) + 0x414)
#define DDRC_PCFGIDVALUECH_0    0x5c000414
#define DDRC_PCFGIDVALUECH_1    0x5c100414
#define DDRC_PCTRL_0(X)         REG32(DDRC_BASE_ADDR(X) + 0x490)
#define DDRC_PCTRL_0_0          0x5c000490
#define DDRC_PCTRL_0_1          0x5c100490
#define DDRC_PCTRL_1(X)         REG32(DDRC_BASE_ADDR(X) + 0x490 + 1*0xb0)
#define DDRC_PCTRL_1_0          0x5c000540
#define DDRC_PCTRL_1_1          0x5c100540
#define DDRC_PCTRL_2(X)         REG32(DDRC_BASE_ADDR(X) + 0x490 + 2*0xb0)
#define DDRC_PCTRL_2_0          0x5c0005f0
#define DDRC_PCTRL_2_1          0x5c1005f0
#define DDRC_PCTRL_3(X)         REG32(DDRC_BASE_ADDR(X) + 0x490 + 3*0xb0)
#define DDRC_PCTRL_3_0          0x5c0006a0
#define DDRC_PCTRL_3_1          0x5c1006a0
#define DDRC_PCFGQOS0_0(X)      REG32(DDRC_BASE_ADDR(X) + 0x494)
#define DDRC_PCFGQOS0_0_0       0x5c000494
#define DDRC_PCFGQOS0_0_1       0x5c100494
#define DDRC_PCFGQOS1_0(X)      REG32(DDRC_BASE_ADDR(X) + 0x498)
#define DDRC_PCFGQOS1_0_0       0x5c000498
#define DDRC_PCFGQOS1_0_1       0x5c100498
#define DDRC_PCFGWQOS0_0(X)     REG32(DDRC_BASE_ADDR(X) + 0x49c)
#define DDRC_PCFGWQOS0_0_0      0x5c00049c
#define DDRC_PCFGWQOS0_0_1      0x5c10049c
#define DDRC_PCFGWQOS1_0(X)     REG32(DDRC_BASE_ADDR(X) + 0x4a0)
#define DDRC_PCFGWQOS1_0_0      0x5c0004a0
#define DDRC_PCFGWQOS1_0_1      0x5c1004a0
#define DDRC_SARBASE0(X)        REG32(DDRC_BASE_ADDR(X) + 0xf04)
#define DDRC_SARBASE0_0         0x5c000f04
#define DDRC_SARBASE0_1         0x5c100f04
#define DDRC_SARSIZE0(X)        REG32(DDRC_BASE_ADDR(X) + 0xf08)
#define DDRC_SARSIZE0_0         0x5c000f08
#define DDRC_SARSIZE0_1         0x5c100f08
#define DDRC_SBRCTL(X)          REG32(DDRC_BASE_ADDR(X) + 0xf24)
#define DDRC_SBRCTL_0           0x5c000f24
#define DDRC_SBRCTL_1           0x5c100f24
#define DDRC_SBRSTAT(X)         REG32(DDRC_BASE_ADDR(X) + 0xf28)
#define DDRC_SBRSTAT_0          0x5c000f28
#define DDRC_SBRSTAT_1          0x5c100f28
#define DDRC_SBRWDATA0(X)       REG32(DDRC_BASE_ADDR(X) + 0xf2c)
#define DDRC_SBRWDATA0_0        0x5c000f2c
#define DDRC_SBRWDATA0_1        0x5c100f2c
#define DDRC_SBRWDATA1(X)       REG32(DDRC_BASE_ADDR(X) + 0xf30)
#define DDRC_SBRWDATA1_0        0x5c000f30
#define DDRC_SBRWDATA1_1        0x5c100f30
#define DDRC_PDCH(X)            REG32(DDRC_BASE_ADDR(X) + 0xf34)
#define DDRC_PDCH_0             0x5c000f34
#define DDRC_PDCH_1             0x5c100f34

#define DDRC_PCFGW_0_0_ADDR     ((vuint8_t*)&(DDRC_PCFGW_0(0)))
#define DDRC_PCFGW_0_1_ADDR     ((vuint8_t*)&(DDRC_PCFGW_0(1)))
#define DDRC_PCFGW_0_2_ADDR     ((vuint8_t*)&(DDRC_PCFGW_0(2)))
#define DDRC_PCFGW_0_3_ADDR     ((vuint8_t*)&(DDRC_PCFGW_0(3)))

#define DDRC_MRCTRL1_0_ADDR     ((vuint8_t*)&(DDRC_MRCTRL1(0)))
#define DDRC_MRCTRL1_1_ADDR     ((vuint8_t*)&(DDRC_MRCTRL1(1)))
#define DDRC_MRCTRL1_2_ADDR     ((vuint8_t*)&(DDRC_MRCTRL1(2)))
#define DDRC_MRCTRL1_3_ADDR     ((vuint8_t*)&(DDRC_MRCTRL1(3)))


// SHADOW registers

#define DDRC_DERATEEN_SHADOW(X)        REG32(DDRC_BASE_ADDR(X) + 0x2020)
#define DDRC_DERATEEN_SHADOW_0  0x5c002020
#define DDRC_DERATEEN_SHADOW_1  0x5c102020
#define DDRC_DERATEINT_SHADOW(X)       REG32(DDRC_BASE_ADDR(X) + 0x2024)
#define DDRC_DERATEINT_SHADOW_0 0x5c002024
#define DDRC_DERATEINT_SHADOW_1 0x5c102024
#define DDRC_RFSHCTL0_SHADOW(X)        REG32(DDRC_BASE_ADDR(X) + 0x2050)
#define DDRC_RFSHCTL0_SHADOW_0  0x5c002050
#define DDRC_RFSHCTL0_SHADOW_1  0x5c102050
#define DDRC_RFSHTMG_SHADOW(X)         REG32(DDRC_BASE_ADDR(X) + 0x2064)
#define DDRC_RFSHTMG_SHADOW_0   0x5c002064
#define DDRC_RFSHTMG_SHADOW_1   0x5c102064
#define DDRC_INIT3_SHADOW(X)           REG32(DDRC_BASE_ADDR(X) + 0x20dc)
#define DDRC_INIT3_SHADOW_0     0x5c0020dc
#define DDRC_INIT3_SHADOW_1     0x5c1020dc
#define DDRC_INIT4_SHADOW(X)           REG32(DDRC_BASE_ADDR(X) + 0x20e0)
#define DDRC_INIT4_SHADOW_0     0x5c0020e0
#define DDRC_INIT4_SHADOW_1     0x5c1020e0
#define DDRC_INIT6_SHADOW(X)           REG32(DDRC_BASE_ADDR(X) + 0x20e8)
#define DDRC_INIT6_SHADOW_0     0x5c0020e8
#define DDRC_INIT6_SHADOW_1     0x5c1020e8
#define DDRC_INIT7_SHADOW(X)           REG32(DDRC_BASE_ADDR(X) + 0x20ec)
#define DDRC_INIT7_SHADOW_0     0x5c0020ec
#define DDRC_INIT7_SHADOW_1     0x5c1020ec
#define DDRC_DRAMTMG0_SHADOW(X)        REG32(DDRC_BASE_ADDR(X) + 0x2100)
#define DDRC_DRAMTMG0_SHADOW_0  0x5c002100
#define DDRC_DRAMTMG0_SHADOW_1  0x5c102100
#define DDRC_DRAMTMG1_SHADOW(X)        REG32(DDRC_BASE_ADDR(X) + 0x2104)
#define DDRC_DRAMTMG1_SHADOW_0  0x5c002104
#define DDRC_DRAMTMG1_SHADOW_1  0x5c102104
#define DDRC_DRAMTMG2_SHADOW(X)        REG32(DDRC_BASE_ADDR(X) + 0x2108)
#define DDRC_DRAMTMG2_SHADOW_0  0x5c002108
#define DDRC_DRAMTMG2_SHADOW_1  0x5c102108
#define DDRC_DRAMTMG3_SHADOW(X)        REG32(DDRC_BASE_ADDR(X) + 0x210c)
#define DDRC_DRAMTMG3_SHADOW_0  0x5c00210c
#define DDRC_DRAMTMG3_SHADOW_1  0x5c10210c
#define DDRC_DRAMTMG4_SHADOW(X)        REG32(DDRC_BASE_ADDR(X) + 0x2110)
#define DDRC_DRAMTMG4_SHADOW_0  0x5c002110
#define DDRC_DRAMTMG4_SHADOW_1  0x5c102110
#define DDRC_DRAMTMG5_SHADOW(X)        REG32(DDRC_BASE_ADDR(X) + 0x2114)
#define DDRC_DRAMTMG5_SHADOW_0  0x5c002114
#define DDRC_DRAMTMG5_SHADOW_1  0x5c102114
#define DDRC_DRAMTMG6_SHADOW(X)        REG32(DDRC_BASE_ADDR(X) + 0x2118)
#define DDRC_DRAMTMG6_SHADOW_0  0x5c002118
#define DDRC_DRAMTMG6_SHADOW_1  0x5c102118
#define DDRC_DRAMTMG7_SHADOW(X)        REG32(DDRC_BASE_ADDR(X) + 0x211c)
#define DDRC_DRAMTMG7_SHADOW_0  0x5c00211c
#define DDRC_DRAMTMG7_SHADOW_1  0x5c10211c
#define DDRC_DRAMTMG8_SHADOW(X)        REG32(DDRC_BASE_ADDR(X) + 0x2120)
#define DDRC_DRAMTMG8_SHADOW_0  0x5c002120
#define DDRC_DRAMTMG8_SHADOW_1  0x5c102120
#define DDRC_DRAMTMG9_SHADOW(X)        REG32(DDRC_BASE_ADDR(X) + 0x2124)
#define DDRC_DRAMTMG9_SHADOW_0  0x5c002124
#define DDRC_DRAMTMG9_SHADOW_1  0x5c102124
#define DDRC_DRAMTMG10_SHADOW(X)       REG32(DDRC_BASE_ADDR(X) + 0x2128)
#define DDRC_DRAMTMG10_SHADOW_0 0x5c002128
#define DDRC_DRAMTMG10_SHADOW_1 0x5c102128
#define DDRC_DRAMTMG11_SHADOW(X)       REG32(DDRC_BASE_ADDR(X) + 0x212c)
#define DDRC_DRAMTMG11_SHADOW_0 0x5c00212c
#define DDRC_DRAMTMG11_SHADOW_1 0x5c10212c
#define DDRC_DRAMTMG12_SHADOW(X)       REG32(DDRC_BASE_ADDR(X) + 0x2130)
#define DDRC_DRAMTMG12_SHADOW_0 0x5c002130
#define DDRC_DRAMTMG12_SHADOW_1 0x5c102130
#define DDRC_DRAMTMG13_SHADOW(X)       REG32(DDRC_BASE_ADDR(X) + 0x2134)
#define DDRC_DRAMTMG13_SHADOW_0 0x5c002134
#define DDRC_DRAMTMG13_SHADOW_1 0x5c102134
#define DDRC_DRAMTMG14_SHADOW(X)       REG32(DDRC_BASE_ADDR(X) + 0x2138)
#define DDRC_DRAMTMG14_SHADOW_0 0x5c002138
#define DDRC_DRAMTMG14_SHADOW_1 0x5c102138
#define DDRC_DRAMTMG15_SHADOW(X)       REG32(DDRC_BASE_ADDR(X) + 0x213C)
#define DDRC_DRAMTMG15_SHADOW_0 0x5c00213c
#define DDRC_DRAMTMG15_SHADOW_1 0x5c10213c
#define DDRC_DRAMTMG16_SHADOW(X)       REG32(DDRC_BASE_ADDR(X) + 0x2140)
#define DDRC_DRAMTMG16_SHADOW_0 0x5c002140
#define DDRC_DRAMTMG16_SHADOW_1 0x5c102140
#define DDRC_ZQCTL0_SHADOW(X)          REG32(DDRC_BASE_ADDR(X) + 0x2180)
#define DDRC_ZQCTL0_SHADOW_0    0x5c002180
#define DDRC_ZQCTL0_SHADOW_1    0x5c102180
#define DDRC_DFITMG0_SHADOW(X)         REG32(DDRC_BASE_ADDR(X) + 0x2190)
#define DDRC_DFITMG0_SHADOW_0   0x5c002190
#define DDRC_DFITMG0_SHADOW_1   0x5c102190
#define DDRC_DFITMG1_SHADOW(X)         REG32(DDRC_BASE_ADDR(X) + 0x2194)
#define DDRC_DFITMG1_SHADOW_0   0x5c002194
#define DDRC_DFITMG1_SHADOW_1   0x5c102194
#define DDRC_DFITMG2_SHADOW(X)         REG32(DDRC_BASE_ADDR(X) + 0x21b4)
#define DDRC_DFITMG2_SHADOW_0   0x5c0021b4
#define DDRC_DFITMG2_SHADOW_1   0x5c1021b4
#define DDRC_DFITMG3_SHADOW(X)         REG32(DDRC_BASE_ADDR(X) + 0x21b8)
#define DDRC_DFITMG3_SHADOW_0   0x5c0021b8
#define DDRC_DFITMG3_SHADOW_1   0x5c1021b8
#define DDRC_ODTCFG_SHADOW(X)          REG32(DDRC_BASE_ADDR(X) + 0x2240)
#define DDRC_ODTCFG_SHADOW_0    0x5c002240
#define DDRC_ODTCFG_SHADOW_1    0x5c102240

#endif
