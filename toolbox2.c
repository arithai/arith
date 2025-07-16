//toolbox.c ArithAI Yang 2025.01.17
//Purpose: Graphics Draw ToolBox
//畫面,畫線 fill_yuv_image AVFrame *pict; frame_index 
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

#define WHITEY 255
#define WHITEU 128
#define WHITEV 128
#define BLACKY 0
#define BLACKU 128
#define BLACKV 128
#define REDY 76
#define REDU 84
#define REDV 255
#define GREENY 149
#define GREENU 43
#define GREENV 21
#define BLUEY 29
#define BLUEU 255
#define BLUEV 107
#define YELLOWY 255
#define YELLOWU 0
#define YELLOWV 148
//淡粉紅
#define PURPLEY 180
#define PURPLEU 170
#define PURPLEV 181
//青色
#define CYANY 178
#define CYANU 171 
#define CYANV 0
    
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
void ptSet(unsigned short x,unsigned short y,int weight);
void ptFree();
void ptListAll();
#define maxFirst 32
extern ST_POINT_LIST *ptFirst,*ptFirstG;
void ptGetFirst();
void ptGetFirstG();
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
int frameWidth,frameHeight;
extern unsigned char *Ybefore;
extern unsigned char *Ubefore;
extern unsigned char *Vbefore;
extern unsigned char *Ynow;
extern unsigned char *Unow;
extern unsigned char *Vnow;
extern unsigned char *Ydiffnow;
extern unsigned char *Udiffnow;
extern unsigned char *Vdiffnow;
extern unsigned char *Yref;
extern unsigned char *Uref;
extern unsigned char *Vref;
extern unsigned char *Rnow;
extern unsigned char *Gnow;
extern unsigned char *Bnow;
extern int Ylinesize,Ulinesize,Vlinesize;
extern bool marginalV(int x,int y);
extern int HXYR[60][60];
extern int HXYG[60][60];
extern int HXYB[60][60];
extern int HXYY[60][60];
extern int HXYV[30][30];
extern int HXYU[30][30];
extern int EX[3840];
extern int EY[3840];
void calc_64x36(AVFrame *pict, int frame_index,
                int width, int height);
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
void initLineList(int width,int height);
void freeLineList();
#define MAX_JUMP_LINE 12
bool marginalLine(int x,int y);
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
void initSlopeList(int width,int height);
void freeSlopeList();
#define sign(x) (x>0?1:-1)
#define maxDeltaSlopeSpace 1
extern bool marginalSlope(int x,int y,int ystart,float ratio,int width,int height);
//XYZST
//Xfi X frame index
//PQR R rect
extern int wp,
    Xfi,XposLine,Xc0,Xc1,Xw,
    Yfi,YposLine,Yc0,Yc1,Yw,
    Zfi,ZposLine,Zc0,Zc1,Zw,
    Sfi,SposSlope,Sc0,Sc1,Sy0,Sw,
    Tfi,TposSlope,Tc0,Tc1,Ty0,Tw
    ;
void FindXYZSTLineList(AVFrame *pict, int frame_index,
                    int width, int height);
extern int Xmax,Xleft,Xright,Ymax,Yup,Ydown;
//#define MAXNR_PT 64
//extern int HX[GLOBAL_WIDTH],HY[GLOBAL_HEIGHT];
extern int HX[GLOBAL_HEIGHT/2],HY[GLOBAL_WIDTH/2];
void calc_histogram(AVFrame *pict, int frame_index,
                          int width, int height);

extern void calc_nb(int x,int y);
extern void calc_ref(int frame_index,int x,int y);
extern int rn[3][3],gn[3][3],bn[3][3];
extern int rb[3][3],gb[3][3],bb[3][3];
extern int re,ge,be,ra,ga,ba;
extern int imaxG,imaxB,imaxR;
#define a(v) (v<128? 0:255)  
void fill_yuv_image(AVFrame *pict, int frame_index,
                    int width, int height)
{
    int x, y, i, r,g,b;
    int x0, y0, x2, y2;
    int x30,y30,xs60,ys60;           
    int Y0,U0,V0,Y1,U1,V1,R0,G0,B0,G1,R1,B1;
    int pos,posU,posV;
    int pos2,pos2U,pos2V;
#if 1
    int Xmid=(Xleft+Xright)/2;
    int Ymid=(Yup+Ydown)/2;
#endif    
    xs60=width/60;
    ys60=height/60;
    i = frame_index; i = i;
#if 1
    printf("%s(%d),h,w=(%d,%d),(%d,%d,%d),(%d,%d,%d)\n",
           __FILE__,__LINE__,height,width, 
           pict->linesize[0],pict->linesize[1],pict->linesize[2],
           filt_frame->linesize[0],filt_frame->linesize[1],filt_frame->linesize[2]
        );  
#endif     
  //printf("%s(%d) (%d,%d) (%X,%X))\n",__FILE__,__LINE__,xs60,ys60,ptFirst,ptFirstG);
#if 0
    /* Y */
    for (y = 0; y < height; y++)
        for (x = 0; x < width; x++)
            pict->data[0][y * pict->linesize[0] + x] = x + y + i * 3;
    /* Cb and Cr */
    for (y = 0; y < height / 2; y++) {
        for (x = 0; x < width / 2; x++) {
            pict->data[1][y * pict->linesize[1] + x] = 128 + y + i * 2;
            pict->data[2][y * pict->linesize[2] + x] = 64 + x + i * 5;
        }
    }
#endif
#if 1
    calc_ref(frame_index,width,height);
    printf("%s(%d) (%d,%d) (%X,%X))\n",__FILE__,__LINE__,xs60,ys60,ptFirst,ptFirstG);
    for (y = 0; y < height; y++) {
      y30=y/ys60;
      y30=y30;
      y2=y/2;
      if(y2==Sy0) {
//      exit(0);
      }
      for (x = 0; x < width; x++) {
        x30=x/xs60;
        x30=x30;
        x2=x/2;
        pos = y * Ylinesize+ x; 
        posU=y2 * Ulinesize + x2; 
        posV=y2 * Vlinesize + x2; 
      //if(frame_index) 
      //  printf("%s(%d)(%4d,%4d) (%6d),%X\n",__FILE__,__LINE__,y,x,HXYG[x30][y30],ptFirstG);
        R1=Rnow[pos];G1=Gnow[pos];B1=Bnow[pos];
        #if 0
        Y1=Ynow[pos];
        U1=Unow[posU];
        V1=Vnow[posV];
        #endif
        #if 1
        Y1=Ydiffnow[pos];
        U1=Udiffnow[posU];
        V1=Vdiffnow[posV];
        #endif
        pos2 = y * pict->linesize[0]+ x; 
        pos2U=y2 * pict->linesize[1] + x2; 
        pos2V=y2 * pict->linesize[2] + x2; 
        if(Yref[pos]==GREENY && Uref[posU]==GREENU && Vref[posV]==GREENV) {
          pict->data[0][pos2]  = GREENY;  //BLUE
          pict->data[1][pos2U] = GREENU; 
          pict->data[2][pos2V] = GREENV;
        }
        else if(Yref[pos]==BLACKY && Uref[posU]==BLACKU && Vref[posV]==BLACKV) {
          pict->data[0][pos2]  = WHITEY;  //BLUE
          pict->data[1][pos2U] = WHITEU; 
          pict->data[2][pos2V] = WHITEV;
        }
        #if 0
        else if(x>Xmid-10&&x<Xmid+10&&y>Ymid-10&&y<Ymid+10) {
          pict->data[0][pos2]  = BLUEY;  //BLUE
          pict->data[1][pos2U] = BLUEU; 
          pict->data[2][pos2V] = BLUEV; 
        }
        #endif
        #if 0
        else if(x>Xmid-10&&x<Xmid+10&&y>Ymid-10&&y<Ymid+10) {
          pict->data[0][pos2]  = BLACKY; //0x0;  
          pict->data[1][pos2U] = BLACKU; //0xFF;
          pict->data[2][pos2V] = BLACKV; //0x0;
        }
        #endif
        #if 0
        else if( (y2 >= YposLine && y2 < (YposLine+5) && x2>=Yc0 && x2<=Yc1) 
              || (x2 >= XposLine && x2 < (XposLine+5) && y2>=Xc0 && y2<=Xc1)
              || (x2 >= ZposLine && x2 < (ZposLine+5) && y2>=Zc0 && y2<=Zc1)
            ) {
//        printf("%s(%d)(%4d,%4d)\n",__FILE__,__LINE__,y,x);
          pict->data[0][pos]  = 0xFF; //YELLOW
          pict->data[1][posU] = 0x00;
          pict->data[2][posV] = 0xFF;
        }
        #endif
        #if 0
        else if(marginalV(x2,y2)) {
          pict->data[0][pos]  = PURPLEY; //0x0;  //BLACK
          pict->data[1][posU] = PURPLEU; //0x33;
          pict->data[2][posV] = PURPLEV; //0x88;
        }
        #endif 
        #if 0
        else if(marginalV(x2,y2)) { //為何邊黃
          pict->data[0][pos]  = 0x0;  
          pict->data[1][posU] = 0x33;
          pict->data[2][posV] = 0x88;
        }
        #endif
        #if 0
        else if(HXYV[x30/2][y30/2] >= ptFirst->pt.w) {
          pict->data[0][pos]  = 0x0;   //GREEN
          pict->data[1][posU] = 0x22;  //correlated line 
          pict->data[2][posV] = 0x66;
        }
        #endif
        #if 0
        else if(HXYV[x30/2][y30/2] >= ptFirst->pt.w) {
          pict->data[0][pos]  = GREENY;   //GREEN
          pict->data[1][posU] = GREENU;  //correlated line 
          pict->data[2][posV] = GREENV;
        }
        #endif
        #if 0
        else if(HXYV[x30/2][y30/2] >= ptFirst->pt.w ) {
          pict->data[0][pos]  = BLUEY;   //BLUE
          pict->data[1][posU] = BLUEU;
          pict->data[2][posV] = BLUEV;
        }
        #endif
        #if 0
        else if(HXYG[x30][y30] >= ptFirstG->pt.w ) {
//        printf("%s(%d)(%4d,%4d) (%6d,%6d) <======\n",__FILE__,__LINE__,y,x,HXYG[x30][y30],ptFirstG->pt.w);
          pict->data[0][pos]  = REDY;   
          pict->data[1][posU] = REDU;
          pict->data[2][posV] = REDV;
          //128,148 WHITEY also YELLOWY 255
          #define z(s,t) ((Yref[(t) * Ylinesize+ (s)]==WHITEY)?0:1) 
          if(y30>5 && y30<55 && x30>5 && x30 <55) {
          
            if((y-y30*ys60)==ys60/2) { //only for corner
              for (x0=x+xs60;x0<width-xs60;x0++) {
                if( ( z(x0-1,y-1)+z(x0,y-1)+z(x0+1,y-1)
                     +z(x0-1,y)  +z(x0,y  )+z(x0+1,y  )
                     +z(x0-1,y+1)+z(x0,y+1)+z(x0+1,y+1))
                   >=4) {
                  Yref[y * Ylinesize+ x0]  =YELLOWY;
                  Uref[y2* Ulinesize+ x0/2]=YELLOWU;
                  Vref[y2* Vlinesize+ x0/2]=YELLOWV;
//                printf("%s(%d)(%4d,%4d)-%4d,(%4d,%4d))\n",__FILE__,__LINE__,y,x,x0,xs60,ys60);
                  break;
                }
              }  
              for (x0=x-xs60;x0>=xs60;x0--) {
                if( ( z(x0-1,y-1)+z(x0,y-1)+z(x0+1,y-1)
                     +z(x0-1,y)  +z(x0,y  )+z(x0+1,y  )
                     +z(x0-1,y+1)+z(x0,y+1)+z(x0+1,y+1))
                   >=4) {
                  Yref[y * Ylinesize+ x0]  =YELLOWY;
                  Uref[y2* Ulinesize+ x0/2]=YELLOWU;
                  Vref[y2* Vlinesize+ x0/2]=YELLOWV;
                  break;
                }
              }
            }

            if((x-x30*xs60)==xs60/2) {
              for (y0=y+ys60;y0<height-ys60;y0++) {
                if( ( z(x-1,y0-1)+z(x,y0-1)+z(x+1,y0-1)
                     +z(x-1,y0)  +z(x,y0  )+z(x+1,y0  )
                     +z(x-1,y0+1)+z(x,y0+1)+z(x+1,y0+1))
                   >=4) {
                  Yref[y0 * Ylinesize+  x] =YELLOWY;
                  Uref[y0/2* Ulinesize+ x2]=YELLOWU;
                  Vref[y0/2* Vlinesize+ x2]=YELLOWV;
//                printf("%s(%d)(%4d,%4d)-%4d,(%4d,%4d))\n",__FILE__,__LINE__,y,x,x0,xs60,ys60);
                  break;
                }
              }  
           
              for (y0=y-ys60;y0>=ys60;y0--) {
                if( ( z(x-1,y0-1)+z(x,y0-1)+z(x+1,y0-1)
                     +z(x-1,y0)  +z(x,y0  )+z(x+1,y0  )
                     +z(x-1,y0+1)+z(x,y0+1)+z(x+1,y0+1))
                   >=4) {
                  Yref[y0 * Ylinesize+  x] =YELLOWY;
                  Uref[y0/2* Ulinesize+ x2]=YELLOWU;
                  Vref[y0/2* Vlinesize+ x2]=YELLOWV;
                  break;
                }
              } 
            }
 
          }  
        }
        #endif  
        else {
          #if 0
     //   printf("%s(%d)(%4d,%4d)\n",__FILE__,__LINE__,y,x);
          if(Rnow[pos]==imaxR && Gnow[pos]==imaxG && Bnow[pos]==imaxB) {
            pict->data[0][pos2]  = CYANY;
            pict->data[1][pos2U] = CYANU;
            pict->data[2][pos2V] = CYANV;          
          }
          else 
          #endif     
          {
            pict->data[0][pos2]  = Yref[pos ];
            pict->data[1][pos2U] = Uref[posU];
            pict->data[2][pos2V] = Vref[posV];
          }
          #if 0
          pict->data[0][pos2]  = Y1;
          pict->data[1][pos2U] = U1;
          pict->data[2][pos2V] = V1;
          #endif
        }
      }    
   // printf("%s(%d)(%4d,%4d)(%5d,%5d,%5d)\n",__FILE__,__LINE__,y,x,pos,posU,posV);
    }
    #if 1 
    printf("%s(%d)[%4d],(%d,%d,%d)(%d,%d,%d)(%d,%d,%d)(%d,%d,%d,%d)(%d,%d,%d,%d)\n",
    __FILE__,__LINE__,
    frame_index,XposLine,Xc0,Xc1,YposLine,Yc0,Yc1,ZposLine,Zc0,Zc1,
    SposSlope,Sc0,Sc1,Sy0,TposSlope,Tc0,Tc1,Ty0);
    #endif
    printf("%s(%d)\n",__FILE__,__LINE__);
    #if 0
    for (x0 = Sc0; x0 < Sc1; x0++) {
//      printf("%s(%d)\n",__FILE__,__LINE__);
      y0=(int)(Sy0+x0*ratioS);
      for (y=0;y<5;y++) {
        if(y0<(height/2-1)) {
//        printf("%s(%d)(%4d,%4d)\n",__FILE__,__LINE__,x0,y0);
          pict->data[0][2*y0 *      pict->linesize[0] + 2*x0]   = BLUEY;
          pict->data[0][2*y0 *      pict->linesize[0] + 2*x0+1] = BLUEY;
          pict->data[0][(2*y0+1) *  pict->linesize[0] + 2*x0]   = BLUEY;
          pict->data[0][(2*y0+1) *  pict->linesize[0] + 2*x0+1] = BLUEY;
          pict->data[1][y0 * pict->linesize[1] + x0] = BLUEU;
          pict->data[2][y0 * pict->linesize[2] + x0] = BLUEV;
          y0++;
        }  
      }          
    }
    #endif
    #if 0
    for (x0 = Tc0; x0 < Tc1; x0++) {
//    printf("%s(%d)\n",__FILE__,__LINE__);
      y0=(int)(Ty0+x0*ratioT);
      for (y=0;y<5;y++) {
        if(y0>0) {
//        printf("%s(%d)(%4d,%4d)\n",__FILE__,__LINE__,x0,y0);
          pict->data[0][2*y0 *      pict->linesize[0] + 2*x0]   = BLUEY;
          pict->data[0][2*y0 *      pict->linesize[0] + 2*x0+1] = BLUEY;
          pict->data[0][(2*y0+1) *  pict->linesize[0] + 2*x0]   = BLUEY;
          pict->data[0][(2*y0+1) *  pict->linesize[0] + 2*x0+1] = BLUEY;
          pict->data[1][y0 * pict->linesize[1] + x0] = BLUEU;
          pict->data[2][y0 * pict->linesize[2] + x0] = BLUEV;
          y0--;
        }  
      }          
    }
    #endif 
#endif
#if 0
    Xmid=800;Ymid=800;
    for (y = 0; y < height; y++)
      for (x = 0; x < width; x++) {
//       if(x>Xmax-10&&x<Xmax+10&&y>Ymax-10&&y<Ymax+10) {
         if(x>Xmid-100&&x<Xmid+100&&y>Ymid-100&&y<Ymid+100) {
           pict->data[0][y * pict->linesize[0] + x] = 0x0;
         }
         else {
           pict->data[0][y * pict->linesize[0] + x] = 0x0;
         }  
      }    
    /* Cb and Cr */
    for (y = 0; y < height/2; y++) {
       for (x = 0; x < width/2; x++) {  
//       if(x>Xmax/2-10&&x<Xmax/2+10&&y>Ymax/2-10&&y<Ymax/2+10) {
         if(x>Xmid/2-100&&x<Xmid/2+100&&y>Ymid/2-100&&y<Ymid/2+100) {
              pict->data[1][y * pict->linesize[1] + x] = 0;
              pict->data[2][y * pict->linesize[2] + x] = 0xFF;
          }
          else {
              pict->data[1][y * pict->linesize[1] + x] = 0x0;
              pict->data[2][y * pict->linesize[2] + x] = 0x0;
          }    
       }
    }
#endif     
#if 0
    if (frame_index > 0) {
      printf("fill_yuv_image %s(%d)\n",__FILE__,__LINE__);
      for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {
          pict->data[0][y * pict->linesize[0] + x] = abs(Ybefore[y * Ylinesize + x]-
                                                     filt_frame->data[0][y * Ylinesize + x]);
        }
      }  
      for (y = 0; y < height/2; y++) {
        for (x = 0; x < width/2; x++) {
          pict->data[1][y * pict->linesize[1] + x] = abs(Ubefore[y * Ulinesize + x]-
                                                     filt_frame->data[1][y * Ulinesize + x]);
          pict->data[2][y * pict->linesize[2] + x] = abs(Vbefore[y * Vlinesize + x]-
                                                     filt_frame->data[2][y * Vlinesize + x]);          
        }
      }        
    }
    else {
      printf("fill_yuv_image %s(%d)\n",__FILE__,__LINE__);
      for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {
          pict->data[0][y * pict->linesize[0] + x] = filt_frame->data[0][y * filt_frame->linesize[0] + x];
        }
      }  
      for (y = 0; y < height/2; y++) {
        for (x = 0; x < width/2; x++) {
          pict->data[1][y * pict->linesize[1] + x] = filt_frame->data[1][y * filt_frame->linesize[1] + x];
          pict->data[2][y * pict->linesize[2] + x] = filt_frame->data[2][y * filt_frame->linesize[2] + x];          
        }
      }        
    }
#endif
#if 0
    if (frame_index>0) {
      printf("fill_yuv_image %s(%d),%3d,(%4d,%4d,%4d)\n",__FILE__,__LINE__,frame_index,
             Ylinesize,Ulinesize,Vlinesize);
      for (y = 0; y < height; y++) {
        y2 = y/2;
        for (x = 0; x < width; x++) {
          x2 = x/2;
          g = abs(Ybefore[y * Ylinesize + x]-Ydiffnow[y * Ylinesize + x]);
          
          Y0 = Ybefore[y *   Ylinesize + x];
          U0 = Ubefore[y2 *  Ulinesize + x2];
          V0 = Vbefore[y2 *  Vlinesize + x2];
          Y1 = Ydiffnow[y *  Ylinesize + x];
          U1 = Udiffnow[y2 * Ulinesize + x2];
          V1 = Vdiffnow[y2 * Vlinesize + x2];
          G0 = YUV2G(Y0,U0,V0); if(G0<64) G0=0; else G0=255;
          G1 = YUV2G(Y1,U1,V1); if(G1<64) G1=0; else G1=255;
          
          g = abs(G0-G1);
          
          if(g > 250) {
            pict->data[0][y *  pict->linesize[0] + x]  = BLACKY;                
            pict->data[1][y2 * pict->linesize[1] + x2] = BLACKU;
            pict->data[2][y2 * pict->linesize[2] + x2] = BLACKV;
          }  
          else {
            pict->data[0][y *  pict->linesize[0] + x]  = WHITEY;                
            pict->data[1][y2 * pict->linesize[1] + x2] = WHITEU;
            pict->data[2][y2 * pict->linesize[2] + x2] = WHITEV;
          }  
        }
      }  
    }
    else {
      printf("fill_yuv_image %s(%d),%3d\n",__FILE__,__LINE__,frame_index);
      for (y = 0; y < height; y++) {
     //   printf("fill_yuv_image %s(%d) (%4d,%4d),%4d,%4d,%4d\n",__FILE__,__LINE__,x,y,
     //          Ylinesize,Ulinesize,Vlinesize);
        y2 = y/2;
        for (x = 0; x < width; x++) {
          x2 = x/2;
          pict->data[0][y *  pict->linesize[0] + x]  = Ydiffnow[y *  Ylinesize + x];
          pict->data[1][y2 * pict->linesize[1] + x2] = Udiffnow[y2 * Ulinesize + x2];
          pict->data[2][y2 * pict->linesize[2] + x2] = Vdiffnow[y2 * Vlinesize + x2];          
        }
      }        
    }
#endif
#if 0
    for (y = 0; y < height; y++)  {
        y2 = y/2;
        for (x = 0; x < width; x++) {
            x2 = x/2;
#if 0       
            pict->data[0][pict->linesize[0] + x]       = filt_frame->data[0][y *  filt_frame->linesize[0] + x];
            pict->data[1][pict->linesize[1] + x2]      = filt_frame->data[1][y2 * filt_frame->linesize[1] + x2];
            pict->data[2][pict->linesize[2] + x2]      = filt_frame->data[2][y2 * filt_frame->linesize[2] + x2];
#else
            pict->data[0][y *  pict->linesize[0] + x]  = Ydiffnow[y *  Ylinesize + x];
            pict->data[1][y2 * pict->linesize[1] + x2] = Udiffnow[y2 * Ulinesize + x2];
            pict->data[2][y2 * pict->linesize[2] + x2] = Vdiffnow[y2 * Vlinesize + x2];
#endif
        }
    }       
#endif    
#if 0
    for (y = 0; y < height; y++)  {
        y2 = y/2;
        for (x = 0; x < width; x++) {
            x2 = x/2;
            r = Rnow[y *  pict->linesize[0] + x];
            g = Gnow[y *  pict->linesize[0] + x];
            b = Bnow[y *  pict->linesize[0] + x];
            Y0 = RGB2Y(r,g,b);
            U0 = RGB2U(r,g,b);
            V0 = RGB2V(r,g,b);            
            pict->data[0][y *  pict->linesize[0] + x]  = Y0;
            pict->data[1][y2 * pict->linesize[1] + x2] = U0;
            pict->data[2][y2 * pict->linesize[2] + x2] = V0;
        }
    }       
#endif
#if 0
    printf("%s(%d)\n",__FILE__,__LINE__);
    calc_ref(frame_index,width,height);
    printf("%s(%d) (%4d,%4d,%4d)\n",__FILE__,__LINE__,frame_index,width,height);
    for (y = 0; y < height; y++)  {
        y2 = y/2;
        for (x = 0; x < width; x++) {
            x2 = x/2;
 //         printf("%s(%d),(%4d,%4d,%4d,%4d))\n",__FILE__,__LINE__,x,y,x2,y2);
            pict->data[0][y *  pict->linesize[0] + x]  = Yref[y *   Ylinesize + x];
            pict->data[1][y2 * pict->linesize[1] + x2] = Uref[y2 *  Ulinesize + x2];
            pict->data[2][y2 * pict->linesize[2] + x2] = Vref[y2 *  Vlinesize + x2];
        }
    }       
#endif 
#if 0
    if (frame_index>0) {
      printf("fill_yuv_image %s(%d),%3d,(%4d,%4d,%4d)\n",__FILE__,__LINE__,frame_index,
             Ylinesize,Ulinesize,Vlinesize);
      for (y = 0; y < height; y++) {
        y2 = y/2;
        for (x = 0; x < width; x++) {
          x2 = x/2;
          calc_nb(x,y);
          
          R1 = rn[1][1]; G1 = gn[1][1]; B1 = bn[1][1];
          R0 = rb[1][1]; G0 = gb[1][1]; B0 = bb[1][1];
          #if 1
          pict->data[0][y *  pict->linesize[0] + x]  = RGB2Y(R1,G1,B1);
          pict->data[1][y2 * pict->linesize[1] + x2] = RGB2U(R1,G1,B1);
          pict->data[2][y2 * pict->linesize[2] + x2] = RGB2V(R1,G1,B1);
          #endif
          #if 0
          pict->data[0][y *  pict->linesize[0] + x]  = RGB2Y(R0,G0,B0);
          pict->data[1][y2 * pict->linesize[1] + x2] = RGB2U(R0,G0,B0);
          pict->data[2][y2 * pict->linesize[2] + x2] = RGB2V(R0,G0,B0);
          #endif          
          #if 1
          g=abs(a(G0)-a(G1));
#if 0
          if (ge>8)
          printf("%s(%d) (%4d,%4d),(%4d,%4d),(%4d,%4d),(%4d,%4d),%4d\n",
                 __FILE__,__LINE__,y,x,gb[1][1],gn[1][1],G0,G1,g,ga,Y1);                       
#endif      
         if(ge> 8 && (abs(a(G1)-a(G0)) > 8 || abs(a(B1)-a(B0)) > 8)  ) { //變動且是邊界
            pict->data[0][y *  pict->linesize[0] + x]  = BLACKY;                
            pict->data[1][y2 * pict->linesize[1] + x2] = BLACKU;
            pict->data[2][y2 * pict->linesize[2] + x2] = BLACKV;
          }  
          else {
            pict->data[0][y *  pict->linesize[0] + x]  = WHITEY;                
            pict->data[1][y2 * pict->linesize[1] + x2] = WHITEU;
            pict->data[2][y2 * pict->linesize[2] + x2] = WHITEV;
          }
          #endif
        }  
      }
    }
    else {
      printf("fill_yuv_image %s(%d),%3d\n",__FILE__,__LINE__,frame_index);
      for (y = 0; y < height; y++) {
     //   printf("fill_yuv_image %s(%d) (%4d,%4d),%4d,%4d,%4d\n",__FILE__,__LINE__,x,y,
     //          Ylinesize,Ulinesize,Vlinesize);
        y2 = y/2;
        for (x = 0; x < width; x++) {
          x2 = x/2;
          #if 0 
          pos = y *  pict->linesize[0] + x;
          R1 = Rnow[pos]; G1 = Gnow[pos]; B1 = Bnow[pos];
          pict->data[0][y *  pict->linesize[0] + x]  = RGB2Y(R1,G1,B1);
          pict->data[1][y2 * pict->linesize[1] + x2] = RGB2U(R1,G1,B1);
          pict->data[2][y2 * pict->linesize[2] + x2] = RGB2V(R1,G1,B1);
          #endif
          #if 1
          pict->data[0][y *  pict->linesize[0] + x]  = Ydiffnow[y *  Ylinesize + x];
          pict->data[1][y2 * pict->linesize[1] + x2] = Udiffnow[y2 * Ulinesize + x2];
          pict->data[2][y2 * pict->linesize[2] + x2] = Vdiffnow[y2 * Vlinesize + x2];
          #endif          
        }
      }        
    }
#endif
    
}
