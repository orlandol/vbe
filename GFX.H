#ifndef __GFX_H
#define __GFX_H

  #include <stdint.h>
  #include <stdio.h>

  #include "vbe.h"

  /*
   *  Image header
   */
  #pragma pack(push, 1)
  struct _image_t
  {
    uint32_t width;
    uint32_t height;
    uint32_t linesize;
    uint32_t datasize;
    uint8_t  rpos;
    uint8_t  gpos;
    uint8_t  bpos;
    uint8_t  bpp;
  };
  typedef struct _image_t image_t;
  #pragma pack(pop)

  /*
   *  Graphics function pointer structure
   */
  struct _gfx_t
  {
    void (* Clear)( vbectx_t*, uint32_t );
    void (* SetPixel)( vbectx_t*, int32_t, int32_t, uint32_t );
    void (* FillRect)( vbectx_t*, int32_t, int32_t,
      int32_t, int32_t, uint32_t );
    void (* PutImage)( vbectx_t*, int32_t, int32_t, image_t* );
  };
  typedef struct _gfx_t gfx_t;

  /*
   *  General functions
   */
  gfx_t* gfx_Create( vbectx_t* p_ctx );

  void gfx_Release( gfx_t** p_gfx );

  /*
   *  Buffer clear functions
   */
  void gfx_Clear8( vbectx_t* p_ctx, uint32_t p_color );

  void gfx_Clear16( vbectx_t* p_ctx, uint32_t p_color );

  void gfx_Clear24( vbectx_t* p_ctx, uint32_t p_color );

  void gfx_Clear32( vbectx_t* p_ctx, uint32_t p_color );

  /*
   *  Set pixel functions
   */
  void gfx_SetPixel8( vbectx_t* p_ctx,
    int32_t p_x, int32_t p_y, uint32_t p_pixel );

  void gfx_SetPixel16( vbectx_t* p_ctx,
    int32_t p_x, int32_t p_y, uint32_t p_pixel );

  void gfx_SetPixel24( vbectx_t* p_ctx,
    int32_t p_x, int32_t p_y, uint32_t p_pixel );

  void gfx_SetPixel32( vbectx_t* p_ctx,
    int32_t p_x, int32_t p_y, uint32_t p_pixel );

  /*
   *  Filled rectangle functions
   */
  void gfx_FillRect8( vbectx_t* p_ctx, int32_t p_x1, int32_t p_y1,
    int32_t p_x2, int32_t p_y2, uint32_t p_pixel );

  void gfx_FillRect16( vbectx_t* p_ctx, int32_t p_x1, int32_t p_y1,
    int32_t p_x2, int32_t p_y2, uint32_t p_pixel );

  void gfx_FillRect24( vbectx_t* p_ctx, int32_t p_x1, int32_t p_y1,
    int32_t p_x2, int32_t p_y2, uint32_t p_pixel );

  void gfx_FillRect32( vbectx_t* p_ctx, int32_t p_x1, int32_t p_y1,
    int32_t p_x2, int32_t p_y2, uint32_t p_pixel );

  /*
   *  General image functions
   */
  image_t* gfx_NewImage( vbectx_t* p_ctx, uint32_t p_width,
    uint32_t p_height );

  void gfx_FreeImage( image_t** p_image );

  /*
   *  Image blitting functions
   */
  void gfx_PutImage8( vbectx_t* p_ctx, int32_t p_x,
    int32_t p_y, image_t* p_image );

  void gfx_PutImage16( vbectx_t* p_ctx, int32_t p_x,
    int32_t p_y, image_t* p_image );

  void gfx_PutImage24( vbectx_t* p_ctx, int32_t p_x,
    int32_t p_y, image_t* p_image );

  void gfx_PutImage32( vbectx_t* p_ctx, int32_t p_x,
    int32_t p_y, image_t* p_image );

#endif
