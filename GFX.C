#include "gfx.h"

  /*
   *  gfx_t* gfx_Create( *p_ctx )
   *
   *  Purpose:
   *    Creates a general graphics interface by
   *    populating a function pointer list based
   *    on p_ctx->bpp.
   *
   *  Returns:
   *    NULL on error
   *    Function pointer list on success
   */
  gfx_t* gfx_Create( vbectx_t* p_ctx )
  {
    gfx_t* l_gfx;

    if( p_ctx == NULL )
    {
      return NULL;
    }

    l_gfx = (gfx_t*)calloc(1, sizeof(gfx_t));
    if( l_gfx == NULL )
    {
      return NULL;
    }

    switch( p_ctx->bpp )
    {
    case 8:
      l_gfx->Clear    = gfx_Clear8;
      l_gfx->SetPixel = gfx_SetPixel8;
      l_gfx->FillRect = gfx_FillRect8;
      l_gfx->PutImage = gfx_PutImage8;
      return l_gfx;

    case 15:
    case 16:
      l_gfx->Clear    = gfx_Clear16;
      l_gfx->SetPixel = gfx_SetPixel16;
      l_gfx->FillRect = gfx_FillRect16;
      l_gfx->PutImage = gfx_PutImage16;
      return l_gfx;

    case 24:
      l_gfx->Clear    = gfx_Clear24;
      l_gfx->SetPixel = gfx_SetPixel24;
      l_gfx->FillRect = gfx_FillRect24;
      l_gfx->PutImage = gfx_PutImage24;
      return l_gfx;

    case 32:
      l_gfx->Clear    = gfx_Clear32;
      l_gfx->SetPixel = gfx_SetPixel32;
      l_gfx->FillRect = gfx_FillRect32;
      l_gfx->PutImage = gfx_PutImage32;
      return l_gfx;
    }

    free( l_gfx );

    return NULL;
  }

  /*
   *  void gfx_Release( **p_gfx )
   *
   *  Purpose:
   *    Releases memory used by general graphics
   *    interface p_gfx.
   *
   *  Returns:
   *    Nothing
   */
  void gfx_Release( gfx_t** p_gfx )
  {
    if( p_gfx )
    {
      if( *p_gfx )
      {
        free( *p_gfx );
      }

      *p_gfx = NULL;
    }
  }

  /*
   *  void gfx_Clear8( *p_ctx, p_pixel )
   *
   *  Purpose:
   *    Clears the entire LFB associated with
   *    p_ctx to specified 8-BPP color.
   *
   *  Returns:
   *    Nothing
   */
  void gfx_Clear8ASM( void* p_lfb,
    uint32_t p_lfbsize, uint32_t p_pixel );
  #pragma aux gfx_Clear8ASM=\
  "  test    edi, edi"\
  "  jz      Clear8_Error"\
  "  rep stosb"\
  "Clear8_Error:"\
  parm[edi][ecx][eax]\
  modify[edi];

  void gfx_Clear8( vbectx_t* p_ctx, uint32_t p_pixel )
  {
    if( p_ctx )
    {
      gfx_Clear8ASM( p_ctx->lfb, p_ctx->lfbsize, p_pixel );
    }
  }

  /*
   *  void gfx_Clear16( *p_ctx, p_pixel )
   *
   *  Purpose:
   *    Clears the entire LFB associated with
   *    p_ctx to specified 15/16-BPP color.
   *
   *  Returns:
   *    Nothing
   */
  void gfx_Clear16ASM( void* p_lfb,
    uint32_t p_lfbsize, uint32_t p_pixel );
  #pragma aux gfx_Clear16ASM=\
  "  test    edi, edi"\
  "  jz      Clear16_Error"\
  "  shr     ecx, 1"\
  "  rep stosw"\
  "Clear16_Error:"\
  parm[edi][ecx][eax]\
  modify[edi];

  void gfx_Clear16( vbectx_t* p_ctx, uint32_t p_pixel )
  {
    if( p_ctx )
    {
      gfx_Clear16ASM( p_ctx->lfb, p_ctx->lfbsize, p_pixel );
    }
  }

  /*
   *  void gfx_Clear24( *p_ctx, p_pixel )
   *
   *  Purpose:
   *    Clears the entire LFB associated with
   *    p_ctx to specified 24-BPP color.
   *
   *  Returns:
   *    Nothing
   */
  void gfx_Clear24ASM( void* p_lfb,
    uint32_t p_lfbsize, uint32_t p_pixel );
  #pragma aux gfx_Clear24ASM=\
  "  test    edi, edi"\
  "  jz      Clear24_Error"\
  "  cmp     ecx, 3"\
  "  jb      Clear24_Error"\
  ""\
  "  mov     edx, eax" /* Prepare last pixel*/\
  "  shl     edx, 8"\
  "  and     eax, 0x00FFFFFF"\
  "  and     edx, 0xFF000000"\
  "  or      eax, edx"\
  ""\
  "Clear24_Loop:"\
  "  sub     ecx, 3"\
  "  jbe     Clear24_Exit"\
  "  mov     [edi], eax" /* Write 4 bytes, advance 3 bytes */\
  "  add     edi, 3"\
  "  jmp     Clear24_Loop"\
  "Clear24_Exit:"\
  "  rol     eax, 8" /* Write last pixel */\
  "  mov     [edi - 1], eax"\
  "Clear24_Error:"\
  parm[edi][ecx][eax]\
  modify[eax ecx edx edi];

  void gfx_Clear24( vbectx_t* p_ctx, uint32_t p_pixel )
  {
    if( p_ctx )
    {
      gfx_Clear24ASM( p_ctx->lfb, p_ctx->lfbsize, p_pixel );
    }
  }

  /*
   *  void gfx_Clear32( *p_ctx, p_pixel )
   *
   *  Purpose:
   *    Clears the entire LFB associated with
   *    p_ctx to specified 32-BPP color.
   *
   *  Returns:
   *    Nothing
   */
  void gfx_Clear32ASM( void* p_lfb,
    uint32_t p_lfbsize, uint32_t p_pixel );
  #pragma aux gfx_Clear32ASM=\
  "  test    edi, edi"\
  "  jz      Clear32_Error"\
  "  shr     ecx, 2"\
  "  rep stosd"\
  "Clear32_Error:"\
  parm[edi][ecx][eax]\
  modify[edi];

  void gfx_Clear32( vbectx_t* p_ctx, uint32_t p_pixel )
  {
    if( p_ctx )
    {
      gfx_Clear32ASM( p_ctx->lfb, p_ctx->lfbsize, p_pixel );
    }
  }

  /*
   *  void gfx_SetPixel8( *p_ctx, p_x, p_y, p_pixel )
   *
   *  Purpose:
   *    Draws an 8-BPP pixel at the specified point
   *    if within the boundaries of p_ctx->clip.
   *
   *  Returns:
   *    Nothing
   */
  void gfx_SetPixel8( vbectx_t* p_ctx,
    int32_t p_x, int32_t p_y, uint32_t p_pixel )
  {
    uint8_t* l_lfb;

    if( p_ctx )
    {
      if( p_ctx->lfb )
      {
        if( (p_x < p_ctx->clip.minx) || (p_y < p_ctx->clip.miny)
          || (p_x > p_ctx->clip.maxx) || (p_y > p_ctx->clip.maxy) )
        {
          return;
        }

        l_lfb = (uint8_t*)(p_ctx->lfb + (p_y * p_ctx->linesize) + p_x);

        *l_lfb = (uint8_t)p_pixel;
      }
    }
  }

  /*
   *  void gfx_SetPixel16( *p_ctx, p_x, p_y, p_pixel )
   *
   *  Purpose:
   *    Draws a 15/16-BPP pixel at the specified point
   *    if within the boundaries of p_ctx->clip.
   *
   *  Returns:
   *    Nothing
   */
  void gfx_SetPixel16( vbectx_t* p_ctx,
    int32_t p_x, int32_t p_y, uint32_t p_pixel )
  {
    uint16_t* l_lfb;

    if( p_ctx )
    {
      if( p_ctx->lfb )
      {
        if( (p_x < p_ctx->clip.minx) || (p_y < p_ctx->clip.miny)
          || (p_x > p_ctx->clip.maxx) || (p_y > p_ctx->clip.maxy) )
        {
          return;
        }

        l_lfb = (uint16_t*)(p_ctx->lfb + (p_y * p_ctx->linesize)
          + (p_x * 2));

        *l_lfb = (uint16_t)p_pixel;
      }
    }
  }

  /*
   *  void gfx_SetPixel24( *p_ctx, p_x, p_y, p_pixel )
   *
   *  Purpose:
   *    Draws an 24-BPP pixel at the specified point
   *    if within the boundaries of p_ctx->clip.
   *
   *  Returns:
   *    Nothing
   */
  void gfx_SetPixel24( vbectx_t* p_ctx,
    int32_t p_x, int32_t p_y, uint32_t p_pixel )
  {
    uint8_t* l_lfb;

    if( p_ctx )
    {
      if( p_ctx->lfb )
      {
        if( (p_x < p_ctx->clip.minx) || (p_y < p_ctx->clip.miny)
          || (p_x > p_ctx->clip.maxx) || (p_y > p_ctx->clip.maxy) )
        {
          return;
        }

        l_lfb = (uint8_t*)(p_ctx->lfb + (p_y * p_ctx->linesize)
          + (p_x * 3));

        l_lfb[0] = (uint8_t)(p_pixel >> 0);
        l_lfb[1] = (uint8_t)(p_pixel >> 8);
        l_lfb[2] = (uint8_t)(p_pixel >> 16);
      }
    }
  }

  /*
   *  void gfx_SetPixel32( *p_ctx, p_x, p_y, p_pixel )
   *
   *  Purpose:
   *    Draws an 32-BPP pixel at the specified point
   *    if within the boundaries of p_ctx->clip.
   *
   *  Returns:
   *    Nothing
   */
  void gfx_SetPixel32( vbectx_t* p_ctx,
    int32_t p_x, int32_t p_y, uint32_t p_pixel )
  {
    uint32_t* l_lfb;

    if( p_ctx )
    {
      if( p_ctx->lfb )
      {
        if( (p_x < p_ctx->clip.minx) || (p_y < p_ctx->clip.miny)
          || (p_x > p_ctx->clip.maxx) || (p_y > p_ctx->clip.maxy) )
        {
          return;
        }

        l_lfb = (uint32_t*)(p_ctx->lfb + (p_y * p_ctx->linesize)
          + (p_x * 4));

        *l_lfb = p_pixel;
      }
    }
  }

  /*
   *  void gfx_FillRect8( *p_ctx, p_x1, p_y1,
   *    p_x2, p_y2, p_pixel )
   *
   *  Purpose:
   *    Draws a solid 8-BPP rectangle from (x1, y1)
   *    to (x2, y2) using specified pixel color,
   *    clipped to boundaries p_ctx->clip.
   *
   *  Returns:
   *    Nothing
   */
  void gfx_FillRect8ASM( uint8_t* p_lfb, uint32_t p_skip,
    uint32_t p_width, uint32_t p_height, uint32_t p_pixel );
  #pragma aux gfx_FillRect8ASM=\
  "  test    edi, edi"\
  "  jz      FillRect8_Error"\
  "  mov     esi, ecx"\
  "FillRect8_Loop:"\
  "  mov     ecx, esi"\
  "  rep stosb"\
  "  add     edi, edx"\
  "  dec     ebx"\
  "  jnz     FillRect8_Loop"\
  "FillRect8_Error:"\
  parm[edi][edx][ecx][ebx][eax]\
  modify[ebx ecx edx esi edi];

  void gfx_FillRect8( vbectx_t* p_ctx, int32_t p_x1, int32_t p_y1,
    int32_t p_x2, int32_t p_y2, uint32_t p_pixel )
  {
    uint8_t* l_lfb;
    uint32_t l_width;
    uint32_t l_height;
    uint32_t l_skip;
    int32_t  l_minx;
    int32_t  l_miny;
    int32_t  l_maxx;
    int32_t  l_maxy;

    if( p_ctx == NULL )
    {
      return;
    }

    if( p_ctx->lfb == NULL )
    {
      return;
    }

    // Clip specified rectangle to context boundaries
    l_minx = p_ctx->clip.minx;
    if( p_x1 >= l_minx )
    {
      l_minx = p_x1;
    }

    l_miny = p_ctx->clip.miny;
    if( p_y1 >= l_miny )
    {
      l_miny = p_y1;
    }

    l_maxx = p_ctx->clip.maxx;
    if( p_x2 <= l_maxx )
    {
      l_maxx = p_x2;
    }

    l_maxy = p_ctx->clip.maxy;
    if( p_y2 <= l_maxy )
    {
      l_maxy = p_y2;
    }

    // Validate boundaries
    if( l_minx >= l_maxx )
    {
      return;
    }

    if( l_miny >= l_maxy )
    {
      return;
    }

    // Initialize loop variables
    l_width  = (l_maxx - l_minx) + 1;
    l_height = (l_maxy - l_miny) + 1;

    l_skip   = p_ctx->linesize - l_width;

    // Calculate buffer offset
    l_lfb = (uint8_t*)(p_ctx->lfb +
      (l_miny * p_ctx->linesize) + l_minx);

    // Draw rectangle
    gfx_FillRect8ASM( l_lfb, l_skip, l_width, l_height, p_pixel );
  }

  /*
   *  void gfx_FillRect16( *p_ctx, p_x1, p_y1,
   *    p_x2, p_y2, p_pixel )
   *
   *  Purpose:
   *    Draws a solid 15/16-BPP rectangle from (x1, y1)
   *    to (x2, y2) using specified pixel color,
   *    clipped to boundaries p_ctx->clip.
   *
   *  Returns:
   *    Nothing
   */
  void gfx_FillRect16ASM( uint16_t* p_lfb, uint32_t p_skip,
    uint32_t p_width, uint32_t p_height, uint32_t p_pixel );
  #pragma aux gfx_FillRect16ASM=\
  "  test    edi, edi"\
  "  jz      FillRect16_Error"\
  "  mov     esi, ecx"\
  "FillRect16_Loop:"\
  "  mov     ecx, esi"\
  "  rep stosw"\
  "  add     edi, edx"\
  "  dec     ebx"\
  "  jnz     FillRect16_Loop"\
  "FillRect16_Error:"\
  parm[edi][edx][ecx][ebx][eax]\
  modify[ebx ecx edx esi edi];

  void gfx_FillRect16( vbectx_t* p_ctx, int32_t p_x1, int32_t p_y1,
    int32_t p_x2, int32_t p_y2, uint32_t p_pixel )
  {
    uint16_t* l_lfb;
    uint32_t  l_width;
    uint32_t  l_height;
    uint32_t  l_skip;
    int32_t   l_minx;
    int32_t   l_miny;
    int32_t   l_maxx;
    int32_t   l_maxy;

    if( p_ctx == NULL )
    {
      return;
    }

    if( p_ctx->lfb == NULL )
    {
      return;
    }

    // Clip specified rectangle to context boundaries
    l_minx = p_ctx->clip.minx;
    if( p_x1 >= l_minx )
    {
      l_minx = p_x1;
    }

    l_miny = p_ctx->clip.miny;
    if( p_y1 >= l_miny )
    {
      l_miny = p_y1;
    }

    l_maxx = p_ctx->clip.maxx;
    if( p_x2 <= l_maxx )
    {
      l_maxx = p_x2;
    }

    l_maxy = p_ctx->clip.maxy;
    if( p_y2 <= l_maxy )
    {
      l_maxy = p_y2;
    }

    // Validate boundaries
    if( l_minx >= l_maxx )
    {
      return;
    }

    if( l_miny >= l_maxy )
    {
      return;
    }

    // Initialize loop variables
    l_width  = (l_maxx - l_minx) + 1;
    l_height = (l_maxy - l_miny) + 1;

    l_skip   = p_ctx->linesize - (l_width * 2);

    // Calculate buffer offset
    l_lfb = (uint16_t*)(p_ctx->lfb +
      (l_miny * p_ctx->linesize) + (l_minx * 2));

    // Draw rectangle
    gfx_FillRect16ASM( l_lfb, l_skip, l_width, l_height, p_pixel );
  }

  /*
   *  void gfx_FillRect24( *p_ctx, p_x1, p_y1,
   *    p_x2, p_y2, p_pixel )
   *
   *  Purpose:
   *    Draws a solid 24-BPP rectangle from (x1, y1)
   *    to (x2, y2) using specified pixel color,
   *    clipped to boundaries p_ctx->clip.
   *
   *  Returns:
   *    Nothing
   */
  void gfx_FillRect24ASM( uint8_t* p_lfb, uint32_t p_skip,
    uint32_t p_width, uint32_t p_height, uint32_t p_pixel );
  #pragma aux gfx_FillRect24ASM=\
  "  test    edi, edi"\
  "  jz      FillRect24_Error"\
  "  lea     ecx, [ecx * 2 + ecx]" /* ecx = (ecx * 3) */\
  "  mov     esi, ecx"\
  ""\
  "FillRect24_Loop:"\
  "  cmp     ecx, 3"\
  "  jbe     FillRect24_ExitLoop"\
  "  mov     [edi], eax" /* Write 4 bytes, advance 3 bytes */\
  "  add     edi, 3"\
  "  sub     ecx, 3"\
  "  jmp     FillRect24_Loop"\
  ""\
  "FillRect24_ExitLoop:"\
  "  mov     cl, [edi + 3]" /* Write last pixel */\
  "  and     eax, 0x00FFFFFF"\
  "  shl     ecx, 24"\
  "  or      eax, ecx"\
  "  mov     [edi], eax"\
  "  add     edi, 3"\
  ""\
  "  mov     ecx, esi" /* Prepare loop for next iteration */\
  "  add     edi, edx"\
  "  dec     ebx"\
  "  jnz     FillRect24_Loop"\
  "FillRect24_Error:"\
  parm[edi][edx][ecx][ebx][eax]\
  modify[ebx ecx edx esi edi];

  void gfx_FillRect24( vbectx_t* p_ctx, int32_t p_x1, int32_t p_y1,
    int32_t p_x2, int32_t p_y2, uint32_t p_pixel )
  {
    uint8_t* l_lfb;
    uint32_t l_width;
    uint32_t l_height;
    uint32_t l_skip;
    int32_t  l_minx;
    int32_t  l_miny;
    int32_t  l_maxx;
    int32_t  l_maxy;

    if( p_ctx == NULL )
    {
      return;
    }

    if( p_ctx->lfb == NULL )
    {
      return;
    }

    // Clip specified rectangle to context boundaries
    l_minx = p_ctx->clip.minx;
    if( p_x1 >= l_minx )
    {
      l_minx = p_x1;
    }

    l_miny = p_ctx->clip.miny;
    if( p_y1 >= l_miny )
    {
      l_miny = p_y1;
    }

    l_maxx = p_ctx->clip.maxx;
    if( p_x2 <= l_maxx )
    {
      l_maxx = p_x2;
    }

    l_maxy = p_ctx->clip.maxy;
    if( p_y2 <= l_maxy )
    {
      l_maxy = p_y2;
    }

    // Validate boundaries
    if( l_minx >= l_maxx )
    {
      return;
    }

    if( l_miny >= l_maxy )
    {
      return;
    }

    // Initialize loop variables
    l_width  = (l_maxx - l_minx) + 1;
    l_height = (l_maxy - l_miny) + 1;

    l_skip   = p_ctx->linesize - (l_width * 3);

    // Calculate buffer offset
    l_lfb = (uint8_t*)(p_ctx->lfb +
      (l_miny * p_ctx->linesize) + (l_minx * 3));

    // Draw rectangle
    gfx_FillRect24ASM( l_lfb, l_skip, l_width, l_height, p_pixel );
  }

  /*
   *  void gfx_FillRect32( *p_ctx, p_x1, p_y1,
   *    p_x2, p_y2, p_pixel )
   *
   *  Purpose:
   *    Draws a solid 32-BPP rectangle from (x1, y1)
   *    to (x2, y2) using specified pixel color,
   *    clipped to boundaries p_ctx->clip.
   *
   *  Returns:
   *    Nothing
   */
  void gfx_FillRect32ASM( uint32_t* p_lfb, uint32_t p_skip,
    uint32_t p_width, uint32_t p_height, uint32_t p_pixel );
  #pragma aux gfx_FillRect32ASM=\
  "  test    edi, edi"\
  "  jz      FillRect32_Error"\
  "  mov     esi, ecx"\
  "FillRect32_Loop:"\
  "  mov     ecx, esi"\
  "  rep stosd"\
  "  add     edi, edx"\
  "  dec     ebx"\
  "  jnz     FillRect32_Loop"\
  "FillRect32_Error:"\
  parm[edi][edx][ecx][ebx][eax]\
  modify[ebx ecx edx esi edi];

  void gfx_FillRect32( vbectx_t* p_ctx, int32_t p_x1, int32_t p_y1,
    int32_t p_x2, int32_t p_y2, uint32_t p_pixel )
  {
    uint32_t* l_lfb;
    uint32_t  l_width;
    uint32_t  l_height;
    uint32_t  l_skip;
    int32_t   l_minx;
    int32_t   l_miny;
    int32_t   l_maxx;
    int32_t   l_maxy;

    if( p_ctx == NULL )
    {
      return;
    }

    if( p_ctx->lfb == NULL )
    {
      return;
    }

    // Clip specified rectangle to context boundaries
    l_minx = p_ctx->clip.minx;
    if( p_x1 >= l_minx )
    {
      l_minx = p_x1;
    }

    l_miny = p_ctx->clip.miny;
    if( p_y1 >= l_miny )
    {
      l_miny = p_y1;
    }

    l_maxx = p_ctx->clip.maxx;
    if( p_x2 <= l_maxx )
    {
      l_maxx = p_x2;
    }

    l_maxy = p_ctx->clip.maxy;
    if( p_y2 <= l_maxy )
    {
      l_maxy = p_y2;
    }

    // Validate boundaries
    if( l_minx >= l_maxx )
    {
      return;
    }

    if( l_miny >= l_maxy )
    {
      return;
    }

    // Initialize loop variables
    l_width  = (l_maxx - l_minx) + 1;
    l_height = (l_maxy - l_miny) + 1;

    l_skip   = p_ctx->linesize - (l_width * 4);

    // Calculate buffer offset
    l_lfb = (uint32_t*)(p_ctx->lfb +
      (l_miny * p_ctx->linesize) + (l_minx * 4));

    // Draw rectangle
    gfx_FillRect32ASM( l_lfb, l_skip, l_width, l_height, p_pixel );
  }

  /*
   *  image_t* gfx_NewImage( *p_ctx, p_width, p_height )
   *
   *  Purpose:
   *    Allocates memory required to hold image of
   *    specified width, height, and depth. Works
   *    for any supported bit depth.
   *
   *  Returns:
   *    NULL on error
   *    Blank Image on success
   */
  image_t* gfx_NewImage( vbectx_t* p_ctx, uint32_t p_width,
    uint32_t p_height )
  {
    image_t* l_img;
    uint32_t l_pixsize;
    uint32_t l_datasize;

    if( p_ctx )
    {
      l_img = NULL;

      l_pixsize  = (p_ctx->bpp + 1) >> 3;
      l_datasize = (p_width * l_pixsize) * p_height;

      switch( p_ctx->bpp )
      {
      case 8:
      case 15:
      case 16:
      case 24:
      case 32:
        l_img = (image_t*)calloc(1, sizeof(image_t) + l_datasize);
        break;
      }

      if( l_img )
      {
        l_img->width    = p_width;
        l_img->height   = p_height;
        l_img->linesize = p_width * l_pixsize;
        l_img->datasize = l_datasize;
        l_img->rpos     = p_ctx->rpos;
        l_img->gpos     = p_ctx->gpos;
        l_img->bpos     = p_ctx->bpos;
        l_img->bpp      = p_ctx->bpp;

        return l_img;
      }
    }

    return NULL;
  }

  /*
   *  void gfx_FreeImage( **p_image )
   *
   *  Purpose:
   *    Releases memory used by specified Image.
   *
   *  Returns:
   *    Nothing
   */
  void gfx_FreeImage( image_t** p_image )
  {
    if( p_image )
    {
      if( *p_image )
      {
        free( *p_image );
      }
      *p_image = NULL;
    }
  }

  /*
   *  void gfx_PutImage8( *p_ctx, p_x, p_y, *p_image )
   *
   *  Purpose:
   *    Draws an 8-BPP image within the boundaries
   *    specified by p_ctx->clip.
   *
   *  Returns:
   *    Nothing
   */
  void gfx_PutImage8ASM( uint8_t* p_lfb, uint32_t p_lfbSkip,
    uint8_t* p_image, uint32_t p_imgSkip,
    uint32_t p_width, uint32_t p_height );
  #pragma aux gfx_PutImage8ASM=\
  "push    edx"\
  "push    eax"\
  ""\
  "mov     eax, ecx"\
  "and     ecx, 3"\
  "shr     eax, 2"\
  "mov     edx, ecx"\
  ""\
  "DrawLoop:"\
  "test    ebx, ebx"\
  "jz      ExitDrawLoop"\
  ""\
  "mov     ecx, eax"\
  "rep movsd"\
  "mov     ecx, edx"\
  "rep movsb"\
  ""\
  "add     esi, [esp + 0]"\
  "add     edi, [esp + 4]"\
  ""\
  "dec     ebx"\
  "jmp     DrawLoop"\
  "ExitDrawLoop:"\
  ""\
  "pop     eax"\
  "pop     edx"\
  parm[edi][edx][esi][eax][ecx][ebx]\
  modify[eax ebx ecx edx esi edi];

  void gfx_PutImage8( vbectx_t* p_ctx, int32_t p_x,
    int32_t p_y, image_t* p_image )
  {
    uint8_t* l_lfb;
    uint8_t* l_image;
    uint32_t l_width;
    uint32_t l_height;
    uint32_t l_lfbSkip;
    uint32_t l_imgSkip;
    int32_t  l_lfbMinx;
    int32_t  l_lfbMiny;
    int32_t  l_lfbMaxx;
    int32_t  l_lfbMaxy;
    int32_t  l_tmpX2;
    int32_t  l_tmpY2;
    int32_t  l_imgLeft;
    int32_t  l_imgTop;

    if( (p_ctx == NULL) || (p_image == NULL) )
    {
      return;
    }

    if( (p_ctx->lfb == NULL) || (p_ctx->bpp != 8) ||
      (p_image->bpp != 8) )
    {
      return;
    }

    // Clip image to context boundaries
    l_lfbMinx = p_ctx->clip.minx;
    if( p_x >= l_lfbMinx )
    {
      l_lfbMinx = p_x;
    }

    l_lfbMiny = p_ctx->clip.miny;
    if( p_y >= l_lfbMiny )
    {
      l_lfbMiny = p_y;
    }

    l_tmpX2 = (p_x + p_image->width) - 1;
    l_tmpY2 = (p_y + p_image->height) - 1;

    l_lfbMaxx = p_ctx->clip.maxx;
    if( l_tmpX2 <= l_lfbMaxx )
    {
      l_lfbMaxx = l_tmpX2;
    }

    l_lfbMaxy = p_ctx->clip.maxy;
    if( l_tmpY2 <= l_lfbMaxy )
    {
      l_lfbMaxy = l_tmpY2;
    }

    // Validate boundaries
    if( l_lfbMinx >= l_lfbMaxx )
    {
      return;
    }

    if( l_lfbMiny >= l_lfbMaxy )
    {
      return;
    }

    // Initialize loop variables
    l_width   = (l_lfbMaxx - l_lfbMinx) + 1;
    l_height  = (l_lfbMaxy - l_lfbMiny) + 1;

    l_lfbSkip = p_ctx->linesize - l_width;

    // Calculate buffer offset
    l_lfb = (uint8_t*)(p_ctx->lfb +
      (l_lfbMiny * p_ctx->linesize) + l_lfbMinx);

    // Calculate image buffer starting point
    l_imgTop  = -(p_y - l_lfbMiny);

    l_imgLeft = -(p_x - l_lfbMinx);

    l_imgSkip = p_image->width - l_width;

    // Calculate image buffer offset
    l_image = ((uint8_t*)p_image) + sizeof(image_t) +
      (l_imgTop * p_image->linesize) + l_imgLeft;

    // Draw image
    gfx_PutImage8ASM( l_lfb, l_lfbSkip, l_image, l_imgSkip,
      l_width, l_height );
  }

  /*
   *  void gfx_PutImage16( *p_ctx, p_x, p_y, *p_image )
   *
   *  Purpose:
   *    Draws a 15/16-BPP image within the boundaries
   *    specified by p_ctx->clip.
   *
   *  Returns:
   *    Nothing
   */
  void gfx_PutImage16ASM( uint8_t* p_lfb, uint32_t p_lfbSkip,
    uint8_t* p_image, uint32_t p_imgSkip,
    uint32_t p_width, uint32_t p_height );
  #pragma aux gfx_PutImage16ASM=\
  "push    edx"\
  "push    eax"\
  ""\
  "mov     eax, ecx"\
  "and     ecx, 1"\
  "shr     eax, 1"\
  "mov     edx, ecx"\
  ""\
  "DrawLoop:"\
  "test    ebx, ebx"\
  "jz      ExitDrawLoop"\
  ""\
  "mov     ecx, eax"\
  "rep movsd"\
  "mov     ecx, edx"\
  "rep movsw"\
  ""\
  "add     esi, [esp + 0]"\
  "add     edi, [esp + 4]"\
  ""\
  "dec     ebx"\
  "jmp     DrawLoop"\
  "ExitDrawLoop:"\
  ""\
  "pop     eax"\
  "pop     edx"\
  parm[edi][edx][esi][eax][ecx][ebx]\
  modify[eax ebx ecx edx esi edi];

  void gfx_PutImage16( vbectx_t* p_ctx, int32_t p_x,
    int32_t p_y, image_t* p_image )
  {
    uint8_t* l_lfb;
    uint8_t* l_image;
    uint32_t l_width;
    uint32_t l_height;
    uint32_t l_lfbSkip;
    uint32_t l_imgSkip;
    int32_t  l_lfbMinx;
    int32_t  l_lfbMiny;
    int32_t  l_lfbMaxx;
    int32_t  l_lfbMaxy;
    int32_t  l_tmpX2;
    int32_t  l_tmpY2;
    int32_t  l_imgLeft;
    int32_t  l_imgTop;
    uint8_t  l_ctxpixsize;
    uint8_t  l_imgpixsize;

    if( (p_ctx == NULL) || (p_image == NULL) )
    {
      return;
    }

    // Check pixel size for 15/16-BPP modes
    l_ctxpixsize = (p_ctx->bpp + 1) / 8;
    l_imgpixsize = (p_image->bpp + 1) / 8;

    if( (p_ctx->lfb == NULL) || (l_ctxpixsize != 2) ||
      (l_imgpixsize != 2) )
    {
      return;
    }

    // Clip image to context boundaries
    l_lfbMinx = p_ctx->clip.minx;
    if( p_x >= l_lfbMinx )
    {
      l_lfbMinx = p_x;
    }

    l_lfbMiny = p_ctx->clip.miny;
    if( p_y >= l_lfbMiny )
    {
      l_lfbMiny = p_y;
    }

    l_tmpX2 = (p_x + p_image->width) - 1;
    l_tmpY2 = (p_y + p_image->height) - 1;

    l_lfbMaxx = p_ctx->clip.maxx;
    if( l_tmpX2 <= l_lfbMaxx )
    {
      l_lfbMaxx = l_tmpX2;
    }

    l_lfbMaxy = p_ctx->clip.maxy;
    if( l_tmpY2 <= l_lfbMaxy )
    {
      l_lfbMaxy = l_tmpY2;
    }

    // Validate boundaries
    if( l_lfbMinx >= l_lfbMaxx )
    {
      return;
    }

    if( l_lfbMiny >= l_lfbMaxy )
    {
      return;
    }

    // Initialize loop variables
    l_width   = (l_lfbMaxx - l_lfbMinx) + 1;
    l_height  = (l_lfbMaxy - l_lfbMiny) + 1;

    l_lfbSkip = p_ctx->linesize - (l_width * 2);

    // Calculate buffer offset
    l_lfb = (uint8_t*)(p_ctx->lfb +
      (l_lfbMiny * p_ctx->linesize) + (l_lfbMinx * 2));

    // Calculate image buffer starting point
    l_imgTop  = -(p_y - l_lfbMiny);

    l_imgLeft = -(p_x - l_lfbMinx);

    l_imgSkip = p_image->width - l_width;

    // Calculate image buffer offset
    l_image = ((uint8_t*)p_image) + sizeof(image_t) +
      (l_imgTop * p_image->linesize) + (l_imgLeft * 2);

    // Draw image
    gfx_PutImage16ASM( l_lfb, l_lfbSkip, l_image, (l_imgSkip * 2),
      l_width, l_height );
  }

  /*
   *  void gfx_PutImage24( *p_ctx, p_x, p_y, *p_image )
   *
   *  Purpose:
   *    Draws a 24-BPP image within the boundaries
   *    specified by p_ctx->clip.
   *
   *  Returns:
   *    Nothing
   */
  void gfx_PutImage24ASM( uint8_t* p_lfb, uint32_t p_lfbSkip,
    uint8_t* p_image, uint32_t p_imgSkip,
    uint32_t p_width, uint32_t p_height );
  #pragma aux gfx_PutImage24ASM=\
  "push    edx"\
  "push    eax"\
  ""\
  "/* Multiply ecx by 3 */"\
  "lea     ecx, [ecx + ecx * 2]"\
  ""\
  "mov     eax, ecx"\
  "and     ecx, 3"\
  "shr     eax, 2"\
  "mov     edx, ecx"\
  ""\
  "DrawLoop:"\
  "test    ebx, ebx"\
  "jz      ExitDrawLoop"\
  ""\
  "mov     ecx, eax"\
  "rep movsd"\
  "mov     ecx, edx"\
  "rep movsb"\
  ""\
  "add     esi, [esp + 0]"\
  "add     edi, [esp + 4]"\
  ""\
  "dec     ebx"\
  "jmp     DrawLoop"\
  "ExitDrawLoop:"\
  ""\
  "pop     eax"\
  "pop     edx"\
  parm[edi][edx][esi][eax][ecx][ebx]\
  modify[eax ebx ecx edx esi edi];

  void gfx_PutImage24( vbectx_t* p_ctx, int32_t p_x,
    int32_t p_y, image_t* p_image )
  {
    uint8_t* l_lfb;
    uint8_t* l_image;
    uint32_t l_width;
    uint32_t l_height;
    uint32_t l_lfbSkip;
    uint32_t l_imgSkip;
    int32_t  l_lfbMinx;
    int32_t  l_lfbMiny;
    int32_t  l_lfbMaxx;
    int32_t  l_lfbMaxy;
    int32_t  l_tmpX2;
    int32_t  l_tmpY2;
    int32_t  l_imgLeft;
    int32_t  l_imgTop;

    if( (p_ctx == NULL) || (p_image == NULL) )
    {
      return;
    }

    if( (p_ctx->lfb == NULL) || (p_ctx->bpp != 24) ||
      (p_image->bpp != 24) )
    {
      return;
    }

    // Clip image to context boundaries
    l_lfbMinx = p_ctx->clip.minx;
    if( p_x >= l_lfbMinx )
    {
      l_lfbMinx = p_x;
    }

    l_lfbMiny = p_ctx->clip.miny;
    if( p_y >= l_lfbMiny )
    {
      l_lfbMiny = p_y;
    }

    l_tmpX2 = (p_x + p_image->width) - 1;
    l_tmpY2 = (p_y + p_image->height) - 1;

    l_lfbMaxx = p_ctx->clip.maxx;
    if( l_tmpX2 <= l_lfbMaxx )
    {
      l_lfbMaxx = l_tmpX2;
    }

    l_lfbMaxy = p_ctx->clip.maxy;
    if( l_tmpY2 <= l_lfbMaxy )
    {
      l_lfbMaxy = l_tmpY2;
    }

    // Validate boundaries
    if( l_lfbMinx >= l_lfbMaxx )
    {
      return;
    }

    if( l_lfbMiny >= l_lfbMaxy )
    {
      return;
    }

    // Initialize loop variables
    l_width   = (l_lfbMaxx - l_lfbMinx) + 1;
    l_height  = (l_lfbMaxy - l_lfbMiny) + 1;

    l_lfbSkip = p_ctx->linesize - (l_width * 3);

    // Calculate buffer offset
    l_lfb = (uint8_t*)(p_ctx->lfb +
      (l_lfbMiny * p_ctx->linesize) + (l_lfbMinx * 3));

    // Calculate image buffer starting point
    l_imgTop  = -(p_y - l_lfbMiny);

    l_imgLeft = -(p_x - l_lfbMinx);

    l_imgSkip = p_image->width - l_width;

    // Calculate image buffer offset
    l_image = ((uint8_t*)p_image) + sizeof(image_t) +
      (l_imgTop * p_image->linesize) + (l_imgLeft * 3);

    // Draw image
    gfx_PutImage24ASM( l_lfb, l_lfbSkip, l_image, (l_imgSkip * 3),
      l_width, l_height );
  }

  /*
   *  void gfx_PutImage32( *p_ctx, p_x, p_y, *p_image )
   *
   *  Purpose:
   *    Draws a 32-BPP image within the boundaries
   *    specified by p_ctx->clip.
   *
   *  Returns:
   *    Nothing
   */
  void gfx_PutImage32ASM( uint8_t* p_lfb, uint32_t p_lfbSkip,
    uint8_t* p_image, uint32_t p_imgSkip,
    uint32_t p_width, uint32_t p_height );
  #pragma aux gfx_PutImage32ASM=\
  "push    edx"\
  "push    eax"\
  ""\
  "mov     eax, ecx"\
  ""\
  "DrawLoop:"\
  "test    ebx, ebx"\
  "jz      ExitDrawLoop"\
  ""\
  "mov     ecx, eax"\
  "rep movsd"\
  ""\
  "add     esi, [esp + 0]"\
  "add     edi, [esp + 4]"\
  ""\
  "dec     ebx"\
  "jmp     DrawLoop"\
  "ExitDrawLoop:"\
  ""\
  "pop     eax"\
  "pop     edx"\
  parm[edi][edx][esi][eax][ecx][ebx]\
  modify[eax ebx ecx edx esi edi];

  void gfx_PutImage32( vbectx_t* p_ctx, int32_t p_x,
    int32_t p_y, image_t* p_image )
  {
    uint8_t* l_lfb;
    uint8_t* l_image;
    uint32_t l_width;
    uint32_t l_height;
    uint32_t l_lfbSkip;
    uint32_t l_imgSkip;
    int32_t  l_lfbMinx;
    int32_t  l_lfbMiny;
    int32_t  l_lfbMaxx;
    int32_t  l_lfbMaxy;
    int32_t  l_tmpX2;
    int32_t  l_tmpY2;
    int32_t  l_imgLeft;
    int32_t  l_imgTop;

    if( (p_ctx == NULL) || (p_image == NULL) )
    {
      return;
    }

    if( (p_ctx->lfb == NULL) || (p_ctx->bpp != 32) ||
      (p_image->bpp != 32) )
    {
      return;
    }

    // Clip image to context boundaries
    l_lfbMinx = p_ctx->clip.minx;
    if( p_x >= l_lfbMinx )
    {
      l_lfbMinx = p_x;
    }

    l_lfbMiny = p_ctx->clip.miny;
    if( p_y >= l_lfbMiny )
    {
      l_lfbMiny = p_y;
    }

    l_tmpX2 = (p_x + p_image->width) - 1;
    l_tmpY2 = (p_y + p_image->height) - 1;

    l_lfbMaxx = p_ctx->clip.maxx;
    if( l_tmpX2 <= l_lfbMaxx )
    {
      l_lfbMaxx = l_tmpX2;
    }

    l_lfbMaxy = p_ctx->clip.maxy;
    if( l_tmpY2 <= l_lfbMaxy )
    {
      l_lfbMaxy = l_tmpY2;
    }

    // Validate boundaries
    if( l_lfbMinx >= l_lfbMaxx )
    {
      return;
    }

    if( l_lfbMiny >= l_lfbMaxy )
    {
      return;
    }

    // Initialize loop variables
    l_width   = (l_lfbMaxx - l_lfbMinx) + 1;
    l_height  = (l_lfbMaxy - l_lfbMiny) + 1;

    l_lfbSkip = p_ctx->linesize - (l_width * 4);

    // Calculate buffer offset
    l_lfb = (uint8_t*)(p_ctx->lfb +
      (l_lfbMiny * p_ctx->linesize) + (l_lfbMinx * 4));

    // Calculate image buffer starting point
    l_imgTop  = -(p_y - l_lfbMiny);

    l_imgLeft = -(p_x - l_lfbMinx);

    l_imgSkip = p_image->width - l_width;

    // Calculate image buffer offset
    l_image = ((uint8_t*)p_image) + sizeof(image_t) +
      (l_imgTop * p_image->linesize) + (l_imgLeft * 4);

    // Draw image
    gfx_PutImage32ASM( l_lfb, l_lfbSkip, l_image, (l_imgSkip * 4),
      l_width, l_height );
  }
