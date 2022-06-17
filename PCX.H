#ifndef __PCX_H
#define __PCX_H

  #include <stdint.h>
  #include <stdio.h>

  #include "vbe.h"
  #include "gfx.h"

  /*
   *  PCX declarations, header, and loading structure
   */
  enum _pcxmfg_t
  {
    pcxMfgZSoft = 10
  };
  typedef enum _pcxmfg_t pcxmfg_t;

  enum _pcxver_t
  {
    pcxVer25        = 0,
    pcxVer28WithPal = 2,
    pcxVer28NoPal   = 3,
    pcxVerPCPBWin   = 4,
    pcxVer30        = 5
  };
  typedef enum _pcxver_t pcxver_t;

  enum _pcxenc_t
  {
    pcxEncRLE = 1
  };
  typedef enum _pcxenc_t pcxenc_t;

  enum _pcxpalinfo_t
  {
    pcxPalColorBW   = 1,
    pcxPalGrayscale = 2
  };
  typedef enum _pcxpalinfo_t pcxpalinfo_t;

  #pragma pack( push, 1 )
  struct _pcxhdr_t
  {
    uint8_t  Manufacturer;
    uint8_t  Version;
    uint8_t  Encoding;
    uint8_t  BitsPerPixel;
    int16_t  XMin;
    int16_t  YMin;
    int16_t  XMax;
    int16_t  YMax;
    uint16_t HDpi;
    uint16_t VDpi;
    rgb_t    Colormap[16];
    uint8_t  Reserved;
    uint8_t  NPlanes;
    uint16_t BytesPerLine;
    uint16_t PaletteInfo;
    uint16_t HScreenSize;
    uint16_t VScreenSize;
    uint8_t  Filler[54];
  };
  typedef struct _pcxhdr_t pcxhdr_t;

  #pragma pack( pop )
  struct _pcx_t
  {
    FILE*    pcxFile;
    uint8_t* lineBuffer;
    pal4_t   pal;
    uint32_t width;
    uint32_t height;
    uint32_t linewidth;
    uint32_t linesize;
    uint8_t  bpp;
  };
  typedef struct _pcx_t pcx_t;

  /*
   *  PCX loading functions
   */
  int pcx_LoadLine8( pcx_t* p_pcx );
  int pcx_LoadLine24( pcx_t* p_pcx );

  int pcx_LoadPal( pcx_t* p_pcx );

  pcx_t* pcx_Open( const char* p_name );
  void pcx_Close( pcx_t** p_pcx );

  image_t* pcx_LoadImage( vbectx_t* p_ctx, const char* p_name,
    rgba_t* p_pal );

#endif
