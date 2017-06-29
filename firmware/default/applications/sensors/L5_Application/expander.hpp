
#ifndef L5_APPLICATION_EXPANDER_HPP_
#define L5_APPLICATION_EXPANDER_HPP_

#define DEF_REG(x)          (*((volatile uint32_t *)x))
#define PCONP               DEF_REG(0x400FC0C4)
#define PCLKSEL0            DEF_REG(0x400FC1A8)
#define PCLKSEL1            DEF_REG(0x400FC1AC)
#define PINSEL3             DEF_REG(0x4002C00C)
#define T1IR                DEF_REG(0x40008000)
#define T1TC                DEF_REG(0x40008008)

#define B_IR_CR1        4
#define F_IR_CR1        (1 << B_IR_CR1)
#define B_CCR_CAP1RE        3
#define B_CCR_CAP1FE        4
#define B_CCR_CAP1I         5

#define F_CCR_CAP1RE        (1 << B_CCR_CAP1RE)
#define F_CCR_CAP1FE        (1 << B_CCR_CAP1FE)
#define F_CCR_CAP1I         (1 << B_CCR_CAP1I)

#define CCR_CAP1_EDGES      (F_CCR_CAP1RE | F_CCR_CAP1FE)

#define EXIF_ADDR_SETTLE                10           /* us */
#define EXIF_DATA_LATCH_PULSE_WIDTH     10           /* us */

#define LVL_LOW                         false
#define LVL_HIGH                        true

#define DIR_IN                          false
#define DIR_OUT                         true


enum expander_identifiers {
    IF_SEL0,    /* P2.0 */
    IF_SEL1,    /* P2.1 */
    IF_SEL2,    /* P2.2 */
    IF_DIN,     /* P2.3 */
    IF_LE,      /* P2.4 */
    IF_DOUT,    /* P2.5 */
    IF_TEST,    /* P2.6 */
    MAX_IF
};


typedef struct {
    expander_identifiers id;    /* Pin ID (IF_xxx) */
    LPC1758_GPIO_Type type;     /* Pin type (P2_xxx) (from gpio.hpp) */
    bool reset_level;           /* False=low, True=high */
    bool direction;             /* False=input, True=output */
} pin_def;


/* Function prototypes */
void expander_init(int irq_pin, void (*irq_callback)(void));
void expander_reset(void);
void expander_set_address(int which);
void expander_write(int which, int state);
bool expander_read(int which);

#endif /* L5_APPLICATION_EXPANDER_HPP_ */
