#include <math.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

//Declaração de variáveis
float A, B, C;
float cubeWidth = 20;
int width = 160, height = 44;
float zBuffer[160 * 44];
char buffer[160 * 44];
int backgroundASCIICode = ' ';
float incrementSpeed = 0.6;
float x, y, z;
int distancefromCam = 100;
float ooz;
float xp, yp; // x and y prime
float K1 = 40;
int idx;

float calculateX(int i, int j, int k) {
    return j * sin(A) * sin(B) * cos(C) - 
    k * cos(A) * sin(A) * cos(C) + 
    j * cos(A) * sin(C) +
    k * sin(A) * sin(C) +
    i * cos(B) * cos(C); 
}

float calculateY(int i, int j, int k) {
    return j * cos(A) * cos(C) +
    k * sin(A) * cos(C) -
    j * sin(A) * sin(B) * sin(C) -
    i * cos(B) * sin(C); 
}

float calculateZ(int i, int j, int k) {
    return k * cos(A) * cos(B) -
    j * sin(A) * cos(B) +
    i * sin(B); 
}

void calculateForSurface(float cubeX, float cubeY, float cubeZ, int ch) {
    x = calculateX(cubeX, cubeY, cubeZ);
    y = calculateY(cubeX, cubeY, cubeZ);
    z = calculateZ(cubeX, cubeY, cubeZ) + distancefromCam;

    ooz = 1 / z;
    xp = (int)(width / 2 - 2 * cubeWidth + K1 * x * ooz * 2);
    yp = (int)(height / 2 + K1 * y * ooz * 2);


    idx = xp + yp * width;
    if(idx >= 0 && idx < width * height) {
        if(ooz > zBuffer[idx]) {
            zBuffer[idx] = ooz;
            buffer[idx] = ch;
        }
    }
}

int main() { 
    printf("\x1b[2J"); // Clear the screen
    while(1) {
        memset(buffer, backgroundASCIICode, width * height * 4); // Clear the buffer
        memset(zBuffer, 0, width * height * 4); // Clear the zBuffer

        for(float cubeX = -cubeWidth; cubeX < cubeWidth; cubeX += incrementSpeed) { // Draw the cube
            for(float cubeY = -cubeWidth; cubeY < cubeWidth; cubeY += incrementSpeed) { // Draw the cube
                calculateForSurface(cubeX, cubeY, -cubeWidth, '.'); // Draw the cube
                calculateForSurface(cubeWidth, cubeY, cubeX, '$'); // Draw the cube
                calculateForSurface(-cubeWidth, cubeY, -cubeX, '~'); // Draw the cube
                calculateForSurface(-cubeX, cubeY, cubeWidth, '#'); // Draw the cube
                calculateForSurface(cubeX, -cubeWidth, -cubeY, ';'); // Draw the cube
                calculateForSurface(cubeX, cubeWidth, cubeY, '+'); // Draw the cube
            }
        }
        printf("\x1b[H"); // Move cursor to the home position
        for(int k = 0; k < height; k++) { // Draw the buffer
            putchar(k % width ? buffer[k] : 10); // Draw the buffer
        }
        A += 0.05; // Rotate the cube
        B += 0.05; // Rotate the cube
        usleep(1000); // Sleep for 1000 milliseconds
    }
    return 0;
}