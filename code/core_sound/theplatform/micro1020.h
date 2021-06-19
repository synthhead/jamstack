#pragma once

#include <stdint.h>

#define F_CPU_ACTUAL 500000000
#define F_CPU        500000000

enum IRQ_NUMBER_t
{
  IRQ_DMA_CH0           = 0,
  IRQ_DMA_CH1           = 1,
  IRQ_DMA_CH2           = 2,
  IRQ_DMA_CH3           = 3,
  IRQ_DMA_CH4           = 4,
  IRQ_DMA_CH5           = 5,
  IRQ_DMA_CH6           = 6,
  IRQ_DMA_CH7           = 7,
  IRQ_DMA_CH8           = 8,
  IRQ_DMA_CH9           = 9,
  IRQ_DMA_CH10          = 10,
  IRQ_DMA_CH11          = 11,
  IRQ_DMA_CH12          = 12,
  IRQ_DMA_CH13          = 13,
  IRQ_DMA_CH14          = 14,
  IRQ_DMA_CH15          = 15,
  IRQ_DMA_ERROR         = 16,
  IRQ_CTI0              = 17,
  IRQ_CTI1              = 18,
  IRQ_CORE_ERROR        = 19,
  IRQ_LPUART1           = 20,
  IRQ_LPUART2           = 21,
  IRQ_LPUART3           = 22,
  IRQ_LPUART4           = 23,
  IRQ_LPUART5           = 24,
  IRQ_LPUART6           = 25,
  IRQ_LPUART7           = 26,
  IRQ_LPUART8           = 27,
  IRQ_LPI2C1            = 28,
  IRQ_LPI2C2            = 29,
  IRQ_LPI2C3            = 30,
  IRQ_LPI2C4            = 31,
  IRQ_LPSPI1            = 32,
  IRQ_LPSPI2            = 33,
  IRQ_LPSPI3            = 34,
  IRQ_LPSPI4            = 35,
  IRQ_CAN1              = 36,
  IRQ_CAN2              = 37,
  IRQ_ADDR_ERR          = 38,
  IRQ_KPP               = 39,
  IRQ_TSC_DIG           = 40,
  IRQ_GPR_IRQ           = 41,
  IRQ_LCDIF             = 42,
  IRQ_CSI               = 43,
  IRQ_PXP               = 44,
  IRQ_WDOG2             = 45,
  IRQ_SNVS_IRQ          = 46,
  IRQ_SNVS_SECURITY     = 47,
  IRQ_SNVS_ONOFF        = 48,
  IRQ_CSU               = 49,
  IRQ_DCP0              = 50,
  IRQ_DCP1              = 51,
  IRQ_DCP2              = 52,
  IRQ_TRNG              = 53,
  IRQ_SJC_IRQ           = 54,
  IRQ_BEE               = 55,
  IRQ_SAI1              = 56,
  IRQ_SAI2              = 57,
  IRQ_SAI3_RX           = 58,
  IRQ_SAI3_TX           = 59,
  IRQ_SPDIF             = 60,
  IRQ_BROWNOUT0         = 61,
  IRQ_Reserved1         = 62,
  IRQ_TEMPERATURE       = 63,
  IRQ_TEMPERATURE_PANIC = 64,
  IRQ_USBPHY0           = 65,
  IRQ_USBPHY1           = 66,
  IRQ_ADC1              = 67,
  IRQ_ADC2              = 68,
  IRQ_DCDC              = 69,
  IRQ_SOFTWARE          = 70,
  IRQ_Reserved2         = 71,
  IRQ_GPIO1_INT0        = 72,
  IRQ_GPIO1_INT1        = 73,
  IRQ_GPIO1_INT2        = 74,
  IRQ_GPIO1_INT3        = 75,
  IRQ_GPIO1_INT4        = 76,
  IRQ_GPIO1_INT5        = 77,
  IRQ_GPIO1_INT6        = 78,
  IRQ_GPIO1_INT7        = 79,
  IRQ_GPIO1_0_15        = 80,
  IRQ_GPIO1_16_31       = 81,
  IRQ_GPIO2_0_15        = 82,
  IRQ_GPIO2_16_31       = 83,
  IRQ_GPIO3_0_15        = 84,
  IRQ_GPIO3_16_31       = 85,
  IRQ_GPIO4_0_15        = 86,
  IRQ_GPIO4_16_31       = 87,
  IRQ_GPIO5_0_15        = 88,
  IRQ_GPIO5_16_31       = 89,
  IRQ_FLEXIO1           = 90,
  IRQ_FLEXIO2           = 91,
  IRQ_WDOG1             = 92,
  IRQ_RTWDOG            = 93,
  IRQ_EWM               = 94,
  IRQ_CCM1              = 95,
  IRQ_CCM2              = 96,
  IRQ_GPC               = 97,
  IRQ_SRC               = 98,
  IRQ_Reserved3         = 99,
  IRQ_GPT1              = 100,
  IRQ_GPT2              = 101,
  IRQ_FLEXPWM1_0        = 102,
  IRQ_FLEXPWM1_1        = 103,
  IRQ_FLEXPWM1_2        = 104,
  IRQ_FLEXPWM1_3        = 105,
  IRQ_FLEXPWM1_FAULT    = 106,
  IRQ_FLEXSPI2          = 107,
  IRQ_FLEXSPI           = 108,
  IRQ_SEMC              = 109,
  IRQ_SDHC1             = 110,
  IRQ_SDHC2             = 111,
  IRQ_USB2              = 112,
  IRQ_USB1              = 113,
  IRQ_ENET              = 114,
  IRQ_ENET_TIMER        = 115,
  IRQ_XBAR1_01          = 116,
  IRQ_XBAR1_23          = 117,
  IRQ_ADC_ETC0          = 118,
  IRQ_ADC_ETC1          = 119,
  IRQ_ADC_ETC2          = 120,
  IRQ_ADC_ETC_ERR       = 121,
  IRQ_PIT               = 122,
  IRQ_ACMP1             = 123,
  IRQ_ACMP2             = 124,
  IRQ_ACMP3             = 125,
  IRQ_ACMP4             = 126,
  IRQ_Reserved4         = 127,
  IRQ_Reserved5         = 128,
  IRQ_ENC1              = 129,
  IRQ_ENC2              = 130,
  IRQ_ENC3              = 131,
  IRQ_ENC4              = 132,
  IRQ_QTIMER1           = 133,
  IRQ_QTIMER2           = 134,
  IRQ_QTIMER3           = 135,
  IRQ_QTIMER4           = 136,
  IRQ_FLEXPWM2_0        = 137,
  IRQ_FLEXPWM2_1        = 138,
  IRQ_FLEXPWM2_2        = 139,
  IRQ_FLEXPWM2_3        = 140,
  IRQ_FLEXPWM2_FAULT    = 141,
  IRQ_FLEXPWM3_0        = 142,
  IRQ_FLEXPWM3_1        = 143,
  IRQ_FLEXPWM3_2        = 144,
  IRQ_FLEXPWM3_3        = 145,
  IRQ_FLEXPWM3_FAULT    = 146,
  IRQ_FLEXPWM4_0        = 147,
  IRQ_FLEXPWM4_1        = 148,
  IRQ_FLEXPWM4_2        = 149,
  IRQ_FLEXPWM4_3        = 150,
  IRQ_FLEXPWM4_FAULT    = 151,
  IRQ_ENET2             = 152,
  IRQ_ENET2_TIMER       = 153,
  IRQ_CAN3              = 154,
  IRQ_Reserved6         = 155,
  IRQ_FLEXIO3           = 156,
  IRQ_GPIO6789          = 157,
  IRQ_SJC_DEBUG         = 158,
  IRQ_NMI_WAKEUP        = 159
};

typedef struct
{
  volatile uint32_t offset000;
  volatile uint32_t offset004;
  volatile uint32_t offset008;
  volatile uint32_t offset00C;
  volatile uint32_t offset010;
  volatile uint32_t offset014;
  volatile uint32_t offset018;
  volatile uint32_t offset01C;
  volatile uint32_t offset020;
  volatile uint32_t offset024;
  volatile uint32_t offset028;
  volatile uint32_t offset02C;
  volatile uint32_t offset030;
  volatile uint32_t offset034;
  volatile uint32_t offset038;
  volatile uint32_t offset03C;
  volatile uint32_t offset040;
  volatile uint32_t offset044;
  volatile uint32_t offset048;
  volatile uint32_t offset04C;
  volatile uint32_t offset050;
  volatile uint32_t offset054;
  volatile uint32_t offset058;
  volatile uint32_t offset05C;
  volatile uint32_t offset060;
  volatile uint32_t offset064;
  volatile uint32_t offset068;
  volatile uint32_t offset06C;
  volatile uint32_t offset070;
  volatile uint32_t offset074;
  volatile uint32_t offset078;
  volatile uint32_t offset07C;
  volatile uint32_t offset080;
  volatile uint32_t offset084;
  volatile uint32_t offset088;
  volatile uint32_t offset08C;
  volatile uint32_t offset090;
  volatile uint32_t offset094;
  volatile uint32_t offset098;
  volatile uint32_t offset09C;
  volatile uint32_t offset0A0;
  volatile uint32_t offset0A4;
  volatile uint32_t offset0A8;
  volatile uint32_t offset0AC;
  volatile uint32_t offset0B0;
  volatile uint32_t offset0B4;
  volatile uint32_t offset0B8;
  volatile uint32_t offset0BC;
  volatile uint32_t offset0C0;
  volatile uint32_t offset0C4;
  volatile uint32_t offset0C8;
  volatile uint32_t offset0CC;
  volatile uint32_t offset0D0;
  volatile uint32_t offset0D4;
  volatile uint32_t offset0D8;
  volatile uint32_t offset0DC;
  volatile uint32_t offset0E0;
  volatile uint32_t offset0E4;
  volatile uint32_t offset0E8;
  volatile uint32_t offset0EC;
  volatile uint32_t offset0F0;
  volatile uint32_t offset0F4;
  volatile uint32_t offset0F8;
  volatile uint32_t offset0FC;
  volatile uint32_t offset100;
  volatile uint32_t offset104;
  volatile uint32_t offset108;
  volatile uint32_t offset10C;
  volatile uint32_t offset110;
  volatile uint32_t offset114;
  volatile uint32_t offset118;
  volatile uint32_t offset11C;
  volatile uint32_t offset120;
  volatile uint32_t offset124;
  volatile uint32_t offset128;
  volatile uint32_t offset12C;
  volatile uint32_t offset130;
  volatile uint32_t offset134;
  volatile uint32_t offset138;
  volatile uint32_t offset13C;
  volatile uint32_t offset140;
  volatile uint32_t offset144;
  volatile uint32_t offset148;
  volatile uint32_t offset14C;
  volatile uint32_t offset150;
  volatile uint32_t offset154;
  volatile uint32_t offset158;
  volatile uint32_t offset15C;
  volatile uint32_t offset160;
  volatile uint32_t offset164;
  volatile uint32_t offset168;
  volatile uint32_t offset16C;
  volatile uint32_t offset170;
  volatile uint32_t offset174;
  volatile uint32_t offset178;
  volatile uint32_t offset17C;
  volatile uint32_t offset180;
  volatile uint32_t offset184;
  volatile uint32_t offset188;
  volatile uint32_t offset18C;
  volatile uint32_t offset190;
  volatile uint32_t offset194;
  volatile uint32_t offset198;
  volatile uint32_t offset19C;
  volatile uint32_t offset1A0;
  volatile uint32_t offset1A4;
  volatile uint32_t offset1A8;
  volatile uint32_t offset1AC;
  volatile uint32_t offset1B0;
  volatile uint32_t offset1B4;
  volatile uint32_t offset1B8;
  volatile uint32_t offset1BC;
  volatile uint32_t offset1C0;
  volatile uint32_t offset1C4;
  volatile uint32_t offset1C8;
  volatile uint32_t offset1CC;
  volatile uint32_t offset1D0;
  volatile uint32_t offset1D4;
  volatile uint32_t offset1D8;
  volatile uint32_t offset1DC;
  volatile uint32_t offset1E0;
  volatile uint32_t offset1E4;
  volatile uint32_t offset1E8;
  volatile uint32_t offset1EC;
  volatile uint32_t offset1F0;
  volatile uint32_t offset1F4;
  volatile uint32_t offset1F8;
  volatile uint32_t offset1FC;
  volatile uint32_t offset200;
  volatile uint32_t offset204;
  volatile uint32_t offset208;
  volatile uint32_t offset20C;
  volatile uint32_t offset210;
  volatile uint32_t offset214;
  volatile uint32_t offset218;
  volatile uint32_t offset21C;
  volatile uint32_t offset220;
  volatile uint32_t offset224;
  volatile uint32_t offset228;
  volatile uint32_t offset22C;
  volatile uint32_t offset230;
  volatile uint32_t offset234;
  volatile uint32_t offset238;
  volatile uint32_t offset23C;
  volatile uint32_t offset240;
  volatile uint32_t offset244;
  volatile uint32_t offset248;
  volatile uint32_t offset24C;
  volatile uint32_t offset250;
  volatile uint32_t offset254;
  volatile uint32_t offset258;
  volatile uint32_t offset25C;
  volatile uint32_t offset260;
  volatile uint32_t offset264;
  volatile uint32_t offset268;
  volatile uint32_t offset26C;
  volatile uint32_t offset270;
  volatile uint32_t offset274;
  volatile uint32_t offset278;
  volatile uint32_t offset27C;
  volatile uint32_t offset280;
  volatile uint32_t offset284;
  volatile uint32_t offset288;
  volatile uint32_t offset28C;
  volatile uint32_t offset290;
  volatile uint32_t offset294;
  volatile uint32_t offset298;
  volatile uint32_t offset29C;
  volatile uint32_t offset2A0;
  volatile uint32_t offset2A4;
  volatile uint32_t offset2A8;
  volatile uint32_t offset2AC;
  volatile uint32_t offset2B0;
  volatile uint32_t offset2B4;
  volatile uint32_t offset2B8;
  volatile uint32_t offset2BC;
  volatile uint32_t offset2C0;
  volatile uint32_t offset2C4;
  volatile uint32_t offset2C8;
  volatile uint32_t offset2CC;
  volatile uint32_t offset2D0;
  volatile uint32_t offset2D4;
  volatile uint32_t offset2D8;
  volatile uint32_t offset2DC;
  volatile uint32_t offset2E0;
  volatile uint32_t offset2E4;
  volatile uint32_t offset2E8;
  volatile uint32_t offset2EC;
  volatile uint32_t offset2F0;
  volatile uint32_t offset2F4;
  volatile uint32_t offset2F8;
  volatile uint32_t offset2FC;
  volatile uint32_t offset300;
  volatile uint32_t offset304;
  volatile uint32_t offset308;
  volatile uint32_t offset30C;
  volatile uint32_t offset310;
  volatile uint32_t offset314;
  volatile uint32_t offset318;
  volatile uint32_t offset31C;
  volatile uint32_t offset320;
  volatile uint32_t offset324;
  volatile uint32_t offset328;
  volatile uint32_t offset32C;
  volatile uint32_t offset330;
  volatile uint32_t offset334;
  volatile uint32_t offset338;
  volatile uint32_t offset33C;
  volatile uint32_t offset340;
  volatile uint32_t offset344;
  volatile uint32_t offset348;
  volatile uint32_t offset34C;
  volatile uint32_t offset350;
  volatile uint32_t offset354;
  volatile uint32_t offset358;
  volatile uint32_t offset35C;
  volatile uint32_t offset360;
  volatile uint32_t offset364;
  volatile uint32_t offset368;
  volatile uint32_t offset36C;
  volatile uint32_t offset370;
  volatile uint32_t offset374;
  volatile uint32_t offset378;
  volatile uint32_t offset37C;
  volatile uint32_t offset380;
  volatile uint32_t offset384;
  volatile uint32_t offset388;
  volatile uint32_t offset38C;
  volatile uint32_t offset390;
  volatile uint32_t offset394;
  volatile uint32_t offset398;
  volatile uint32_t offset39C;
  volatile uint32_t offset3A0;
  volatile uint32_t offset3A4;
  volatile uint32_t offset3A8;
  volatile uint32_t offset3AC;
  volatile uint32_t offset3B0;
  volatile uint32_t offset3B4;
  volatile uint32_t offset3B8;
  volatile uint32_t offset3BC;
  volatile uint32_t offset3C0;
  volatile uint32_t offset3C4;
  volatile uint32_t offset3C8;
  volatile uint32_t offset3CC;
  volatile uint32_t offset3D0;
  volatile uint32_t offset3D4;
  volatile uint32_t offset3D8;
  volatile uint32_t offset3DC;
  volatile uint32_t offset3E0;
  volatile uint32_t offset3E4;
  volatile uint32_t offset3E8;
  volatile uint32_t offset3EC;
  volatile uint32_t offset3F0;
  volatile uint32_t offset3F4;
  volatile uint32_t offset3F8;
  volatile uint32_t offset3FC;
} IMXRT_REGISTER32_t;

#define DMA_TCD_ATTR_SSIZE(n) (((n)&0x7) << 8)
#define DMA_TCD_ATTR_DSIZE(n) (((n)&0x7) << 0)
#define DMA_TCD_CSR_INTHALF   0x0004
#define DMA_TCD_CSR_INTMAJOR  0x0002

#define IMXRT_I2S1 (*(IMXRT_REGISTER32_t *)0x40384000)
#define I2S1_TDR0  (IMXRT_I2S1.offset020)
#define I2S1_TCSR  (IMXRT_I2S1.offset008)
#define I2S1_RCSR  (IMXRT_I2S1.offset088)

#define DMAMUX_SOURCE_SAI1_RX 19
#define DMAMUX_SOURCE_SAI1_TX 20

#define I2S_RCSR_RE_ZM   ((uint32_t)0x80000000)
#define I2S_RCSR_BCE_ZM  ((uint32_t)0x10000000)
#define I2S_TCSR_TE_ZM   ((uint32_t)0x80000000)
#define I2S_TCSR_BCE_ZM  ((uint32_t)0x10000000)
#define I2S_TCSR_FRDE_ZM ((uint32_t)0x00000001)

#define SCB_CACHE_DCCMVAC  (*(volatile uint32_t *)0xE000EF68)
#define SCB_CACHE_DCCIMVAC (*(volatile uint32_t *)0xE000EF70)
#define SCB_CACHE_DCIMVAC  (*(volatile uint32_t *)0xE000EF5C)

__attribute__((always_inline, unused)) static inline void arm_dcache_flush(void *addr, uint32_t size)
{
  uint32_t location = (uint32_t)addr & 0xFFFFFFE0;
  uint32_t end_addr = (uint32_t)addr + size;
  asm volatile("" : : : "memory");
  asm("dsb");
  do
  {
    SCB_CACHE_DCCMVAC = location;
    location += 32;
  } while(location < end_addr);
  asm("dsb");
  asm("isb");
}

__attribute__((always_inline, unused)) static inline void arm_dcache_delete(void *addr, uint32_t size)
{
  uint32_t location = (uint32_t)addr & 0xFFFFFFE0;
  uint32_t end_addr = (uint32_t)addr + size;
  asm volatile("" : : : "memory");
  asm("dsb");
  do
  {
    SCB_CACHE_DCIMVAC = location;
    location += 32;
  } while(location < end_addr);
  asm("dsb");
  asm("isb");
}

__attribute__((always_inline, unused)) static inline void arm_dcache_flush_delete(void *addr, uint32_t size)
{
  uint32_t location = (uint32_t)addr & 0xFFFFFFE0;
  uint32_t end_addr = (uint32_t)addr + size;
  asm volatile("" : : : "memory");
  asm("dsb");
  do
  {
    SCB_CACHE_DCCIMVAC = location;
    location += 32;
  } while(location < end_addr);
  asm("dsb");
  asm("isb");
}

#define __zm_disable_irq() __asm__ volatile("CPSID i" ::: "memory");
#define __zm_enable_irq()  __asm__ volatile("CPSIE i" ::: "memory");

#define IMXRT_CCM  (*(IMXRT_REGISTER32_t *)0x400FC000)
#define CCM_CCGR5  (IMXRT_CCM.offset07C)
#define CCM_CSCMR1 (IMXRT_CCM.offset01C)
#define CCM_CS1CDR (IMXRT_CCM.offset028)

#define IMXRT_IOMUXC_GPR                       (*(IMXRT_REGISTER32_t *)0x400AC000)
#define IOMUXC_GPR_GPR1                        (IMXRT_IOMUXC_GPR.offset004)
#define IOMUXC_GPR_GPR1_SAI1_MCLK1_SEL_ZM(n)   ((uint32_t)(((n)&0x07) << 0))
#define IOMUXC_GPR_GPR1_SAI1_MCLK1_SEL_MASK_ZM IOMUXC_GPR_GPR1_SAI1_MCLK1_SEL_ZM(7)
#define IOMUXC_GPR_GPR1_SAI1_MCLK_DIR_ZM       ((uint32_t)(1 << 19))

#define IMXRT_IOMUXC_b                    (*(IMXRT_REGISTER32_t *)0x401F8400)
#define IOMUXC_SAI1_RX_DATA0_SELECT_INPUT (IMXRT_IOMUXC_b.offset194)

#define CCM_CCGR5_SAI1(n)              ((uint32_t)(((n)&0x03) << 18))
#define CCM_CSCMR1_SAI1_CLK_SEL_ZM(n)  ((uint32_t)(((n)&0x03) << 10))
#define CCM_CS1CDR_SAI1_CLK_PRED_ZM(n) ((uint32_t)(((n)&0x07) << 6))
#define CCM_CS1CDR_SAI1_CLK_PODF_ZM(n) ((uint32_t)(((n)&0x3F) << 0))
#define CCM_CCGR5_DMA(n)               ((uint32_t)(((n)&0x03) << 6))

#define CCM_CCGR_ON 3

#define CCM_CSCMR1_SAI1_CLK_SEL_MASK_ZM  (CCM_CSCMR1_SAI1_CLK_SEL_ZM(0x03))
#define CCM_CS1CDR_SAI1_CLK_PRED_MASK_ZM (CCM_CS1CDR_SAI1_CLK_PRED_ZM(0x07))
#define CCM_CS1CDR_SAI1_CLK_PODF_MASK_ZM (CCM_CS1CDR_SAI1_CLK_PODF_ZM(0x3f))

#define I2S1_TMR  (IMXRT_I2S1.offset060)
#define I2S1_TCR1 (IMXRT_I2S1.offset00C)
#define I2S1_TCR2 (IMXRT_I2S1.offset010)
#define I2S1_TCR3 (IMXRT_I2S1.offset014)
#define I2S1_TCR4 (IMXRT_I2S1.offset018)
#define I2S1_TCR5 (IMXRT_I2S1.offset01C)
#define I2S1_RMR  (IMXRT_I2S1.offset0E0)
#define I2S1_RCR1 (IMXRT_I2S1.offset08C)
#define I2S1_RCR2 (IMXRT_I2S1.offset090)
#define I2S1_RCR3 (IMXRT_I2S1.offset094)
#define I2S1_RCR4 (IMXRT_I2S1.offset098)
#define I2S1_RCR5 (IMXRT_I2S1.offset09C)
#define I2S1_RDR0 (IMXRT_I2S1.offset0A0)

#define I2S_RCSR_RE_ZM   ((uint32_t)0x80000000)
#define I2S_RCSR_BCE_ZM  ((uint32_t)0x10000000)
#define I2S_RCSR_FRDE_ZM ((uint32_t)0x00000001)
#define I2S_RCSR_FR_ZM   ((uint32_t)0x02000000)

#define I2S_TCR2_BCD_ZM     ((uint32_t)1 << 24)
#define I2S_TCR1_RFW_ZM(n)  ((uint32_t)n & 0x1f)
#define I2S_TCR2_SYNC_ZM(n) ((uint32_t)(n & 3) << 30)
#define I2S_TCR2_BCP_ZM     ((uint32_t)1 << 25)
#define I2S_TCR2_DIV_ZM(n)  ((uint32_t)n & 0xff)
#define I2S_TCR2_MSEL_ZM(n) ((uint32_t)(n & 3) << 26)
#define I2S_TCR3_TCE_ZM     ((uint32_t)0x10000)
#define I2S_TCR4_FRSZ_ZM(n) ((uint32_t)(n & 0x0f) << 16)
#define I2S_TCR4_SYWD_ZM(n) ((uint32_t)(n & 0x1f) << 8)
#define I2S_TCR4_MF_ZM      ((uint32_t)0x10)
#define I2S_TCR4_FSD_ZM     ((uint32_t)1)
#define I2S_TCR4_FSE_ZM     ((uint32_t)8)
#define I2S_TCR4_FSP_ZM     ((uint32_t)1 << 1)
#define I2S_TCR5_WNW_ZM(n)  ((uint32_t)(n & 0x1f) << 24)
#define I2S_TCR5_W0W_ZM(n)  ((uint32_t)(n & 0x1f) << 16)
#define I2S_TCR5_FBT_ZM(n)  ((uint32_t)(n & 0x1f) << 8)

#define I2S_RCR1_RFW_ZM(n)  ((uint32_t)n & 0x1f)
#define I2S_RCR2_SYNC_ZM(n) ((uint32_t)(n & 3) << 30)
#define I2S_RCR2_BCP_ZM     ((uint32_t)1 << 25)
#define I2S_RCR2_BCD_ZM     ((uint32_t)1 << 24)
#define I2S_RCR2_DIV_ZM(n)  ((uint32_t)n & 0xff)
#define I2S_RCR2_MSEL_ZM(n) ((uint32_t)(n & 3) << 26)
#define I2S_RCR3_RCE_ZM     ((uint32_t)0x10000)
#define I2S_RCR4_FRSZ_ZM(n) ((uint32_t)(n & 0x0f) << 16)
#define I2S_RCR4_SYWD_ZM(n) ((uint32_t)(n & 0x1f) << 8)
#define I2S_RCR4_MF_ZM      ((uint32_t)0x10)
#define I2S_RCR4_FSE_ZM     ((uint32_t)8)
#define I2S_RCR4_FSP_ZM     ((uint32_t)1 << 1)
#define I2S_RCR4_FSD_ZM     ((uint32_t)1)
#define I2S_RCR5_WNW_ZM(n)  ((uint32_t)(n & 0x1f) << 24)
#define I2S_RCR5_W0W_ZM(n)  ((uint32_t)(n & 0x1f) << 16)
#define I2S_RCR5_FBT_ZM(n)  ((uint32_t)(n & 0x1f) << 8)

#define DMA_TCD_BITER_ELINK ((uint16_t)1 << 15)
#define DMA_TCD_CSR_DREQ    0x0008
#define DMA_TCD_CSR_DONE    0x0080
#define DMA_TCD_CSR_ESG     0x0010

typedef struct
{
  volatile uint32_t CR;
  volatile uint32_t ES;
  uint32_t          unused1;
  volatile uint32_t ERQ;
  uint32_t          unused2;
  volatile uint32_t EEI;
  volatile uint8_t  CEEI;
  volatile uint8_t  SEEI;
  volatile uint8_t  CERQ;
  volatile uint8_t  SERQ;
  volatile uint8_t  CDNE;
  volatile uint8_t  SSRT;
  volatile uint8_t  CERR;
  volatile uint8_t  CINT;
  uint32_t          unused3;
  volatile uint32_t INT;
  uint32_t          unused4;
  volatile uint32_t ERR;
  uint32_t          unused5;
  volatile uint32_t HRS;
  uint32_t          unused6;
  uint32_t          unused7;
  uint32_t          unused8;
  volatile uint32_t EARS;
  uint32_t          unused[46];
  volatile uint8_t  DCHPRI3;
  volatile uint8_t  DCHPRI2;
  volatile uint8_t  DCHPRI1;
  volatile uint8_t  DCHPRI0;
  volatile uint8_t  DCHPRI7;
  volatile uint8_t  DCHPRI6;
  volatile uint8_t  DCHPRI5;
  volatile uint8_t  DCHPRI4;
  volatile uint8_t  DCHPRI11;
  volatile uint8_t  DCHPRI10;
  volatile uint8_t  DCHPRI9;
  volatile uint8_t  DCHPRI8;
  volatile uint8_t  DCHPRI15;
  volatile uint8_t  DCHPRI14;
  volatile uint8_t  DCHPRI13;
  volatile uint8_t  DCHPRI12;
  volatile uint8_t  DCHPRI19;
  volatile uint8_t  DCHPRI18;
  volatile uint8_t  DCHPRI17;
  volatile uint8_t  DCHPRI16;
  volatile uint8_t  DCHPRI23;
  volatile uint8_t  DCHPRI22;
  volatile uint8_t  DCHPRI21;
  volatile uint8_t  DCHPRI20;
  volatile uint8_t  DCHPRI27;
  volatile uint8_t  DCHPRI26;
  volatile uint8_t  DCHPRI25;
  volatile uint8_t  DCHPRI24;
  volatile uint8_t  DCHPRI31;
  volatile uint8_t  DCHPRI30;
  volatile uint8_t  DCHPRI29;
  volatile uint8_t  DCHPRI28;
} IMXRT_DMA_t;

#define IMXRT_DMA   (*(IMXRT_DMA_t *)0x400E8000)
#define DMA_CR      (IMXRT_DMA.CR)
#define DMA_SSRT    (IMXRT_DMA.SSRT)
#define DMA_CINT    (IMXRT_DMA.CINT)
#define DMA_SERQ    (IMXRT_DMA.SERQ)
#define DMA_CERQ    (IMXRT_DMA.CERQ)
#define DMA_CDNE    (IMXRT_DMA.CDNE)
#define DMA_ERR     (IMXRT_DMA.ERR)
#define DMA_CERR    (IMXRT_DMA.CERR)
#define DMA_DCHPRI3 (IMXRT_DMA.DCHPRI3)
#define DMA_CEEI    (IMXRT_DMA.CEEI)

#define DMA_CR_GRP1PRI_ZM ((uint32_t)(1 << 10))
#define DMA_CR_EMLM_ZM    ((uint32_t)(1 << 7))
#define DMA_CR_EDBG_ZM    ((uint32_t)(1 << 1))

#define IMXRT_DMAMUX         (*(IMXRT_REGISTER32_t *)0x400EC000)
#define DMAMUX_CHCFG0        (IMXRT_DMAMUX.offset000)
#define DMAMUX_CHCFG_ENBL_ZM ((uint32_t)(1 << 31))

#define DMA_TCD_BITER_ELINKYES_LINKCH(n)   (((n)&0x1F) << 9)
#define DMA_TCD_BITER_ELINKYES_LINKCH_MASK 0x3E00
#define DMA_TCD_BITER_ELINKYES_ELINK       0x8000

#define DMA_TCD_CSR_MAJORLINKCH(n)   (((n)&0x1F) << 8)
#define DMA_TCD_CSR_MAJORLINKCH_MASK 0x1F00
#define DMA_TCD_CSR_MAJORELINK       0x0020

#define IMXRT_CCM_ANALOG           (*(IMXRT_REGISTER32_t *)0x400D8000)
#define CCM_ANALOG_PLL_AUDIO       (IMXRT_CCM_ANALOG.offset070)
#define CCM_ANALOG_PLL_AUDIO_NUM   (IMXRT_CCM_ANALOG.offset080)
#define CCM_ANALOG_PLL_AUDIO_DENOM (IMXRT_CCM_ANALOG.offset090)
#define CCM_ANALOG_MISC2           (IMXRT_CCM_ANALOG.offset170)

#define CCM_ANALOG_PLL_AUDIO_ENABLE_ZM             ((uint32_t)(1 << 13))
#define CCM_ANALOG_PLL_AUDIO_BYPASS_ZM             ((uint32_t)(1 << 16))
#define CCM_ANALOG_PLL_AUDIO_POST_DIV_SELECT_ZM(n) ((uint32_t)(((n)&0x03) << 19))
#define CCM_ANALOG_PLL_AUDIO_DIV_SELECT_ZM(n)      ((uint32_t)((n) & ((1 << 6) - 1)))
#define CCM_ANALOG_PLL_AUDIO_NUM_MASK              (((1 << 29) - 1))
#define CCM_ANALOG_PLL_AUDIO_DENOM_MASK            (((1 << 29) - 1))
#define CCM_ANALOG_PLL_AUDIO_POWERDOWN_ZM          ((uint32_t)(1 << 12))
#define CCM_ANALOG_PLL_AUDIO_LOCK_ZM               ((uint32_t)(1 << 31))
#define CCM_ANALOG_MISC2_DIV_MSB                   ((uint32_t)(1 << 23))
#define CCM_ANALOG_MISC2_DIV_LSB                   ((uint32_t)(1 << 15))

#define SYST_CSR           (*(volatile uint32_t *)0xE000E010)
#define SYST_CSR_COUNTFLAG ((uint32_t)(1 << 16))
#define SYST_CSR_CLKSOURCE ((uint32_t)(1 << 2))
#define SYST_CSR_TICKINT   ((uint32_t)(1 << 1))
#define SYST_CSR_ENABLE    ((uint32_t)(1 << 0))
#define SYST_RVR           (*(volatile uint32_t *)0xE000E014)
#define SYST_CVR           (*(volatile uint32_t *)0xE000E018)
#define SYST_CALIB         (*(const uint32_t *)0xE000E01C)

#define SCB_SHPR3 (*(volatile uint32_t *)0xE000ED20)

#define ARM_DEMCR              (*(volatile uint32_t *)0xE000EDFC)
#define ARM_DEMCR_TRCENA       (1 << 24)
#define ARM_DWT_CTRL           (*(volatile uint32_t *)0xE0001000)
#define ARM_DWT_CTRL_CYCCNTENA (1 << 0)
#define ARM_DWT_CYCCNT         (*(volatile uint32_t *)0xE0001004)

#define IMXRT_OCOTP_VALUE (*(IMXRT_REGISTER32_t *)0x401F4400)
#define HW_OCOTP_MAC0     (IMXRT_OCOTP_VALUE.offset220)

#define IMXRT_PMU                    (*(IMXRT_REGISTER32_t *)0x400D8000)
#define PMU_REG_3P0                  (IMXRT_PMU.offset120)
#define PMU_REG_3P0_OUTPUT_TRG_ZM(n) ((uint32_t)(((n)&0x1F) << 8))
#define PMU_REG_3P0_BO_OFFSET_ZM(n)  ((uint32_t)(((n)&0x07) << 4))

#define PMU_REG_3P0_ENABLE_LINREG_ZM ((uint32_t)(1 << 0))

#define CCM_CCGR6           (IMXRT_CCM.offset080)
#define CCM_CCGR6_USBOH3(n) ((uint32_t)(((n)&0x03) << 0))

#define IMXRT_USB1        (*(IMXRT_REGISTER32_t *)0x402E0000)
#define USB1_BURSTSIZE    (IMXRT_USB1.offset160)
#define USB1_TXFILLTUNING (IMXRT_USB1.offset164)

#define IMXRT_USBPHY1            (*(IMXRT_REGISTER32_t *)0x400D9000)
#define USBPHY1_PWD              (IMXRT_USBPHY1.offset000)
#define USBPHY_PWD_RXPWDRX_ZM    ((uint32_t)(1 << 20))
#define USBPHY_PWD_RXPWDDIFF_ZM  ((uint32_t)(1 << 19))
#define USBPHY_PWD_RXPWD1PT1_ZM  ((uint32_t)(1 << 18))
#define USBPHY_PWD_RXPWDENV_ZM   ((uint32_t)(1 << 17))
#define USBPHY_PWD_TXPWDV2I_ZM   ((uint32_t)(1 << 12))
#define USBPHY_PWD_TXPWDIBIAS_ZM ((uint32_t)(1 << 11))
#define USBPHY_PWD_TXPWDFS_ZM    ((uint32_t)(1 << 10))

#define USB_USBMODE_CM_ZM(n) ((uint32_t)(((n)&0x03) << 0))

#define USB1_USBMODE           (IMXRT_USB1.offset1A8)
#define USB_USBMODE_CM_MASK_ZM USB_USBMODE_CM_ZM(3)

#define USBPHY1_CTRL_SET      (IMXRT_USBPHY1.offset034)
#define USBPHY_CTRL_SFTRST_ZM ((uint32_t)(1 << 31))

#define USB1_USBCMD            (IMXRT_USB1.offset140)
#define USB_USBCMD_RST_ZM      ((uint32_t)(1 << 1))
#define USBPHY1_CTRL_CLR       (IMXRT_USBPHY1.offset038)
#define USBPHY_CTRL_CLKGATE_ZM ((uint32_t)(1 << 30))

#define USB_USBMODE_SLOM_ZM   ((uint32_t)(1 << 3))
#define USB1_ENDPOINTLISTADDR (IMXRT_USB1.offset158)
#define USB1_USBINTR          (IMXRT_USB1.offset148)
#define USB_USBINTR_UEE_ZM    ((uint32_t)(1 << 1))
#define USB_USBINTR_UE_ZM     ((uint32_t)(1 << 0))
#define USB_USBINTR_URE_ZM    ((uint32_t)(1 << 6))
#define USB_USBINTR_SLE_ZM    ((uint32_t)(1 << 8))

#define USB_USBCMD_RS_ZM    ((uint32_t)(1 << 0))
#define USB1_USBSTS         (IMXRT_USB1.offset144)
#define USB_USBSTS_UI_ZM    ((uint32_t)(1 << 0))
#define USB1_ENDPTSETUPSTAT (IMXRT_USB1.offset1AC)
#define USB_USBCMD_SUTW_ZM  ((uint32_t)(1 << 13))

#define USB1_ENDPTFLUSH    (IMXRT_USB1.offset1B4)
#define USB1_ENDPTCOMPLETE (IMXRT_USB1.offset1BC)

#define USB_USBSTS_URI_ZM ((uint32_t)(1 << 6))
#define USB1_ENDPTPRIME   (IMXRT_USB1.offset1B0)
#define USB1_PORTSC1      (IMXRT_USB1.offset184)
#define USB_PORTSC1_PR_ZM ((uint32_t)(1 << 8))

#define USB_USBSTS_TI0_ZM  ((uint32_t)(1 << 24))
#define USB_USBSTS_TI1_ZM  ((uint32_t)(1 << 25))
#define USB_USBSTS_PCI_ZM  ((uint32_t)(1 << 2))
#define USB_PORTSC1_HSP_ZM ((uint32_t)(1 << 9))
#define USB_USBSTS_SLI_ZM  ((uint32_t)(1 << 8))
#define USB_USBSTS_UEI_ZM  ((uint32_t)(1 << 1))
#define USB_USBINTR_SRE_ZM ((uint32_t)(1 << 7))
#define USB_USBSTS_SRI_ZM  ((uint32_t)(1 << 7))

#define USB1_DEVICEADDR           (IMXRT_USB1.offset154)
#define USB_DEVICEADDR_USBADRA_ZM ((uint32_t)(1 << 24))

#define USB1_ENDPTCTRL2 (IMXRT_USB1.offset1C8)
#define USB1_ENDPTCTRL3 (IMXRT_USB1.offset1CC)
#define USB1_ENDPTCTRL0 (IMXRT_USB1.offset1C0)

#define USB_ENDPTCTRL_TXS_ZM ((uint32_t)(1 << 16))
#define USB_ENDPTCTRL_RXS_ZM ((uint32_t)(1 << 0))

#define USB_USBCMD_ATDTW_ZM ((uint32_t)(1 << 14))
#define USB1_ENDPTSTATUS    (IMXRT_USB1.offset1B8)

#define CCM_ANALOG_PLL_USB1     (IMXRT_CCM_ANALOG.offset010)
#define CCM_ANALOG_PLL_USB1_SET (IMXRT_CCM_ANALOG.offset014)
#define CCM_ANALOG_PLL_USB1_CLR (IMXRT_CCM_ANALOG.offset018)

#define CCM_ANALOG_PLL_USB1_DIV_SELECT_ZM  ((uint32_t)(1 << 1))
#define CCM_ANALOG_PLL_USB1_BYPASS_ZM      ((uint32_t)(1 << 16))
#define CCM_ANALOG_PLL_USB1_POWER_ZM       ((uint32_t)(1 << 12))
#define CCM_ANALOG_PLL_USB1_ENABLE_ZM      ((uint32_t)(1 << 13))
#define CCM_ANALOG_PLL_USB1_EN_USB_CLKS_ZM ((uint32_t)(1 << 6))
#define CCM_ANALOG_PLL_USB1_LOCK_ZM        ((uint32_t)(1 << 31))

#define IMXRT_SNVS    (*(IMXRT_REGISTER32_t *)0x400D4000)
#define SNVS_HPLR     (IMXRT_SNVS.offset000)
#define SNVS_HPCOMR   (IMXRT_SNVS.offset004)
#define SNVS_HPCR     (IMXRT_SNVS.offset008)
#define SNVS_HPSICR   (IMXRT_SNVS.offset00C)
#define SNVS_HPSVCR   (IMXRT_SNVS.offset010)
#define SNVS_HPSR     (IMXRT_SNVS.offset014)
#define SNVS_HPSVSR   (IMXRT_SNVS.offset018)
#define SNVS_HPHACIVR (IMXRT_SNVS.offset01C)
#define SNVS_HPHACR   (IMXRT_SNVS.offset020)
#define SNVS_HPRTCMR  (IMXRT_SNVS.offset024)
#define SNVS_HPRTCLR  (IMXRT_SNVS.offset028)
#define SNVS_HPTAMR   (IMXRT_SNVS.offset02C)
#define SNVS_HPTALR   (IMXRT_SNVS.offset030)
#define SNVS_LPLR     (IMXRT_SNVS.offset034)
#define SNVS_LPCR     (IMXRT_SNVS.offset038)
#define SNVS_LPMKCR   (IMXRT_SNVS.offset03C)
#define SNVS_LPSVCR   (IMXRT_SNVS.offset040)
#define SNVS_LPTDCR   (IMXRT_SNVS.offset048)
#define SNVS_LPSR     (IMXRT_SNVS.offset04C)
#define SNVS_LPSRTCMR (IMXRT_SNVS.offset050)
#define SNVS_LPSRTCLR (IMXRT_SNVS.offset054)
#define SNVS_LPTAR    (IMXRT_SNVS.offset058)
#define SNVS_LPSMCMR  (IMXRT_SNVS.offset05C)
#define SNVS_LPSMCLR  (IMXRT_SNVS.offset060)
#define SNVS_LPPGDR   (IMXRT_SNVS.offset064)
#define SNVS_LPGPR    (IMXRT_SNVS.offset068)
#define IMXRT_SNVS_b  (*(IMXRT_REGISTER32_t *)0x400D4800)
#define SNVS_HPVIDR1  (IMXRT_SNVS_b.offset3F8)
#define SNVS_HPVIDR2  (IMXRT_SNVS_b.offset3FC)
