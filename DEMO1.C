#include <stdio.h>
#include <string.h>

#include "vbe.h"
#include "gfx.h"

  char demo1_WaitKey( void );
  #pragma aux demo1_WaitKey=\
  "  xor     ax, ax"\
  "  int     0x16"\
  value[al];

  void demo1_Run8( uint32_t p_width, uint32_t p_height )
  {
    vbectx_t* l_ctx;
    pal4_t    l_pal;
    int32_t   l_centerx;
    int32_t   l_centery;
    uint32_t  l_mode;

    l_mode = vbe_FindMode(p_width, p_height, 8);

    l_ctx = vbe_SetMode( l_mode );
    if( l_ctx )
    {
      vbe_GetPal( l_ctx, l_pal );

      l_pal[253].r = 16;
      l_pal[253].g = 32;
      l_pal[253].b = 64;

      l_pal[254].r = 128;
      l_pal[254].g = 64;
      l_pal[254].b = 32;

      l_pal[255].r = 64;
      l_pal[255].g = 128;
      l_pal[255].b = 255;
      vbe_SetPal( l_ctx, l_pal );

      gfx_Clear8( l_ctx, 255 );

      l_centerx = l_ctx->width / 2;
      l_centery = l_ctx->height / 2;

      vbe_SetClip( l_ctx, l_centerx - 100, l_centery - 100,
        l_centerx + 100, l_centery + 100 );

      gfx_FillRect8( l_ctx, 0, 0,
        l_ctx->width - 1, l_ctx->height - 1, 253 );

      gfx_SetPixel8( l_ctx, l_centerx - 2, l_centery + 0, 254 );
      gfx_SetPixel8( l_ctx, l_centerx - 1, l_centery + 0, 254 );
      gfx_SetPixel8( l_ctx, l_centerx + 0, l_centery + 0, 254 );
      gfx_SetPixel8( l_ctx, l_centerx + 1, l_centery + 0, 254 );
      gfx_SetPixel8( l_ctx, l_centerx + 2, l_centery + 0, 254 );
      gfx_SetPixel8( l_ctx, l_centerx + 0, l_centery - 2, 254 );
      gfx_SetPixel8( l_ctx, l_centerx + 0, l_centery - 1, 254 );
      gfx_SetPixel8( l_ctx, l_centerx + 0, l_centery + 0, 254 );
      gfx_SetPixel8( l_ctx, l_centerx + 0, l_centery + 1, 254 );
      gfx_SetPixel8( l_ctx, l_centerx + 0, l_centery + 2, 254 );

      demo1_WaitKey();

      vbe_CloseMode( &l_ctx );
    }
    else
    {
      printf( "VBE mode %#.4x not supported\n", l_mode );
      printf( "\n" );
    }
  }

  void demo1_Run15( uint32_t p_width, uint32_t p_height )
  {
    vbectx_t* l_ctx;
    int32_t   l_centerx;
    int32_t   l_centery;
    uint32_t  l_pixel;
    uint32_t  l_mode;

    l_mode = vbe_FindMode(p_width, p_height, 15);

    l_ctx = vbe_SetMode( l_mode );
    if( l_ctx )
    {
      l_pixel = vbe_EncodePixel(l_ctx, 64, 128, 255);
      gfx_Clear16( l_ctx, l_pixel );

      l_centerx = l_ctx->width / 2;
      l_centery = l_ctx->height / 2;

      vbe_SetClip( l_ctx, l_centerx - 100, l_centery - 100,
        l_centerx + 100, l_centery + 100 );

      l_pixel = vbe_EncodePixel(l_ctx, 16, 32, 64 );
      gfx_FillRect16( l_ctx, 0, 0,
        l_ctx->width - 1, l_ctx->height - 1, l_pixel );

      l_pixel = vbe_EncodePixel(l_ctx, 128, 64, 32);
      gfx_SetPixel16( l_ctx, l_centerx - 2, l_centery + 0, l_pixel );
      gfx_SetPixel16( l_ctx, l_centerx - 1, l_centery + 0, l_pixel );
      gfx_SetPixel16( l_ctx, l_centerx + 0, l_centery + 0, l_pixel );
      gfx_SetPixel16( l_ctx, l_centerx + 1, l_centery + 0, l_pixel );
      gfx_SetPixel16( l_ctx, l_centerx + 2, l_centery + 0, l_pixel );
      gfx_SetPixel16( l_ctx, l_centerx + 0, l_centery - 2, l_pixel );
      gfx_SetPixel16( l_ctx, l_centerx + 0, l_centery - 1, l_pixel );
      gfx_SetPixel16( l_ctx, l_centerx + 0, l_centery + 0, l_pixel );
      gfx_SetPixel16( l_ctx, l_centerx + 0, l_centery + 1, l_pixel );
      gfx_SetPixel16( l_ctx, l_centerx + 0, l_centery + 2, l_pixel );

      demo1_WaitKey();

      vbe_CloseMode( &l_ctx );
    }
    else
    {
      printf( "VBE mode %#.4x not supported\n", l_mode );
      printf( "\n" );
    }
  }

  void demo1_Run16( uint32_t p_width, uint32_t p_height )
  {
    vbectx_t* l_ctx;
    int32_t   l_centerx;
    int32_t   l_centery;
    uint32_t  l_pixel;
    uint32_t  l_mode;

    l_mode = vbe_FindMode(p_width, p_height, 16);

    l_ctx = vbe_SetMode( l_mode );
    if( l_ctx )
    {
      l_pixel = vbe_EncodePixel(l_ctx, 64, 128, 255);

      gfx_Clear16( l_ctx, l_pixel );

      l_centerx = l_ctx->width / 2;
      l_centery = l_ctx->height / 2;

      vbe_SetClip( l_ctx, l_centerx - 100, l_centery - 100,
        l_centerx + 100, l_centery + 100 );

      l_pixel = vbe_EncodePixel(l_ctx, 16, 32, 64 );
      gfx_FillRect16( l_ctx, 0, 0,
        l_ctx->width - 1, l_ctx->height - 1, l_pixel );

      l_pixel = vbe_EncodePixel(l_ctx, 128, 64, 32);
      gfx_SetPixel16( l_ctx, l_centerx - 2, l_centery + 0, l_pixel );
      gfx_SetPixel16( l_ctx, l_centerx - 1, l_centery + 0, l_pixel );
      gfx_SetPixel16( l_ctx, l_centerx + 0, l_centery + 0, l_pixel );
      gfx_SetPixel16( l_ctx, l_centerx + 1, l_centery + 0, l_pixel );
      gfx_SetPixel16( l_ctx, l_centerx + 2, l_centery + 0, l_pixel );
      gfx_SetPixel16( l_ctx, l_centerx + 0, l_centery - 2, l_pixel );
      gfx_SetPixel16( l_ctx, l_centerx + 0, l_centery - 1, l_pixel );
      gfx_SetPixel16( l_ctx, l_centerx + 0, l_centery + 0, l_pixel );
      gfx_SetPixel16( l_ctx, l_centerx + 0, l_centery + 1, l_pixel );
      gfx_SetPixel16( l_ctx, l_centerx + 0, l_centery + 2, l_pixel );

      demo1_WaitKey();

      vbe_CloseMode( &l_ctx );
    }
    else
    {
      printf( "VBE mode %#.4x not supported\n", l_mode );
      printf( "\n" );
    }
  }

  void demo1_Run24( uint32_t p_width, uint32_t p_height )
  {
    vbectx_t* l_ctx;
    int32_t   l_centerx;
    int32_t   l_centery;
    uint32_t  l_pixel;
    uint32_t  l_mode;

    l_mode = vbe_FindMode(p_width, p_height, 24);

    l_ctx = vbe_SetMode( l_mode );
    if( l_ctx )
    {
      l_pixel = vbe_EncodePixel(l_ctx, 64, 128, 255);

      l_centerx = l_ctx->width / 2;
      l_centery = l_ctx->height / 2;

      vbe_SetClip( l_ctx, l_centerx - 100, l_centery - 100,
        l_centerx + 100, l_centery + 100 );

      gfx_Clear24( l_ctx, l_pixel );

      l_pixel = vbe_EncodePixel(l_ctx, 16, 32, 64 );
      gfx_FillRect24( l_ctx, 0, 0,
        l_ctx->width - 1, l_ctx->height - 1, l_pixel );

      l_pixel = vbe_EncodePixel(l_ctx, 128, 64, 32);
      gfx_SetPixel24( l_ctx, l_centerx - 2, l_centery + 0, l_pixel );
      gfx_SetPixel24( l_ctx, l_centerx - 1, l_centery + 0, l_pixel );
      gfx_SetPixel24( l_ctx, l_centerx + 0, l_centery + 0, l_pixel );
      gfx_SetPixel24( l_ctx, l_centerx + 1, l_centery + 0, l_pixel );
      gfx_SetPixel24( l_ctx, l_centerx + 2, l_centery + 0, l_pixel );
      gfx_SetPixel24( l_ctx, l_centerx + 0, l_centery - 2, l_pixel );
      gfx_SetPixel24( l_ctx, l_centerx + 0, l_centery - 1, l_pixel );
      gfx_SetPixel24( l_ctx, l_centerx + 0, l_centery + 0, l_pixel );
      gfx_SetPixel24( l_ctx, l_centerx + 0, l_centery + 1, l_pixel );
      gfx_SetPixel24( l_ctx, l_centerx + 0, l_centery + 2, l_pixel );

      demo1_WaitKey();

      vbe_CloseMode( &l_ctx );
    }
    else
    {
      printf( "VBE mode %#.4x not supported\n", l_mode );
      printf( "\n" );
    }
  }

  void demo1_Run32( uint32_t p_width, uint32_t p_height )
  {
    vbectx_t* l_ctx;
    int32_t   l_centerx;
    int32_t   l_centery;
    uint32_t  l_pixel;
    uint32_t  l_mode;

    l_mode = vbe_FindMode(p_width, p_height, 32);

    l_ctx = vbe_SetMode( l_mode );
    if( l_ctx )
    {
      l_pixel = vbe_EncodePixel(l_ctx, 64, 128, 255);

      l_centerx = l_ctx->width / 2;
      l_centery = l_ctx->height / 2;

      vbe_SetClip( l_ctx, l_centerx - 100, l_centery - 100,
        l_centerx + 100, l_centery + 100 );

      gfx_Clear32( l_ctx, l_pixel );

      l_pixel = vbe_EncodePixel(l_ctx, 16, 32, 64 );
      gfx_FillRect32( l_ctx, 0, 0,
        l_ctx->width - 1, l_ctx->height - 1, l_pixel );

      l_pixel = vbe_EncodePixel(l_ctx, 128, 64, 32);
      gfx_SetPixel32( l_ctx, l_centerx - 2, l_centery + 0, l_pixel );
      gfx_SetPixel32( l_ctx, l_centerx - 1, l_centery + 0, l_pixel );
      gfx_SetPixel32( l_ctx, l_centerx + 0, l_centery + 0, l_pixel );
      gfx_SetPixel32( l_ctx, l_centerx + 1, l_centery + 0, l_pixel );
      gfx_SetPixel32( l_ctx, l_centerx + 2, l_centery + 0, l_pixel );
      gfx_SetPixel32( l_ctx, l_centerx + 0, l_centery - 2, l_pixel );
      gfx_SetPixel32( l_ctx, l_centerx + 0, l_centery - 1, l_pixel );
      gfx_SetPixel32( l_ctx, l_centerx + 0, l_centery + 0, l_pixel );
      gfx_SetPixel32( l_ctx, l_centerx + 0, l_centery + 1, l_pixel );
      gfx_SetPixel32( l_ctx, l_centerx + 0, l_centery + 2, l_pixel );

      demo1_WaitKey();

      vbe_CloseMode( &l_ctx );
    }
    else
    {
      printf( "VBE mode %#.4x not supported\n", l_mode );
      printf( "\n" );
    }
  }

int main( int argc, char* argv[] )
{
  uint32_t l_demoWidth;
  uint32_t l_demoHeight;

  if( vbe_Init() == 0 )
  {
    printf( "ERROR: VBE driver not supported.\n" );

    return 0;
  }

  l_demoWidth  = 640;
  l_demoHeight = 480;

  demo1_Run8( l_demoWidth, l_demoHeight );

  demo1_Run15( l_demoWidth, l_demoHeight );

  demo1_Run16( l_demoWidth, l_demoHeight );

  demo1_Run24( l_demoWidth, l_demoHeight );

  demo1_Run32( l_demoWidth, l_demoHeight );

  vbe_Done( NULL );

  return 0;
}
