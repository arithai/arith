//toolbox.c ArithAI Yang 2025.01.17
//Purpose: Graphics Draw ToolBox
//礶,礶絬 fill_yuv_image AVFrame *pict; frame_index 
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
void ptSet(unsigned short x,unsigned short y,int weight);
void ptFree();
void ptListAll();
#define maxFirst 32
extern ST_POINT_LIST *ptFirst;
void ptGetFirst();
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
extern bool marginal(int x,int y);
extern int HXYV[60][60];
extern int HXYY[30][30];
extern int HXYU[30][30];
extern int EX[3840];
extern int EY[3840];
void calc_64x36(AVFrame *pict, int frame_index,
                int width, int height);
                //娩絫,,狠翴,癬翴,沧翴
#define edgeSpace      1               //オ娩絫ぃ衡
#define maxDeltaSpace  1               //程ぃ琌絬フ
#define minPointNrLine 6               //程ぶ絬翴计
typedef struct STLINE {
  short c0;  //0..3840-1 =coordinate
  short c1;  //0..65535
  int w;     //翴计
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
  int w;     //翴计
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

void fill_yuv_image(AVFrame *pict, int frame_index,
                           int width, int height)
{
    int x, y, i;
    int x0, y0;
    int x30,y30,xs60,ys60;
#if 0
    int Xmid=(Xleft+Xright)/2;
    int Ymid=(Yup+Ydown)/2;
#endif    
    xs60=width/60;
    ys60=height/60;
    i = frame_index; i = i;
    printf("h,w=(%d,%d),(%d,%d,%d),(%d,%d,%d)\n",height,width, 
             pict->linesize[0],pict->linesize[1],pict->linesize[2],
             filt_frame->linesize[0],filt_frame->linesize[1],filt_frame->linesize[2]
           );
    ptGetFirst();       
    printf("%s(%d) (%d,%d)\n",__FILE__,__LINE__,xs60,ys60);
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
    for (y = 0; y < height; y++) {
      y30=y/ys60;
      y30=y30;
      if((y/2)==Sy0) {
//      exit(0);
      }
      for (x = 0; x < width; x++) {
        x30=x/xs60;
        x30=x30; 
        #if 0
//      if(x>Xmax-10&&x<Xmax+10&&y>Ymax-10&&y<Ymax+10) {
        if(x>Xmid-10&&x<Xmid+10&&y>Ymid-10&&y<Ymid+10) {
          pict->data[0][y * pict->linesize[0] + x] = 0x0;
        }
        #endif
        #if 1
        if(    (y/2 >= YposLine && y/2 < (YposLine+5) && x/2>=Yc0 && x/2<=Yc1) 
            || (x/2 >= XposLine && x/2 < (XposLine+5) && y/2>=Xc0 && y/2<=Xc1)
            || (x/2 >= ZposLine && x/2 < (ZposLine+5) && y/2>=Zc0 && y/2<=Zc1)
            ) {
          pict->data[0][y * pict->linesize[0] + x] = 0xFF;
        }
        #endif
        #if 1
        else if(marginal(x/2,y/2) || HXYV[x30/2][y30/2] >= ptFirst->pt.w) {
          pict->data[0][y * pict->linesize[0] + x] = 0x0;
        }
        #endif
        #if 1
//      printf("%s(%d) %X,%X (%d,%d)\n",__FILE__,__LINE__,ptHead,ptFirst,x30,y30);
        else if(HXYV[x30/2][y30/2] >= ptFirst->pt.w ) {
          pict->data[0][y * pict->linesize[0] + x] = 0x0;
        }
        #endif  
        else {
          pict->data[0][y * pict->linesize[0] + x] = filt_frame->data[0][y * filt_frame->linesize[0] + x];
        }  
      }    
    }  
    printf("%s(%d)[%4d],(%d,%d,%d)(%d,%d,%d)(%d,%d,%d)(%d,%d,%d,%d)(%d,%d,%d,%d)\n",
    __FILE__,__LINE__,
    frame_index,XposLine,Xc0,Xc1,YposLine,Yc0,Yc1,ZposLine,Zc0,Zc1,
    SposSlope,Sc0,Sc1,Sy0,TposSlope,Tc0,Tc1,Ty0);
    /* Cb and Cr */
    for (y = 0; y < height/2; y++) {
      y30=y/ys60;
 //ST     
      for (x = 0; x < width/2; x++) { 
        x30=x/xs60; 
        #if 0
//      if(x>Xmax/2-10&&x<Xmax/2+10&&y>Ymax/2-10&&y<Ymax/2+10) {
        if(x>Xmid/2-10&&x<Xmid/2+10&&y>Ymid/2-10&&y<Ymid/2+10) {
          pict->data[1][y * pict->linesize[1] + x] = 0xFF;
          pict->data[2][y * pict->linesize[2] + x] = 0x0;
        }
        #endif
        #if 1
        if(    (y >= YposLine && y < (YposLine+5) && x>=Yc0 && x<=Yc1) 
            || (x >= XposLine && x < (XposLine+5) && y>=Xc0 && y<=Xc1)
            || (x >= ZposLine && x < (ZposLine+5) && y>=Zc0 && y<=Zc1)
            ) {
          pict->data[1][y * pict->linesize[1] + x] = 0x00;
          pict->data[2][y * pict->linesize[2] + x] = 0xFF;
          if(frame_index>30) 
          {
//          printf("%s(%d)(%4d)(%3d,%3d,%3d)\n",__FILE__,__LINE__,frame_index,XposLine,YposLine,ZposLine);; exit(0);
          }  
        }
        #endif
        #if 1
        else if(marginal(x,y)) {
          pict->data[1][y * pict->linesize[1] + x] = 0x33;
          pict->data[2][y * pict->linesize[2] + x] = 0x88;
        }
        else if (HXYV[x30][y30] >= ptFirst->pt.w) {
          pict->data[1][y * pict->linesize[1] + x] = 0x22;
          pict->data[2][y * pict->linesize[2] + x] = 0x66;
        }
        #endif           
        #if 1
        else if(HXYV[x30][y30] >= ptFirst->pt.w) {
          pict->data[1][y * pict->linesize[1] + x] = 0xFF;
          pict->data[2][y * pict->linesize[2] + x] = 0x0;
        }
        #endif  
        else {
          pict->data[1][y * pict->linesize[1] + x] = filt_frame->data[1][y * filt_frame->linesize[1] + x];
          pict->data[2][y * pict->linesize[2] + x] = filt_frame->data[2][y * filt_frame->linesize[2] + x];          
        }    
      } 
    }
    for (x0 = Sc0; x0 < Sc1; x0++) {
      y0=(int)(Sy0+x0*ratioS);
      for (y=0;y<5;y++) {
        if(y0<(height/2-1)) {
//        printf("%s(%d)(%4d,%4d)\n",__FILE__,__LINE__,x0,y0);
          pict->data[0][2*y0 *      pict->linesize[0] + 2*x0]   = 29;
          pict->data[0][2*y0 *      pict->linesize[0] + 2*x0+1] = 29;
          pict->data[0][(2*y0+1) *  pict->linesize[0] + 2*x0]   = 29;
          pict->data[0][(2*y0+1) *  pict->linesize[0] + 2*x0+1] = 29;
          pict->data[1][y0 * pict->linesize[1] + x0] = 255;
          pict->data[2][y0 * pict->linesize[2] + x0] = 107;
          y0++;
        }  
      }          
    } 
    for (x0 = Tc0; x0 < Tc1; x0++) {
      y0=(int)(Ty0+x0*ratioT);
      for (y=0;y<5;y++) {
        if(y0>0) {
//        printf("%s(%d)(%4d,%4d)\n",__FILE__,__LINE__,x0,y0);
          pict->data[0][2*y0 *      pict->linesize[0] + 2*x0]   = 29;
          pict->data[0][2*y0 *      pict->linesize[0] + 2*x0+1] = 29;
          pict->data[0][(2*y0+1) *  pict->linesize[0] + 2*x0]   = 29;
          pict->data[0][(2*y0+1) *  pict->linesize[0] + 2*x0+1] = 29;
          pict->data[1][y0 * pict->linesize[1] + x0] = 255;
          pict->data[2][y0 * pict->linesize[2] + x0] = 107;
          y0--;
        }  
      }          
    } 
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
}

