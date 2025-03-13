
/* Program Title: Drenok Onslaught Project
Program Description: Alien shooting game
Project Members: Áine, Maria, Hana*/

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
void menu (void);
void shoot(uint16_t x, uint16_t *targetx, uint16_t *targety, int *target_direction, uint16_t *score, uint16_t *lives, int *game_over, uint16_t *win, uint16_t *target_speed, uint16_t kara_lives);
void showScore(uint16_t score);
void alienMove(uint16_t *targetx, uint16_t *targety, int *target_direction, int *game_over, uint16_t *kara_lives, uint16_t *win, uint16_t *target_speed);
void showHearts(uint16_t kara_lives);

volatile uint32_t milliseconds;

const uint16_t alien[]= 
{
	65535,65535,65535,65535,65535,65535,65535,0,0,65535,65535,65535,65535,62893,65535,65535,5549,45188,55494,5549,45188,5549,45188,0,0,65535,5549,45188,62364,3171,5549,45188,45188,0,31975,65535,59986,47830,20347,0,0,65535,45188,59986,38317,65535,45188,59986,65535,65535,65535,65535,38317,65535,65535,0,0,65535,62364,38317,65535,62893,65535,65535,65535,23254,65535,59193,62893,65535,65535,0,0,62364,59193,45188,45188,20347,65535,65535,65535,27218,59986,63421,20347,65535,65535,0,0,65535,62893,20347,62893,65535,65535,5549,62364,3171,65535,65535,65535,65535,5549,0,0,65535,65535,65535,20347,65535,65535,45188,59986,62629,20347,0,0,0,0,59986,62629,20347,0,0,0,0,59986,20347,20347,65535,65535,0,0,0,0,20347,65535,65535,0,0,0,0,20347,65535,59986,62629,20347,0,0,0,0,59986,22461,65535,0,0,0,0,0,63950,20347,65535,65535,0,0,0,0,20347,65535,20347,0,0,0,0,59986,48887,59986,22461,65535,0,0,0,0,59986,22461,65535,0,0,0,0,20347,65535,20347,65535,20347,0,0,0,0,20347,65535,20347,0,0,0,0,0,64478
};
const uint16_t bullet[]=
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

const uint16_t heart[]=
{
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,40224,40224,0,40224,40224,0,0,0,0,0,0,0,0,40224,40224,40224,40224,40224,40224,40224,0,0,0,0,0,0,0,40224,40224,40224,40224,40224,40224,40224,0,0,0,0,0,0,0,0,40224,40224,40224,40224,40224,0,0,0,0,0,0,0,0,0,0,40224,40224,40224,0,0,0,0,0,0,0,0,0,0,0,0,40224,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
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
		uint16_t kara_lives = 2;
		uint16_t win = 2;//So that neither the "game over" or "you win" screen pop up when the game starts
		uint16_t target_speed = 1;

		menu();

		putImage(x,y,16,16,kara1,0,0);

		showHearts(kara_lives);//Calls showHearts when kara_lives=2
		
		//Main game code:
		while(game_over == 0)
		{
			showScore(score);
			alienMove(&targetx, &targety, &target_direction, &game_over, &kara_lives, &win, &target_speed);//Controls Alien Movement & losing the game

			//Buttons:
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
				shoot(x, &targetx, &targety, &target_direction, &score, &alien_lives, &game_over, &win, &target_speed, kara_lives);//Controls shooting & winning game
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

		//Winning and losing responses:
		fillRectangle(0,0,128,160,0);
		if(win == 1)
		{
			GPIOA->ODR |= (1<<1);//Turn on green light
			GPIOB->ODR |= (1<<1);//Turn on buzzer
			printTextX2("You Win",0,60,RGBToWord(0xff,0xff,0), 0);
			delay(2000);
			GPIOA->ODR &=~ (1<<1);//Turn off Green Light
			GPIOB->ODR &=~(1<<1);//Turn off Buzzer
		}
		else if(win == 0)
		{
			GPIOA->ODR |=(1<<0);//Turn on red light
			printTextX2("Game Over",0,60,RGBToWord(0xff,0xff,0), 0);
			delay(2000);
			GPIOA->ODR &=~(1<<0);//turn off red light
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
	pinMode(GPIOA,0,1);//red light(pink wire)
	pinMode(GPIOA,1,1);//green light(green wire)
	pinMode (GPIOB,1,1);//buzzer
}


void menu()
{
	fillRectangle(0,0,128,160,0);
	
	drawLine(1, 0, 127, 0, RGBToWord(0xff, 0xff, 0) );
	drawLine(127, 0, 127, 100, RGBToWord(0xff, 0xff, 0) );
	drawLine(127, 100, 1, 100, RGBToWord(0xff, 0xff, 0) );
	drawLine(1, 100, 1, 0, RGBToWord(0xff, 0xff, 0) );
	printTextX2("Drenok", 5, 10, RGBToWord(0xff, 0xff, 0), 0);
	printTextX2("Onslaught", 5, 30, RGBToWord(0xff, 0xff, 0), 0);
	printTextX2("Press down", 5, 60, RGBToWord(0xff, 0xff, 0), 0);
	printTextX2("for rules", 5, 80, RGBToWord(0xff, 0xff, 0), 0);
	printText("Shoot alien 5", 5, 120, RGBToWord(0xff, 0xff, 0), 0);
	printText("times before", 5, 130, RGBToWord(0xff, 0xff, 0), 0 );
	printText("it reaches you", 5, 140, RGBToWord(0xff, 0xff, 0), 0 );
	

	// Wait for the down button to be pressed
	while ((GPIOA->IDR & (1 << 11)) != 0)
	{
		// Wait for button press
	}

	// Clear the text before continuing
	fillRectangle(1, 0, 128, 160, 0);  

	// Wait a bit to debounce the button
	delay(300);

	printText("Up->Shoot", 5, 10, RGBToWord(0xff, 0xff, 0), 0);
	printText("Right->Move right", 5, 20, RGBToWord(0xff, 0xff, 0), 0);
	printText("Left->Move left", 5, 30, RGBToWord(0xff, 0xff, 0), 0);
	printTextX2("Press down", 5, 60, RGBToWord(0xff, 0xff, 0), 0);
	printTextX2("to start", 5, 80, RGBToWord(0xff, 0xff, 0), 0);

	// Wait for the down button to be pressed
	while ((GPIOA->IDR & (1 << 11)) != 0)
	{
		// Wait for button press
	}

	// Clear the text before continuing
	fillRectangle(5, 10, 200, 160, 0);  

	// Wait a bit to debounce the button
	delay(100);					
				
}


void shoot(uint16_t x, uint16_t *targetx, uint16_t *targety, int *target_direction, uint16_t *score, uint16_t *alien_lives, int *game_over, uint16_t *win, uint16_t *target_speed, uint16_t kara_lives)
{
	for(uint16_t y = 124 ; y > 0; y--) 
	{
		//Bullet momvement:
		putImage(x,y,12,16,bullet,0,0);
		delay(2);
		fillRectangle(x,y,12,16,0);

		//When bullet shoots alien
		if (isInside(*targetx,*targety,12,16,x,y))
		{
			fillRectangle(*targetx,*targety,16,13,0);
			*targetx = 0;
			*targety = 10;
			*target_direction = 1;
			*target_speed = *target_speed + 1;
			*score = *score + 1;
			*alien_lives = *alien_lives - 1;
		}

		if (isInside(100, 0, 12, 16, x, y ) || isInside(110, 0, 12, 16, x, y ))//If bullet is inside same coordinates as hearts
		{
			showHearts(kara_lives);//Redraw hearts to stop bullet erasing:
		}

		 if(*alien_lives==0)
		 {
			*win = 1;//win game
			*game_over = 1;
			showScore(*score);//Prints 5(last value) before program exits function and restarts:
		 }
	}
}


void showScore(uint16_t score)
{
	char score_text[32];
	snprintf(score_text, 32, "Score: %d", score);//formats text and stores it in a string (score_text).
	printText(score_text, 0, 0, RGBToWord(0xff, 0xff, 0), 0);
}


void alienMove(uint16_t *targetx, uint16_t *targety, int *target_direction, int *game_over, uint16_t *kara_lives, uint16_t *win, uint16_t *target_speed) 
{
	uint16_t step = 10;

	fillRectangle(*targetx,*targety,16,13,0);
	*targetx = (*targetx + ((step+*target_speed) * (*target_direction)));//Move alien by 10/-10 (depending on its direction)
	if ((*target_direction == 1 && *targetx > 90) || (*target_direction == -1 && *targetx < 20) )
	{
		*target_direction = -(*target_direction);//Change/Maintain target direction
		*targety = *targety + *target_speed + 13;
		if (*targety > 130) 
		{
			*targetx = 0;
			*targety = 10;
			*target_direction = 1;
			*kara_lives = *kara_lives - 1;
			showHearts(*kara_lives);//calls showHearts when kara_lives changes

			if (*kara_lives == 0)
			{
				*win = 0;//lose game
				*game_over = 1;
			}
		}
	}
	putImage(*targetx,*targety,16,13,alien,0,0);
}


void showHearts(uint16_t kara_lives)
{
	fillRectangle(100, 0, 28, 12, 0);

	for(int i = 0; i < kara_lives; i++)//kara_lives = 2 when called in main, kara_lives =1 and then 0 when called in alienMove
	{
		putImage(100 + i*10,0,14,12,heart,0,0);
	}
}
