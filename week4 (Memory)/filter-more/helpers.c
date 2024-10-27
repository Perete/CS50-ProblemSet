#include "helpers.h"
#include <stdio.h>
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // printf("Photo Size: %d x %d (h x w)\n", height, width);
    for(int i=0; i<height; i++){
        for(int j=0; j<width; j++){
            int avrg = (int)round((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed)/3.0);
            image[i][j].rgbtBlue = avrg;
            image[i][j].rgbtGreen = avrg;
            image[i][j].rgbtRed = avrg;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    int halfWidth = (width-1)/2;
    for(int i=0; i< height; i++){
        for(int j=0; j<=halfWidth; j++){
            RGBTRIPLE temp = image[i][j];
            image[i][j] = image[i][width-j-1];
            image[i][width-j-1] = temp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    int avgRed, avgGreen, avgBlue, n;
    RGBTRIPLE imageCopy[height][width];

    for(int i=0; i< height; i++){
        for(int j=0; j< width; j++){

            avgRed = 0;
            avgGreen = 0;
            avgBlue = 0;
            n=0;

            for(int k=-1; k<2; k++){
                for(int m=-1; m<2; m++){
                    if((i+k)>=0 && (i+k)<height && (j+m)>=0 && (j+m)<width){
                        avgRed =  avgRed + image[i+k][j+m].rgbtRed;
                        avgGreen = avgGreen + image[i+k][j+m].rgbtGreen;
                        avgBlue = avgBlue + image[i+k][j+m].rgbtBlue;
                        n++;
                    }
                }
            }

            imageCopy[i][j].rgbtRed = (int) round(avgRed/(float)n);
            imageCopy[i][j].rgbtGreen = (int) round(avgGreen/(float)n);
            imageCopy[i][j].rgbtBlue = (int) round(avgBlue/(float)n);

        }
    }

    for(int i=0; i< height; i++){
        for(int j=0; j< width; j++){
            image[i][j] = imageCopy[i][j];
        }
    }

    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    typedef struct
    {
        int sumGx;
        int sumGy;
    }__attribute__((__packed__))
    CONVOLUTIONSUM;

    int Gx[3][3] = {{-1, 0, 1},
                    {-2, 0, 2},
                    {-1, 0, 1}};

    int Gy[3][3] = {{-1,-2,-1},
                    { 0, 0, 0},
                    {1, 2, 1}};
    int n;
    CONVOLUTIONSUM redPixel, greenPixel, bluePixel;
    RGBTRIPLE imageCopy[height][width];

    for(int i=0; i< height; i++){
        for(int j=0; j< width; j++){

            redPixel.sumGx = 0;
            redPixel.sumGy = 0;
            greenPixel.sumGx = 0;
            greenPixel.sumGy = 0;
            bluePixel.sumGx = 0;
            bluePixel.sumGy = 0;

            for(int k=-1; k<2; k++){
                for(int m=-1; m<2; m++){
                    if((i+k)>=0 && (i+k)<height && (j+m)>=0 && (j+m)<width){
                        redPixel.sumGx =  redPixel.sumGx + image[i+k][j+m].rgbtRed*Gx[k+1][m+1];
                        redPixel.sumGy =  redPixel.sumGy + image[i+k][j+m].rgbtRed*Gy[k+1][m+1];

                        greenPixel.sumGx =  greenPixel.sumGx + image[i+k][j+m].rgbtGreen*Gx[k+1][m+1];
                        greenPixel.sumGy =  greenPixel.sumGy + image[i+k][j+m].rgbtGreen*Gy[k+1][m+1];

                        bluePixel.sumGx =  bluePixel.sumGx + image[i+k][j+m].rgbtBlue*Gx[k+1][m+1];
                        bluePixel.sumGy =  bluePixel.sumGy + image[i+k][j+m].rgbtBlue*Gy[k+1][m+1];
                    }
                }
            }

            int redMagnitude = (int)round(sqrt(pow(redPixel.sumGx,2) + pow(redPixel.sumGy,2)));
            int greenMagnitude = (int)round(sqrt(pow(greenPixel.sumGx,2) + pow(greenPixel.sumGy,2)));
            int blueMagnitude = (int)round(sqrt(pow(bluePixel.sumGx,2) + pow(bluePixel.sumGy,2)));

            imageCopy[i][j].rgbtRed = redMagnitude > 255 ? 255: redMagnitude;
            imageCopy[i][j].rgbtGreen = greenMagnitude > 255 ? 255: greenMagnitude;
            imageCopy[i][j].rgbtBlue = blueMagnitude > 255 ? 255: blueMagnitude;

        }
    }

    for(int i=0; i< height; i++){
        for(int j=0; j< width; j++){
            image[i][j] = imageCopy[i][j];
        }
    }

    return;
}
