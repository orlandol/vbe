#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

  char* pcxFileName = "";

  /*
   *  24-bit RGB Structure and Palette
   */
  #pragma pack( push, 1 )
  struct _rgb_t
  {
    unsigned char r;
    unsigned char g;
    unsigned char b;
  };
  typedef struct _rgb_t rgb_t;
  #pragma pack( pop )

  typedef rgb_t pal3_t[256];

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


  pcxhdr_t pcxHdr;

  int LoadHeader( FILE* p_in )
  {
    if( fread(&pcxHdr, 1, sizeof(pcxHdr), p_in) == sizeof(pcxHdr) )
    {
      if( (pcxHdr.Manufacturer == pcxMfgZSoft) &&
          (pcxHdr.Version == pcxVer30) &&
          (pcxHdr.Encoding == pcxEncRLE) )
      {
        return -1;
      }
    }

    memset( &pcxHdr, 0, sizeof(pcxHdr) );

    return 0;
  }


  unsigned char* pcxData;
  uint32_t       pcxDataSize;

  int LoadData( FILE* p_in )
  {
    uint32_t       width;
    uint32_t       height;
    uint32_t       i;
    uint32_t       ofs;
    uint32_t       plane;
    uint32_t       dataLineSize;
    uint32_t       lineSize;
    uint32_t       pixSize;
    uint32_t       size;
    unsigned char* lineBuf;
    unsigned char* pcxDataTmp;
    unsigned char  run;
    unsigned char  pixel;
    unsigned char  bpp;

    if( pcxData )
    {
      free( pcxData );
    }
    pcxData = NULL;

    width  = (pcxHdr.XMax - pcxHdr.XMin) + 1;
    height = (pcxHdr.YMax - pcxHdr.YMin) + 1;

    dataLineSize = width * pcxHdr.NPlanes;

    lineSize = pcxHdr.BytesPerLine * pcxHdr.NPlanes;

    bpp = pcxHdr.BitsPerPixel * pcxHdr.NPlanes;
    if( (bpp != 8) && (bpp != 24) )
    {
      return 0;
    }

    pixSize = (bpp + 7) / 8;

    pcxDataSize = width * height * pcxHdr.NPlanes;
    if( pcxDataSize == 0 )
    {
      return 0;
    }

    pcxData = malloc(pcxDataSize);
    if( pcxData == NULL )
    {
      return 0;
    }

    lineBuf = malloc(lineSize);
    if( lineBuf == NULL )
    {
      free( pcxData );

      return 0;
    }

    size = 0;

    pcxDataTmp = pcxData;

    while( height )
    {
      for( plane = 0; plane < pcxHdr.NPlanes; plane++ )
      {
        ofs = plane;

        i = 0;
        while( i < pcxHdr.BytesPerLine )
        {
          if( fread(&pixel, 1, 1, p_in) != 1 )
          {
            free( pcxData );
            pcxData = NULL;

            free( lineBuf );

            return 0;
          }

          if( pixel >= 0xC0 )
          {
            run = pixel & 0x3F;

            if( fread(&pixel, 1, 1, p_in) != 1 )
            {
              free( pcxData );
              pcxData = NULL;

              free( lineBuf );

              return 0;
            }

            size += run;

            i += run;

            while( run )
            {
              lineBuf[ofs] = pixel;
              ofs += pixSize;

              run--;
            }
          }
          else
          {
            size++;

            i++;

            lineBuf[ofs] = pixel;
            ofs += pixSize;
          }
        }
      }

      memcpy( pcxDataTmp, lineBuf, dataLineSize );
      pcxDataTmp += dataLineSize;

      height--;
    }

    free( lineBuf );

    return -1;
  }


  pal3_t pcxPal;

  int LoadPal( FILE* p_in )
  {
    uint32_t i;
    char     sig;
    uint8_t  bpp;

    bpp = pcxHdr.BitsPerPixel * pcxHdr.NPlanes;
    if( bpp != 8 )
    {
      return 0;
    }

    if( fseek(p_in, -769, SEEK_END) != 0 )
    {
      return 0;
    }

    if( fread(&sig, 1, 1, p_in) != 1 )
    {
      return 0;
    }

    if( sig != 12 )
    {
      return 0;
    }

    if( fread(pcxPal, 1, 768, p_in) != 768 )
    {
      memset( pcxPal, 0, sizeof(pcxPal) );

      return 0;
    }

    return -1;
  }


  void DumpHeader()
  {
    FILE*   out;
    uint8_t i;
    uint8_t j;

    out = fopen("pcxhdr.txt", "w");
    if( out )
    {
      fprintf( out, "PCX Header for \"%s\"\n\n", pcxFileName );
      fprintf( out, "Manufacturer: %u\n", pcxHdr.Manufacturer );
      fprintf( out, "     Version: %u\n", pcxHdr.Version );
      fprintf( out, "    Encoding: %u\n", pcxHdr.Encoding );
      fprintf( out, "BitsPerPixel: %u (%u)\n",
        pcxHdr.BitsPerPixel,
        pcxHdr.BitsPerPixel * pcxHdr.NPlanes );
      fprintf( out, "        XMin: %i\n", pcxHdr.XMin );
      fprintf( out, "        YMin: %i\n", pcxHdr.YMin );
      fprintf( out, "        XMax: %i\n", pcxHdr.XMax );
      fprintf( out, "        YMax: %i\n", pcxHdr.YMax );
      fprintf( out, "        HDpi: %u\n", pcxHdr.HDpi );
      fprintf( out, "        VDpi: %u\n", pcxHdr.VDpi );
      fprintf( out, "    Reserved: %u\n", pcxHdr.Reserved );
      fprintf( out, "     NPlanes: %u\n", pcxHdr.NPlanes );
      fprintf( out, "BytesPerLine: %u\n", pcxHdr.BytesPerLine );
      fprintf( out, " PaletteInfo: %u\n", pcxHdr.PaletteInfo );
      fprintf( out, " HScreenSize: %u\n", pcxHdr.HScreenSize );
      fprintf( out, " VScreenSize: %u\n", pcxHdr.VScreenSize );

      fprintf( out, "\nColormap:\n{\n" );
      for( i = 0; i < 16; i++ )
      {
        fprintf( out, "  "\
          "[%#.2u]: R = %.2X; G = %.2X; B = %.2X;\n",
          i,
          pcxHdr.Colormap[i].r,
          pcxHdr.Colormap[i].g,
          pcxHdr.Colormap[i].b );
      }
      fprintf( out, "}\n" );

      fprintf( out, "\nFiller:\n{\n" );
      j = 0;
      for( i = 0; i < 54; i++ )
      {
        if( j == 0 )
        {
          fprintf( out, " " );
        }

        fprintf( out, " %.2X", pcxHdr.Filler[i] );

        j++;

        if( j == 18 )
        {
          fprintf( out, "\n" );

          j = 0;
        }
      }
      fprintf( out, "}\n" );

      fclose( out );
    }
  }

  void DumpData()
  {
    FILE* out;

    out = fopen("pcxdata", "wb");
    if( out )
    {
      if( fwrite(pcxData, 1, pcxDataSize, out) != pcxDataSize )
      {
        printf( "ERROR: Partial data dump\n" );
      }

      fclose( out );
    }
  }

  void DumpPal()
  {
    FILE*    out;
    pal3_t   pal;
    uint32_t i;
    uint8_t  bpp;

    bpp = pcxHdr.BitsPerPixel * pcxHdr.NPlanes;

    if( bpp == 8 )
    {
      out = fopen("pcxpal", "wb");
      if( out )
      {
        if( fwrite(pcxPal, 1, sizeof(pcxPal), out) != sizeof(pcxPal) )
        {
          printf( "ERROR: Partial palette dump\n" );
        }

        fclose( out );
      }
    }
  }

int main( int argc, char* argv[] )
{
  FILE* pcx;
/*
uint8_t* vmem = (uint8_t*)0x000A0000;
uint8_t* data;
uint32_t width;
uint32_t height;
*/

  pcxData = NULL;

  if( argc < 2 )
  {
    printf( "Usage: pcx file.pcx\n" );

    return 1;
  }

  pcx = fopen(argv[1], "rb");
  if( pcx == NULL )
  {
    printf( "Unable to open file \"%s\"\n", argv[1] );

    return 2;
  }

  pcxFileName = argv[1];

  if( LoadHeader(pcx) )
  {
    printf( "Dumping header...\n" );
    DumpHeader();
  }

  if( LoadData(pcx) )
  {
    printf( "Dumping data...\n" );
    DumpData();
  }

  if( LoadPal(pcx) )
  {
    printf( "Dumping palette...\n" );
    DumpPal();
  }

/*
  __asm
  {
    pushad

    mov     ax, 0x13
    int     0x10

    popad
  }

  data = pcxData;

  width  = pcxHdr.XMax - pcxHdr.XMin + 1;
  height = pcxHdr.YMax - pcxHdr.YMin + 1;

  while( height )
  {
    memcpy( vmem, data, width );

    vmem += 320;
    data += width;

    height--;
  }

  __asm
  {
    pushad

    xor     ax, ax
    int     0x16

    mov     ax, 0x03
    int     0x10

    popad
  }
*/

  if( pcxData )
  {
    free( pcxData );
  }
  pcxData = NULL;

  fclose( pcx );

  return 0;
}
