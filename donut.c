#include "donut.h"

void hidecursor()
{
   HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
   CONSOLE_CURSOR_INFO info;
   info.dwSize = 100;
   info.bVisible = FALSE;
   SetConsoleCursorInfo(consoleHandle, &info);
}

int main(void){

  hideCursor();

  float A = 0;
  float B = 0;

  //Screen Size

  CONSOLE_SCREEN_BUFFER_INFO csbi;

  GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);

  int screen_width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
  int screen_height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;


  //Campo de vision
  const float K1 = 30;

  int paused = 0; 

  while(1){

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

    float zBuffer[screen_height*screen_width];
    memset(zBuffer,0, screen_height*screen_width*sizeof(float));

    char charBucket[screen_height*screen_width];
    memset(charBucket,' ', screen_height*screen_width*sizeof(char));

    for(float theta = 0; theta<=6.28; theta+=THETA_SPC){

        for(float phi = 0; phi <=6.28; phi+=PHI_SPC){

          //Pre-calculamos los senos/cosenos
          float cosA = cos(A);
          float cosB = cos(B);
          float sinA = sin(A);
          float sinB = sin(B);
          float cosPhi = cos(phi);
          float cosThe = cos(theta);
          float sinPhi = sin(phi);
          float sinThe = sin(theta);

          //Pre-calculamos x,y del circulo antes de rotar
          float circlex = R2 + R1*cosThe;
          float circley = R1*sin(theta);

          //Renderizar la dona
          float x = circlex*(cosB*cosPhi + sinA*sinB*sinPhi) - circley*cosA*sinB;
          float y = circlex*(cosPhi*sinB - cosB*sinA*sinPhi) + circley*cosA*cosB;
          float z = K2+cosA*circlex*sinPhi+circley*sinA;
          float z_1 = 1/z;

          //Proyeccion 2D

          int xP = (int)(screen_width/2 + K1*x*z_1);
          int yP = (int)(screen_height/2 - K1*y*z_1*0.5);

          //Aventarle luz

          float L = cosPhi*cosThe*sinB - cosA*cosThe*sinPhi-sinA*sinThe+cosB*(cosA*sinThe-cosThe*sinA*sinPhi);

          if (L<0)
            L = 0;

          int position = xP + yP * screen_width;

          //Asignar caracteres 
      
          if (position >= 0 && position < screen_width * screen_height) {
          if (z_1 > zBuffer[position]) {
              zBuffer[position] = z_1;
              int luminance_index = (int)(L * 8);
              luminance_index = luminance_index > 11 ? 11 : (luminance_index < 0 ? 0 : luminance_index);
              charBucket[position] = ".,-~:;=!*#$@"[luminance_index]; 
            }
          }
      }
    }

    //Imprimir

    printf("\x1b[H");
    for(int i=0; i<screen_height*screen_width;i++){
      putchar(i % screen_width ? charBucket[i]: '\n');
    }

    A+=0.05;
    B+=0.05;


  } 


}