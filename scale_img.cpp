#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_image.h"
#include "stdio.h"
#include <stdlib.h>

#include <SDL/SDL_rotozoom.h>

int main(int argc, char* argv[])
{
    if (argc == 1) 
    {   
        printf("Usage : scale_img input_file <width> <heigth> <preview> output_file\n");
        printf("      : input_file for image in png, bmp, jpg\n");
        printf("      : width for scaling\n");
        printf("      : heigth for scaling\n");
        printf("      : preview image yes or no, q exit\n");
        printf("      : input_file name  for image.\n");
        printf("      : if no name is given it will be\n");
        printf("        <name>_<width>x<height>.bmp\n");
        printf("scale_img 1942.png 320 224 0 out.png\n");
        printf("scale_img 1942.png 320 224 1 out.png\n");
        printf("scale_img 1942.png 320 224 0\n");
        return 0;        
    }
    

    long w, h, preview;
    char text[255] = "\0";
    char in[255] = "\0";
    char out[255] = "\0";
    char buf[1024];
    char png_buf[1024];
    char *ptr;
    char delimiter[] = ".";

    strcpy(text, argv[1]);
    w = strtol(argv[2], NULL, 10);
    h = strtol(argv[3], NULL, 10);
    preview = strtol(argv[4], NULL, 10);
    if (argc != 5) 
        strcpy(out, argv[5]);


    printf("scale image %s to %i x %i\n", text, w, h);
    SDL_Surface *screen = NULL;
    SDL_Surface *image = NULL;

    if (argc == 5)
    {
        // build name for output file
        strcpy(in, text);

        ptr  = strtok(in, delimiter);
        snprintf(buf, sizeof(buf), "%s_%ix%i.bmp", ptr, w, h);  
        snprintf(png_buf, sizeof(buf), "%s_%ix%i.png", ptr, w, h);  
    }
    else
    {
        snprintf(buf, sizeof(buf), "%s.bmp", out);  
        snprintf(png_buf, sizeof(buf), "%s", out);  
    }

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL_Init failed - %s\n", SDL_GetError());
        return 1;
    }

    SDL_ShowCursor(SDL_DISABLE);



    image = IMG_Load(text);
    if (!image)
    {
        fprintf(stderr, "IMG_Load failed - %s\n", IMG_GetError());
        SDL_Quit();
        return 1;
    }

   

    float x = w / (float)image->w;
    float y = h / (float)image->h ;
//    printf("image->w=%f, image->h=%f\n", (float)image->w, (float)image->h);
//    printf("x=%f, y=%f\n", x, y);
    SDL_Surface* scaled_image = rotozoomSurfaceXY(image, 0.0, x, y, 1);

    SDL_SaveBMP(scaled_image, buf);

    char convert_buf[1024];
    char rm_buf[1024];

    snprintf(convert_buf, sizeof(convert_buf), "convert %s %s", buf, png_buf);
    snprintf(rm_buf, sizeof(rm_buf), "rm %s", buf);

    system(convert_buf);
    system(rm_buf);
    printf ("image %s was written\n", png_buf);  
    if (preview == 1) 
    { 

        screen = SDL_SetVideoMode(320, 240, 24, SDL_FULLSCREEN);

        if (!screen)
        {
            printf("SetVideoMode failed - %s\n", SDL_GetError());
            SDL_FreeSurface(screen);
            SDL_Quit();
            return 1;
        }

        SDL_Surface* _buffer;
       
        _buffer = SDL_CreateRGBSurface(SDL_SWSURFACE,
          320, 240, 32, // w,h,bpp
          0x000000ff, 0x0000ff00, 0x00ff0000, 0x00000000); // rgba masks, for big-endian

        bool _running = true;
        while (_running) {
     	    SDL_Event event;
    		while(SDL_PollEvent(&event) != 0 ) {
                switch(event.type) {
            	    case SDL_KEYDOWN:
            	        printf("key down\n");
            	        _running = false;

            	        break;
            	    case SDL_QUIT:
            	        printf("quit\n");
            	        _running = false;
                } //switch
            } //while

            // draw background
            SDL_BlitSurface(scaled_image, 0, _buffer, 0);

            SDL_BlitSurface(_buffer, NULL, screen, NULL);

            SDL_Flip(screen);

        } //while
        SDL_FreeSurface( _buffer );

    } //if 
    SDL_FreeSurface( scaled_image );
    SDL_FreeSurface( image );
    SDL_FreeSurface( screen );


    SDL_Quit();

    return 0;

}

