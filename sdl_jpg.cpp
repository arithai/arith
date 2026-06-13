#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <math.h>
#include <cstring>
#include <vector>
struct Button {
  SDL_Rect rect;
  bool isPressed;
  char fstr[19];
};
bool isMouseOver(int mx, int my, const SDL_Rect& rect) {
  return (mx >= rect.x && mx <= rect.x + rect.w &&
          my >= rect.y && my <= rect.y + rect.h);
}
#include <unistd.h>
SDL_Point pt[3][20];
SDL_Point ptClick;
int nPt[3];
FILE *ptFile=NULL;
char fDirectory[30];
char ptfname[256];
char picSN[20];
int NpicSN;
#define MAX_LINE_LENGTH 256
char buffer[MAX_LINE_LENGTH];
void initfDirectory(void) {
  strcpy(ptfname,"data.txt"); 
  strcpy(fDirectory,"../VID");
  if (access(fDirectory, F_OK) == 0) {
  }
  else {
    fDirectory[0]=0;
  }
  if (access(ptfname, F_OK) == 0) {
    ptFile = fopen(ptfname,"rt");
    // Read line-by-line until fgets returns NULL
    while (fgets(buffer, sizeof(buffer), ptFile) != NULL) {
      if (access(buffer, F_OK) == 0) {
        strcpy(fDirectory,buffer);
        break;
      }  
    }
    if(ptFile!=NULL) fclose(ptFile); ptFile=NULL;
  }
}
void initpicSN(void) {
  int i,x,j;
  char snstr[6];
  if(fDirectory[0]==0) return;
  sprintf(ptfname,"%s/data.txt",fDirectory);  
  if (access(ptfname, F_OK) == 0) {
    // file exists
    printf("ptfname=%s\n",ptfname);
    ptFile = fopen(ptfname,"rt");

    // Read line-by-line until fgets returns NULL
    i=0;
    NpicSN=0;
    while (fgets(buffer, sizeof(buffer), ptFile) != NULL) {
      j=strlen(buffer);if(buffer[j-1]==0x0A) buffer[j-1]=0x0;
      x=atoi(buffer);
      sprintf(snstr,"%d",x);
//    printf("serial=[%4d],[%s],%02X\n",x,buffer,buffer[j-1]);
      if (!memcmp(buffer,snstr,strlen(buffer))) {
        picSN[i]=x;
        i++;
        NpicSN=i;
      }
      if(i>15) break;
    }
    if(ptFile!=NULL) fclose(ptFile); ptFile=NULL;
  }
}
void initPoint(int picID) {
  char coor[6];
  int totp=0;
  int x,y;
  char *split;
  ptClick.x = -1;
  ptClick.y = -1;
  sprintf(ptfname,"%s/point%d.txt",fDirectory,picID);  
  if (access(ptfname, F_OK) == 0) {
    // file exists
    ptFile = fopen(ptfname,"rt");

    // Read line-by-line until fgets returns NULL
    while (fgets(buffer, sizeof(buffer), ptFile) != NULL) {
        split=strstr(buffer,",");
        if(split!=NULL && split!=buffer) {
          memcpy(coor,buffer,split-buffer);coor[split-buffer]=0;
          x=atoi(coor);
          strcpy(coor,split+1);
          y=atoi(coor);
          pt[picID][totp].x=x; pt[picID][totp].y=y;
          printf("[%d],x=%4d,y=%4d\n", totp,pt[picID][totp].x, pt[picID][totp].y);
          totp++;
          if(totp>15) break;
        } 
    }
    if(ptFile!=NULL) fclose(ptFile); ptFile=NULL;
    nPt[picID] = totp;
  } else {
    // file doesn't exist
    ptFile = fopen(ptfname,"wt");
    if(ptFile!=NULL) fclose(ptFile); ptFile=NULL;
    nPt[picID] = 0;
  }
}
int getPoint(int picID,int x,int y) {
  int n;
  //search
  for(n=0;n<nPt[picID];n++) {
    if(x==pt[picID][n].x && y==pt[picID][n].y) {
      return n;
    }
  }
  return -1;
}
int putPoint(int picID,int x,int y) {
  int n=nPt[picID],i;
  char buffer[MAX_LINE_LENGTH];
  char ptfname[256];
  if(getPoint(picID,x,y)==-1) 
  {
    pt[picID][n].x=x;pt[picID][n].y=y;
    sprintf(ptfname,"point%d.txt",picID);  
    n++;if(n>15) n=15; nPt[picID]=n; 
    ptFile = fopen(ptfname,"wt");
    for(i=0;i<nPt[picID];i++) {
        sprintf(buffer,"%d,%d\n", pt[picID][i].x, pt[picID][i].y);
        fputs(buffer, ptFile);
    }
    fclose(ptFile);

    return n;
  }
  return n;
}
int nowpicID=0;
void DrawCircle(SDL_Renderer *renderer, int32_t centreX, int32_t centreY, int32_t radius) {
    const int32_t diameter = (radius * 2);
    int32_t x = (radius - 1);
    int32_t y = 0;
    int32_t tx = 1;
    int32_t ty = 1;
    int32_t error = (tx - diameter);

    while (x >= y) {
        //  Each of the following renders an octant of the circle
        SDL_RenderDrawPoint(renderer, centreX + x, centreY - y);
        SDL_RenderDrawPoint(renderer, centreX + x, centreY + y);
        SDL_RenderDrawPoint(renderer, centreX - x, centreY - y);
        SDL_RenderDrawPoint(renderer, centreX - x, centreY + y);
        SDL_RenderDrawPoint(renderer, centreX + y, centreY - x);
        SDL_RenderDrawPoint(renderer, centreX + y, centreY + x);
        SDL_RenderDrawPoint(renderer, centreX - y, centreY - x);
        SDL_RenderDrawPoint(renderer, centreX - y, centreY + x);

        if (error <= 0) {
            ++y;
            error += ty;
            ty += 2;
        }
        if (error > 0) {
            --x;
            tx += 2;
            error += (tx - diameter);
        }
    }
}

// Configure button specs: X, Y, Width, Height
Button myButton[4]= { { { 100, 90, 80, 80 }, false, "" },
                      { { 200, 90, 80, 80 }, false, "" },
                      { { 300, 90, 80, 80 }, false, "" },
                      { { 400, 90, 80, 80 }, false, "" },
                    };
const int width = 2160;
const int height = 3840;
void Draw4K(SDL_Surface* surface,SDL_Renderer* renderer0) {
    SDL_Rect img_rect2;
    img_rect2.x    = 0;
    img_rect2.y    = 0;
    img_rect2.w    = width;
    img_rect2.h    = height;
    // Create a hidden window & 4K software renderer
    SDL_Surface* surface3 = IMG_Load(myButton[0].fstr);
//  SDL_Window* window2 = SDL_CreateWindow("4K Circle", 0, 0, width, height, SDL_WINDOW_HIDDEN);
    SDL_Window* window2 = SDL_CreateWindow("4K Circle", 0, 0, width, height, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer2 = SDL_CreateRenderer(window2, -1, SDL_RENDERER_SOFTWARE);
    SDL_Texture* texture2 = SDL_CreateTextureFromSurface(renderer2, surface3);
    SDL_Surface* surface2 = SDL_ConvertSurfaceFormat(surface3, SDL_PIXELFORMAT_ARGB8888, 0);
    
    // Set background to black
//  SDL_SetRenderDrawColor(renderer2, 255, 255, 255, 255);
//  SDL_RenderClear(renderer2);

//  SDL_RenderCopy(renderer2, texture, &img_rect2, &img_rect2);
//  SDL_RenderPresent(renderer2);

    // Draw a white circle in the center with a radius of 500 pixels
    printf("%d\n",__LINE__);
//    SDL_SetRenderDrawColor(renderer2, 255, 255, 255, 255);
//    SDL_RenderClear(renderer2);
    SDL_RenderCopy(renderer2, texture2, NULL, &img_rect2); 
//  SDL_RenderPresent(renderer2);    //present renderer
    SDL_SetRenderDrawColor(renderer2, 0,  0, 0, 255);
    DrawCircle(renderer2, 100, 100, 100);   
    SDL_RenderPresent(renderer2);    //present renderer
    SDL_Delay(500);
//  SDL_RenderPresent(renderer2);
    SDL_RenderReadPixels(renderer2, NULL,surface2->format->format, surface2->pixels, surface2->pitch);
    SDL_RenderClear(renderer2); 

    // Allocate buffer for 4K pixels
//  std::vector<Uint32> pixels(width * height);
//  SDL_LockSurface(surface);
    // Calculate the total size in bytes (pitch * height)
    // Assuming 32-bit (4 bytes) pixels, we divide by 4 to get the vector size
    size_t pixelCount = (surface2->w * surface2->h) ;
    printf("%d,%d,%d,%d,%d\n",__LINE__,pixelCount,surface2->pitch,surface2->h,surface2->w);  
    // Create the vector and directly copy the pixel memory
    Uint32* pixelsData = static_cast<Uint32*>(surface2->pixels);
    std::vector<Uint32> pixels2(pixelsData, pixelsData + pixelCount);
//  std::vector<Uint32> pixels(pixels2);
    // Unlock surface when done
//  SDL_RenderReadPixels(renderer2, NULL, SDL_PIXELFORMAT_ARGB8888, surface->pixels, surface->pitch);

    // Read the pixels from the renderer into memory
//  SDL_RenderReadPixels(renderer2, NULL, SDL_PIXELFORMAT_ARGB8888, pixels.data(), width * sizeof(Uint32));

    // Save as 4K JPG (quality: 90)
//  stbi_write_jpg("output_4k.jpg", width, height, 4, pixels.data(), 90);
    sprintf(ptfname,"%s/4k.jpg",fDirectory);  
//  stbi_write_jpg(ptfname, width, height, 4, pixels.data(), 90);
    stbi_write_jpg(ptfname, width, height, 4, pixels2.data(), 90);
//  SDL_UnlockSurface(surface);

    // Clean up
    SDL_DestroyRenderer(renderer2);
    SDL_DestroyWindow(window2);
}
int main(int argc, char* argv[]) {
    int i;
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL Init Failed: %s\n", SDL_GetError());
        return 1;
    }

    initfDirectory();
    printf("Get directory [%s]<======\n",fDirectory);
    
    initpicSN();
    if(NpicSN<3) {
      printf("No pictures are assigned.\n");exit(0);
    } 
    for(i=0;i<NpicSN;i++) {
      printf("picSN[%4d]=%04d\n",i,picSN[i]);
    }

    initPoint(picSN[0]);
//  putPoint(0,7,7);
    initPoint(picSN[1]);
    initPoint(picSN[2]);

    // Initialize SDL_image for JPG
    int flags = IMG_INIT_JPG;
    if ((IMG_Init(flags) & flags) != flags) {
        printf("IMG Init Failed: %s\n", IMG_GetError());
        SDL_Quit();
    }

    // Create Window and Renderer
    SDL_Window* window = SDL_CreateWindow("Real Size JPG", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1056, 594, SDL_WINDOW_SHOWN);
//  SDL_Window* window = SDL_CreateWindow("Real Size JPG", 0, 0, width, height, SDL_WINDOW_SHOWN);
//  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

    // Load JPG into a surface
    sprintf(myButton[0].fstr,"%s/x%04d.jpg",fDirectory,picSN[0]);
    sprintf(myButton[1].fstr,"%s/x%04d.jpg",fDirectory,picSN[1]);
    sprintf(myButton[2].fstr,"%s/x%04d.jpg",fDirectory,picSN[2]);
    SDL_Surface* surface = IMG_Load(myButton[0].fstr);
    if (!surface) {
        printf("Failed to load image: %s\n", IMG_GetError());
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    // Convert surface to texture
    SDL_Rect img_rect;
#if 1
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
#else
    SDL_Texture* texture = SDL_CreateTexture(renderer, 
        SDL_PIXELFORMAT_RGBA8888, 
        SDL_TEXTUREACCESS_STREAMING, 
        surface->w, surface->h);

    SDL_LockTexture(texture, &img_rect, &surface->pixels, &surface->pitch);
    // paint into surface pixels
    SDL_UnlockTexture(texture);
#endif
    SDL_QueryTexture(texture, NULL, NULL, &img_rect.w, &img_rect.h);
    printf("w=%4d,h=%4d\n",img_rect.w, img_rect.h);
//  SDL_FreeSurface(surface); // We don't need the surface anymore

    // Set up source rectangle (e.g., cropping a 400x300 chunk starting at x=50, y=50)
    SDL_Rect srcRect = {1200, 1000, 1056, 594};
     
    // Set up destination rectangle (same size for true 1:1 "real" size)
    SDL_Rect destRect = {0, 0, 960, 540}; 
    int zoomFactor = 20; // Pixels to scale per scroll
    int offset_x = 0, offset_y = 0;
    bool is_dragging = false;
    bool is_zooming = false;
    bool is_ClickingOnly = false;
    bool is_leftclick = false;
    // Rendering loop 
    int quit = 0;
    SDL_Event e;
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            }
            else if (e.type == SDL_MOUSEWHEEL) {
                is_dragging = false;
                is_zooming = true;
                SDL_QueryTexture(texture, NULL, NULL, &img_rect.w, &img_rect.h);
                printf("w=%4d,h=%4d\n",img_rect.w, img_rect.h);
                // Adjust dimensions on mouse wheel
                if (e.wheel.y > 0) { // Scroll Up (Zoom In)
                    destRect.w += zoomFactor;
                    destRect.h += zoomFactor;
                    destRect.x -= zoomFactor / 2; // Center the zoom
                    destRect.y -= zoomFactor / 2;
                } else if (e.wheel.y < 0) { // Scroll Down (Zoom Out)
                    destRect.w -= zoomFactor;
                    destRect.h -= zoomFactor;
                    destRect.x += zoomFactor / 2;
                    destRect.y += zoomFactor / 2;
                }
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN) {
                int mouse_x, mouse_y;
                is_zooming = false;
                is_ClickingOnly = false;
                Uint32 mouseState = SDL_GetMouseState(&mouse_x, &mouse_y);
                if (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT)) {
                  is_leftclick = true;
                }
                //right click cancel it
                if (mouseState & SDL_BUTTON(SDL_BUTTON_RIGHT)) {
                  is_leftclick = false;
                }
                printf("mx=%4d,my=%4d,ix=%4d,iy=%4d,iw=%4d,ih=%4d\n",
                      mouse_x, mouse_y,
                      img_rect.x,img_rect.y,img_rect.w,img_rect.h);
                if (isMouseOver(mouse_x, mouse_y, myButton[0].rect)) {
                      myButton[0].isPressed = true;
                      printf("Button Clicked!\n");
                      surface = IMG_Load(myButton[0].fstr);
                      texture = SDL_CreateTextureFromSurface(renderer, surface);
                      nowpicID=0;
                }
                else if (isMouseOver(mouse_x, mouse_y, myButton[1].rect)) {
                      myButton[1].isPressed = true;
                      printf("Button Clicked!\n");
                      surface = IMG_Load(myButton[1].fstr);
                      texture = SDL_CreateTextureFromSurface(renderer, surface);
                      nowpicID=1;
                }
                else if (isMouseOver(mouse_x, mouse_y, myButton[2].rect)) {
                      myButton[2].isPressed = true;
                      printf("Button Clicked!\n");
                      surface = IMG_Load(myButton[2].fstr);
                      texture = SDL_CreateTextureFromSurface(renderer, surface);                     
                      nowpicID=2;
                }
                else if (isMouseOver(mouse_x, mouse_y, myButton[3].rect)) {
                      myButton[3].isPressed = true;
                      Draw4K(surface,renderer);
                      nowpicID=-1;
                }
                else {
                  if (mouse_x >= img_rect.x && mouse_x <= img_rect.x + img_rect.w &&
                    mouse_y >= img_rect.y && mouse_y <= img_rect.y + img_rect.h) {
                    is_dragging = true;
                    offset_x = mouse_x - img_rect.x;
                    offset_y = mouse_y - img_rect.y;
                    printf("mx=%4d,my=%4d,ix=%4d,iy=%4d,iw=%4d,ih=%4d\n",
                        mouse_x, mouse_y,
                        img_rect.x,img_rect.y,img_rect.w,img_rect.h);

                  }
                  is_ClickingOnly = true;
                }
              
            } else if (e.type == SDL_MOUSEBUTTONUP) {
                if(is_ClickingOnly) {
                  int mouse_x, mouse_y;
                  int x,y;
                  SDL_GetMouseState(&mouse_x, &mouse_y);
                  x = mouse_x - img_rect.x;   
                  y = mouse_y - img_rect.y;
                  if(ptClick.x==-1 && ptClick.y==-1) {
                    ptClick.x = x;
                    ptClick.y = y;
                  }
                  else {
                    x = x-ptClick.x;
                    y = y-ptClick.y;
                    if(is_leftclick) {
                      // Left click action here
                      if(sqrt(x*x+y*y)<50) { 
                        putPoint(nowpicID, ptClick.x, ptClick.y);
                        printf("Click,%3d,%4d,%4d,%4d,%4d\n",nowpicID, mouse_x, mouse_y,
                           img_rect.x,img_rect.y);
                      }
                    }
                    ptClick.x=-1;
                    ptClick.y=-1;
                  }
                } 
                is_dragging = false;
                if(  img_rect.x<=0 && (img_rect.x+img_rect.w)>=0
                   &&img_rect.y<=0 && (img_rect.y+img_rect.h)>=0 ) {
                   srcRect.x = -img_rect.x;
                   srcRect.y = -img_rect.y;
                   zoomFactor = 20;
                   printf("sx=%4d,sy=%4d,ix=%4d,iy=%4d,iw=%4d,ih=%4d\n",
                      srcRect.x, srcRect.y,
                      img_rect.x,img_rect.y,img_rect.w,img_rect.h);
                  if (e.button.button == SDL_BUTTON_LEFT) {
                      myButton[0].isPressed = false; // Reset state when released
                      myButton[1].isPressed = false; // Reset state when released
                      myButton[2].isPressed = false; // Reset state when released
                  }
                }
            } else if (e.type == SDL_MOUSEMOTION) {
                if (is_dragging) {
                    int mouse_x, mouse_y;
                    SDL_GetMouseState(&mouse_x, &mouse_y);
                    img_rect.x = mouse_x - offset_x;
                    img_rect.y = mouse_y - offset_y;

                    printf("mx=%4d,my=%4d,ix=%4d,iy=%4d,iw=%4d,ih=%4d\n",
                        mouse_x, mouse_y,
                        img_rect.x,img_rect.y,img_rect.w,img_rect.h);


                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Draw the cropped portion
        if(is_zooming) 
        {
          SDL_RenderCopy(renderer, texture, &srcRect, &destRect);
        }
        else { 
          SDL_RenderCopy(renderer, texture, NULL, &img_rect);
        }

        if (myButton[0].isPressed) {
            SDL_SetRenderDrawColor(renderer, 46, 204, 113, 255); // Green click accent
        } else {
            SDL_SetRenderDrawColor(renderer, 52, 152, 219, 255); // Blue normal mode
        }
        SDL_RenderFillRect(renderer, &myButton[0].rect);
              
        if (myButton[1].isPressed) {
            SDL_SetRenderDrawColor(renderer, 46, 204, 113, 255); // Green click accent
        } else {
            SDL_SetRenderDrawColor(renderer, 52, 152, 219, 255); // Blue normal mode
        }
        SDL_RenderFillRect(renderer, &myButton[1].rect);
        
        if (myButton[2].isPressed) {
            SDL_SetRenderDrawColor(renderer, 46, 204, 113, 255); // Green click accent
        } else {
            SDL_SetRenderDrawColor(renderer, 52, 152, 219, 255); // Blue normal mode
        }
        SDL_RenderFillRect(renderer, &myButton[2].rect);

        if (myButton[3].isPressed) {
            SDL_SetRenderDrawColor(renderer, 46, 204, 113, 255); // Green click accent
        } else {
            SDL_SetRenderDrawColor(renderer, 52, 152, 219, 255); // Blue normal mode
        }
        SDL_RenderFillRect(renderer, &myButton[3].rect);
//DrawCircle
        int n=nPt[nowpicID];
        if(n>10) n=10;
        for(int i=0;i<n;i++) {
//        circleColor(renderer, pt[nowpicID][i].x, pt[nowpicID][i].y, 50, 0xFF0000FF);
          SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); 
          DrawCircle(renderer, pt[nowpicID][i].x-srcRect.x, pt[nowpicID][i].y-srcRect.y, 50);
        }
        if(ptClick.x>=0 && ptClick.y>=0) {
          SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); 
          DrawCircle(renderer, ptClick.x-srcRect.x, ptClick.y-srcRect.y, 50);
        } 

        SDL_RenderPresent(renderer);
    }

    // Clean up
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}
