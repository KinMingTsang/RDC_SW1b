/**
 * user.c
 * 
 * This is where you'll write your code.
 * 
 * You may create files and include them here 
 * and whatnot.
 */

#include "user.h"
#include "emulator.h"
#include "qdbmp.h"
char obsticlepositions[100]; //(x,y,type)
int obsticle_counter;
char ballposition[100]; //in format (x, y , type)
char currentposition[1] = {"(0, 0)"};
bool tennis;
int ballcounter;
bool tabletennis;
int threshold_for_colour_detection = 10;

extern uint64_t iter; //  This variable keeps track of how many times loop() was run.
                      //  Feel free to use it.

void setup(void)
{
    //  Initialise your program here.
    //  This function will be called once before loop().

    //  You will probably want to start out by initialising some components.
    emwrite(1, 100); //  Initialises the left wheel motor.
    emread_int(5);
    emread_int(15);
    emread_int(16);
    emread_int(17);
    emread_int(18);
    emread_int(19);
    emwrite(2, 1, 0.5);
    //  N.B. You can only initialise and configure components within this function.
    //       (Of course, you can still call another function which does the init.)
}

/************************************************************************************
 1. Determine whether it is a tennis ball or table tennis ball
 2. Store the position of the ball
 ************************************************************************************/

void indentify_size_of_ball(BMP *bmp)
{
    int red, green, blue, avgred, avggreen, avgblue;

    for (int i = 0; i < BMP_GetHeight(camera.jpg); i++)
    {
        for (int j = 0; j < BMP_GetWidth(camera.jpg); j++)
        {
            for (a = 0; a < 3; a++)
            {
                for (b = 0; b <= 3; b++)
                {
                    BMP_GetPixelRGB(bmp, i + a, j + b, red, green, blue);
                    avggreen += green;
                    avgred += red;
                    avgblue += blue;
                }
            }

            ball_type_detection(avgred / 9, avggreen / 9, avgblue / 9);
            if (tennis)
            {
                ballposition[ballcounter] = currenposition[0] + 1;
                ballcounter++;
            }
            else if (tabletennis)
            {
                ballposition[ballcounter] == currentposition[0] + 1;
            }
        }
    }
}

void ball_type_detection(float red, float green, float blue)
{
    float error_between_tabletennis = (red - 190) * (red - 190) + (green - 193) * (green - 193) + (blue - 54) * (blue - 54);
    float error_between_tennis = (red - 255) * (red - 255) + (green - 201) * (green - 201) + (blue - 92) * (blue - 92);
    float error_between_tabletennis2 = (red - 247) * (red - 247) + (green - 245) * (green - 245) + (blue - 250) * (blue - 250);
    //Sample error : ~~59
    if ((error_between_tabletennis <= threshold_for_colour_detection * threshold_for_colour_detection) && (error_between_tabletennis > 65))
    {
        tabletennis = true;
        tennis = false;
    }
    else if ((error_between_tennis <= threshold_for_colour_detection * threshold_for_colour_detection) && (error_between_tennis > 65))
    {
        tennis = true;
        tabletennis = false;
    }
    else if ((error_between_tabletennis2 <= threshold_for_colour_detection * threshold_for_colour_detection) && (error_between_tabletennis2 > 65))
    {
        tabletennis = true;
        tennis = false;
    }
}

void moveto(int x, int y) //path planning
{
}

void readposition(int stored[],int index)
{
}

void writeposition(int store[], int distance, char message, int currentcounter)
{
    store[i] = (currentposition[0] + distance, message);
}

void obsticle_detection(int x,int y)
{
    for (int i = 0; i < sizeof(obsticlepositions); i++)//Check the current storage
    {
        if ((i == readpositions(,)) && (read == readposition(,)))
        {
            moveto(x,y);
        }
    }
    if (em_write(4, 15) || em_write(4, 16)) // IR sensor detection
    {
        writeposition(obsticlepositions, obsticle_counter);
        obsticle_counter++;
        moveto(x,y);
    }
}

void grabbing(bool grabcommand, int type)
{
    if (grabcommand)
    {
        emwrite(10, type);
        if (!grab_code(type))
        {
            moveto(currentposition[1] + 0.2);
            grabbing(grabcommand, type);
        }
    }
}

void loop(void)
{
    //  This function will be called continuously, and spaced out by approximately the same amount of time.
    //  Write your code in here.
    //  N.B. Your program will timeout if no emwrite() output is provided within ~500ms.
    /*************
     1. Grab the ball
     2. Identify the size of ball
     *******************/

    emwrite(13, 1, 0.5);
    if (emred_camera(camera.jpg))
    {
        indentify_size_of_ball(camera.jpg);
        if (tennis)
        {
            grabbing(true, 1);
        }
        else if (tabletennis)
        {
            if (grab(2))
            {
                grabbing(true, 2);
            }
        }
    }

    emwrite(0); //  This will output a WAIT command -- which basically does nothing.
}
