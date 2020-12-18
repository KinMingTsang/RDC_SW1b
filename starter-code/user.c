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
float obsticlepositions[100][3]; //(x,y,0=point 1=bar)
int obsticle_counter = 0;
float ballposition[100][3];           //in format (x, y,0 = tennis 1 = table tennis)
float currentposition[1][2] = {[1, 1]}; //in format (x,y)
int ballcounter = 0;
int threshold_for_colour_detection = 10;

extern uint64_t iter; //  This variable keeps track of how many times loop() was run.
                      //  Feel free to use it.

/***************************************************************************
 ToDO: 
    1. Smooth path planning
    2. Rotation angle calculation
    3. Continuously Update the current position of the car
 ***************************************************************************/
void moveto(int x, int y) //path planning
{
}

/***************************************************************************
 ToDO: 
    1. Setting the Speed of the motor 
 ***************************************************************************/
void Speedcontrol(int Lspeed,int Rspeed)
{

}

/***************************************************************************
 ToDO: 
    1. Need to figure our  x y coordinate
    2. Need to evaluate the distance with the motor speed
 ***************************************************************************/
void writeposition(int store[], int distance, int message, int currentcounter)
{
    if (message == 3) //Three means this is an update for current position
    {
        store[0][0] = currentposition[0][0] + distance;
        store[0][1] = currentposition[0][1] + distance;
    }
    else
    {
        store[currentcounter][0] = currentposition[0][0]+distance;
        store[currentcounter][1] = currentposition[0][1]+distance;
        store[currentcounter][2] = message ;
        currentcounter++;
    }
}

/***************************************************************************
 ToDO: 
    1. Need to modify the IR sensor detection
    2. Estimate the distance more correct
 ***************************************************************************/

void obsticle_detection(int x, int y)
{
    for (int i = 0; i < sizeof(obsticlepositions); i++) //Check the current storage
    {
        if ((x == obsticlepositions[i][0] && (y == obsticleposition[0][1])))
        {
            moveto(x - 1, y);
            break;
        }
    }
    if (em_write(4, 15) ) // IR sensor detection
    {
        writeposition(obsticlepositions, 0.3, 0, obsticle_counter);
        obsticle_counter++;
        moveto(x, y);
    }
    else if(em_write(4, 16))
    {
        
    }
}
/***************************************************************************
 ToDO: 
   Grabbing feature for the balls
 ***************************************************************************/
void grabbing(bool grabcommand, int type)
{
    if (grabcommand)
    {
        emwrite(10, type);
        if (!grab_code(type)) //Regrabbing if the position is not correct.
        {
            moveto(currentposition[1] + 0.2);
            grabbing(grabcommand, type);
        }
    }
}


/************************************************************************************
 1. Determine whether it is a tennis ball or table tennis ball
 2. Store the position of the ball
 ************************************************************************************/

void indentify_size_of_ball(BMP *bmp)
{
    int red, green, blue;
    float avgred, avggreen, avgblue;

    for (int i = 0; i < BMP_GetHeight(bmp); i++)
    {
        for (int j = 0; j < BMP_GetWidth(bmp); j++)
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
            avgred = avgred / 9;
            avggreen = avggreen / 9;
            avgblue = avgblue / 9

            if (((avgred - 255) * (avgred - 255) + (avggreen - 201) * (avggreen - 201) + (avgblue - 92) * (avgblue - 92)) < 50) //detecting the colour difference between table tennis and current colour
            {
                writeposition(ballposition,1,1,ballcounter);
            }
            else if (((avgred - 190) * (avgred - 190) + (avggreen - 193) * (avggreen - 193) + (avgblue - 54) * (avgblue - 54)) < 50)
            {
                writeposition(ballposition,1,0,ballcounter);
                break;
            }
            avgblue  = 0;
            avggreen = 0;
            avgred   = 0;
        }
    }
}


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
