#include <stdint.h>

#include <murax.h>

/* MAP PMOD-LED to GPIO */
/* PMOD3 */
//uint8_t led_map[8] = {7, 6, 5, 4, 0, 1, 2, 3};

/* PMOD2 */
uint8_t led_map[8] = {15, 14, 13, 12, 8, 9, 10, 11};

/* LED_x to gpio */
uint32_t make_led(uint8_t l)
{
    uint32_t g = 0;
    uint32_t i;

    for(i = 0; i < 8; i++) {
        if (l & (0x1 << i)) {
            g |= (0x1 << led_map[i]);
        }
    }

    return g;
}

void uart_putc(char ch)
{
    uart_write(UART, ch);
}

void uart_puts(char *s)
{
    uint32_t i;
    for(i = 0; s[i] != '\0'; i++) {
        uart_putc(s[i]);
    }
}

static char sys_banner[] = {"MuraxSoC system buildtime [" __TIME__" " __DATE__ "] " "rev 1.0\r\n"};

void main() {
	volatile uint32_t a = 1, b = 2, c = 3;
	uint32_t result = 0;

	interruptCtrl_init(TIMER_INTERRUPT);
	prescaler_init(TIMER_PRESCALER);
	timer_init(TIMER_A);

	TIMER_PRESCALER->LIMIT = 12000-1; //1 ms rate

	//TIMER_A->LIMIT = 1000-1;  //1 second rate
	TIMER_A->LIMIT = 300-1;  //1 second rate
	TIMER_A->CLEARS_TICKS = 0x00010002;

	TIMER_INTERRUPT->PENDINGS = 0xF;
	TIMER_INTERRUPT->MASKS = 0x1;

	GPIO_A->OUTPUT_ENABLE = 0x000000FF;
	GPIO_A->OUTPUT = 0x00000000;

	UART->STATUS = 2; //Enable RX interrupts

	//UART->DATA = 'B';

    uart_puts("## MuraxSoC ##\r\n");
    uart_puts(sys_banner);
    uart_puts("Hello, iCESugar!\r\n");

	while(1){
#if 0
		result += a;
		result += b + c;
		for(uint32_t idx = 0;idx < 50000;idx++) asm volatile("");
		GPIO_A->OUTPUT = (GPIO_A->OUTPUT & ~0x3F) | ((GPIO_A->OUTPUT + 1) & 0x3F);  //Counter on LED[5:0]
#endif
		//GPIO_A->OUTPUT ^= 0xFF;
	}
}

void irqCallback(){
    static uint32_t x = 0, g = 0;
	if(TIMER_INTERRUPT->PENDINGS & 1){  //Timer A interrupt
		//GPIO_A->OUTPUT ^= 0x80; //Toogle led 7
		//GPIO_A->OUTPUT = ~(make_led(0x1 << ((x++) % 8)));
		//GPIO_A->OUTPUT = (0x1 << ((x++) % 8));

        //g = ~(0x1 << LED_0_OFF);
		//GPIO_A->OUTPUT = ~(0x1 << LED_0_OFF);
		//GPIO_A->OUTPUT = 0x7F;

        g = ~(make_led(0x1 << ((x++) % 8)));
        //g = ~(make_led(0x1 << 0));

		GPIO_A->OUTPUT = g;

		TIMER_INTERRUPT->PENDINGS = 1;
	}
	while(UART->STATUS & (1 << 9)){ //UART RX interrupt
		UART->DATA = (UART->DATA) & 0xFF;
	}
}



