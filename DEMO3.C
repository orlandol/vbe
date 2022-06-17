#include <stdio.h>
#include <string.h>

#include "vbe.h"
#include "gfx.h"
#include "pcx.h"

  /*
   *  Demo runs for 8/15/16/24/32-BPP as follows
   *
   *  A =  8-BPP PCX
   *  B = 24-BPP PCX
   *
   *  --------------
   *  |AA        BB| <-- Non-Clipped
   *  |AA        BB|
   *  |            |
   *  | A        B | <-- Clipped
   *  |            |
   *  --------------
   *
   *  Waits for a keypress
   */

  char demo3_WaitKey( void );
  #pragma aux demo3_WaitKey=\
  "  xor     ax, ax"\
  "  int     0x16"\
  value[al];

  void demo3_Run8( uint32_t p_width, uint32_t p_height )
  {
    vbectx_t* l_ctx;
    image_t*  l_img8;
    image_t*  l_img24;
    pal4_t    l_pal;
    uint32_t  l_mode;
    int32_t   l_clipX1;
    int32_t   l_clipY1;
    int32_t   l_clipX2;
    int32_t   l_clipY2;
    int32_t   l_x;
    int32_t   l_y;
    int       l_i;

    l_mode = vbe_FindMode(p_width, p_height, 8);

    l_ctx = vbe_SetMode( l_mode );
    if( l_ctx )
    {
      // Load 8-BPP and 24-BPP PCX for comparison
      l_img8  = pcx_LoadImage(l_ctx, "FMP8.PCX", l_pal);
      l_img24 = pcx_LoadImage(l_ctx, "FMP24.PCX", NULL);

      // Set VBE palette to PCX palette
      vbe_SetPal( l_ctx, l_pal );

      // Draw 8-BPP PCX to upper left corner
      l_x = 0;
      l_y = 0;
      gfx_PutImage8( l_ctx, l_x, l_y, l_img8 );

      // Draw 24-BPP PCX to upper right corner
      l_x = l_ctx->width - l_img24->width;
      l_y = 0;
      gfx_PutImage8( l_ctx, l_x, l_y, l_img24 );

      // Set clip boundaries for next two images,
      //   based on 8-BPP PCX bounds
      l_clipX1 = 0 + (l_img8->width / 2);
      l_clipY1 = 0 + (l_img8->height / 2);
      l_clipX2 = l_ctx->width - (l_img8->width / 2);
      l_clipY2 = l_ctx->height - (l_img8->height / 2);
      vbe_SetClip( l_ctx,
        l_clipX1, l_clipY1, l_clipX2, l_clipY2 );

      // Draw 8-BPP PCX to lower left corner
      l_x = 0;
      l_y = l_ctx->height - l_img8->height;
      gfx_PutImage8( l_ctx, l_x, l_y, l_img8 );

      // Draw 24-BPP PCX to lower right corner
      l_x = l_ctx->width - l_img24->width;
      l_y = l_ctx->height - l_img24->height;
      gfx_PutImage8( l_ctx, l_x, l_y, l_img24 );

      demo3_WaitKey();

      gfx_FreeImage( &l_img8 );
      gfx_FreeImage( &l_img24 );

      vbe_CloseMode( &l_ctx );
    }
    else
    {
      printf( "VBE mode %#.4x not supported\n", l_mode );
      printf( "\n" );
    }
  }

  void demo3_Run15( uint32_t p_width, uint32_t p_height )
  {
    vbectx_t* l_ctx;
    image_t*  l_img8;
    image_t*  l_img24;
    uint32_t  l_mode;
    int32_t   l_clipX1;
    int32_t   l_clipY1;
    int32_t   l_clipX2;
    int32_t   l_clipY2;
    int32_t   l_x;
    int32_t   l_y;
    int       l_i;

    l_mode = vbe_FindMode(p_width, p_height, 15);

    l_ctx = vbe_SetMode( l_mode );
    if( l_ctx )
    {
      l_img8  = pcx_LoadImage(l_ctx, "FMP8.PCX", NULL);
      l_img24 = pcx_LoadImage(l_ctx, "FMP24.PCX", NULL);

      // Draw 8-BPP PCX to upper left corner
      l_x = 0;
      l_y = 0;
      gfx_PutImage16( l_ctx, l_x, l_y, l_img8 );

      // Draw 24-BPP PCX to upper right corner
      l_x = l_ctx->width - l_img24->width;
      l_y = 0;
      gfx_PutImage16( l_ctx, l_x, l_y, l_img24 );

      // Set clip boundaries for next two images,
      //   based on 8-BPP PCX bounds
      l_clipX1 = 0 + (l_img8->width / 2);
      l_clipY1 = 0 + (l_img8->height / 2);
      l_clipX2 = l_ctx->width - (l_img8->width / 2);
      l_clipY2 = l_ctx->height - (l_img8->height / 2);
      vbe_SetClip( l_ctx,
        l_clipX1, l_clipY1, l_clipX2, l_clipY2 );

      // Draw 8-BPP PCX to lower left corner
      l_x = 0;
      l_y = l_ctx->height - l_img8->height;
      gfx_PutImage16( l_ctx, l_x, l_y, l_img8 );

      // Draw 24-BPP PCX to lower right corner
      l_x = l_ctx->width - l_img24->width;
      l_y = l_ctx->height - l_img24->height;
      gfx_PutImage16( l_ctx, l_x, l_y, l_img24 );

      demo3_WaitKey();

      gfx_FreeImage( &l_img8 );
      gfx_FreeImage( &l_img24 );

      vbe_CloseMode( &l_ctx );
    }
    else
    {
      printf( "VBE mode %#.4x not supported\n", l_mode );
      printf( "\n" );
    }
  }

  void demo3_Run16( uint32_t p_width, uint32_t p_height )
  {
    vbectx_t* l_ctx;
    image_t*  l_img8;
    image_t*  l_img24;
    uint32_t  l_mode;
    int32_t   l_clipX1;
    int32_t   l_clipY1;
    int32_t   l_clipX2;
    int32_t   l_clipY2;
    int32_t   l_x;
    int32_t   l_y;
    int       l_i;

    l_mode = vbe_FindMode(p_width, p_height, 16);

    l_ctx = vbe_SetMode( l_mode );
    if( l_ctx )
    {
      l_img8  = pcx_LoadImage(l_ctx, "FMP8.PCX", NULL);
      l_img24 = pcx_LoadImage(l_ctx, "FMP24.PCX", NULL);

      // Draw 8-BPP PCX to upper left corner
      l_x = 0;
      l_y = 0;
      gfx_PutImage16( l_ctx, l_x, l_y, l_img8 );

      // Draw 24-BPP PCX to upper right corner
      l_x = l_ctx->width - l_img24->width;
      l_y = 0;
      gfx_PutImage16( l_ctx, l_x, l_y, l_img24 );

      // Set clip boundaries for next two images,
      //   based on 8-BPP PCX bounds
      l_clipX1 = 0 + (l_img8->width / 2);
      l_clipY1 = 0 + (l_img8->height / 2);
      l_clipX2 = l_ctx->width - (l_img8->width / 2);
      l_clipY2 = l_ctx->height - (l_img8->height / 2);
      vbe_SetClip( l_ctx,
        l_clipX1, l_clipY1, l_clipX2, l_clipY2 );

      // Draw 8-BPP PCX to lower left corner
      l_x = 0;
      l_y = l_ctx->height - l_img8->height;
      gfx_PutImage16( l_ctx, l_x, l_y, l_img8 );

      // Draw 24-BPP PCX to lower right corner
      l_x = l_ctx->width - l_img24->width;
      l_y = l_ctx->height - l_img24->height;
      gfx_PutImage16( l_ctx, l_x, l_y, l_img24 );

      demo3_WaitKey();

      gfx_FreeImage( &l_img8 );
      gfx_FreeImage( &l_img24 );

      vbe_CloseMode( &l_ctx );
    }
    else
    {
      printf( "VBE mode %#.4x not supported\n", l_mode );
      printf( "\n" );
    }
  }

  void demo3_Run24( uint32_t p_width, uint32_t p_height )
  {
    vbectx_t* l_ctx;
    image_t*  l_img8;
    image_t*  l_img24;
    uint32_t  l_mode;
    int32_t   l_clipX1;
    int32_t   l_clipY1;
    int32_t   l_clipX2;
    int32_t   l_clipY2;
    int32_t   l_x;
    int32_t   l_y;
    int       l_i;

    l_mode = vbe_FindMode(p_width, p_height, 24);

    l_ctx = vbe_SetMode( l_mode );
    if( l_ctx )
    {
      l_img8  = pcx_LoadImage(l_ctx, "FMP8.PCX", NULL);
      l_img24 = pcx_LoadImage(l_ctx, "FMP24.PCX", NULL);

      // Draw 8-BPP PCX to upper left corner
      l_x = 0;
      l_y = 0;
      gfx_PutImage24( l_ctx, l_x, l_y, l_img8 );

      // Draw 24-BPP PCX to upper right corner
      l_x = l_ctx->width - l_img24->width;
      l_y = 0;
      gfx_PutImage24( l_ctx, l_x, l_y, l_img24 );

      // Set clip boundaries for next two images,
      //   based on 8-BPP PCX bounds
      l_clipX1 = 0 + (l_img8->width / 2);
      l_clipY1 = 0 + (l_img8->height / 2);
      l_clipX2 = l_ctx->width - (l_img8->width / 2);
      l_clipY2 = l_ctx->height - (l_img8->height / 2);
      vbe_SetClip( l_ctx,
        l_clipX1, l_clipY1, l_clipX2, l_clipY2 );

      // Draw 8-BPP PCX to lower left corner
      l_x = 0;
      l_y = l_ctx->height - l_img8->height;
      gfx_PutImage24( l_ctx, l_x, l_y, l_img8 );

      // Draw 24-BPP PCX to lower right corner
      l_x = l_ctx->width - l_img24->width;
      l_y = l_ctx->height - l_img24->height;
      gfx_PutImage24( l_ctx, l_x, l_y, l_img24 );

      demo3_WaitKey();

      gfx_FreeImage( &l_img8 );
      gfx_FreeImage( &l_img24 );

      vbe_CloseMode( &l_ctx );
    }
    else
    {
      printf( "VBE mode %#.4x not supported\n", l_mode );
      printf( "\n" );
    }
  }

  void demo3_Run32( uint32_t p_width, uint32_t p_height )
  {
    vbectx_t* l_ctx;
    image_t*  l_img8;
    image_t*  l_img24;
    uint32_t  l_mode;
    int32_t   l_clipX1;
    int32_t   l_clipY1;
    int32_t   l_clipX2;
    int32_t   l_clipY2;
    int32_t   l_x;
    int32_t   l_y;
    int       l_i;

    l_mode = vbe_FindMode(p_width, p_height, 32);

    l_ctx = vbe_SetMode( l_mode );
    if( l_ctx )
    {
      l_img8  = pcx_LoadImage(l_ctx, "FMP8.PCX", NULL);
      l_img24 = pcx_LoadImage(l_ctx, "FMP24.PCX", NULL);

      // Draw 8-BPP PCX to upper left corner
      l_x = 0;
      l_y = 0;
      gfx_PutImage32( l_ctx, l_x, l_y, l_img8 );

      // Draw 24-BPP PCX to upper right corner
      l_x = l_ctx->width - l_img24->width;
      l_y = 0;
      gfx_PutImage32( l_ctx, l_x, l_y, l_img24 );

      // Set clip boundaries for next two images,
      //   based on 8-BPP PCX bounds
      l_clipX1 = 0 + (l_img8->width / 2);
      l_clipY1 = 0 + (l_img8->height / 2);
      l_clipX2 = l_ctx->width - (l_img8->width / 2);
      l_clipY2 = l_ctx->height - (l_img8->height / 2);
      vbe_SetClip( l_ctx,
        l_clipX1, l_clipY1, l_clipX2, l_clipY2 );

      // Draw 8-BPP PCX to lower left corner
      l_x = 0;
      l_y = l_ctx->height - l_img8->height;
      gfx_PutImage32( l_ctx, l_x, l_y, l_img8 );

      // Draw 24-BPP PCX to lower right corner
      l_x = l_ctx->width - l_img24->width;
      l_y = l_ctx->height - l_img24->height;
      gfx_PutImage32( l_ctx, l_x, l_y, l_img24 );

      demo3_WaitKey();

      gfx_FreeImage( &l_img8 );
      gfx_FreeImage( &l_img24 );

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

  demo3_Run8( l_demoWidth, l_demoHeight );

  demo3_Run15( l_demoWidth, l_demoHeight );

  demo3_Run16( l_demoWidth, l_demoHeight );

  demo3_Run24( l_demoWidth, l_demoHeight );

  demo3_Run32( l_demoWidth, l_demoHeight );

  vbe_Done( NULL );

  return 0;
}
