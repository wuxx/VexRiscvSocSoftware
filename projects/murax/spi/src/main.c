#include <stdint.h>

#include <murax.h>

/* SPI */
void uart_putc(char ch);

#define ILI9341_TFTWIDTH  240
#define ILI9341_TFTHEIGHT 320

#define ILI9341_NOP     0x00
#define ILI9341_SWRESET 0x01
#define ILI9341_RDDID   0x04
#define ILI9341_RDDST   0x09

#define ILI9341_SLPIN   0x10
#define ILI9341_SLPOUT  0x11
#define ILI9341_PTLON   0x12
#define ILI9341_NORON   0x13

#define ILI9341_RDMODE  0x0A
#define ILI9341_RDMADCTL  0x0B
#define ILI9341_RDPIXFMT  0x0C
#define ILI9341_RDIMGFMT  0x0D
#define ILI9341_RDSELFDIAG  0x0F

#define ILI9341_INVOFF  0x20
#define ILI9341_INVON   0x21
#define ILI9341_GAMMASET 0x26
#define ILI9341_DISPOFF 0x28
#define ILI9341_DISPON  0x29

#define ILI9341_CASET   0x2A
#define ILI9341_PASET   0x2B
#define ILI9341_RAMWR   0x2C
#define ILI9341_RAMRD   0x2E

#define ILI9341_PTLAR   0x30
#define ILI9341_MADCTL  0x36
#define ILI9341_PIXFMT  0x3A

#define ILI9341_FRMCTR1 0xB1
#define ILI9341_FRMCTR2 0xB2
#define ILI9341_FRMCTR3 0xB3
#define ILI9341_INVCTR  0xB4
#define ILI9341_DFUNCTR 0xB6

#define ILI9341_PWCTR1  0xC0
#define ILI9341_PWCTR2  0xC1
#define ILI9341_PWCTR3  0xC2
#define ILI9341_PWCTR4  0xC3
#define ILI9341_PWCTR5  0xC4
#define ILI9341_VMCTR1  0xC5
#define ILI9341_VMCTR2  0xC7

#define ILI9341_RDID1   0xDA
#define ILI9341_RDID2   0xDB
#define ILI9341_RDID3   0xDC
#define ILI9341_RDID4   0xDD

#define ILI9341_GMCTRP1 0xE0
#define ILI9341_GMCTRN1 0xE1
/*
#define ILI9341_PWCTR6  0xFC

*/

// Color definitions
#define ILI9341_BLACK       0x0000      /*   0,   0,   0 */
#define ILI9341_NAVY        0x000F      /*   0,   0, 128 */
#define ILI9341_DARKGREEN   0x03E0      /*   0, 128,   0 */
#define ILI9341_DARKCYAN    0x03EF      /*   0, 128, 128 */
#define ILI9341_MAROON      0x7800      /* 128,   0,   0 */
#define ILI9341_PURPLE      0x780F      /* 128,   0, 128 */
#define ILI9341_OLIVE       0x7BE0      /* 128, 128,   0 */
#define ILI9341_LIGHTGREY   0xC618      /* 192, 192, 192 */
#define ILI9341_DARKGREY    0x7BEF      /* 128, 128, 128 */
#define ILI9341_BLUE        0x001F      /*   0,   0, 255 */
#define ILI9341_GREEN       0x07E0      /*   0, 255,   0 */
#define ILI9341_CYAN        0x07FF      /*   0, 255, 255 */
#define ILI9341_RED         0xF800      /* 255,   0,   0 */
#define ILI9341_MAGENTA     0xF81F      /* 255,   0, 255 */
#define ILI9341_YELLOW      0xFFE0      /* 255, 255,   0 */
#define ILI9341_WHITE       0xFFFF      /* 255, 255, 255 */
#define ILI9341_ORANGE      0xFD20      /* 255, 165,   0 */
#define ILI9341_GREENYELLOW 0xAFE5      /* 173, 255,  47 */
#define ILI9341_PINK        0xF81F

#define GPIO_TFT_DC		(0xA0)

#define GPIO_SPI_CS		(0xA1)
#define GPIO_SPI_CLK	(0xA2)
#define GPIO_SPI_MOSI	(0xA3)
#define GPIO_SPI_MISO	(0xA4)

void delay(int x)
{
	int i, j;
	for(i = 0; i < x; i++) {
	    for(j = 0; j < 100; j++) {
            uart_putc('d');
        }
    }
}

#define _OUTPUT  1
#define _INPUT   0

#define HIGH  1
#define LOW   0

void pinMode(uint32_t gpio, uint32_t mode)
{
    /* TODO: */
    return;
}

uint32_t digitalRead(uint32_t gpio)
{
    /* TODO: */
    return 0;
}

void digitalWrite(uint32_t gpio, uint32_t b)
{
    uint32_t offset = gpio & 0xF;

    if (b) {
        GPIO_A->OUTPUT |= (0x1 << offset);
    } else {
        GPIO_A->OUTPUT &= (~(0x1 << offset));
    }

}

int spi_init()
{
    pinMode(GPIO_TFT_DC,  _OUTPUT);
  
	pinMode(GPIO_SPI_CS,  _OUTPUT);
	pinMode(GPIO_SPI_CLK, _OUTPUT);
	pinMode(GPIO_SPI_MOSI, _OUTPUT);
	pinMode(GPIO_SPI_MISO, _INPUT);
	
	return 0;
}
uint8_t spi_read()
{
	  uint8_t r = 0;

    for (uint8_t i=0; i<8; i++) {
      digitalWrite(GPIO_SPI_CLK, LOW);
      digitalWrite(GPIO_SPI_CLK, HIGH);
      r <<= 1;
	  
      if (digitalRead(GPIO_SPI_MISO)) {
		r |= 0x1;	  
	  }	
    }

	return r;
}

uint8_t spi_write(uint8_t c)
{
	for(uint8_t bit = 0x80; bit; bit >>= 1) {
		if(c & bit) {
			digitalWrite(GPIO_SPI_MOSI, HIGH); 
		} else {
			digitalWrite(GPIO_SPI_MOSI, LOW); 
		}
		digitalWrite(GPIO_SPI_CLK, HIGH);
		digitalWrite(GPIO_SPI_CLK, LOW);
	}

	return 0;
}

void tft_write_cmd(uint8_t c)
{

  digitalWrite(GPIO_TFT_DC, LOW);
  digitalWrite(GPIO_SPI_CLK, LOW);
  digitalWrite(GPIO_SPI_CS, LOW);

  spi_write(c);

  digitalWrite(GPIO_SPI_CS, HIGH);

}

void tft_write_data(uint8_t c)
{
  digitalWrite(GPIO_TFT_DC, HIGH);
  digitalWrite(GPIO_SPI_CS, LOW);

  spi_write(c);

  digitalWrite(GPIO_SPI_CS, HIGH);
}

void tft_set_addr_window(uint16_t x0, uint16_t y0, uint16_t x1,
 uint16_t y1) {

  digitalWrite(GPIO_TFT_DC, LOW);
  
  spi_write(ILI9341_CASET); // Column addr set

  digitalWrite(GPIO_TFT_DC, HIGH);

  spi_write(x0 >> 8);
  spi_write(x0 & 0xFF);     // XSTART 
  spi_write(x1 >> 8);
  spi_write(x1 & 0xFF);     // XEND

  digitalWrite(GPIO_TFT_DC, LOW);
  spi_write(ILI9341_PASET); // Row addr set
  digitalWrite(GPIO_TFT_DC, HIGH);
    
  spi_write(y0>>8);
  spi_write(y0);     // YSTART
  spi_write(y1>>8);
  spi_write(y1);     // YEND

  digitalWrite(GPIO_TFT_DC, LOW);
  spi_write(ILI9341_RAMWR); // write to RAM
  digitalWrite(GPIO_TFT_DC, HIGH);
}

#if 1
uint16_t _width  = ILI9341_TFTWIDTH;
uint16_t _height = ILI9341_TFTHEIGHT;
#else
uint16_t _width  = ILI9341_TFTHEIGHT;
uint16_t _height = ILI9341_TFTWIDTH;
#endif

void tft_draw_pixel(int16_t x, int16_t y, uint16_t color)
{
    if((x < 0) || (x >= _width) || (y < 0) || (y >= _height)) {
        return;
    }

	digitalWrite(GPIO_SPI_CS, LOW);
	tft_set_addr_window(x, y, x + 1, y + 1);
	
	spi_write(color >> 8);
    spi_write(color);
	
	digitalWrite(GPIO_SPI_CS, HIGH);
}

void tft_fill_rect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{

  // rudimentary clipping (drawChar w/big text requires this)
  if((x >= _width) || (y >= _height)) return;
  if((x + w - 1) >= _width)  w = _width  - x;
  if((y + h - 1) >= _height) h = _height - y;
  
  digitalWrite(GPIO_SPI_CS, LOW);
  
  tft_set_addr_window(x, y, x+w-1, y+h-1);

  uint8_t hi = color >> 8, lo = color;

  for(y=h; y>0; y--) {
    for(x=w; x>0; x--) {
      spi_write(hi);
      spi_write(lo);
    }
  }

  digitalWrite(GPIO_SPI_CS, HIGH);
  
}

int tft_fill_screen(uint16_t color)
{
	tft_fill_rect(0, 0,  240, 320, color);
}

#if 1
int tft_init()
{
  spi_init();
	
  tft_write_cmd(0xEF);
  tft_write_data(0x03);
  tft_write_data(0x80);
  tft_write_data(0x02);

  tft_write_cmd(0xCF);  
  tft_write_data(0x00); 
  tft_write_data(0XC1); 
  tft_write_data(0X30); 

  tft_write_cmd(0xED);  
  tft_write_data(0x64); 
  tft_write_data(0x03); 
  tft_write_data(0X12); 
  tft_write_data(0X81); 
 
  tft_write_cmd(0xE8);  
  tft_write_data(0x85); 
  tft_write_data(0x00); 
  tft_write_data(0x78); 

  tft_write_cmd(0xCB);  
  tft_write_data(0x39); 
  tft_write_data(0x2C); 
  tft_write_data(0x00); 
  tft_write_data(0x34); 
  tft_write_data(0x02); 
 
  tft_write_cmd(0xF7);  
  tft_write_data(0x20); 

  tft_write_cmd(0xEA);  
  tft_write_data(0x00); 
  tft_write_data(0x00); 
 
  tft_write_cmd(ILI9341_PWCTR1);    //Power control 
  tft_write_data(0x23);   //VRH[5:0] 
 
  tft_write_cmd(ILI9341_PWCTR2);    //Power control 
  tft_write_data(0x10);   //SAP[2:0];BT[3:0] 
 
  tft_write_cmd(ILI9341_VMCTR1);    //VCM control 
  tft_write_data(0x3e); //¶Ô±È¶Èµ÷½Ú
  tft_write_data(0x28); 
  
  tft_write_cmd(ILI9341_VMCTR2);    //VCM control2 
  tft_write_data(0x86);  //--
 
  tft_write_cmd(ILI9341_MADCTL);    // Memory Access Control 
  tft_write_data(0x48);

  tft_write_cmd(ILI9341_PIXFMT);    
  tft_write_data(0x55); 
  
  tft_write_cmd(ILI9341_FRMCTR1);    
  tft_write_data(0x00);  
  tft_write_data(0x18); 
 
  tft_write_cmd(ILI9341_DFUNCTR);    // Display Function Control 
  tft_write_data(0x08); 
  tft_write_data(0x82);
  tft_write_data(0x27);  
 
  tft_write_cmd(0xF2);    // 3Gamma Function Disable 
  tft_write_data(0x00); 
 
  tft_write_cmd(ILI9341_GAMMASET);    //Gamma curve selected 
  tft_write_data(0x01); 
 
  tft_write_cmd(ILI9341_GMCTRP1);    //Set Gamma 
  tft_write_data(0x0F); 
  tft_write_data(0x31); 
  tft_write_data(0x2B); 
  tft_write_data(0x0C); 
  tft_write_data(0x0E); 
  tft_write_data(0x08); 
  tft_write_data(0x4E); 
  tft_write_data(0xF1); 
  tft_write_data(0x37); 
  tft_write_data(0x07); 
  tft_write_data(0x10); 
  tft_write_data(0x03); 
  tft_write_data(0x0E); 
  tft_write_data(0x09); 
  tft_write_data(0x00); 
  
  tft_write_cmd(ILI9341_GMCTRN1);    //Set Gamma 
  tft_write_data(0x00); 
  tft_write_data(0x0E); 
  tft_write_data(0x14); 
  tft_write_data(0x03); 
  tft_write_data(0x11); 
  tft_write_data(0x07); 
  tft_write_data(0x31); 
  tft_write_data(0xC1); 
  tft_write_data(0x48); 
  tft_write_data(0x08); 
  tft_write_data(0x0F); 
  tft_write_data(0x0C); 
  tft_write_data(0x31); 
  tft_write_data(0x36); 
  tft_write_data(0x0F); 

  tft_write_cmd(ILI9341_SLPOUT);    //Exit Sleep 

  delay(120);
  
  tft_write_cmd(ILI9341_DISPON);    //Display on 
  
}
#endif

#if 0
char sys_banner[] = {"mb system buildtime [" __TIME__ " " __DATE__ "] " "rev 1.0"};

void setup() {
  delay(1000);
  
  LOG.begin(LOG_UART_BAUDRATE);
  
  //SPI.setFrequency(ESP_SPI_FREQ);

  LOG_INFO("%s\r\n", sys_banner);
  LOG_INFO("reset-reason: %s\r\n", ESP.getResetReason().c_str());
  LOG_INFO("reset-info:   %s\r\n", ESP.getResetInfo().c_str());

  tft_init();

}

void loop() {
#if 0
  while(1) {
      tft.fillScreen(ILI9341_NAVY);
      delay(100);
      tft.fillScreen(ILI9341_BLUE);
      delay(100);
      tft.fillScreen(ILI9341_RED);
      delay(100);
    }
#endif
	while (1) {
    ESP.wdtFeed();
    LOG_ERR("tft_fill_screen start\r\n");
		tft_fill_screen(ILI9341_BLUE);
    LOG_ERR("tft_fill_screen end\r\n");
#if 0    
		tft_draw_pixel(10, 10, ILI9341_BLUE);
		tft_draw_pixel(20, 20, ILI9341_GREEN);
		tft_draw_pixel(40, 40, ILI9341_RED);
    tft_draw_pixel(100, 200, ILI9341_PINK);
    tft_draw_pixel(240, 300, ILI9341_BLACK);
#endif    
		delay(100);
	}
}
#endif

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

	TIMER_A->LIMIT = 1000-1;  //1 second rate
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

    tft_init();

	while(1) {
#if 0
		result += a;
		result += b + c;
		for(uint32_t idx = 0;idx < 50000;idx++) asm volatile("");
		GPIO_A->OUTPUT = (GPIO_A->OUTPUT & ~0x3F) | ((GPIO_A->OUTPUT + 1) & 0x3F);  //Counter on LED[5:0]
#endif
		//GPIO_A->OUTPUT ^= 0xFF;

        uart_puts("tft_fill_screen start\r\n");
        tft_fill_screen(ILI9341_BLUE);
        uart_puts("tft_fill_screen end\r\n");
	}
}

void irqCallback(){
	if(TIMER_INTERRUPT->PENDINGS & 1){  //Timer A interrupt
		//GPIO_A->OUTPUT ^= 0x80; //Toogle led 7
		TIMER_INTERRUPT->PENDINGS = 1;
	}
	while(UART->STATUS & (1 << 9)){ //UART RX interrupt
		UART->DATA = (UART->DATA) & 0xFF;
	}
}



