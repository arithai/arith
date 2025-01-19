//act.c ArithAI Yang 2025.01.17
//Purpose:Arithmetics/Artificial/Algorithmic Catgory Theory
//找面,找線 AVFrame *filt_frame; frame_index 
#define _XOPEN_SOURCE 600 /* for usleep */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavfilter/buffersink.h>
#include <libavfilter/buffersrc.h>
#include <libavutil/opt.h>

#define CLIP(X) ( (X) > 255 ? 255 : (X) < 0 ? 0 : X)

// RGB -> YUV
#define RGB2Y(R, G, B) CLIP(( (  66 * (R) + 129 * (G) +  25 * (B) + 128) >> 8) +  16)
#define RGB2U(R, G, B) CLIP(( ( -38 * (R) -  74 * (G) + 112 * (B) + 128) >> 8) + 128)
#define RGB2V(R, G, B) CLIP(( ( 112 * (R) -  94 * (G) -  18 * (B) + 128) >> 8) + 128)

// YUV -> RGB
#define C(Y) ( (Y) - 16  )
#define D(U) ( (U) - 128 )
#define E(V) ( (V) - 128 )

#define YUV2R(Y, U, V) CLIP(( 298 * C(Y)              + 409 * E(V) + 128) >> 8)
#define YUV2G(Y, U, V) CLIP(( 298 * C(Y) - 100 * D(U) - 208 * E(V) + 128) >> 8)
#define YUV2B(Y, U, V) CLIP(( 298 * C(Y) + 516 * D(U)              + 128) >> 8)

// RGB -> YCbCr
#define CRGB2Y(R, G, B) CLIP((19595 * R + 38470 * G + 7471 * B ) >> 16)
#define CRGB2Cb(R, G, B) CLIP((36962 * (B - CLIP((19595 * R + 38470 * G + 7471 * B ) >> 16) ) >> 16) + 128)
#define CRGB2Cr(R, G, B) CLIP((46727 * (R - CLIP((19595 * R + 38470 * G + 7471 * B ) >> 16) ) >> 16) + 128)

// YCbCr -> RGB
#define CYCbCr2R(Y, Cb, Cr) CLIP( Y + ( 91881 * Cr >> 16 ) - 179 )
#define CYCbCr2G(Y, Cb, Cr) CLIP( Y - (( 22544 * Cb + 46793 * Cr ) >> 16) + 135)
#define CYCbCr2B(Y, Cb, Cr) CLIP( Y + (116129 * Cb >> 16 ) - 226 )

#define GLOBAL_WIDTH  640*6
#define GLOBAL_HEIGHT 360*6

typedef struct STPOINT {
  unsigned short x;  //0..3840-1
  unsigned short y;  //0..2160-1
  unsigned short nr; //0..65535
  int w;
} ST_POINT;
typedef struct STPONTLIST {
  ST_POINT pt;
  struct STPONTLIST *next;
} ST_POINT_LIST;
extern ST_POINT_LIST *ptHead,*ptTail;
void ptSet(unsigned short x,unsigned short y,int weight) {
//printf("%s(%d)\n",__FILE__,__LINE__);  
  ST_POINT_LIST *ptList,*ptListNext,*ptListTemp;
  ptListNext=(ST_POINT_LIST *)malloc(sizeof(ST_POINT_LIST));
  ptListNext->pt.x=x;
  ptListNext->pt.y=y;
  ptListNext->pt.w=weight;
  if(ptHead==NULL) {
     ptHead=ptListNext;
     ptTail=ptListNext;
     ptHead->next=NULL;
     return;
  }
//printf("%s(%d)\n",__FILE__,__LINE__);  
  ptList=ptHead;
  while(ptList!=NULL && ptList->pt.w>=weight) { //>=稱先到先贏   8,7,7,5
//  printf("%s(%d)\n",__FILE__,__LINE__);  
    if(ptList->next!=NULL) {
      if(ptList->next->pt.w<weight) break;
    }
    ptList = ptList->next;
  }
//printf("%s(%d) %X %X\n",__FILE__,__LINE__,ptHead,ptList);  
  if(ptList==ptHead) {    //insert in Head
    ptListNext->next=ptHead;
    ptHead=ptListNext;
//  printf("%s(%d) head %X %X\n",__FILE__,__LINE__,ptHead,ptList);  
  }
  else if(ptList==NULL) { //insert in Tail
    ptTail->next=ptListNext;
    ptListNext->next=NULL;
    ptTail=ptListNext;
//  printf("%s(%d) tail %X %X\n",__FILE__,__LINE__,ptHead,ptList);  
  }
  else {                  //insert in middle 
    ptListTemp=ptList->next;
    ptList->next=ptListNext;
    ptListNext->next=ptListTemp;
//  printf("%s(%d) middle %X %X\n",__FILE__,__LINE__,ptHead,ptList);  
  }  
}
void ptFree() {
  ST_POINT_LIST *ptList,*ptListTemp;
  ptList=ptHead;
  while(ptList!=NULL) { 
    ptListTemp=ptList->next;
    free(ptList);
    ptList=ptListTemp;
  }
  ptHead=NULL;
  ptTail=NULL;
}
void ptListAll() {
  ST_POINT_LIST *ptList,*ptListTemp;
  int x30,y30;
  ptList=ptHead;
  while(ptList!=NULL) { //
    if(ptList->pt.w>104159) 
    {
      x30=ptList->pt.x;y30=ptList->pt.y;
      x30=x30;y30=y30;
//    printf("%s(%d) (%4d,%4d,%7d)\n",__FILE__,__LINE__,x30,y30,ptList->pt.w);
    }    
    ptListTemp=ptList->next;
    ptList=ptListTemp;
  }
}
#define maxFirst 32
ST_POINT_LIST *ptFirst;
void ptGetFirst() {
  ST_POINT_LIST *ptList,*ptListTemp;
  int x30,y30;
  int i=0;
  ptList=ptHead;
  while(ptList!=NULL && i<maxFirst) { 
//  printf("%s(%d) %lX,%d\n",__FILE__,__LINE__,(long)ptList,i);
    if(ptList->pt.w>380000) {
      x30=ptList->pt.x;y30=ptList->pt.y;
      x30=x30;y30=y30;
//    printf("(%4d,%4d,%7d)\n",x30,y30,ptList->pt.w);
    }    
    i++;    
    ptListTemp=ptList->next;
    ptList=ptListTemp;
  }
  ptFirst=ptList;
//printf("%s(%d) %lX\n",__FILE__,__LINE__,(long)ptFirst);
}

#include <string.h>
#include <math.h>
#include <libavutil/avassert.h>
#include <libavutil/channel_layout.h>
#include <libavutil/opt.h>
#include <libavutil/mathematics.h>
#include <libavutil/timestamp.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>
#define STREAM_DURATION   10.0
#define STREAM_FRAME_RATE 25 /* 25 images/s */
#define STREAM_PIX_FMT    AV_PIX_FMT_YUV420P /* default pix_fmt */
#define SCALE_FLAGS SWS_BICUBIC
// a wrapper around a single output AVStream
typedef struct OutputStream {
    AVStream *st;
    AVCodecContext *enc;
    /* pts of the next frame that will be generated */
    int64_t next_pts;
    int samples_count;
    AVFrame *frame;
    AVFrame *tmp_frame;
    AVPacket *tmp_pkt;
    float t, tincr, tincr2;
    struct SwsContext *sws_ctx;
    struct SwrContext *swr_ctx;
} OutputStream;
#include <stdbool.h>
#define MAX_JUMP 2
extern AVFrame *filt_frame;
bool marginal(x,y) {
  int diff;
  if(x>0 && y>0) { //v change!
    diff=abs(
       filt_frame->data[2][(y-1) * filt_frame->linesize[2] + x    ]+
       filt_frame->data[2][     y* filt_frame->linesize[2] + (x-1)]-
     2*filt_frame->data[2][     y* filt_frame->linesize[2] + x    ]);
//  printf("%s(%d) diff=%d\n",__FILE__,__LINE__,diff);
    if(diff > MAX_JUMP) 
     return true;  
  } 
  return false;
}
int HXYV[60][60];
int HXYY[30][30];
int HXYU[30][30];
int EX[3840];
int EY[3840];
void calc_64x36(AVFrame *pict, int frame_index,
                int width, int height) { //60x60
  int x,y,x30,y30,x60,y60;
  int xs60,ys60;  
  ptFree();    
  memset(HXYY,0,sizeof(HXYY));
  memset(HXYU,0,sizeof(HXYU));
  memset(HXYV,0,sizeof(HXYV));
  memset(EX,  0,sizeof(EX));
  memset(EY,  0,sizeof(EY));
  xs60=width/60;
  ys60=height/60;
#if 0  
  for(y30=0;y30<30;y30++) {
    for(x30=0;x30<30;x30++) {
      if(HXYV[x30][y30]>0) 
        printf("====(%4d,%4d,%7d,%7d,%7d)\n",x30*64*2,y30*36*2,HXYY[x30*2][y30*2],HXYU[x30][y30],HXYV[x30][y30]);    
    }
  }  
#endif  
  for (y = 0; y < height; y++) {
    y60=y/ys60; 
    for (x = 0; x < width; x++) {
      x60=x/xs60;  
      HXYY[x60][y60]+=filt_frame->data[0][y * filt_frame->linesize[0] + x];
    }
  }      
  for (y = 0; y < height/2; y++) {
    y30=y/ys60; 
    for (x = 0; x < width/2; x++) {
      x30=x/xs60;  
      HXYU[x30][y30]+=filt_frame->data[1][y * filt_frame->linesize[1] + x];  
      HXYV[x30][y30]+=filt_frame->data[2][y * filt_frame->linesize[2] + x];  
      if(marginal(x/2,y/2)) {
        EX[x]++;;
        EY[y]++;;
      }
    }
  }  
  for(y30=0;y30<30;y30++) {
    for(x30=0;x30<30;x30++) {
//    printf("%s(%d)\n",__FILE__,__LINE__);         
      ptSet((unsigned short) x30,(unsigned short) y30,HXYV[x30][y30]);
      #if 0
      if(HXYV[x30][y30]>380000) 
        printf("(%4d,%4d,%7d,%7d,%7d)\n",x30*64*2,y30*36*2,HXYY[x30*2][y30*2],HXYU[x30][y30],HXYV[x30][y30]);
      #endif  
    }
  }
  ptListAll();
  printf("calc_64x36 (%d,%d) %lX end\n",width,height,(long)ptHead);
  return;
}
//邊緣,界,端點,起點,終點
#define edgeSpace      1               //左右上下邊緣不算
#define maxDeltaSpace  1               //最長不是線的空白
#define minPointNrLine 6               //最少線的點數
typedef struct STLINE {
  short c0;  //0..3840-1 =coordinate
  short c1;  //0..65535
  int w;     //點數
} ST_LINE;
typedef struct STLINELIST {
  ST_LINE L;
  struct STLINELIST *next;
} ST_LINE_LIST;
extern ST_LINE_LIST *lnHead;
void initLineList(int width,int height) {
  int i;
  ST_LINE_LIST *lnList,*lnListNext;
  lnListNext=(ST_LINE_LIST *)malloc(sizeof(ST_LINE_LIST));
  lnListNext->L.c0=0;
  lnListNext->L.c1=0;
  lnListNext->L.w=0;
  lnHead=lnListNext;
  lnList=lnHead;
  for(i=edgeSpace+1;i<width/2-edgeSpace-2;i++) {
    lnListNext=(ST_LINE_LIST *)malloc(sizeof(ST_LINE_LIST));
    lnListNext->L.c0=0;
    lnListNext->L.c1=0;
    lnListNext->L.w=0;
    lnList->next=lnListNext;
    lnList=lnListNext;
  }
  lnList->next=NULL;
}
void freeLineList() {
  ST_LINE_LIST *lnList,*lnListNext;
  if(lnHead!=NULL) {
    lnList=lnHead;
    while(lnList->next!=NULL) {
      lnListNext=lnList->next;
      free(lnList);
      lnList=lnListNext;
    }
    free(lnList);
  }
  lnHead=NULL;
}
#define MAX_JUMP_LINE 12
bool marginalLine(int x,int y) {
  int diff,i,x0,y0;
  for (i=0;i<maxDeltaSpace;i++) {
    x0=x-i;
    y0=y-i;
    if(x0>0 && y0>0) { //v change!
      diff=abs(
        filt_frame->data[2][(y0-1) * filt_frame->linesize[2] + x0    ]+
        filt_frame->data[2][     y0* filt_frame->linesize[2] + (x0-1)]-
      2*filt_frame->data[2][     y0* filt_frame->linesize[2] + x0    ]);
    //printf("%s(%d) diff=%d\n",__FILE__,__LINE__,diff);
      if(diff > MAX_JUMP_LINE) 
       return true;  
    }
  }   
  return false;
}  

//(x0,y0 ...x,y,... x1,y1)
#define ratioS  1.33333
#define ratioT -1.33333
#define MAX_JUMP_SLOPE 12
typedef struct STSLOPE { //Slope A,B
  short y0;
  short c0;  //0..3840-1 =coordinate
  short c1;  //0..65535
  int w;     //點數
} ST_SLOPE;
typedef struct STSLOPELIST {
  ST_SLOPE s;
  struct STSLOPELIST *next;
} ST_SLOPE_LIST;
extern ST_SLOPE_LIST *slHead;
void initSlopeList(int width,int height) {
  int i;
  ST_SLOPE_LIST *slList,*slListNext;
  slListNext=(ST_SLOPE_LIST *)malloc(sizeof(ST_SLOPE_LIST));
  slListNext->s.y0=0;
  slListNext->s.c0=0;
  slListNext->s.c1=0;
  slListNext->s.w=0;
  slHead=slListNext;
  slList=slHead;
  for(i=edgeSpace+1;i<width/2-edgeSpace-2;i++) {
    slListNext=(ST_SLOPE_LIST *)malloc(sizeof(ST_SLOPE_LIST));
    slListNext->s.y0=0;
    slListNext->s.c0=0;
    slListNext->s.c1=0;
    slListNext->s.w=0;
    slList->next=slListNext;
    slList=slListNext;
  }
  slList->next=NULL;
}
void freeSlopeList() {
  ST_SLOPE_LIST *slList,*slListNext;
  if(slHead!=NULL) {
    slList=slHead;
    while(slList->next!=NULL) {
      slListNext=slList->next;
      free(slList);
      slList=slListNext;
    }
    free(slList);
  }
  slHead=NULL;
}
#define sign(x) (x>0?1:-1)
#define maxDeltaSlopeSpace 1
bool marginalSlope(int x,int y,int ystart,float ratio,int width,int height) {
  int diff,i,x0,y0;
  int g=sign(ratio); //g<0 uppper g>0 lower
  for (i=-maxDeltaSlopeSpace;i<maxDeltaSlopeSpace;i++) {
    x0=x+i;
    y0=(int)(ystart+ratio*x0);
    if(x0>0 && y0>0 && x0<width && y0<height ) { //v change!
      diff=abs(
        filt_frame->data[2][ (y0+2*g) * filt_frame->linesize[2] + x0 ]+
        filt_frame->data[2][      y0  * filt_frame->linesize[2] + (x0-2)]-
      2*filt_frame->data[2][      y0  * filt_frame->linesize[2] + x0    ]);
    //printf("%s(%d) diff=%d\n",__FILE__,__LINE__,diff);
      if(diff > MAX_JUMP_SLOPE) 
       return true;  
    }
  }   
  return false;
}  
//XYZST
//Xfi X frame index
//PQR R rect
int wp,
    Xfi,XposLine,Xc0,Xc1,Xw,
    Yfi,YposLine,Yc0,Yc1,Yw,
    Zfi,ZposLine,Zc0,Zc1,Zw,
    Sfi,SposSlope,Sc0,Sc1,Sy0,Sw,
    Tfi,TposSlope,Tc0,Tc1,Ty0,Tw
    ;
void FindXYZSTLineList(AVFrame *pict, int frame_index,
                    int width, int height) { //60x60
  int Y,U,V,R,G,B;
  int c0,c1,w,countJump;
  bool isStart;
  ST_LINE_LIST *lnList,*lnListNext;
  ST_SLOPE_LIST *slList,*slListNext;
//x0,y0,t,x=x0+t,y=[y0+rt],t jump
  int  x0,y0,x,y;
  
  c0=0;c1=0;    
  c0=c0;c1=c1; 
//Y     
  lnListNext=lnHead;   
  lnList=lnListNext;
//printf("%s(%d)\n",__FILE__,__LINE__);
  Yfi=frame_index;
  Yw = 0;
  for (y = edgeSpace+maxDeltaSpace; y < height/2-edgeSpace-1; y++) {
    w = 0;
    c0=0;c1=0;
    isStart=true;
    countJump=0;  
    for (x = edgeSpace+maxDeltaSpace; x < width/2-edgeSpace-1; x++) {      
#if 1      
      Y =  filt_frame->data[0][y*2 * filt_frame->linesize[0] + x*2];
      U =  filt_frame->data[1][y * filt_frame->linesize[1] + x];
      V =  filt_frame->data[2][y * filt_frame->linesize[2] + x];
      R = YUV2R(Y,U,V);
      G = YUV2G(Y,U,V);
      B = YUV2B(Y,U,V);
//    printf("%s(%d) (%4d,%4d) yuv (%3d,%3d,%3d) rgb (%3d,%3d,%3d)\n",__FILE__,__LINE__,
//           x,y,Y,U,V,R,G,B);
//    if(x>410) exit(0);
      R = R; G= G; B = B;       
#endif        
//    printf("%s(%d)fi=%3d,x=(%4d,%4d),ypos=%4d,(%d,%d),(%d,countJump=%d)\n",__FILE__,__LINE__,Yfi,x,y,YposLine,c0,c1,w,countJump);        
      if(marginalLine(x,y)){
        if(isStart) {
          isStart=false;
          w=1;
          c0=x;
//        printf("%s(%d) (%4d,%4d),%4d\n",__FILE__,__LINE__,x,y,c0);
        }
        else {
          w++;
        }  
        countJump=1;
        c1=x; //so End
//      printf("%s(%d)fi=%3d,x=(%4d,%4d),ypos=%4d,(%d,%d),(%d,countJump=%d)\n",__FILE__,__LINE__,Yfi,x,y,YposLine,c0,c1,w,countJump);        
      }
      else {
        if(isStart==false) { 
          if(countJump) {
            countJump++;
//          printf("%s(%d) countJump=%4d",__FILE__,__LINE__,countJump);
            if(countJump>maxDeltaSpace) {
              if(w>Yw) {
                Yw=w;
                lnList->L.w=Yw;
                YposLine=y;
                Yc0=c0;
                lnList->L.c0=Yc0;
                Yc1=c1;
                lnList->L.c1=Yc1;
              }  
              countJump=0;         
              isStart=true;
//            printf("%s(%d)fi=%3d,x=(%4d,%4d),ypos=%4d,(%d,%d),(%d,%d)\n",__FILE__,__LINE__,Yfi,x,y,YposLine,Yc0,Yc1,w,Yw);        
//            if(Yw>100) exit(0);
            }            
          }  
        }
      }
    }
    if(w>Yw) {
      Yw=w;
      lnList->L.w=Yw;
      Yfi=frame_index;
      YposLine=y;
      Yc0=c0;
      lnList->L.c0=Yc0;
      Yc1=c1;
      lnList->L.c1=Yc1;
    }   
    lnListNext=lnList->next;
    if(lnListNext==NULL) break;
    lnList=lnListNext;  
  }  
  printf("%s(%d)fi=%3d,y=%d,(%d,%d),%d\n",__FILE__,__LINE__,Yfi,YposLine,Yc0,Yc1,Yw);
  
  //X     
  lnListNext=lnHead;   
  lnList=lnListNext;
//printf("%s(%d)\n",__FILE__,__LINE__);
  Xfi=frame_index;
  Xw = 0;
  for (x = width/2-edgeSpace-1; x > edgeSpace+maxDeltaSpace; x--) {
    w = 0;
    c0=0;c1=0;
    isStart=true;
    countJump=0;  
    for (y = edgeSpace+maxDeltaSpace; y < height/2-edgeSpace-1; y++) {      
#if 1      
      Y =  filt_frame->data[0][y*2 * filt_frame->linesize[0] + x*2];
      U =  filt_frame->data[1][y * filt_frame->linesize[1] + x];
      V =  filt_frame->data[2][y * filt_frame->linesize[2] + x];
      R = YUV2R(Y,U,V);
      G = YUV2G(Y,U,V);
      B = YUV2B(Y,U,V);
//    printf("%s(%d) (%4d,%4d) yuv (%3d,%3d,%3d) rgb (%3d,%3d,%3d)\n",__FILE__,__LINE__,
//           x,y,Y,U,V,R,G,B);
//    if(x>410) exit(0);
      R = R; G= G; B = B;       
#endif        
//    printf("%s(%d)fi=%3d,x=(%4d,%4d),ypos=%4d,(%d,%d),(%d,countJump=%d)\n",__FILE__,__LINE__,Yfi,x,y,YposLine,c0,c1,w,countJump);        
      if(marginalLine(x,y)){
        if(isStart) {
          isStart=false;
          w=1;
          c0=y;
//        printf("%s(%d) (%4d,%4d),%4d\n",__FILE__,__LINE__,x,y,c0);
        }
        else {
          w++;
        }  
        countJump=1;
        c1=y; //so End
//      printf("%s(%d)fi=%3d,x=(%4d,%4d),ypos=%4d,(%d,%d),(%d,countJump=%d)\n",__FILE__,__LINE__,Yfi,x,y,YposLine,c0,c1,w,countJump);        
      }
      else {
        if(isStart==false) { 
          if(countJump) {
            countJump++;
//          printf("%s(%d) countJump=%4d",__FILE__,__LINE__,countJump);
            if(countJump>maxDeltaSpace) {
              if(w>(Xw+2)) {
                Xw=w;
                lnList->L.w=Xw;
                XposLine=x;
                Xc0=c0;
                lnList->L.c0=Xc0;
                Xc1=c1;
                lnList->L.c1=Xc1;
              }  
              countJump=0;         
              isStart=true;
//            printf("%s(%d)fi=%3d,x=(%4d,%4d),xpos=%4d,(%d,%d),(%d,%d)\n",__FILE__,__LINE__,Xfi,x,y,XposLine,Xc0,Xc1,w,Xw);        
//            if(Xw>100) exit(0);
            }            
          }  
        }
      }
    }
    if(w>(Xw+2)) {
      Xw=w;
      lnList->L.w=Xw;
      Xfi=frame_index;
      XposLine=x;
      Xc0=c0;
      lnList->L.c0=Xc0;
      Xc1=c1;
      lnList->L.c1=Xc1;
    }   
    lnListNext=lnList->next;
    if(lnListNext==NULL) break;
    lnList=lnListNext;  
  }  
  printf("%s(%d)fi=%3d,x=%d,(%d,%d),%d\n",__FILE__,__LINE__,Xfi,XposLine,Xc0,Xc1,Xw);  
//if(Xw>100) exit(0);

  //Z     
  lnListNext=lnHead;   
  lnList=lnListNext;
//printf("%s(%d)\n",__FILE__,__LINE__);
  Zfi=frame_index;
  Zw = 0;
  for (x = edgeSpace+maxDeltaSpace; x < width/2-edgeSpace-1; x++) {
    w = 0;
    c0=0;c1=0;
    isStart=true;
    countJump=0;  
    for (y = edgeSpace+maxDeltaSpace; y < height/2-edgeSpace-1; y++) {      
#if 1      
      Y =  filt_frame->data[0][y*2 * filt_frame->linesize[0] + x*2];
      U =  filt_frame->data[1][y * filt_frame->linesize[1] + x];
      V =  filt_frame->data[2][y * filt_frame->linesize[2] + x];
      R = YUV2R(Y,U,V);
      G = YUV2G(Y,U,V);
      B = YUV2B(Y,U,V);
//    printf("%s(%d) (%4d,%4d) yuv (%3d,%3d,%3d) rgb (%3d,%3d,%3d)\n",__FILE__,__LINE__,
//           x,y,Y,U,V,R,G,B);
//    if(x>410) exit(0);
      R = R; G= G; B = B;       
#endif        
//    printf("%s(%d)fi=%3d,x=(%4d,%4d),ypos=%4d,(%d,%d),(%d,countJump=%d)\n",__FILE__,__LINE__,Yfi,x,y,YposLine,c0,c1,w,countJump);        
      if(marginalLine(x,y)){
        if(isStart) {
          isStart=false;
          w=1;
          c0=y;
//        printf("%s(%d) (%4d,%4d),%4d\n",__FILE__,__LINE__,x,y,c0);
        }
        else {
          w++;
        }  
        countJump=1;
        c1=y; //so End
//      printf("%s(%d)fi=%3d,x=(%4d,%4d),ypos=%4d,(%d,%d),(%d,countJump=%d)\n",__FILE__,__LINE__,Yfi,x,y,YposLine,c0,c1,w,countJump);        
      }
      else {
        if(isStart==false) { 
          if(countJump) {
            countJump++;
//          printf("%s(%d) countJump=%4d",__FILE__,__LINE__,countJump);
            if(countJump>maxDeltaSpace) {
              if(w>Zw) {
                Zw=w;
                lnList->L.w=Zw;
                ZposLine=x;
                Zc0=c0;
                lnList->L.c0=Zc0;
                Zc1=c1;
                lnList->L.c1=Zc1;
              }  
              countJump=0;         
              isStart=true;
//            printf("%s(%d)fi=%3d,z=(%4d,%4d),zpos=%4d,(%d,%d),(%d,%d)\n",__FILE__,__LINE__,Zfi,x,y,ZposLine,Zc0,Zc1,w,Zw);        
//            if(Yw>100) exit(0);
            }            
          }  
        }
      }
    }
    if(w>Zw) {
      Zw=w;
      lnList->L.w=Zw;
      Zfi=frame_index;
      ZposLine=x;
      Zc0=c0;
      lnList->L.c0=Zc0;
      Zc1=c1;
      lnList->L.c1=Zc1;
    }   
    lnListNext=lnList->next;
    if(lnListNext==NULL) break;
    lnList=lnListNext;  
  }  
  printf("%s(%d)fi=%3d,z=%d,(%d,%d),%d\n",__FILE__,__LINE__,Xfi,ZposLine,Zc0,Zc1,Zw);  

//S
//int  x0,y0,x,y,x1,y1;
  slList=slHead;
  slListNext=slList;
  Sfi=frame_index;
  Sw = 0;
  for (y0 = edgeSpace+maxDeltaSpace; y0 < height/2-edgeSpace-1; y0++) {
    w = 0;
    c0=0;c1=0;
    isStart=true;
    countJump=0;  
    for (x0 = edgeSpace+maxDeltaSpace; x0 < width/2-edgeSpace-1; x0++) {
//    printf("%s(%d)fi=%3d,s=(%4d,%4d),spos=%4d,(%d,%d),(%d,%d)\n",__FILE__,__LINE__,Sfi,x,y,SposSlope,Sc0,Sc1,w,Sw);        
      x = x0;      
      y = (int)(0.49+y0+x*ratioS);
//    printf("%s(%d) %d,%d,%d,%d,%10.5f\n",__FILE__,__LINE__,x0,x,y0,y,ratioS);exit(0);
      if(y>height/2-edgeSpace-1) break;
#if 1      
      Y =  filt_frame->data[0][y*2 * filt_frame->linesize[0] + x*2];
      U =  filt_frame->data[1][y * filt_frame->linesize[1] + x];
      V =  filt_frame->data[2][y * filt_frame->linesize[2] + x];
      R = YUV2R(Y,U,V);
      G = YUV2G(Y,U,V);
      B = YUV2B(Y,U,V);
      R = R; G= G; B = B;       
#endif    
//    printf("%s(%d)fi=%3d,s=(%4d,%4d),spos=%4d,(%d,%d),(%d,%d)\n",__FILE__,__LINE__,Sfi,x,y,SposSlope,Sc0,Sc1,w,Sw);        
      if(marginalSlope(x,y,y0,ratioS,width/2,height/2)){
        if(isStart) {
          isStart=false;
          w=1;
          c0=x;
//        printf("%s(%d)fi=%3d,s=(%4d,%4d),spos=%4d,(%d,%d),(%d,%d)\n",__FILE__,__LINE__,Sfi,x,y,SposSlope,Sc0,Sc1,w,Sw);        
        }
        else {
          w++;
        }  
        countJump=1;
        c1=x; //so End
//      printf("%s(%d)fi=%3d,s=(%4d,%4d),spos=%4d,(%d,%d),(%d,%d)\n",__FILE__,__LINE__,Sfi,x,y,SposSlope,Sc0,Sc1,w,Sw);        
      }
      else {
        if(isStart==false) { 
          if(countJump) {
            countJump++;
//          printf("%s(%d)fi=%3d,s=(%4d,%4d),spos=%4d,(%d,%d),(%d,%d)\n",__FILE__,__LINE__,Sfi,x,y,SposSlope,Sc0,Sc1,w,Sw);        
            if(countJump>maxDeltaSpace) {
              if(w>Sw) {
                Sw=w;
                slList->s.w=Sw;
                SposSlope=y;
                Sc0=c0;
                slList->s.c0=Sc0;
                Sc1=c1;
                slList->s.c1=Sc1;
                Sy0 = y0;
              }  
              countJump=0;         
              isStart=true;
//            printf("%s(%d)fi=%3d,s=(%4d,%4d),spos=%4d,(%d,%d),(%d,%d),%d\n",__FILE__,__LINE__,Sfi,x,y,SposSlope,Sc0,Sc1,w,Sw,Sy0);        
//            if(Sw>70) exit(0);
            }            
          }  
        }
      }
    }
    if(w>Sw) {
      Sw=w;
      slList->s.w=Sw;
      Sfi=frame_index;
      SposSlope=y;
      Sc0=c0;
      slList->s.c0=Sc0;
      Sc1=c1;
      slList->s.c1=Sc1;
      Sy0 = y0;
//      printf("%s(%d)fi=%3d,s=(%4d,%4d),spos=%4d,(%d,%d),(%d,%d),%d\n",__FILE__,__LINE__,Sfi,x,y,SposSlope,Sc0,Sc1,w,Sw,Sy0);        
//      if(Sw>70) exit(0);
    }   
    slListNext=slList->next;
    if(slListNext==NULL) break;
    slList=slListNext;  
  }  
  printf("%s(%d)fi=%3d,s=%d,(%d,%d,%d),%d\n",__FILE__,__LINE__,Sfi,SposSlope,Sc0,Sc1,Sy0,Sw);

//T
//int  x0,y0,x,y,x1,y1;
  slList=slHead;
  slListNext=slList;
  Tfi=frame_index;
  Tw = 0;
  for (y0 = edgeSpace+maxDeltaSpace; y0 < height/2-edgeSpace-1; y0++) {
    w = 0;
    c0=0;c1=0;
    isStart=true;
    countJump=0;  
    for (x0 = edgeSpace+maxDeltaSpace; x0 < width/2-edgeSpace-1; x0++) {
//    printf("%s(%d)fi=%3d,s=(%4d,%4d),spos=%4d,(%d,%d),(%d,%d)\n",__FILE__,__LINE__,Sfi,x,y,SposSlope,Sc0,Sc1,w,Sw);        
      x = x0;      
      y = (int)(0.49+y0+x*ratioT);
      if(y<edgeSpace+maxDeltaSpace) break;
#if 1      
      Y =  filt_frame->data[0][y*2 * filt_frame->linesize[0] + x*2];
      U =  filt_frame->data[1][y * filt_frame->linesize[1] + x];
      V =  filt_frame->data[2][y * filt_frame->linesize[2] + x];
      R = YUV2R(Y,U,V);
      G = YUV2G(Y,U,V);
      B = YUV2B(Y,U,V);
      R = R; G= G; B = B;       
#endif    
//    printf("%s(%d)fi=%3d,s=(%4d,%4d),spos=%4d,(%d,%d),(%d,%d)\n",__FILE__,__LINE__,Sfi,x,y,SposSlope,Sc0,Sc1,w,Sw);        
      if(marginalSlope(x,y,y0,ratioT,width/2,height/2)){
        if(isStart) {
          isStart=false;
          w=1;
          c0=x;
//        printf("%s(%d)fi=%3d,s=(%4d,%4d),spos=%4d,(%d,%d),(%d,%d)\n",__FILE__,__LINE__,Sfi,x,y,SposSlope,Sc0,Sc1,w,Sw);        
        }
        else {
          w++;
        }  
        countJump=1;
        c1=x; //so End
        if(Tfi==7)printf("%s(%d)fi=%3d,t=(%4d,%4d),tpos=%4d,(%d,%d),(%d,%d),%d,%d\n",__FILE__,__LINE__,Tfi,x,y,TposSlope,Tc0,Tc1,w,Tw,Ty0,y0);        
      }
      else {
        if(isStart==false) { 
          if(countJump) {
            countJump++;
//          printf("%s(%d)fi=%3d,s=(%4d,%4d),spos=%4d,(%d,%d),(%d,%d)\n",__FILE__,__LINE__,Sfi,x,y,SposSlope,Sc0,Sc1,w,Sw);        
            if(countJump>maxDeltaSpace) {
              if(w>Tw) {
                Tw=w;
                slList->s.w=Tw;
                TposSlope=y;
                Tc0=c0;
                slList->s.c0=Tc0;
                Tc1=c1;
                slList->s.c1=Tc1;
                Ty0 = y0;
              }  
              countJump=0;         
              isStart=true;
              if(Tfi==7)printf("%s(%d)fi=%3d,t=(%4d,%4d),tpos=%4d,(%d,%d),(%d,%d),%d,%d\n",__FILE__,__LINE__,Tfi,x,y,TposSlope,Tc0,Tc1,w,Tw,Ty0,y0);        
  //          if(Tw>70) exit(0);
            }            
          }  
        }
      }
    }
    if(w>Tw) {
      Tw=w;
      slList->s.w=Tw;
      TposSlope=y;
      Tc0=c0;
      slList->s.c0=Tc0;
      Tc1=c1;
      slList->s.c1=Tc1;
      Ty0 = y0;
      if(Tfi==7)printf("%s(%d)fi=%3d,t=(%4d,%4d),tpos=%4d,(%d,%d),(%d,%d),%d,%d\n",__FILE__,__LINE__,Tfi,x,y,TposSlope,Tc0,Tc1,w,Tw,Ty0,y0);        
//    if(Tw>70) exit(0);
    }  
    slListNext=slList->next;
    if(slListNext==NULL) break;
    slList=slListNext;  
  }  
  if(Tfi==7)printf("%s(%d)fi=%3d,t=%d,(%d,%d,%d),%d\n",__FILE__,__LINE__,Tfi,TposSlope,Tc0,Tc1,Ty0,Tw);

  printf("FindXYZSTLineList (%d,%d) %lX end\n",width,height,(long)ptHead);
//if(Tfi==7) exit(0);

  return;
}
extern int Xmax,Xleft,Xright,Ymax,Yup,Ydown;
//#define MAXNR_PT 64
//extern int HX[GLOBAL_WIDTH],HY[GLOBAL_HEIGHT];
extern int HX[GLOBAL_HEIGHT/2],HY[GLOBAL_WIDTH/2];
void calc_histogram(AVFrame *pict, int frame_index,
                          int width, int height) 
{                          
    int x, y,i,maxv,maxi;
    i = frame_index;
    printf("i=%d,(%d,%d)\n",i,width,height);
    maxi=0;
    maxv=0; 
    for (y = 0; y < height/2; y++) {
       HX[y]=0;  
       for (x = 0; x < width/2; x++) {
          HX[y]+=filt_frame->data[2][y * filt_frame->linesize[2] + x];  
       }
       if(HX[y]>maxv) {
         maxi=y;
         maxv=HX[y]; 
       }
    }
    Ymax=maxi*2;   
    for (y = 0; y < height/2; y++) {
       if(HX[y]>=maxv-30) break;
    }
    Yup=2*y;
    for (y = height/2-1; y > 0; y--) {
       if(HX[y]>=maxv-30) break;
    }
    Ydown=2*y;    
    printf("Ymax=(%d,%d,%d)%d\n",Ymax,Yup,Ydown,maxv);
    maxi=0;
    maxv=0; 
    for (x = 0; x < width/2; x++) { 
       HY[x]=0;  
       for (y = 0; y < height/2; y++) {
          HY[x]+=filt_frame->data[2][y * filt_frame->linesize[2] + x];  
       } 
       if(HY[x]>maxv) {
         maxi=x;
         maxv=HY[x]; 
       }
    }   
    Xmax=maxi*2;
    for (x = 0; x < width/2; x++) {
       if(HY[x]>=maxv-30) break;
    }
    Xleft=2*x;
    for (x = width/2-1; x > 0; x--) {
       if(HY[x]>=maxv-30) break;
    }
    Xright=2*x;
    printf("Xmax=(%d,%d,%d)%d\n",Xmax,Xleft,Xright,maxv);
}

