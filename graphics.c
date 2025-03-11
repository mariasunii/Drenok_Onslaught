
#include <stm32f031x6.h>
#include <stdio.h>
#include "display.h"
void initClock(void);
void initSysTick(void);
void SysTick_Handler(void);
void delay(volatile uint32_t dly);
void setupIO();
int isInside(uint16_t x1, uint16_t y1, uint16_t w, uint16_t h, uint16_t px, uint16_t py);
void enablePullUp(GPIO_TypeDef *Port, uint32_t BitNumber);
void pinMode(GPIO_TypeDef *Port, uint32_t BitNumber, uint32_t Mode);
void shoot(uint16_t x, uint16_t *targetx, uint16_t *targety, int *target_direction, uint16_t *score, uint16_t *lives, int *game_over);
void alienMove(uint16_t *targetx, uint16_t *targety, int *target_direction, int *game_over);

volatile uint32_t milliseconds;

const uint16_t deco3[]= 
{
	65535,65535,65535,65535,65535,65535,65535,0,0,65535,65535,65535,65535,62893,65535,65535,5549,45188,55494,5549,45188,5549,45188,0,0,65535,5549,45188,62364,3171,5549,45188,45188,0,31975,65535,59986,47830,20347,0,0,65535,45188,59986,38317,65535,45188,59986,65535,65535,65535,65535,38317,65535,65535,0,0,65535,62364,38317,65535,62893,65535,65535,65535,23254,65535,59193,62893,65535,65535,0,0,62364,59193,45188,45188,20347,65535,65535,65535,27218,59986,63421,20347,65535,65535,0,0,65535,62893,20347,62893,65535,65535,5549,62364,3171,65535,65535,65535,65535,5549,0,0,65535,65535,65535,20347,65535,65535,45188,59986,62629,20347,0,0,0,0,59986,62629,20347,0,0,0,0,59986,20347,20347,65535,65535,0,0,0,0,20347,65535,65535,0,0,0,0,20347,65535,59986,62629,20347,0,0,0,0,59986,22461,65535,0,0,0,0,0,63950,20347,65535,65535,0,0,0,0,20347,65535,20347,0,0,0,0,59986,48887,59986,22461,65535,0,0,0,0,59986,22461,65535,0,0,0,0,20347,65535,20347,65535,20347,0,0,0,0,20347,65535,20347,0,0,0,0,0,64478
};
const uint16_t dg1[]=
{
	0,0,16142,16142,16142,16142,16142,16142,16142,16142,0,0,0,0,0,16142,16142,16142,16142,16142,16142,0,0,0,0,0,16142,16142,16142,16142,16142,16142,16142,16142,0,0,0,0,16142,16142,16142,1994,1994,16142,16142,16142,0,0,0,0,16142,16142,16142,1994,16142,1994,16142,16142,0,0,0,0,16142,16142,16142,1994,16142,1994,16142,16142,0,0,0,0,16142,16142,16142,1994,16142,1994,16142,16142,0,0,0,0,16142,16142,16142,1994,1994,16142,16142,16142,0,0,0,0,16142,16142,16142,16142,16142,16142,16142,16142,0,0,0,0,16142,16142,16142,1994,1994,1994,16142,16142,0,0,0,0,16142,16142,16142,1994,16142,16142,16142,16142,0,0,0,0,16142,16142,16142,1994,16142,16142,16142,16142,0,0,0,0,16142,16142,16142,1994,16142,1994,16142,16142,0,0,0,0,16142,16142,16142,1994,1994,1994,16142,16142,0,0,0,0,0,16142,16142,16142,16142,16142,16142,0,0,0,0,0,0,16142,16142,16142,16142,16142,16142,0,0,0,
};

const uint16_t kara1[]=
{
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4368,6168,6168,6168,4368,4368,6168,6168,0,0,0,0,0,0,0,4368,6168,6168,6168,6168,6168,6168,6168,6168,7712,0,0,0,0,0,0,4368,6168,6168,7712,7712,7712,7712,7712,7712,5912,0,0,0,0,0,0,4368,4368,4368,6168,6168,6168,6168,6168,6168,4368,0,0,0,0,0,0,4368,4368,6168,6168,6168,6168,6168,38748,6168,6168,0,0,0,0,0,0,4368,4368,6168,6168,6168,6168,38748,38748,38748,6168,0,0,0,0,0,4368,4368,4368,6168,38748,63454,17705,38748,63454,17705,6168,0,0,0,0,0,4368,4368,4368,4368,38748,56303,2108,38748,56303,2108,0,0,0,0,0,0,4368,4368,4368,0,38251,38748,38748,38748,38748,38251,0,0,0,0,0,0,0,4368,0,0,0,17705,62011,62011,17705,0,0,0,0,0,0,0,0,0,0,0,38748,17705,17705,17705,17705,38748,0,0,0,0,0,0,0,0,0,38748,38748,4368,4368,4368,4368,38748,38748,0,0,0,0,0,0,0,0,0,0,17705,17705,17705,17705,0,0,0,0,0,0,0,0,0,0,0,0,18233,0,0,18233,0,0,0,0,0,0,0,0,0,0,0,0,5400,0,0,5400,0,0,0,0,0,0
};

const uint16_t kara2[]=
{
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4368,6168,6168,6168,4368,4368,6168,6168,0,0,0,0,0,0,0,4368,6168,6168,6168,6168,6168,6168,6168,6168,7712,0,0,0,0,0,0,4368,6168,6168,7712,7712,7712,7712,7712,7712,5912,0,0,0,0,0,0,4368,4368,4368,6168,6168,6168,6168,6168,6168,4368,0,0,0,0,0,0,4368,4368,6168,6168,6168,6168,6168,38748,6168,6168,0,0,0,0,0,0,4368,4368,6168,6168,6168,6168,38748,38748,38748,6168,0,0,0,0,0,4368,4368,4368,6168,38748,63454,17705,38748,63454,17705,6168,0,0,0,0,0,4368,4368,4368,4368,38748,56303,2108,38748,56303,2108,0,0,0,0,0,0,4368,4368,4368,0,38251,38748,38748,38748,38748,38251,0,0,0,0,0,0,0,4368,0,0,0,17705,62011,62011,17705,0,0,0,0,0,0,0,0,0,0,0,38748,17705,17705,17705,17705,38748,0,0,0,0,0,0,0,0,0,38748,38748,4368,4368,4368,4368,38748,38748,0,0,0,0,0,0,0,0,0,0,17705,17705,17705,17705,0,0,0,0,0,0,0,0,0,0,0,18233,18233,0,0,18233,0,0,0,0,0,0,0,0,0,0,0,5400,0,0,0,5400,0,0,0,0,0,0
};

int main()
{
	initClock();
	initSysTick();
	setupIO();

	while(1) 
	{
		int hinverted = 0;
		int vinverted = 0;
		int toggle = 0;
		int hmoved = 0;
		int vmoved = 0;
		uint16_t x = 50;
		uint16_t y = 140;
		uint16_t oldx = x;
		uint16_t oldy = y;
		uint16_t targetx = 0;
		uint16_t targety = 10;
		int target_direction = 1;
		int game_over = 0;
		uint16_t score = 0; 
		uint16_t alien_lives = 5;

		fillRectangle(0,0,200,200,0);
		printTextX2("Drenok", 5, 10, RGBToWord(0xff, 0xff, 0), 0);
		printTextX2("Onslaught", 5, 30, RGBToWord(0xff, 0xff, 0), 0);
		printTextX2("Press down", 5, 60, RGBToWord(0xff, 0xff, 0), 0);
		printTextX2("to start", 5, 80, RGBToWord(0xff, 0xff, 0), 0);
		printText("Shoot alien 5", 5, 100, RGBToWord(0xff, 0xff, 0), 0);
		printText("times before", 5, 110, RGBToWord(0xff, 0xff, 0), 0 );
		printText("it reaches you", 5, 120, RGBToWord(0xff, 0xff, 0), 0 );
		

		// Wait for the down button to be pressed
		while ((GPIOA->IDR & (1 << 11)) != 0)
		{
			// Wait for button press
		}

		// Clear the text before continuing
		fillRectangle(5, 10, 200, 160, 0);  // Clear text area by drawing a black rectangle

		// Wait a bit to debounce the button
		delay(100);
		putImage(x,y,16,16,kara1,0,0);
		while(game_over == 0)
		{
			char score_text[32];
			snprintf(score_text, 32, "Score: %d", score);
			printText(score_text, 0, 0, RGBToWord(0xff, 0xff, 0), 0);
			alienMove(&targetx, &targety, &target_direction, &game_over);

			hmoved = vmoved = 0;
			hinverted = vinverted = 0;
			if ((GPIOB->IDR & (1 << 4))==0) // right pressed
			{					
				if (x < 110)
				{
					x = x + 1;
					hmoved = 1;
					hinverted=0;
				}						
			}
			if ((GPIOB->IDR & (1 << 5))==0) // left pressed
			{			
				if (x > 10)
				{
					x = x - 1;
					hmoved = 1;
					hinverted=1;
				}			
			}
			if ( (GPIOA->IDR & (1 << 8)) == 0) // up pressed
			{			
				shoot(x, &targetx, &targety, &target_direction, &score, &alien_lives, &game_over);
			}
			if (hmoved)
			{
				// only redraw if there has been some movement (reduces flicker)
				fillRectangle(oldx,oldy,12,16,0);
				oldx = x;
				oldy = y;					
				if (hmoved)
				{
					if (toggle)
						putImage(x,y,16,16,kara1,hinverted,0);
					else
						putImage(x,y,16,16,kara2,hinverted,0);
					toggle = toggle ^ 1;
				}
			}		
			delay(50);
		}
	}	
	return 0;
}

void initSysTick(void)
{
	SysTick->LOAD = 48000;
	SysTick->CTRL = 7;
	SysTick->VAL = 10;
	__asm(" cpsie i "); // enable interrupts
}

void SysTick_Handler(void)
{
	milliseconds++;
}

void initClock(void)
{
// This is potentially a dangerous function as it could
// result in a system with an invalid clock signal - result: a stuck system
        // Set the PLL up
        // First ensure PLL is disabled
        RCC->CR &= ~(1u<<24);
        while( (RCC->CR & (1 <<25))); // wait for PLL ready to be cleared
        
// Warning here: if system clock is greater than 24MHz then wait-state(s) need to be
// inserted into Flash memory interface
				
        FLASH->ACR |= (1 << 0);
        FLASH->ACR &=~((1u << 2) | (1u<<1));
        // Turn on FLASH prefetch buffer
        FLASH->ACR |= (1 << 4);
        // set PLL multiplier to 12 (yielding 48MHz)
        RCC->CFGR &= ~((1u<<21) | (1u<<20) | (1u<<19) | (1u<<18));
        RCC->CFGR |= ((1<<21) | (1<<19) ); 

        // Need to limit ADC clock to below 14MHz so will change ADC prescaler to 4
        RCC->CFGR |= (1<<14);

        // and turn the PLL back on again
        RCC->CR |= (1<<24);        
        // set PLL as system clock source 
        RCC->CFGR |= (1<<1);
}

void delay(volatile uint32_t dly)
{
	uint32_t end_time = dly + milliseconds;
	while(milliseconds != end_time)
		__asm(" wfi "); // sleep
}

void enablePullUp(GPIO_TypeDef *Port, uint32_t BitNumber)
{
	Port->PUPDR = Port->PUPDR &~(3u << BitNumber*2); // clear pull-up resistor bits
	Port->PUPDR = Port->PUPDR | (1u << BitNumber*2); // set pull-up bit
}

void pinMode(GPIO_TypeDef *Port, uint32_t BitNumber, uint32_t Mode)
{
	/*
	*/
	uint32_t mode_value = Port->MODER;
	Mode = Mode << (2 * BitNumber);
	mode_value = mode_value & ~(3u << (BitNumber * 2));
	mode_value = mode_value | Mode;
	Port->MODER = mode_value;
}

int isInside(uint16_t x1, uint16_t y1, uint16_t w, uint16_t h, uint16_t px, uint16_t py)
{
	// checks to see if point px,py is within the rectange defined by x,y,w,h
	uint16_t x2,y2;
	x2 = x1+w;
	y2 = y1+h;
	int rvalue = 0;
	if ( (px >= x1) && (px <= x2))
	{
		// ok, x constraint met
		if ( (py >= y1) && (py <= y2))
			rvalue = 1;
	}
	return rvalue;
}

void setupIO()
{
	RCC->AHBENR |= (1 << 18) + (1 << 17); // enable Ports A and B
	display_begin();
	pinMode(GPIOB,4,0);
	pinMode(GPIOB,5,0);
	pinMode(GPIOA,8,0);
	pinMode(GPIOA,11,0);
	enablePullUp(GPIOB,4);
	enablePullUp(GPIOB,5);
	enablePullUp(GPIOA,11);
	enablePullUp(GPIOA,8);
}

void shoot(uint16_t x, uint16_t *targetx, uint16_t *targety, int *target_direction, uint16_t *score, uint16_t *alien_lives, int *game_over)
{
	for(uint16_t y = 124 ; y > 0; y--) 
	{
		putImage(x,y,12,16,dg1,0,0);
		delay(2);
		fillRectangle(x,y,12,16,0);
		if (isInside(*targetx,*targety,12,16,x,y))
		{
			fillRectangle(*targetx,*targety,16,13,0);
			*targetx = 0;
			*targety = 10;
			*target_direction = 1;
			*score = *score + 1;
			*alien_lives = *alien_lives - 1;
		}
		 if(*alien_lives==0)
		 {
			printTextX2("Game Over",0,60,RGBToWord(0xff,0xff,0), 0);
			delay(950);
			*game_over = 1;
		 }
	}
}

void alienMove(uint16_t *targetx, uint16_t *targety, int *target_direction, int *game_over) 
{
	uint16_t step = 10;

	fillRectangle(*targetx,*targety,16,13,0);
	*targetx = *targetx + (step * (*target_direction));//Move alien by 10/-10 (depending on its direction)
	if ((*target_direction == 1 && *targetx > 90) || (*target_direction == -1 && *targetx < 20) )
	{
		*target_direction = -(*target_direction);//Change/Maintain target direction
		*targety = *targety + 13;
		if (*targety > 130) 
		{
			printTextX2("Game Over",0,60,RGBToWord(0xff,0xff,0), 0);
			delay(950);
			*game_over = 1;
		}
	}
	putImage(*targetx,*targety,16,13,deco3,0,0);
}
