#include "pcx.h"

  /* General note on PCX decoding at the end of this file */

  /*
   *  int pcx_LoadLine8( *p_pcx )
   *
   *  Purpose:
   *    Decodes an 8-BPP line from a PCX file into
   *    the Line Buffer.
   *
   *  Returns:
   *    0 on error
   *    -1 and output in p_dst on success
   */
  int pcx_LoadLine8( pcx_t* p_pcx )
  {
    uint8_t* l_lineBuffer;
    uint32_t l_i;
    uint8_t  l_run;
    uint8_t  l_pixel;

    if( p_pcx == NULL )
    {
      return 0;
    }

    if( p_pcx->pcxFile == NULL )
    {
      return 0;
    }

    l_lineBuffer = p_pcx->lineBuffer;
    if( l_lineBuffer == NULL )
    {
      return 0;
    }

    l_i = 0;
    while( l_i < p_pcx->linewidth )
    {
      if( fread(&l_pixel, 1, 1, p_pcx->pcxFile) != 1 )
      {
        return 0;
      }

      if( l_pixel >= 0xC0 )
      {
        l_run = l_pixel & 0x3F;

        if( fread(&l_pixel, 1, 1, p_pcx->pcxFile) != 1 )
        {
          return 0;
        }

        memset( l_lineBuffer + l_i, l_pixel, l_run );
      }
      else
      {
        l_run = 1;

        l_lineBuffer[l_i] = l_pixel;
      }

      l_i += l_run;
    }

    return -1;
  }

  /*
   *  int pcx_LoadLine24( *p_pcx )
   *
   *  Purpose:
   *    Decodes a 24-BPP line from a PCX file into
   *    the Line Buffer.
   *
   *  Returns:
   *    0 on error
   *    -1 and output in p_dst on success
   */
  int pcx_LoadLine24( pcx_t* p_pcx )
  {
    uint8_t* l_lineBuffer;
    uint32_t l_i;
    uint32_t l_ofs;
    uint32_t l_plane;
    uint8_t  l_run;
    uint8_t  l_component;

    if( p_pcx == NULL )
    {
      return 0;
    }

    if( p_pcx->pcxFile == NULL )
    {
      return 0;
    }

    l_lineBuffer = p_pcx->lineBuffer;
    if( l_lineBuffer == NULL )
    {
      return 0;
    }

    for( l_plane = 0; l_plane < 3; l_plane++ )
    {
      l_ofs = l_plane;

      l_i = 0;
      while( l_i < p_pcx->linewidth )
      {
        if( fread(&l_component, 1, 1, p_pcx->pcxFile) != 1 )
        {
          return 0;
        }

        if( l_component >= 0xC0 )
        {
          l_run = l_component & 0x3F;

          if( fread(&l_component, 1, 1, p_pcx->pcxFile) != 1 )
          {
            return 0;
          }

          l_i += l_run;

          while( l_run )
          {
            l_lineBuffer[l_ofs] = l_component;
            l_ofs += 3;

            l_run--;
          }
        }
        else
        {
          l_i++;

          l_lineBuffer[l_ofs] = l_component;
          l_ofs += 3;
        }
      }
    }

    return -1;
  }

  /*
   *  int pcx_LoadPal( *p_pcx )
   *
   *  Purpose:
   *    Validates and loads the palette from
   *    an 8-BPP PCX file.
   *
   *  Returns:
   *    0 on error
   *    -1 and Palette in p_pal on success
   */
  int pcx_LoadPal( pcx_t* p_pcx )
  {
    uint32_t l_i;
    uint8_t  l_palSig;
    pal3_t   l_pal;

    if( p_pcx == NULL )
    {
      return 0;
    }

    if( (p_pcx->pcxFile == NULL) || (p_pcx->bpp != 8) )
    {
      return 0;
    }

    if( fseek(p_pcx->pcxFile, -769, SEEK_END) != 0 )
    {
      return 0;
    }

    if( fread(&l_palSig, 1, 1, p_pcx->pcxFile) != 1 )
    {
      return 0;
    }

    if( l_palSig != 12 )
    {
      return 0;
    }

    if( fread(l_pal, 1, 768, p_pcx->pcxFile) != 768 )
    {
      return 0;
    }

    for( l_i = 0; l_i < 256; l_i++ )
    {
      p_pcx->pal[l_i].r = l_pal[l_i].r;
      p_pcx->pal[l_i].g = l_pal[l_i].g;
      p_pcx->pal[l_i].b = l_pal[l_i].b;
      p_pcx->pal[l_i].a = 0;
    }

    return -1;
  }

  /*
   *  pcx_t* pcx_Open( *p_name )
   *
   *  Purpose:
   *    Opens PCX file, and validates header. Only
   *    opens 8-BPP PCX files with a palette, or
   *    24-BPP PCX files.
   *
   *  Returns:
   *    NULL on error
   *    PCX structure on success
   */
  pcx_t* pcx_Open( const char* p_name )
  {
    pcx_t*   l_pcx;
    FILE*    l_pcxFile;
    uint8_t* l_lineBuffer;
    pcxhdr_t l_pcxHdr;
    uint32_t l_width;
    uint32_t l_height;
    uint32_t l_linesize;
    uint8_t  l_bpp;

    l_pcx        = NULL;
    l_pcxFile    = NULL;
    l_lineBuffer = NULL;

    // Validate file name and open PCX file
    if( p_name == NULL )
    {
      goto ExitError;
    }

    if( *p_name == '\0' )
    {
      goto ExitError;
    }

    l_pcxFile = fopen(p_name, "rb");
    if( l_pcxFile == NULL )
    {
      goto ExitError;
    }

    // Load PCX header
    if( fread(&l_pcxHdr, 1, sizeof(l_pcxHdr), l_pcxFile) !=
      sizeof(l_pcxHdr) )
    {
      goto ExitError;
    }

    // Validate header and initialize calculations
    if( (l_pcxHdr.Manufacturer != pcxMfgZSoft) ||
        (l_pcxHdr.Version != pcxVer30) ||
        (l_pcxHdr.Encoding != pcxEncRLE) )
    {
      goto ExitError;
    }

    l_width    = (l_pcxHdr.XMax - l_pcxHdr.XMin) + 1;
    l_height   = (l_pcxHdr.YMax - l_pcxHdr.YMin) + 1;
    l_linesize = l_pcxHdr.BytesPerLine * l_pcxHdr.NPlanes;
    l_bpp      = l_pcxHdr.BitsPerPixel * l_pcxHdr.NPlanes;

    if( (l_bpp != 8) && (l_bpp != 24) )
    {
      goto ExitError;
    }

    // Allocate line buffer for proper decoding
    l_lineBuffer = malloc(l_linesize);
    if( l_lineBuffer == NULL )
    {
      goto ExitError;
    }

    // Allocate pcx state buffer
    l_pcx = calloc(1, sizeof(pcx_t));
    if( l_pcx == NULL )
    {
      goto ExitError;
    }

    l_pcx->pcxFile    = l_pcxFile;
    l_pcx->lineBuffer = l_lineBuffer;
    l_pcx->width      = l_width;
    l_pcx->height     = l_height;
    l_pcx->linewidth  = l_pcxHdr.BytesPerLine;
    l_pcx->linesize   = l_linesize;
    l_pcx->bpp        = l_bpp;

    if( l_bpp == 8 )
    {
      if( pcx_LoadPal(l_pcx) == 0 )
      {
        goto ExitError;
      }
    }

    if( fseek(l_pcxFile, sizeof(pcxhdr_t), SEEK_SET) == 0 )
    {
      return l_pcx;
    }

    // Release any memory allocated on error
  ExitError:
    if( l_pcx )
    {
      free( l_pcx );
    }

    if( l_pcxFile )
    {
      fclose( l_pcxFile );
    }

    if( l_lineBuffer )
    {
      free( l_lineBuffer );
    }

    return NULL;
  }

  /*
   *  void pcx_Close( **p_pcx )
   *
   *  Purpose:
   *    Closes PCX file, and releases any memory
   *    used by the PCX structure.
   *
   *  Returns:
   *    Nothing
   */
  void pcx_Close( pcx_t** p_pcx )
  {
    if( p_pcx )
    {
      if( *p_pcx )
      {
        // Close file
        if( (*p_pcx)->pcxFile )
        {
          fclose( (*p_pcx)->pcxFile );
        }
        (*p_pcx)->pcxFile = NULL;

        // Release PCX line buffer
        if( (*p_pcx)->lineBuffer )
        {
          free( (*p_pcx)->lineBuffer );
        }
        (*p_pcx)->lineBuffer = NULL;
      }

      // Release PCX structure
      free( *p_pcx );
      *p_pcx = NULL;
    }
  }

  /*
   *  image_t* pcx_LoadImage( *p_ctx, *p_name, *p_pal )
   *
   *  Purpose:
   *    Loads the contents of a PCX into an Image.
   *    Performs minimal pixel conversion, but
   *    does not quantize.
   *
   *  Returns:
   *    NULL on error
   *    Image on success
   */
  image_t* pcx_LoadImage( vbectx_t* p_ctx, const char* p_name,
    rgba_t* p_pal )
  {
    image_t*  l_image;
    uint32_t* l_imageBuffer4;
    uint16_t* l_imageBuffer2;
    uint8_t*  l_imageBuffer1;
    pcx_t*    l_pcx;
    uint32_t  l_cvt8[256];
    uint32_t  l_i;
    uint32_t  l_j;
    uint32_t  l_k;
    uint32_t  l_pix4;
    uint16_t  l_pix2;
    uint8_t   l_pix1;
    uint8_t   l_r;
    uint8_t   l_g;
    uint8_t   l_b;

    if( !(p_ctx && p_name) )
    {
      return NULL;
    }

    // Open PCX file
    l_pcx = pcx_Open(p_name);
    if( l_pcx == NULL )
    {
      goto ExitError;
    }

    // Allocate image
    l_image = gfx_NewImage(p_ctx, l_pcx->width, l_pcx->height);
    if( l_image == NULL )
    {
      goto ExitError;
    }

    // Point to image data
    l_imageBuffer1 = ((uint8_t*)l_image) + sizeof(image_t);

    // Translate 8-BPP PCX to destination 8/15/16/24/32-BPP
    if( l_pcx->bpp == 8 )
    {
      // Initialize pixel conversion array
      switch( p_ctx->bpp )
      {
      case 15:
      case 16:
      case 24:
      case 32:
        // Pre-calculate pixel values from RGB values
        for( l_i = 0; l_i < 256; l_i++ )
        {
          l_r = l_pcx->pal[l_i].r;
          l_g = l_pcx->pal[l_i].g;
          l_b = l_pcx->pal[l_i].b;

          l_cvt8[l_i] = vbe_EncodePixel(p_ctx, l_r, l_g, l_b);
        }
        break;
      }

      switch( p_ctx->bpp )
      {
      case 8:
        for( l_i = 0; l_i < l_image->height; l_i++ )
        {
          // Decode 8-BPP pixels
          if( pcx_LoadLine8(l_pcx) == 0 )
          {
            goto ExitError;
          }

          // Copy pixels to destination buffer
          memcpy( l_imageBuffer1, l_pcx->lineBuffer, l_image->width );

          // Point to next line
          l_imageBuffer1 += l_image->linesize;
        }
        goto ExitSuccess;

      case 15:
      case 16:
        // Alias pointer to avoid unions or typecasts
        l_imageBuffer2 = (uint16_t*)l_imageBuffer1;

        for( l_i = 0; l_i < l_image->height; l_i++ )
        {
          // Decode 8-BPP pixels
          if( pcx_LoadLine8(l_pcx) == 0 )
          {
            goto ExitError;
          }

          // Translate pixels to destination buffer
          for( l_j = 0; l_j < l_image->width; l_j++ )
          {
            l_pix1 = l_pcx->lineBuffer[l_j];

            l_imageBuffer2[l_j] = (uint16_t)l_cvt8[l_pix1];
          }

          // Point to next line, and avoid an ugly typecast
          l_imageBuffer1 += l_image->linesize;
          l_imageBuffer2 = (uint16_t*)l_imageBuffer1;
        }
        goto ExitSuccess;

      case 24:
        for( l_i = 0; l_i < l_image->height; l_i++ )
        {
          // Decode 8-BPP pixels
          if( pcx_LoadLine8(l_pcx) == 0 )
          {
            goto ExitError;
          }

          // Translate pixels to destination buffer
          for( l_j = 0; l_j < l_image->width; l_j++ )
          {
            l_pix1 = l_pcx->lineBuffer[l_j];

            l_pix4 = l_cvt8[l_pix1];

            l_imageBuffer1[0] = (uint8_t)(l_pix4 >> 0);
            l_imageBuffer1[1] = (uint8_t)(l_pix4 >> 8);
            l_imageBuffer1[2] = (uint8_t)(l_pix4 >> 16);

            l_imageBuffer1 += 3;
          }
        }
        goto ExitSuccess;

      case 32:
        // Alias pointer to avoid unions or typecasts
        l_imageBuffer4 = (uint32_t*)l_imageBuffer1;

        for( l_i = 0; l_i < l_image->height; l_i++ )
        {
          // Decode 8-BPP pixels
          if( pcx_LoadLine8(l_pcx) == 0 )
          {
            goto ExitError;
          }

          // Translate pixels to destination buffer
          for( l_j = 0; l_j < l_image->width; l_j++ )
          {
            l_pix1 = l_pcx->lineBuffer[l_j];

            l_imageBuffer4[l_j] = l_cvt8[l_pix1];
          }

          // Point to next line, and avoid an ugly typecast
          l_imageBuffer1 += l_image->linesize;
          l_imageBuffer4 = (uint32_t*)l_imageBuffer1;
        }
        goto ExitSuccess;
      }

      goto ExitError;
    }

    // Translate 24-BPP PCX to 24/32-BPP
    if( l_pcx->bpp == 24 )
    {
      switch( p_ctx->bpp )
      {
      case 24:
        for( l_i = 0; l_i < l_image->height; l_i++ )
        {
          // Decode 24-BPP pixels
          if( pcx_LoadLine24(l_pcx) == 0 )
          {
            goto ExitError;
          }

          // Translate pixels to destination buffer
          l_j = 0;
          l_k = 0;
          for( l_j = 0; l_j < l_image->width; l_j++ )
          {
            l_r = l_pcx->lineBuffer[l_k + 0];
            l_g = l_pcx->lineBuffer[l_k + 1];
            l_b = l_pcx->lineBuffer[l_k + 2];

            l_pix4 = vbe_EncodePixel(p_ctx, l_r, l_g, l_b);

            l_imageBuffer4[l_k + 0] = (uint8_t)(l_pix4 >> 0);
            l_imageBuffer4[l_k + 1] = (uint8_t)(l_pix4 >> 8);
            l_imageBuffer4[l_k + 2] = (uint8_t)(l_pix4 >> 16);

            l_k += 3;
          }

          l_imageBuffer1 += l_image->linesize;
        }
        goto ExitSuccess;

      case 32:
        // Alias pointer to avoid unions or typecasts
        l_imageBuffer4 = (uint32_t*)l_imageBuffer1;

        for( l_i = 0; l_i < l_image->height; l_i++ )
        {
          // Decode 24-BPP pixels
          if( pcx_LoadLine24(l_pcx) == 0 )
          {
            goto ExitError;
          }

          // Translate pixels to destination buffer
          l_j = 0;
          l_k = 0;
          for( l_j = 0; l_j < l_image->width; l_j++ )
          {
            l_r = l_pcx->lineBuffer[l_k + 0];
            l_g = l_pcx->lineBuffer[l_k + 1];
            l_b = l_pcx->lineBuffer[l_k + 2];
            l_k += 3;

            l_pix4 = vbe_EncodePixel(p_ctx, l_r, l_g, l_b);

            l_imageBuffer4[l_j] = l_pix4;
          }

          // Point to next line, and avoid an ugly typecast
          l_imageBuffer1 += l_image->linesize;
          l_imageBuffer4 = (uint32_t*)l_imageBuffer1;
        }
        goto ExitSuccess;
      }

      goto ExitError;
    }

  ExitSuccess:
    if( (l_pcx->bpp == 8) && p_pal )
    {
      memcpy( p_pal, l_pcx->pal, sizeof(pal4_t) );
    }

    pcx_Close( &l_pcx );

    return l_image;

  ExitError:
    if( l_image )
    {
      free( l_image );
    }

    pcx_Close( &l_pcx );

    return NULL;
  }

  /*
   *  PCX files are encoded using an even length
   *  buffer. As a result, the PCX file is decoded
   *  into a line buffer, then copied to its
   *  destination. Otherwise, the decoder would
   *  be slightly more complicated due to having
   *  to special case the last pixel in one of
   *  three ways.
   */
