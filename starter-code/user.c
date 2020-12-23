#include "user.h"
#include "emulator.h"
#include "qdbmp.h"
#include <math.h>
#include <time.h>
#define PI 3.141592653589793238462643383
#define timemaintain 0.4
float obsticlepositions[100][3]; //(x,y,0=point 1=Xbar  2=Ybar)
int obsticle_counter = 23;
float currentposition[1][2] = {}; //in format (x,y)
int threshold_for_colour_detection = 10;
bool isBlue = false;
int turns = 0; ////Magnetic Sensor
int magvalue, magvalue2;
int irvalue, irvalue2;
float noisetimeadjust = 0.0;
int num_of_turn = 0;
bool is_data_requested = false;
int TeamColour = 0;   //0 represent blue 1 represent red
extern uint64_t iter; //  This variable keeps track of how many times loop() was run.
                      //  Feel free to use it.

/***************************************************************************
 ToDO: 
    1. Setting the Speed of the motor 
 ***************************************************************************/

void writeposition(int distance, int angle, int message)
{
    if (message == 3) //Three means this is an update for current position
    {
        currentposition[0][0] = currentposition[0][0] + distance;
        currentposition[0][1] = currentposition[0][1] + distance;
    }
    else if ((message < 3) && (message >= 0))
    {
        if (angle != 0)
        {
            obsticlepositions[obsticle_counter][0] = currentposition[0][0] + distance;
            obsticlepositions[obsticle_counter][1] = currentposition[0][1] + distance;
        }

        else
        {
            obsticlepositions[obsticle_counter][0] = currentposition[0][0];
            obsticlepositions[obsticle_counter][1] = currentposition[0][1] * tan(angle)-currentposition[0][1];
        }
        obsticlepositions[obsticle_counter][2] = message;
        obsticle_counter++;
    }
}

void delay(int number_of_seconds)
{
    // Converting time into milli_seconds
    int milli_seconds = 1000 * number_of_seconds;

    // Storing start time
    clock_t start_time = clock();

    // looping till required time is not achieved
    while (clock() < start_time + milli_seconds)
        ;
}

int factorial(int k)
{
    if (k == 0)
    {
        return 1;
    }
    else
    {
        return k * factorial(k - 1);
    }
}

int binomialCoefficient(int n, int r)
{
    return factorial(n) / (factorial(r) * factorial(n - r));
}

void Speedcontrol(int Lspeed, int Rspeed) //Negative value = backward, Positive = forward
{
    emwrite(3, 100, 0, Lspeed); // Set wheel motor 0 voltage
    emwrite(3, 101, 0, Rspeed); // Set wheel motor 1 voltage
}

void Median_Filter()
{
}

void ObsticleLoading()
{
    obsticlepositions[0][0] = 1;
    obsticlepositions[0][1] = 1;
    obsticlepositions[0][2] = 1;

    obsticlepositions[1][0] = 2;
    obsticlepositions[1][1] = 1;
    obsticlepositions[1][2] = 1;

    obsticlepositions[2][0] = 3;
    obsticlepositions[2][1] = 1;
    obsticlepositions[2][2] = 2;

    obsticlepositions[3][0] = 3;
    obsticlepositions[3][1] = 2;
    obsticlepositions[3][2] = 2;

    obsticlepositions[4][0] = 3.3;
    obsticlepositions[4][1] = 5;
    obsticlepositions[4][2] = 1;

    obsticlepositions[5][0] = 4;
    obsticlepositions[5][1] = 5;
    obsticlepositions[5][2] = 1;

    obsticlepositions[6][0] = 5;
    obsticlepositions[6][1] = 5;
    obsticlepositions[6][2] = 1;

    obsticlepositions[7][0] = 6;
    obsticlepositions[7][1] = 2;
    obsticlepositions[7][2] = 2;

    obsticlepositions[8][0] = 6;
    obsticlepositions[8][1] = 3;
    obsticlepositions[8][2] = 2;

    obsticlepositions[9][0] = 6;
    obsticlepositions[9][1] = 4;
    obsticlepositions[9][2] = 2;

    obsticlepositions[10][0] = 6;
    obsticlepositions[10][1] = 5;
    obsticlepositions[10][2] = 2;

    obsticlepositions[11][0] = 6;
    obsticlepositions[11][1] = 6;
    obsticlepositions[10][2] = 2;

    obsticlepositions[12][0] = 0.1;
    obsticlepositions[12][1] = 3.3;
    obsticlepositions[12][2] = 0;

    obsticlepositions[13][0] = 1.8;
    obsticlepositions[13][1] = 5.4;
    obsticlepositions[13][2] = 0;

    obsticlepositions[14][0] = 1.0;
    obsticlepositions[14][1] = 7.8;
    obsticlepositions[14][2] = 0;

    obsticlepositions[15][0] = 3.5;
    obsticlepositions[15][1] = 8.3;
    obsticlepositions[15][2] = 0;

    obsticlepositions[16][0] = 5.0;
    obsticlepositions[16][1] = 10.0;
    obsticlepositions[16][2] = 0;

    obsticlepositions[17][0] = 6.5;
    obsticlepositions[17][1] = 8.5;
    obsticlepositions[17][2] = 0;

    obsticlepositions[18][0] = 8.0;
    obsticlepositions[18][1] = 10.0;
    obsticlepositions[18][2] = 0;

    obsticlepositions[19][0] = -1.0;
    obsticlepositions[19][1] = 9.7;
    obsticlepositions[19][2] = 1;

    obsticlepositions[20][0] = 0.0;
    obsticlepositions[20][1] = 9.7;
    obsticlepositions[20][2] = 1;

    obsticlepositions[21][0] = 1.0;
    obsticlepositions[21][1] = 9.7;
    obsticlepositions[21][2] = 2;

    obsticlepositions[22][0] = 1.0;
    obsticlepositions[22][1] = 10.0;
    obsticlepositions[22][2] = 11;

    currentposition[0][0] = 0;
    currentposition[0][1] = 0;

}

/***************************************************************************
 ToDO: 
   Grabbing feature for the balls
 ***************************************************************************/

void GrabandThrow(int ThrowOrGrab, int type)
{
    if (ThrowOrGrab == 10)
    {
        emwrite(ThrowOrGrab, type);
    }
    else if (ThrowOrGrab == 11)
    {
        emwrite(11); //Throwing
    }
}

/**************************************************************************
 *  ToDO:
 * 1. Magnetic Sensor Parameters 
 * 2. IR sensor Parameters
 * 
 *************************************************************************/
/***************************************************************************
 ToDO: 
    1. Need to modify the IR sensor detection
    2. Estimate the distance more correct
 ***************************************************************************/

/**
void MagneticSensors()
{
    int Mag1 = 6;
    int Mag2 = 7;
    if (emread_magnetic_sensor(&Mag1, &magvalue))
    {
        if (emread_magnetic_sensor(&Mag2, &magvalue2))
        {
            emdebug("%d %d\n", magvalue, magvalue2);
            if ((magvalue < 130) && (magvalue2 >= 130))
            { //check turn right?
                Speedcontrol(20, -20);
                if ((magvalue >= 130) && (magvalue2 < 130))
                {
                    Speedcontrol(-20, 20);
                }
            }
            else if ((magvalue >= 100) && (magvalue2 < 100))
            { //check trun left?
                Speedcontrol(-20, 20);
                if ((magvalue < 100) && (magvalue2 >= 100))
                {
                    Speedcontrol(-20, 20);
                }
            }
            else if ((magvalue >= 100) && (magvalue2 >= 100))
            { //check during turning
                if (magvalue > magvalue2)
                {
                    Speedcontrol(20, 0);
                    num_of_turn += 1;
                }
                else if (magvalue2 > magvalue)
                {
                    Speedcontrol(0, 20);
                    num_of_turn += 1;
                }
            }
            else
            {
                Speedcontrol(24, 24);
            }
            if (num_of_turn > 21 && turns == 0)
            { //after turning
                if (abs(magvalue - magvalue2) > 30)
                {
                    if (magvalue > magvalue2)
                    {
                        Speedcontrol(-24, 24);
                    }
                    else if (magvalue2 > magvalue)
                    {
                        Speedcontrol(24, -24);
                    }
                }
                else
                {
                    Speedcontrol(24, 24);
                }
            }
        }
    }
}
int num_of_true = 0;
void IRsensor()
{
    int IR1 = 15;
    int IR2 = 16;
    if (emread_ir_sensor(&IR1, &irvalue))
    {
        if (emread_ir_sensor(&IR2, &irvalue2))
        {
            emdebug("%d %d\n", irvalue, irvalue2);
            if (irvalue + irvalue2 == 4201474)
            {
                if (num_of_true < 45)
                {
                    Speedcontrol(24, 24);
                    num_of_true += 1;
                }
                else
                {
                    Speedcontrol(0, 0);
                    emwrite(10, 3);
                    turns += 1;
                    GrabandThrow(10, 3);
                    Speedcontrol(-20, 20);
                }
            }
        }
    }
}*/

/***************************************************************************
 ToDO: 
    1. Smooth path planning( Done )
    2. Rotation angle calculation(Done)
    3. Continuously Update the current position of the car(Done)
 ***************************************************************************/

void Rotate(float angle)
{
    int time = (int)angle / 360 - noisetimeadjust;
    Speedcontrol(24, 0);
    delay(time);
}

void moveTo(int x, int y)
{
    if (x < currentposition[0][0])
    {
        Rotate(PI);
        Speedcontrol(-24, -24);
        delay(timemaintain);
        Speedcontrol(0, 0);
        writeposition(timemaintain, 0, 3);
    }

    else if (x > currentposition[0][0])
    {
        Speedcontrol(24, 24);
        delay(timemaintain);
        Speedcontrol(0, 0);
        writeposition(timemaintain, 0, 3);
    }

    else if (currentposition[0][1] > y)
    {
        Rotate(PI / 2);
        Speedcontrol(24, 24);
        writeposition(timemaintain, 0, 3);
    }

    else if (currentposition[0][1] < y)
    {
        Rotate(-PI / 2);
        Speedcontrol(24, 24);
        writeposition(timemaintain, 0, 3);
    }

    else if (((x < currentposition[0][0]) && (currentposition[0][1] < y)) || ((x < currentposition[0][0]) && (currentposition[0][1] > y)) || ((x > currentposition[0][0]) && (currentposition[0][1] > y)) || ((x > currentposition[0][0]) && (currentposition[0][1] < y)))
    {
        moveTo(x, currentposition[0][1]);
        moveTo(x, y);
    }
}

void PathPlanning(int x, int y) //path planning
{
    double xu = 0.0, yu = 0.0, u = 0.0;
    for (u = 0.0; u <= 1.0; u += 0.1)
    {
        for (int i = 1; i <= obsticle_counter - 1; i++)
        {
            xu += binomialCoefficient(obsticle_counter, i) * pow(1 - u, i) * obsticlepositions[i][0];
            yu += binomialCoefficient(obsticle_counter, i) * pow(1 - u, i) * obsticlepositions[i][1];
        }

        xu = xu + x + currentposition[0][0];
        yu = yu + y + currentposition[0][1];

        moveTo(xu, yu);
    }
}

int color = 0;
void scan_color()
{
    BMP *bmp;
    emwrite(4, 5);
    if (iter == 2)
    {
        if (emread_camera(&bmp))
        {
            //BMP* bmp = emread_camera(&bmp);
            UCHAR r, g, b;
            int width = BMP_GetWidth(bmp);
            int height = BMP_GetHeight(bmp);
            for (int i = 20; i < height; i++)
            {
                for (int j = 20; j < width; j++)
                {
                    BMP_GetPixelRGB(bmp, i, j, &r, &g, &b);
                    //emdebug("%d %d %d\n", (int)r, (int)g, (int)b);
                    if (((int)r < 5) && ((int)g < 5) && ((int)b > 170))
                    {
                        color = 1; //blue
                    }
                    else if (((int)r > 170) && ((int)g < 5) && ((int)b > 5))
                    {
                        color = 2; //red
                    }
                }
            }
            emdebug("the color is %d", color);
            //BMP_WriteFile(bmp, "C:\\Users\\Nicole Lui\\Desktop\\HKUST\\RT\\RDC\\Emulator-Release-1.1.0\\Emulator-Release-1.1.0\\images\\out.bmp");
            //emdebug("%s\n",BMP_GetErrorDescription());
            BMP_Free(bmp); // Free bmp after usage.
        }
    }
}

void setup(void)
{
    //  Initialise your program here.
    //  This function will be called once before loop().
    //  You will probably want to start out by initialising some components.
    emwrite(1, 5); // Initialise the camera.
    //emwrite(2, 5, -10, -10, -80.15);  // Position the camera at an offset of (0, 20) cm and with no angular offset.
    emwrite(2, 5, 0, 0, 0);
    emwrite(1, 100); // Initialise the left motor.
    emwrite(1, 101); // right motor
    emwrite(1, 6); // mag sensor
    emwrite(2, 6, -15, -40);
    emwrite(1, 7); // mag sesor
    emwrite(2, 7, 15, -40);
    emwrite(1, 15); // IR
    emwrite(2, 15, -12, -40, 2*PI);
    emwrite(1, 16); // IR
    emwrite(2, 16, 12, -40, 2*PI);
    emwrite(1, 10); //line sensor
    emwrite(2, 10, -25, -40);
    emwrite(1, 11); //line sensor
    emwrite(2, 11, 25, -40);
    ObsticleLoading();
}

void loop(void)
{
    //  This function will be called continuously, and spaced out by approximately the same amount of time.
    //  Write your code in here.
    //  N.B. Your program will timeout if no emwrite() output is provided within ~500ms.
    scan_color();
    PathPlanning(4,0);

    emwrite(0); //  This will output a WAIT command -- which basically does nothing.
}
