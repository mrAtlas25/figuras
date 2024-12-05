#include "cube.h"

void hideCursor()
{
   HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
   CONSOLE_CURSOR_INFO info;
   info.dwSize = 100;
   info.bVisible = FALSE;
   SetConsoleCursorInfo(consoleHandle, &info);
}

float A = 0;
float B = 0;
float C = 0;


float zBuffer[SCREEN_HEIGHT*SCREEN_WIDTH];
char charBucket[SCREEN_HEIGHT*SCREEN_WIDTH];



void rotarCara(float i, float j, float k, char ch){

    float sinA=sin(A);
    float cosA=cos(A);
    float sinB=sin(B);
    float cosB=cos(B);
    float sinC=sin(C);
    float cosC=cos(C);

    float x = j*sinA*sinB*cosC - k*cosA*sinB*cosC + j*cosA*sinC + k*sinA*sinC + i*cosB*cosC;
    float y = j*cosA*cosC+k*sinA*cosC-j*sinA*sinB*sinC+k*cosA*sinB*sinC-i*cosB*sinC;
    float z = k*cosA*cosB-j*sinA*cosB+i*sinB+K2;

    float z_1 = 1/z;

    int xP = (int)(SCREEN_WIDTH/2 + K1*x*z_1*2);
    int yP = (int)(SCREEN_HEIGHT/2 + K1*y*z_1);

    int position = xP + yP * SCREEN_WIDTH;

    if (position >= 0 && position < SCREEN_WIDTH * SCREEN_HEIGHT){

        if (z_1 > zBuffer[position]) {
            zBuffer[position] = z_1;
            charBucket[position] = ch; 
        }
    }


}

int main(void){

    
    hideCursor();

    int paused = 0;

    while(1){

        memset(zBuffer,0, SCREEN_HEIGHT*SCREEN_WIDTH*sizeof(float));
        memset(charBucket,' ', SCREEN_HEIGHT*SCREEN_WIDTH*sizeof(char));

        if (_kbhit()) {
                char ch = _getch();
                if (ch == ' ')
                    paused = !paused;
                
                else if(ch == '\n' || ch == '\r')
                    break;
            }

        if (paused) {
                Sleep(100); 
                continue;
            }


        for(float i = -CUBE_WIDTH; i < CUBE_WIDTH; i+=RESOLUTION){

            for(float j = -CUBE_WIDTH; j < CUBE_WIDTH; j+=RESOLUTION){

                rotarCara(i, j, -CUBE_WIDTH, '.');
                rotarCara(i, j, CUBE_WIDTH, '~');
                rotarCara(i, CUBE_WIDTH, j, '!');
                rotarCara(i, -CUBE_WIDTH, j, '#');
                rotarCara(CUBE_WIDTH, i, j, '*');
                rotarCara(-CUBE_WIDTH, i, j, '@');
            }
        }

        printf("\x1b[H");

        for(int i=0; i<SCREEN_HEIGHT*SCREEN_WIDTH;i++){
            putchar(i % SCREEN_WIDTH ? charBucket[i]: '\n' );
        }

        A+=0.05;
        B+=0.05;
        C+=0.01; 

    }



}

