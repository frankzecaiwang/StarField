#ifndef STAR_MOTION_H
#define STAR_MOTION_H

#include "math.h"
#define ERUPTION      1
#define FALLING       2
#define ORIG_WIDTH    15
#define ORIG_LENGTH   15
#define MAX_STEP      30
#define MIN_STEP      25
#define INVALID_STEP  (-1)
#define VISIO_LEN     200
#define MAX_STAR      120
#define TIMER_LEN     60  //60ms
#define FIRST_INIT    0
#define NON_FIRST_INIT    1

int g_init_width  = 0;
int g_init_height  = 0;

int g_current_width  = 0;
int g_current_height  = 0;

int random(int min, int max)
{
	//return the random value between min and max, which includes min and not max
	
	if(max>min)
	{
		return rand()%(max-min)+min;
	}
	else 
	{
		return rand()%(min-max)+max;	
	}	
}

struct STAR_STRUCT
{
	//int pos_x;
	//int pos_y;
	//int width;
	//int length;
	
	int m_dir;
	
	int current_step;
	int min_step;
	
	int color_r;
	int color_g;
	int color_b;
	
	int distance;
	
	int rotation_anglar; //start from 0
	
	STAR_STRUCT()
	{
		m_dir = FALLING;	
	}
	
	void set_dir(int dir)
	{
		m_dir = dir;	
	}
	
	
	void get_current_xy(int *x, int *y, int *size_width, int *size_height)
	{
		//double dRadius = sqrt(pos_x*pos_x + pos_y*pos_y);
		
		int step = 0;
		
		step = current_step>=0? current_step:0;
		
		*x = int(distance * cos(rotation_anglar/180.0*M_PI) * step * 1.0 / MAX_STEP);
		*y = int(distance * sin(rotation_anglar/180.0*M_PI) * step * 1.0 / MAX_STEP);
		
		*size_width = int(ORIG_LENGTH * step * 1.0 / MAX_STEP);
		*size_height = int(ORIG_WIDTH * step * 1.0 / MAX_STEP);	
	}
	
	void step_on(void)
	{
		
		if(m_dir == ERUPTION)
		{
			if(current_step >= MAX_STEP-1 || current_step == -1)
			{
				initialization(NON_FIRST_INIT);		
			}	
			else
			{
				current_step ++;		
				//rotation_anglar += 2;		
			}
			
		}
		else
		{
			if(current_step <= min_step)
			{
				initialization(1);		
			}	
			else
			{
				current_step --;		
				//rotation_anglar -= 2;		
			}			
		}
	}
	
	void initialization(int init_time)
	{
		int pos_x;
		int pos_y;
		for(int i=0; i<10; i++)
		{
			pos_x = (rand() % g_init_width)- g_init_width/2;	
			pos_y = (rand() % g_init_height)- g_init_height/2;
					
			distance = 	int(sqrt(pos_x*pos_x + pos_y*pos_y));
			
			if(distance>120)
			{
				rotation_anglar	= int(asin(pos_y*1.0/distance)/M_PI*180.0);
				
				if(pos_x<0 && pos_y>0 ) rotation_anglar += 180;
				if(pos_x<0 && pos_y<0 ) rotation_anglar -= 180;
				
				//rotation_anglar = (rand() % (3))*120 + (rand()%60);
				
				break;
			}
			
		};
		
	   									
		color_r = (rand() % 256);
		color_g = (rand() % 256);
		color_b = (rand() % 256);
		
		if(init_time == FIRST_INIT)
		{
			if(m_dir == ERUPTION) current_step = random(0, MAX_STEP);	
			if(m_dir == FALLING) 
			{
				min_step = 0;
				current_step = random(0,MAX_STEP);	
			}	
		}
		else
		{
			if(m_dir == ERUPTION) current_step = random(0, MAX_STEP);	
			if(m_dir == FALLING) 
			{
				min_step = random(0,MAX_STEP);
				current_step = MAX_STEP;	
			}
		}
						
	}	
	
};

#endif
