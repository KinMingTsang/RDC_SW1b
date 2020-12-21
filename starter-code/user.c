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
#include<math.h> 
float obsticlepositions[100][3]; //(x,y,0=point 1=bar)
int obsticle_counter = 0;
float currentposition[1][2] = {[1, 1]}; //in format (x,y)
int threshold_for_colour_detection = 10;
extern uint64_t iter; //  This variable keeps track of how many times loop() was run.
                      //  Feel free to use it.

/***************************************************************************
 ToDO: 
    1. Smooth path planning( Done )
    2. Rotation angle calculation(Doing)
    3. Continuously Update the current position of the car
 ***************************************************************************/
void PathPlanning(int x, int y) //path planning
{
    double xu = 0.0, yu = 0.0, u = 0.0 ;
    for (u = 0.0; u <= 1.0; u += 0.0001)
    {
        for (i == 0; i_ < obsticle_counter-1; i++)
        {
            xu += pow(1 - u, i+1) * obsticlepositions[i][0];
            yu += pow(1 - u, i+1) * obsticlepositions[i][1];
        }
        xu = x + pow(1 - u, obsticle_counter) * currentposition[0][0];
        yu =y + pow(1 - u, obsticle_counter) * currentposition[0][1];
        move(xu,yu);
    }
}

void move(int x, int y)//Adjust Angel
{
    if((x<currentposition[0][0])&&(currentposition[0][1]==y))
    {
        Rotate(1.5*M_PI);
        Speedcontrol(24,24);
    }
    else if((x>currentposition[0][0])&&(currentposition[0][1]==y))
    {
        Rotate(M_PI/2);
        Speedcontrol(24,24);
    }
    else if((x==currentposition[0][0])&&(currentposition[0][1]>y))
    {
        Rotate(M_PI);
        Speedcontrol(24,24);
    }
    else if((x==currentposition[0][0])&&(currentposition[0][1]<y))
    {
        Rotate(M_PI/2);
        Speedcontrol(24,24);
    }
    else if((x<currentposition[0][0])&&(currentposition[0][1]<y))
    {
        Rotate(M_PI/2);
        Speedcontrol(24,24);
    }
    else if((x<currentposition[0][0])&&(currentposition[0][1]>y))
    {
        Rotate(M_PI/2);
        Speedcontrol(24,24);
    }
    else if((x>currentposition[0][0])&&(currentposition[0][1]>y))
    {
        Rotate(M_PI/2);
        Speedcontrol(24,24);
    }
    else if((x>currentposition[0][0])&&(currentposition[0][1]<y))
    {
        Rotate(M_PI/2);
        Speedcontrol(24,24);
    }
}


void Rotate(float angle)
{
    config(0,0,angle);
}

/***************************************************************************
 ToDO: 
    1. Setting the Speed of the motor 
 ***************************************************************************/
void Speedcontrol(int Lspeed, int Rspeed)
{
        emwrite(3, 20, 0, Lspeed.0); // Set wheel motor 0 voltage 
        emwrite(3, 21, 0, Rspeed.0); // Set wheel motor 1 voltage
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
        store[currentcounter][0] = currentposition[0][0] + distance;
        store[currentcounter][1] = currentposition[0][1] + distance;
        store[currentcounter][2] = message;
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
        if ((x == obsticlepositions[i][0] && (y == obsticleposition[0][i])))
        {
            Speedcontrol()
            break;
        }
    }

    if (em_write(4, 15)) // IR sensor detection
    {
        writeposition(obsticlepositions, 0.3, 0, obsticle_counter);
        obsticle_counter++;
        moveto(x, y);
    }

    else if (em_write(4, 16))
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
 1. Determine whether it is on the track or not 
 2. Reset the angle  or turning speed
 ************************************************************************************/

void indentify_path(BMP *bmp)
{
    int red, green, blue;
    float avgred, avggreen, avgblue;

    for (int i = BMP_GetHeight(bmp)/2; i < BMP_GetHeight(bmp)+50; i++)
    {
        for (int j = BMP_GetWidth(bmp) / 2; j++)
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
        }
    }
    for (int i = 0; i < BMP_GetHeight(bmp); i++)
    {
        for (int j = BMP_GetWidth(bmp) / 2; j++)
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
        }
    }

    avgred = avgred / 100;
    avggreen = avggreen / 100;
    avgblue = avgblue / 100;

    if (2916 - ((avgred - 190) * (avgred - 190) + (avggreen - 193) * (avggreen - 193) + (avgblue - 54) * (avgblue - 54)) < 50) //detecting the colour difference between the route and current colour
    {
        Speedcontrol(24),24;
    }
}

void MageneticSensors()
{

}

void setup(void)
{
    //  Initialise your program here.
    //  This function will be called once before loop().

    //  You will probably want to start out by initialising some components.
    emwrite(1, 100); //  Initialises the LEFT wheel motor.
    emwrite(1, 101); //  Initialises the RIGHT wheel motor.
    emread_int(5);
    emread_int(15);
    emread_int(16);
    emread_int(17);
    emread_int(18);
    emread_int(19);
    emwrite(2, 1, 0.5);
    emwrite(2, 5, 0, 20, 0.0);
    //  N.B. You can only initialise and configure components within this function.
    //       (Of course, you can still call another function which does the init.)
}

void loop(void)
{
    //  This function will be called continuously, and spaced out by approximately the same amount of time.
    //  Write your code in here.
    //  N.B. Your program will timeout if no emwrite() output is provided within ~500ms.

    emwrite(0); //  This will output a WAIT command -- which basically does nothing.
}
