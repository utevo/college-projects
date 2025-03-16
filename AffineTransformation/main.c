#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include <SDL/SDL.h>

#include "x86_function.h"

const char *fileName = "picture.bmp";

SDL_Surface *screen = NULL;
SDL_Surface *picture = NULL;

SDL_Surface *openPictureInDisplayFormat(const char *name) {
  SDL_Surface *picture = SDL_LoadBMP(name);
  SDL_Surface *newPicture = NULL;

  newPicture = SDL_DisplayFormat(picture);

  SDL_FreeSurface(picture);
  return newPicture;
}

void multipleMatrix(double *x, double *y) {
  double tmp[9];

  tmp[0] = x[0] * y[0] + x[1] * y[3] + x[2] * y[6];
  tmp[1] = x[0] * y[1] + x[1] * y[4] + x[2] * y[7];
  tmp[2] = x[0] * y[2] + x[1] * y[5] + x[2] * y[8];
  tmp[3] = x[3] * y[0] + x[4] * y[3] + x[5] * y[6];
  tmp[4] = x[3] * y[1] + x[4] * y[4] + x[5] * y[7];
  tmp[5] = x[3] * y[2] + x[4] * y[5] + x[5] * y[8];
  tmp[6] = x[6] * y[0] + x[7] * y[3] + x[8] * y[6];
  tmp[7] = x[6] * y[1] + x[7] * y[4] + x[8] * y[7];
  tmp[8] = x[6] * y[2] + x[7] * y[5] + x[8] * y[8];

  x[0] = tmp[0];
  x[1] = tmp[1];
  x[2] = tmp[2];
  x[3] = tmp[3];
  x[4] = tmp[4];
  x[5] = tmp[5];
  x[6] = tmp[6];
  x[7] = tmp[7];
  x[8] = tmp[8];
}

int main(int argc, char *argv[]) {
  SDL_Init(SDL_INIT_EVERYTHING);

  picture = SDL_LoadBMP(fileName);
  size_t width = picture->w;
  size_t height = picture->h;

  screen = SDL_SetVideoMode(width, height, 32, SDL_SWSURFACE);

  picture = openPictureInDisplayFormat(fileName);

  printf("width: %ld\nheight: %ld\n", width, height);

  SDL_Event event;
  int quit = 0;

  double angle = 0;
  double translationX = 0;
  double translationY = 0;
  double scalar = 1;

  while (!quit) {
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_KEYDOWN:

          switch (event.key.keysym.sym) {
            case SDLK_q:
              angle += 0.2;
              break;
            case SDLK_e:
              angle -= 0.2;
              break;

            case SDLK_z:
              scalar += 0.1 * scalar;
              break;

            case SDLK_c:
              scalar -= 0.1 * scalar;
              break;

            case SDLK_x:
              scalar = 1.0;
              break;

            case SDLK_w:
              translationY -= 50;
              break;

            case SDLK_s:
              translationY += 50;
              break;

            case SDLK_a:
              translationX += 50;
              break;

            case SDLK_d:
              translationX -= 50;
              break;

            default:
              break;
          }
          break;

        case SDL_QUIT:
          quit = 1;
          break;

        default:
          break;
      }
    }

    double matrix[] = 
    {
    	1, 0, 0, 
    	0, 1, 0, 
    	0, 0, 1
    };

    double sinValue = sin(angle);
    double cosValue = cos(angle);

    double rotationMatrix[] = 
    {
    	cosValue, sinValue, 0, 
    	-sinValue, cosValue, 0,
    	0, 0, 1
    };

    double translationMatrix[] = 
    {
    	1, 0, translationX, 
    	0, 1, translationY, 
    	0, 0, 1
    };

    double scalingMatrix[] = 
    {
    	scalar, 0, 0, 
    	0, scalar, 0, 
    	0, 0, 1
    };

    multipleMatrix(matrix, scalingMatrix);
    multipleMatrix(matrix, rotationMatrix);
    multipleMatrix(matrix, translationMatrix);

    x86_function(screen->pixels, picture->pixels, width, height, matrix);

    SDL_Flip(screen);
    SDL_Delay(10);
  }

  SDL_FreeSurface(picture);
  SDL_Quit();

  return 0;
}