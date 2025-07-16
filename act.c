//act.c ArithAI Yang 2025.01.17
//Purpose:Algebraic/Arithmetics/Artificial/Algorithmic Catgory Theory
//找面,找線 AVFrame *filt_frame; frame_index 
#define _XOPEN_SOURCE 600 /* for usleep */
#include <unistd.h>
#include <stdio.h>
#include <string.h>
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

#define BROWNY 113
#define BROWNU 64
#define BROWNV 138

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
extern ST_POINT_LIST *ptHead,*ptTail,*ptHeadG,*ptTailG;
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
//G is green point,leaves,inside of leaves
void ptSetG(unsigned short x,unsigned short y,int weight) {
//printf("%s(%d)\n",__FILE__,__LINE__);  
  ST_POINT_LIST *ptListG,*ptListGNext,*ptListGTemp;
  ptListGNext=(ST_POINT_LIST *)malloc(sizeof(ST_POINT_LIST));
  ptListGNext->pt.x=x;
  ptListGNext->pt.y=y;
  ptListGNext->pt.w=weight;
  if(ptHeadG==NULL) {
     ptHeadG=ptListGNext;
     ptTailG=ptListGNext;
     ptHeadG->next=NULL;
     return;
  }
//printf("%s(%d)\n",__FILE__,__LINE__);  
  ptListG=ptHeadG;
  while(ptListG!=NULL && ptListG->pt.w>=weight) { //>=稱先到先贏   8,7,7,5
//  printf("%s(%d)\n",__FILE__,__LINE__);  
    if(ptListG->next!=NULL) {
      if(ptListG->next->pt.w<weight) break;
    }
    ptListG = ptListG->next;
  }
//printf("%s(%d) %X %X\n",__FILE__,__LINE__,ptHead,ptList);  
  if(ptListG==ptHeadG) {    //insert in Head
    ptListGNext->next=ptHeadG;
    ptHeadG=ptListGNext;
//  printf("%s(%d) head %X %X\n",__FILE__,__LINE__,ptHead,ptList);  
  }
  else if(ptListG==NULL) { //insert in Tail
    ptTailG->next=ptListGNext;
    ptListGNext->next=NULL;
    ptTailG=ptListGNext;
//  printf("%s(%d) tail %X %X\n",__FILE__,__LINE__,ptHead,ptList);  
  }
  else {                  //insert in middle 
    ptListGTemp=ptListG->next;
    ptListG->next=ptListGNext;
    ptListGNext->next=ptListGTemp;
//  printf("%s(%d) middle %X %X\n",__FILE__,__LINE__,ptHead,ptList);  
  }  
}
void ptFreeG() {
  ST_POINT_LIST *ptListG,*ptListGTemp;
  ptListG=ptHeadG;
  while(ptListG!=NULL) { 
    ptListGTemp=ptListG->next;
    free(ptListG);
    ptListG=ptListGTemp;
  }
  ptHeadG=NULL;
  ptTailG=NULL;
}
void ptListAllG() {
  ST_POINT_LIST *ptListG,*ptListGTemp;
  int x30,y30;
  ptListG=ptHeadG;
  while(ptListG!=NULL) { //
    if(ptListG->pt.w>104159) 
    {
      x30=ptListG->pt.x;y30=ptListG->pt.y;
      x30=x30;y30=y30;
//    printf("%s(%d) (%4d,%4d,%7d)\n",__FILE__,__LINE__,x30,y30,ptList->pt.w);
    }    
    ptListGTemp=ptListG->next;
    ptListG=ptListGTemp;
  }
}
//end G
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

#define maxFirstG 256
ST_POINT_LIST *ptFirstG;
void ptGetFirstG() {
  ST_POINT_LIST *ptListG,*ptListGTemp;
  int x30,y30;
  int i=0;
  ptListG=ptHeadG;
  while(ptListG!=NULL && i<maxFirstG) { 
//  printf("%s(%d) %lX,%d\n",__FILE__,__LINE__,(long)ptListG,i);
    if(ptListG->pt.w>380000) {
      x30=ptListG->pt.x;y30=ptListG->pt.y;
      x30=x30;y30=y30;
//    printf("(%4d,%4d,%7d)\n",x30,y30,ptListG->pt.w);
    }    
    i++;    
    ptListGTemp=ptListG->next;
    ptListG=ptListGTemp;
  }
  ptFirstG=ptListG;
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
extern int frameWidth,frameHeight;
extern unsigned char *Ybefore;
extern unsigned char *Ubefore;
extern unsigned char *Vbefore;
extern unsigned char *Ynow;
extern unsigned char *Unow;
extern unsigned char *Vnow;
extern unsigned char *Rbefore;
extern unsigned char *Gbefore;
extern unsigned char *Bbefore;
extern unsigned char *Ydiffnow;
extern unsigned char *Udiffnow;
extern unsigned char *Vdiffnow;
extern unsigned char *Rnow;
extern unsigned char *Gnow;
extern unsigned char *Bnow;
extern int Ylinesize,Ulinesize,Vlinesize;
bool marginalV(x,y) {
  int diff;
//printf("%s(%d) (%4d,%4d)\n",__FILE__,__LINE__,x,y);
  if(x>0 && y>0) { //v change!
    diff=abs(
       Vnow[(y-1) * filt_frame->linesize[2] + x    ]+
       Vnow[     y* filt_frame->linesize[2] + (x-1)]-
     2*Vnow[     y* filt_frame->linesize[2] + x    ]);
//  printf("%s(%d) diff=%d\n",__FILE__,__LINE__,diff);
    if(diff > MAX_JUMP) 
     return true;  
  } 
  return false;
}
int HXYR[60][60];
int HXYG[60][60];
int HXYB[60][60];
int HXYY[60][60];
int HXYU[30][30];
int HXYV[30][30];
int EX[3840];
int EY[3840];
int HGBR[256*256*256];
int imaxG,imaxB,imaxR; //suppose they are leaves
void calc_64x36(AVFrame *pict, int frame_index,
                int width, int height) { //60x60
  int i,x,y,x2,y2,x30,y30,x60,y60;
  int xs60,ys60;  
  int pos,posU,posV;
  int maxGBR,iGBR;
  ptFree();    
  ptFreeG();    
  memset(HXYR,0,60*60);
  memset(HXYG,0,60*60);
  memset(HXYB,0,60*60);

  memset(HXYY,0,60*60);
  memset(HXYU,0,30*30);
  memset(HXYV,0,30*30);
  memset(EX,  0,3840);
  memset(EY,  0,3840);

  memset(HGBR,0,256*256*256);
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
//i=13268131;printf("%s(%d),(%7d,%7d)<===========\n",__FILE__,__LINE__,i,HGBR[i]);
  for (y = 0; y < height; y++) {
    y60=y/ys60; 
    for (x = 0; x < width; x++) {
      x60=x/xs60;  
      pos=y * Ylinesize + x;
      HXYY[x60][y60]+=Ynow[pos];
      HXYR[x60][y60]+=Rnow[pos];
      if( Bnow[pos] < Gnow[pos] && (  //leaves only
          Rnow[pos] < Gnow[pos] &&
          Gnow[pos] > 96
//        ( (Rnow[pos] < Bnow[pos]) || (2*Rnow[pos] < (Bnow[pos]+Gnow[pos]) ) )
      ) ) { //maybe leaves not  Green tree trunk 
        HXYG[x60][y60]+=Gnow[pos];
        i=Gnow[pos]*65536+Bnow[pos]*256+Rnow[pos];
        HGBR[i]=HGBR[i]+1;
      }
      HXYB[x60][y60]+=Bnow[pos];
    }
  }      
  for (y2 = 0; y2 < height/2; y2++) {
    y30=y2/ys60;
    for (x2 = 0; x2 < width/2; x2++) {
      x30=x2/xs60;  
      HXYU[x30][y30]+=Unow[y2 * Ulinesize + x2];  
      HXYV[x30][y30]+=Vnow[y2 * Vlinesize + x2];  
      if(marginalV(x2,y2)) {
        EX[x2]++;;
        EY[y2]++;;
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
//printf("calc_64x36 (%d,%d) %lX end\n",width,height,(long)ptHead);
//i=13268131;printf("%s(%d),(%7d,%7d)<===========\n",__FILE__,__LINE__,i,HGBR[i]);

  for(y60=5;y60<55;y60++) { //reove border
    for(x60=5;x60<55;x60++) {  //remove border
//    printf("%s(%d)\n",__FILE__,__LINE__);         
      ptSetG((unsigned short) x60,(unsigned short) y60,HXYG[x60][y60]);
      #if 0
      if(HXYG[x60][y30]>380000) 
        printf("(%4d,%4d,%7d)\n",x60*64*2,y60*36*2,HXYG[x60][y60]);
      #endif  
    }
  }
  ptListAllG();
//printf("calc_64x36 (%4d,%4d) %lX end\n",width,height,(long)ptHeadG);
//i=13268131;printf("%s(%d),(%7d,%7d)<===========\n",__FILE__,__LINE__,i,HGBR[i]);

  maxGBR=0;iGBR=0;
  for (i=0;i<256*256*256;i++) {
    if(HGBR[i]>maxGBR) {
      iGBR=i;
      maxGBR=HGBR[i];
    }
  }
  imaxG=(iGBR/65536);
  imaxB=((iGBR/256)%256);
  imaxR=(iGBR%256);
  printf("%s(%d) (%3d,%5d),(%3d,%3d,%3d) <=======\n",
    __FILE__,__LINE__,iGBR,maxGBR,imaxR,imaxG,imaxB);
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

//  if(x==538 &&  y==598)
//    printf("%s(%d)\n",__FILE__,__LINE__);

  for (i=0;i<maxDeltaSpace;i++) {
    x0=x-i;
    y0=y-i;
    if(x0>0 && y0>0) { //v change!

      diff=abs(
        filt_frame->data[0][(y0-1) * filt_frame->linesize[0] + x0    ]+
        filt_frame->data[1][     y0* filt_frame->linesize[1] + (x0-1)]-
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
  int pos,Y,U,V,R,G,B;
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
//    Y =  filt_frame->data[0][y*2 * filt_frame->linesize[0] + x*2];
//    U =  filt_frame->data[1][y * filt_frame->linesize[1] + x];
//    V =  filt_frame->data[2][y * filt_frame->linesize[2] + x];
      pos = y*2 * filt_frame->linesize[0] + x*2;
      R = Rnow[pos]; //YUV2R(Y,U,V);
      G = Gnow[pos]; //YUV2G(Y,U,V);
      B = Bnow[pos]; //YUV2B(Y,U,V);
//    printf("%s(%d) (%4d,%4d) yuv (%3d,%3d,%3d) rgb (%3d,%3d,%3d)\n",__FILE__,__LINE__,
//           x,y,Y,U,V,R,G,B);
//    if(x>410) exit(0);
      R = R; G= G; B = B;  //no use RGB     
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
//  printf("%s(%d)fi=%3d,y=%d,(%d,%d),%d\n",__FILE__,__LINE__,Yfi,YposLine,Yc0,Yc1,Yw);
  
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
//    printf("%s(%d) (%4d,%4d)\n",__FILE__,__LINE__,y,x);
      if(Xfi>1 && x==538 &&  y>597)
//    printf("%s(%d)fi=%3d,x=(%4d,%4d),ypos=%4d,(%d,%d),(%d,countJump=%d)\n",__FILE__,__LINE__,Yfi,x,y,YposLine,c0,c1,w,countJump);        
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
//    if(Xfi>1 && x==538 &&  y==598)
//      printf("%s(%d),fi=%3d,(%4d,%4d),(%4d,%4d)\n",__FILE__,__LINE__,Yfi,x,y,
//             width/2,height/2);
      if(marginalLine(x,y)){
//      if(Xfi>1 && x==538 &&  y==598)
//        printf("%s(%d),fi=%3d,(%4d,%4d),(%4d,%4d)\n",__FILE__,__LINE__,Yfi,x,y,
//               width/2,height/2);
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
//          printf("%s(%d) countJump=%4d %X\n",__FILE__,__LINE__,countJump,lnList);
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
 //             printf("%s(%d)fi=%3d,x=(%4d,%4d),xpos=%4d,(%d,%d),(%d,%d)\n",__FILE__,__LINE__,Xfi,x,y,XposLine,Xc0,Xc1,w,Xw);        
//            if(Xw>100) exit(0);
            }            
          }  
        }
      }
    }
//    printf("%s(%d)fi=%3d,x=(%4d,%4d),xpos=%4d,(%d,%d),(%d,%d)\n",__FILE__,__LINE__,Xfi,x,y,XposLine,Xc0,Xc1,w,Xw);        
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
  printf("%s(%d),fi=%3d,x=%d,(%d,%d),%d\n",__FILE__,__LINE__,Xfi,XposLine,Xc0,Xc1,Xw);  
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
  printf("%s(%d),fi=%3d,z=%d,(%d,%d),%d\n",__FILE__,__LINE__,Xfi,ZposLine,Zc0,Zc1,Zw);  

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
  printf("%s(%d),fi=%3d,s=%d,(%d,%d,%d),%d\n",__FILE__,__LINE__,Sfi,SposSlope,Sc0,Sc1,Sy0,Sw);

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
      //if(Tfi==7)printf("%s(%d)fi=%3d,t=(%4d,%4d),tpos=%4d,(%d,%d),(%d,%d),%d,%d\n",__FILE__,__LINE__,Tfi,x,y,TposSlope,Tc0,Tc1,w,Tw,Ty0,y0);        
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
            //if(Tfi==7)printf("%s(%d)fi=%3d,t=(%4d,%4d),tpos=%4d,(%d,%d),(%d,%d),%d,%d\n",__FILE__,__LINE__,Tfi,x,y,TposSlope,Tc0,Tc1,w,Tw,Ty0,y0);        
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
//b:before n:now
extern int rn[3][3],gn[3][3],bn[3][3];
extern int rb[3][3],gb[3][3],bb[3][3];
extern int re,ge,be,ra,ga,ba;
#undef a(v)
#define a(v) (v>128? 255:0)   
void calc_matrix(int x,int y) {
  int x2,y2,x0,y0,Y,U,V,r11,g11,b11,r11a,g11a,b11a;
  x0 = x-1;if(x0<0) x0=0;
    
  y0 = y-1;if(y0<0) y0=0;      
  x2 = x0/2;
  y2 = y0/2;
  Y=filt_frame->data[0][y0* filt_frame->linesize[0] + x0];
  U=filt_frame->data[1][y2* filt_frame->linesize[1] + x2];
  V=filt_frame->data[2][y2* filt_frame->linesize[2] + x2];
  rn[0][0]=YUV2R(Y,U,V);
  gn[0][0]=YUV2G(Y,U,V);
  bn[0][0]=YUV2B(Y,U,V);  
  
  y0 = y;      
  y2 = y0/2;      
  Y=filt_frame->data[0][y0* filt_frame->linesize[0] + x0];
  U=filt_frame->data[1][y2* filt_frame->linesize[1] + x2];
  V=filt_frame->data[2][y2* filt_frame->linesize[2] + x2];
  rn[0][1]=YUV2R(Y,U,V);
  gn[0][1]=YUV2G(Y,U,V);
  bn[0][1]=YUV2B(Y,U,V);  
  
  y0 = y+1;if(y0==filt_frame->height) y0=filt_frame->height-1;      
  y2 = y0/2;      
  Y=filt_frame->data[0][y0* filt_frame->linesize[0] + x0];
  U=filt_frame->data[1][y2* filt_frame->linesize[1] + x2];
  V=filt_frame->data[2][y2* filt_frame->linesize[2] + x2];
  rn[0][2]=YUV2R(Y,U,V);
  gn[0][2]=YUV2G(Y,U,V);
  bn[0][2]=YUV2B(Y,U,V);
             
  x0 = x;
    
  y0 = y-1;if(y0<0) y0=0;      
  x2 = x0/2;
  y2 = y0/2;      
  Y=filt_frame->data[0][y0* filt_frame->linesize[0] + x0];
  U=filt_frame->data[1][y2* filt_frame->linesize[1] + x2];
  V=filt_frame->data[2][y2* filt_frame->linesize[2] + x2];
  rn[1][0]=YUV2R(Y,U,V);
  gn[1][0]=YUV2G(Y,U,V);
  bn[1][0]=YUV2B(Y,U,V);
  
  y0 = y;      
  y2 = y0/2;      
  Y=filt_frame->data[0][y0* filt_frame->linesize[0] + x0];
  U=filt_frame->data[1][y2* filt_frame->linesize[1] + x2];
  V=filt_frame->data[2][y2* filt_frame->linesize[2] + x2];
  rn[1][1]=YUV2R(Y,U,V);
  gn[1][1]=YUV2G(Y,U,V);
  bn[1][1]=YUV2B(Y,U,V);
      
  Rnow[y*filt_frame->linesize[0]+x]=rn[1][1];
  Gnow[y*filt_frame->linesize[0]+x]=gn[1][1];
  Bnow[y*filt_frame->linesize[0]+x]=bn[1][1];
      
  y0 = y+1;if(y0==filt_frame->height) y0=filt_frame->height-1;      
  y2 = y0/2;      
  Y=filt_frame->data[0][y0* filt_frame->linesize[0] + x0];
  U=filt_frame->data[1][y2* filt_frame->linesize[1] + x2];
  V=filt_frame->data[2][y2* filt_frame->linesize[2] + x2];
  rn[1][2]=YUV2R(Y,U,V);
  gn[1][2]=YUV2G(Y,U,V);
  bn[1][2]=YUV2B(Y,U,V);

  x0 = x+1;if(x0==filt_frame->width) x0=filt_frame->width-1; 
    
  y0 = y-1;if(y0<0) y0=0;      
  x2 = x0/2;
  y2 = y0/2;      
  Y=filt_frame->data[0][y0* filt_frame->linesize[0] + x0];
  U=filt_frame->data[1][y2* filt_frame->linesize[1] + x2];
  V=filt_frame->data[2][y2* filt_frame->linesize[2] + x2];
  rn[2][0]=YUV2R(Y,U,V);
  gn[2][0]=YUV2G(Y,U,V);
  bn[2][0]=YUV2B(Y,U,V);
  
  y0 = y;      
  y2 = y0/2;      
  Y=filt_frame->data[0][y0* filt_frame->linesize[0] + x0];
  U=filt_frame->data[1][y2* filt_frame->linesize[1] + x2];
  V=filt_frame->data[2][y2* filt_frame->linesize[2] + x2];
  rn[2][1]=YUV2R(Y,U,V);
  gn[2][1]=YUV2G(Y,U,V);
  bn[2][1]=YUV2B(Y,U,V);

  y0 = y+1;if(y0==filt_frame->height) y0=filt_frame->height-1;      
  y2 = y0/2;      
  Y=filt_frame->data[0][y0* filt_frame->linesize[0] + x0];
  U=filt_frame->data[1][y2* filt_frame->linesize[1] + x2];
  V=filt_frame->data[2][y2* filt_frame->linesize[2] + x2];
  rn[2][2]=YUV2R(Y,U,V);
  gn[2][2]=YUV2G(Y,U,V);
  bn[2][2]=YUV2B(Y,U,V);
  
  r11=rn[1][1];
  re=abs(r11-rn[0][0])+abs(r11-rn[0][1])+abs(r11-rn[0][2])+
     abs(r11-rn[1][0])                  +abs(r11-rn[1][2])+
     abs(r11-rn[2][0])+abs(r11-rn[2][1])+abs(r11-rn[2][2]);
  g11=gn[1][1];
  ge=abs(g11-gn[0][0])+abs(g11-gn[0][1])+abs(g11-gn[0][2])+
     abs(g11-gn[1][0])                  +abs(g11-gn[1][2])+
     abs(g11-gn[2][0])+abs(g11-gn[2][1])+abs(g11-gn[2][2]);
  b11=bn[1][1];
  be=abs(b11-bn[0][0])+abs(b11-bn[0][1])+abs(b11-bn[0][2])+
     abs(b11-bn[1][0])                  +abs(b11-bn[1][2])+
     abs(b11-bn[2][0])+abs(b11-bn[2][1])+abs(b11-bn[2][2]);  

  r11a=a(r11); 
  ra=abs(r11a-a(rn[0][0]))+abs(r11a-a(rn[0][1]))+abs(r11a-a(rn[0][2]))+
     abs(r11a-a(rn[1][0]))                      +abs(r11a-a(rn[1][2]))+
     abs(r11a-a(rn[2][0]))+abs(r11a-a(rn[2][1]))+abs(r11a-a(rn[2][2]));
  g11a=a(g11); 
  ga=abs(g11a-a(gn[0][0]))+abs(g11a-a(gn[0][1]))+abs(g11a-a(gn[0][2]))+
     abs(g11a-a(gn[1][0]))                      +abs(g11a-a(gn[1][2]))+
     abs(g11a-a(gn[2][0]))+abs(g11a-a(gn[2][1]))+abs(g11a-a(gn[2][2]));
  b11a=a(b11); 
  ba=abs(b11a-a(bn[0][0]))+abs(b11a-a(bn[0][1]))+abs(b11a-a(bn[0][2]))+
     abs(b11a-a(bn[1][0]))                      +abs(b11a-a(bn[1][2]))+
     abs(b11a-a(bn[2][0]))+abs(b11a-a(bn[2][1]))+abs(b11a-a(bn[2][2]));
}

extern int frameWidth,frameHeight;
void calc_nb(int x,int y) {
  int x2,y2,x0,y0,Y,U,V,r11,g11,b11,r11a,g11a,b11a;
  int pos;
  int w,h;

  w = frameWidth; h = frameHeight;
  x0 = x-1;if(x0<0) x0=0;
    
  y0 = y-1;if(y0<0) y0=0;      
  pos = y0*Ylinesize + x0;
  rn[0][0]=Rnow[pos];   gn[0][0]=Gnow[pos];   bn[0][0]=Bnow[pos];  
  rb[0][0]=Rbefore[pos];gb[0][0]=Gbefore[pos];bb[0][0]=Bbefore[pos];  

  
  y0 = y;      
  pos = y0*Ylinesize + x0;
  rn[0][1]=Rnow[pos];   gn[0][1]=Gnow[pos];   bn[0][1]=Bnow[pos];  
  rb[0][1]=Rbefore[pos];gb[0][1]=Gbefore[pos];bb[0][1]=Bbefore[pos];  
  
  y0 = y+1;if(y0==h) y0=h-1;      
  pos = y0*Ylinesize + x0;
  rn[0][2]=Rnow[pos];   gn[0][2]=Gnow[pos];   bn[0][2]=Bnow[pos];  
  rb[0][2]=Rbefore[pos];gb[0][2]=Gbefore[pos];bb[0][2]=Bbefore[pos];  
             
  x0 = x;  
  y0 = y-1;if(y0<0) y0=0;      
  pos = y0*Ylinesize + x0;
  rn[1][0]=Rnow[pos];   gn[1][0]=Gnow[pos];   bn[1][0]=Bnow[pos];  
  rb[1][0]=Rbefore[pos];gb[1][0]=Gbefore[pos];bb[1][0]=Bbefore[pos];  

  y0 = y;      
  pos = y0*Ylinesize + x0;
  rn[1][1]=Rnow[pos];   gn[1][1]=Gnow[pos];   bn[1][1]=Bnow[pos];  
  rb[1][1]=Rbefore[pos];gb[1][1]=Gbefore[pos];bb[1][1]=Bbefore[pos];  

  y0 = y+1;if(y0==h) y0=h-1;      
  pos = y0*Ylinesize + x0;
  rn[1][2]=Rnow[pos];   gn[1][2]=Gnow[pos];   bn[1][2]=Bnow[pos];  
  rb[1][2]=Rbefore[pos];gb[1][2]=Gbefore[pos];bb[1][2]=Bbefore[pos];  

  x0 = x+1;if(x0==w) x0=w-1; 
    
  y0 = y-1;if(y0<0) y0=0;      
  pos = y0*Ylinesize + x0;
  rn[2][0]=Rnow[pos];   gn[2][0]=Gnow[pos];   bn[2][0]=Bnow[pos];  
  rb[2][0]=Rbefore[pos];gb[2][0]=Gbefore[pos];bb[2][0]=Bbefore[pos];  
  
  y0 = y;      
  pos = y0*Ylinesize + x0;
  rn[2][1]=Rnow[pos];   gn[2][1]=Gnow[pos];   bn[2][1]=Bnow[pos];  
  rb[2][1]=Rbefore[pos];gb[2][1]=Gbefore[pos];bb[2][1]=Bbefore[pos];  

  y0 = y+1;if(y0==h) y0=h-1;      
  pos = y0*Ylinesize + x0;
  rn[2][2]=Rnow[pos];   gn[2][2]=Gnow[pos];   bn[2][2]=Bnow[pos];  
  rb[2][2]=Rbefore[pos];gb[2][2]=Gbefore[pos];bb[2][2]=Bbefore[pos];  
  
  r11=rn[1][1];
  re=abs(r11-rn[0][0])+abs(r11-rn[0][1])+abs(r11-rn[0][2])+
     abs(r11-rn[1][0])                  +abs(r11-rn[1][2])+
     abs(r11-rn[2][0])+abs(r11-rn[2][1])+abs(r11-rn[2][2]);
  g11=gn[1][1];
  ge=abs(g11-gn[0][0])+abs(g11-gn[0][1])+abs(g11-gn[0][2])+
     abs(g11-gn[1][0])                  +abs(g11-gn[1][2])+
     abs(g11-gn[2][0])+abs(g11-gn[2][1])+abs(g11-gn[2][2]);
  b11=bn[1][1];
  be=abs(b11-bn[0][0])+abs(b11-bn[0][1])+abs(b11-bn[0][2])+
     abs(b11-bn[1][0])                  +abs(b11-bn[1][2])+
     abs(b11-bn[2][0])+abs(b11-bn[2][1])+abs(b11-bn[2][2]);  

  r11a=a(r11); 
  ra=abs(r11a-a(rn[0][0]))+abs(r11a-a(rn[0][1]))+abs(r11a-a(rn[0][2]))+
     abs(r11a-a(rn[1][0]))                      +abs(r11a-a(rn[1][2]))+
     abs(r11a-a(rn[2][0]))+abs(r11a-a(rn[2][1]))+abs(r11a-a(rn[2][2]));
  g11a=a(g11); 
  ga=abs(g11a-a(gn[0][0]))+abs(g11a-a(gn[0][1]))+abs(g11a-a(gn[0][2]))+
     abs(g11a-a(gn[1][0]))                      +abs(g11a-a(gn[1][2]))+
     abs(g11a-a(gn[2][0]))+abs(g11a-a(gn[2][1]))+abs(g11a-a(gn[2][2]));
  b11a=a(b11); 
  ba=abs(b11a-a(bn[0][0]))+abs(b11a-a(bn[0][1]))+abs(b11a-a(bn[0][2]))+
     abs(b11a-a(bn[1][0]))                      +abs(b11a-a(bn[1][2]))+
     abs(b11a-a(bn[2][0]))+abs(b11a-a(bn[2][1]))+abs(b11a-a(bn[2][2]));
}

extern unsigned char *Yref;
extern unsigned char *Uref;
extern unsigned char *Vref;
//called x edge  ------
typedef struct ST_EDGE {
  int x;
  int y[5]; //y[3]<y[4] up/down 1,2 is left right
  struct ST_EDGE *next; //x0<x1<...
} ST_EDGE;
ST_EDGE *edgeHead=NULL,*edgeTail;
void freeEdge(void) {
  ST_EDGE *edgeList=edgeHead,*edgeTemp;
  while(edgeList!=NULL) {
    edgeTemp=edgeList;
    edgeList=edgeList->next;
    free(edgeTemp);
  }
  edgeHead=NULL;
  edgeTail=NULL;
}
void insertEdgeToHead(ST_EDGE *edgeList) {
  edgeList->next=edgeHead;
  edgeHead=edgeList; 
}
void insertEdgeToTail(ST_EDGE *edgeList) {
  edgeTail->next=edgeList;
  edgeTail=edgeList; //edgeList->next must be NULL
}
void setEdge(int x,int y,int index) { //index 0:LR edge //1:upper //2:down
  ST_EDGE *edgeNext,*edgeList,*edgeTemp;

//printf("%s(%d) (%4d,%4d,%2d)\n",__FILE__,__LINE__,x,y,index);
  edgeNext=(ST_EDGE *)malloc(sizeof(ST_EDGE));
  edgeNext->x=x;
  memset(edgeNext->y,0,5);
  edgeNext->y[index]=y;
  if(edgeHead==NULL) {
     edgeHead=edgeNext;
     edgeTail=edgeNext;
     edgeHead->next=NULL;
     return;
  }
  edgeList=edgeHead;
  while(edgeList!=NULL && edgeList->x<x) { //先到去平均
    if(edgeList->next!=NULL) {
      if(edgeList->next->x>x) break;
    }
    edgeList=edgeList->next;
  }  
//  if(x==321 && edgeList!=NULL) {
//    printf("%s(%d) (%4d,%4d,%4d,%4d,%2d)\n",__FILE__,__LINE__,edgeHead->x,edgeList->x,x,y,index);
//    if(edgeHead==edgeList)
//      printf("%s(%d)\n",__FILE__,__LINE__);
//  }
  if(edgeList==NULL) { //insert to Tail
    edgeTail->next=edgeNext;
    edgeNext->next=NULL;
    edgeTail=edgeNext;
  }
  else if(edgeList->x==x) { //replace or average or upper
//    if(x==321) {
//      printf("%s(%d) replace or average or upper\n",__FILE__,__LINE__);
//    }
//  if(edgeList->y[index]==0) 
     edgeList->y[index]=y;
//  else 
      edgeList->y[index]=(edgeList->y[index]+3*y)/4;
    free(edgeNext); //no use
    edgeNext=NULL;
  }
  else if(x>edgeList->x)  { //insert in next/tail
//    if(x==321) {
//      printf("%s(%d) insert to next/tail\n",__FILE__,__LINE__);
//    }
    edgeTemp=edgeList->next;
    edgeList->next=edgeNext;
    edgeNext->next=edgeTemp;
  } 
  else {
    edgeNext->next=edgeHead;
    edgeHead=edgeNext; 
//    if(x==321) {
//      printf("%s(%d) (%4d,%4d) insert to Head\n",__FILE__,__LINE__,edgeHead->x,edgeHead->next->x);
//    }
  }
}
void setref(int x,int y,unsigned char Y1,unsigned char U1,unsigned char V1) {
  int x2,y2,x4,y4,pos;
  x2=x/2;
  y2=y/2;
  Uref[y2 * Ulinesize + x2] = U1;
  Vref[y2 * Vlinesize + x2] = V1;
  x4=x2*2;
  y4=y2*2; 
  pos=y4 * Ylinesize + x4;
  Yref[pos]    = Y1;   
  Yref[pos+1]  = Y1;
  pos+=Ylinesize;   
  Yref[pos]    = Y1;   
  Yref[pos+1]  = Y1;
}
void DrawEdge(int width,int height,int index) {
  ST_EDGE *edgeList=edgeHead;
  int x0,y0,x1,y1,x2,y2,x3,y3;
  int pos,posU,posV;
  x0=0;x1=0;
  while(edgeList!=NULL) {
    if (edgeList->y[index]>0) {
      if(x1==0) { //first
        x1=edgeList->x;
        y1=edgeList->y[index];
      }
      else {
        x0=x1;
        y0=y1;
        x1=edgeList->x;
        y1=edgeList->y[index];
//      printf("%s(%d)(%4d,%4d)-(%4d,%4d)\n",__FILE__,__LINE__,x0,y0,x1,y1);
//DrawLine
        if(x0>=x1) {
          printf("%s(%d)(%4d,%4d)\n",__FILE__,__LINE__,x0,x1);
          edgeList=edgeList->next; exit(0);
          continue;
        }
        for(x3=x0;x3<=x1;x3++) {
          y3=y0+(y1-y0)*(x3-x0)/(x1-x0);      
          if(y3>0 && y3<height) {
            setref(x3,y3,GREENY,GREENU,GREENV);
          }
        } 
      }
    } 
    edgeList=edgeList->next;
  }
}
#define ax(v) (v>72? 255:0)  
void calc_ref(int frame_index,int width,int height) {
  int g,R1,G1,B1,R0,G0,B0;
  int x, y, x0, y0, x2, y2, x4, y4;
  int x30,y30,xs60,ys60;
  ST_EDGE *edgeList;
  for (y = 0; y < height; y++)  {
    y2 = y/2;
    for (x = 0; x < width; x++) {
      x2 = x/2;
      if(frame_index>0) {
        calc_nb(x,y);
  //frame_index > 0 LBM Lattice Boltzmann method
        R1 = rn[1][1]; G1 = gn[1][1]; B1 = bn[1][1];
        R0 = rb[1][1]; G0 = gb[1][1]; B0 = bb[1][1];
        g=abs(a(G0)-a(G1));
        #if 0
        if(ge> 8 && abs(ax(G1)-ax(G0)) > 8 ) { //變動且是邊界
        #else
        if(marginalV(x2,y2)) {
        #endif
          setref(x,y,BLACKY,BLACKU,BLACKV);
        }  
        else { 
          setref(x,y,WHITEY,WHITEU,WHITEV);
        //setref(x,y,Ynow[y*Ylinesize+x],Unow[y2*Ulinesize + x2],Vnow[y2*Vlinesize + x2]);
        }
      }
      else {
      #if 1
        setref(x,y,Ydiffnow[y*Ylinesize+x],Udiffnow[y2*Ulinesize + x2],Vdiffnow[y2*Vlinesize + x2]);
      #endif
      }
    }
  }
//printf("%s(%d) (%d,%d) (%X,%X)\n",__FILE__,__LINE__,y,x,ptFirst,ptFirstG);

  #if 1
  freeEdge();
  ptGetFirst();       
  ptGetFirstG();       
  xs60=width/60;
  ys60=height/60;
  for (y = 0; y < height; y++)  {
    y2 = y/2;
    y30=y/ys60;
    y30=y30;
    y2=y/2;
    for (x = 0; x < width; x++) {
      x2 = x/2;
      x30=x/xs60;
      x30=x30;
      x2=x/2;
      if(HXYG[x30][y30] >= ptFirstG->pt.w ) {
//      printf("%s(%d)(%4d,%4d) (%6d,%6d) <======\n",__FILE__,__LINE__,y,x,HXYG[x30][y30],ptFirstG->pt.w);
        //128,148 WHITEY also YELLOWY 255
        setref(x,y,REDY,REDU,REDV);
        #define z(s,t) (( Yref[(t) * Ylinesize+ (s)]==BLACKY )?1:0) 
        if(y30>5 && y30<55 && x30>5 && x30 <55 &&
           ((y-y30*ys60)==ys60/2 || (x-x30*xs60)==xs60/2) ) {
          //left
          for (x0=x-xs60;x0>=xs60;x0--) {
            #if 0
            if ( marginalVref(x,y0) ) {
            #endif
            #if 1   
            if( ( z(x0-1,y-1)+z(x0,y-1)+z(x0+1,y-1)
                 +z(x0-1,y)  +z(x0,y  )+z(x0+1,y  )
                 +z(x0-1,y+1)+z(x0,y+1)+z(x0+1,y+1))
               >=4) {
            #endif
              setref(x0,y,PURPLEY,PURPLEU,PURPLEV);
            //printf("%s(%d) (%4d,%4d,%4d)\n",__FILE__,__LINE__,x0,y30*ys60+ys60/2,0);
              setEdge(x0,y,1);
              break;
            }
          }
          //right
          for (x0=x+xs60;x0<width-xs60;x0++) {
            #if 0
            if ( marginalVref(x,y0) ) {
            #endif
            #if 1   
            if( ( z(x0-1,y-1)+z(x0,y-1)+z(x0+1,y-1)
                 +z(x0-1,y)  +z(x0,y  )+z(x0+1,y  )
                 +z(x0-1,y+1)+z(x0,y+1)+z(x0+1,y+1))
               >=4) {
            #endif
              setref(x0,y,PURPLEY,PURPLEU,PURPLEV);
            //printf("%s(%d) (%4d,%4d,%4d)\n",__FILE__,__LINE__,x0,y30*ys60+ys60/2,0);
              setEdge(x0,y,2);
              break;
            }
          } 
          //upper 
          for (y0=y-ys60;y0>=ys60;y0--) {
            #if 0
            if ( marginalVref(x,y0) ) {
            #endif
            #if 1   
            if (( z(x-1,y0-1)+z(x,y0-1)+z(x+1,y0-1)
                 +z(x-1,y0)  +z(x,y0  )+z(x+1,y0  )
                 +z(x-1,y0+1)+z(x,y0+1)+z(x+1,y0+1))
               >=4) {
            #endif
              setref(x,y0,PURPLEY,PURPLEU,PURPLEV);
              setEdge(x,y0,3);
              break;
            }
          }
          //down
          for (y0=y+ys60;y0<height-ys60;y0++) {
            #if 0
            if ( marginalVref(x,y0) ) {
            #endif
            #if 1   
            if((  z(x-1,y0-1)+z(x,y0-1)+z(x+1,y0-1)
                 +z(x-1,y0)  +z(x,y0  )+z(x+1,y0  )
                 +z(x-1,y0+1)+z(x,y0+1)+z(x+1,y0+1))
               >=4) {
            #endif 
              setref(x,y0,BLUEY,BLUEU,BLUEV);
              setEdge(x,y0,4); //down
            //printf("%s(%d)(%4d,%4d)-%4d,(%4d,%4d))\n",__FILE__,__LINE__,y,x,x0,xs60,ys60);
              break;
            }
          }         
        }   
      }
    }
  }

  DrawEdge(width,height,1); //Up
  DrawEdge(width,height,2); //Down
  DrawEdge(width,height,3); //Left
  DrawEdge(width,height,4); //Right
  #endif  
}


