#include <stdio.h>
#include <string.h>

#include "vbe.h"
#include "gfx.h"

  void demo2_ShowDriverInfo( void )
  {
    VBEDriverInfo_t l_drvInfo;

    memset( &l_drvInfo, 0, sizeof(l_drvInfo) );

    vbe_GetDriverInfo( &l_drvInfo );

    // Display select driver information
    printf( "VBE Driver Info\n" );
    printf( "---------------\n" );

    printf( "   Signature: '%c%c%c%c'\n",
      l_drvInfo.VBESignature[0], l_drvInfo.VBESignature[1],
      l_drvInfo.VBESignature[2], l_drvInfo.VBESignature[3] );

    printf( "     Version: %X.%X\n",
      (l_drvInfo.VBEVersion >> 8), (l_drvInfo.VBEVersion & 0xFF) );

    printf( "  OEM String: '%s'\n", (char*)l_drvInfo.OEMStringPtr );

    printf( "Capabilities: %0.8X\n", l_drvInfo.Capabilities );
    if( l_drvInfo.Capabilities & VBE_CAPS_DAC8 )
    {
      printf( "  * DAC registers switchable to 8-Bits\n" );
    }
    else
    {
      printf( "  * DAC registers fixed at 6-Bits\n" );
    }
    if( l_drvInfo.Capabilities & VBE_CAPS_NOTVGA )
    {
      printf( "  * Video card is not VGA compatible\n" );
    }
    if( l_drvInfo.Capabilities & VBE_CAPS_BLANKFN9 )
    {
      printf( "  * Use Blank Bit in Function 09h\n" );
    }

    printf( "Video Memory: %u\n", l_drvInfo.TotalMemory * 64 );

    if( l_drvInfo.VBEVersion >= 0x0200 )
    {
      printf( "OEM Soft Rev: %0.4X\n", l_drvInfo.OemSoftwareRev );
      if( l_drvInfo.OemVendorNamePtr )
      {
        printf( "  OEM Vendor: '%s'\n",
          (char*)l_drvInfo.OemVendorNamePtr );
      }
      if( l_drvInfo.OemProductNamePtr )
      {
        printf( " OEM Product: '%s'\n",
          (char*)l_drvInfo.OemProductNamePtr );
      }
      if( l_drvInfo.OemProductRevPtr )
      {
        printf( "OEM Revision: '%s'\n",
          (char*)l_drvInfo.OemProductRevPtr );
      }
    }
    printf( "\n" );
  }

  void demo2_ShowAvailableModes( void )
  {
    VBEDriverInfo_t l_drvInfo;
    VBEModeInfo_t   l_modeInfo;
    uint16_t*       l_modeList;
    uint32_t        l_i;
    uint16_t        l_mode;

    printf( "Available Modes:\n" );

    l_i = 0;

    memset( &l_drvInfo, 0, sizeof(l_drvInfo) );

    vbe_GetDriverInfo( &l_drvInfo );

    l_modeList = (uint16_t*)l_drvInfo.VideoModePtr;

    l_mode = *l_modeList;
    while( l_mode != 0xFFFF )
    {
      l_mode = *l_modeList;

      memset( &l_modeInfo, 0, sizeof(l_modeInfo) );
      if( vbe_GetModeInfo(l_mode, &l_modeInfo) )
      {
        switch( l_modeInfo.BitsPerPixel )
        {
        case 8:
        case 15:
        case 16:
        case 24:
        case 32:
          if( l_i == 14 )
          {
            printf( "\n" );

            l_i = 0;
          }
          l_i++;

          printf( " %0.4X", l_mode );

          break;
        }
      }

      l_modeList++;
    }

  }

  void demo2_ShowModeInfo( uint16_t p_mode )
  {
    VBEDriverInfo_t l_drvInfo;
    VBEModeInfo_t   l_modeInfo;

    memset( &l_drvInfo, 0, sizeof(l_drvInfo) );
    memset( &l_modeInfo, 0, sizeof(l_modeInfo) );

    vbe_GetDriverInfo( &l_drvInfo );

    if( vbe_GetModeInfo(p_mode, &l_modeInfo) )
    {
      switch( l_modeInfo.BitsPerPixel )
      {
      case 8:
      case 15:
      case 16:
      case 24:
      case 32:
        break;

      default:
        printf( "Mode %#.4X not 8/15/16/24/32-BPP\n", p_mode );
        return;
      }
    }
    else
    {
      printf( "Mode %#.4X not supported\n", p_mode );
      return;
    }

    // Display select mode information
    printf( "Mode Information\n" );
    printf( "----------------\n" );

    // Mode specified and validated, display mode information
    printf( "        Attributes: %.4X\n", l_modeInfo.ModeAttributes );
    if( l_modeInfo.ModeAttributes & VBE_MA_MODEHW )
    {
      printf( "          * Mode supported by hardware\n" );
    }
    if( l_modeInfo.ModeAttributes & VBE_MA_HASLFB )
    {
      printf( "          * Linear Frame Buffer supported\n" );
    }
    if( l_modeInfo.ModeAttributes & VBE_MA_HW3BUF )
    {
      printf( "          * Hardware Triple Buffering supported\n" );
    }

    printf( "Bytes Per Scanline: %u\n", l_modeInfo.BytesPerScanline );
    printf( "             Width: %u\n", l_modeInfo.XResolution );
    printf( "            Height: %u\n", l_modeInfo.YResolution );
    printf( "    Bits Per Pixel: %u\n", l_modeInfo.BitsPerPixel );

    printf( "      Memory Model: " );
    switch( l_modeInfo.MemoryModel )
    {
    case VBE_MM_PACKED:
      printf( "Packed Pixel\n" );
      break;

    case VBE_MM_DCOLOR:
      printf( "Direct Color\n" );
      break;

    case VBE_MM_YUV:
      printf( "YUV\n" );
      break;

    default:
      printf( "%.2X\n", l_modeInfo.MemoryModel );
    }

    if( (l_modeInfo.MemoryModel == VBE_MM_DCOLOR) ||
      (l_modeInfo.MemoryModel == VBE_MM_YUV) )
    {
      printf( "     Red Mask Size: %u\n", l_modeInfo.RedMaskSize );
      printf( "     Red Field Pos: %u\n", l_modeInfo.RedFieldPosition );
      printf( "   Green Mask Size: %u\n", l_modeInfo.GreenMaskSize );
      printf( "   Green Field Pos: %u\n", l_modeInfo.GreenFieldPosition );
      printf( "    Blue Mask Size: %u\n", l_modeInfo.BlueMaskSize );
      printf( "    Blue Field Pos: %u\n", l_modeInfo.BlueFieldPosition );
      printf( "    Rsvd Mask Size: %u\n", l_modeInfo.RsvdMaskSize );
      printf( "    Rsvd Field Pos: %u\n", l_modeInfo.RsvdFieldPosition );
    }

    if( l_drvInfo.VBEVersion >= 0x0200 )
    {
      printf( " Phys Frame Buffer: %.8X\n", l_modeInfo.PhysBasePtr );
      printf( " Off-Screen Offset: %u\n", l_modeInfo.Reserved2 );
      printf( "   Off-Screen Size: %u\n", l_modeInfo.Reserved3 );
    }
  }

  void demo2_ShowEachAvailableModeInfo( void )
  {
    VBEDriverInfo_t l_drvInfo;
    uint16_t*       l_modeList;
    uint16_t        l_mode;

    memset( &l_drvInfo, 0, sizeof(l_drvInfo) );

    vbe_GetDriverInfo( &l_drvInfo );

    l_modeList = (uint16_t*)l_drvInfo.VideoModePtr;
    if( l_modeList == NULL )
    {
      return;
    }

    l_mode = *l_modeList;

    while( l_mode != 0xFFFF )
    {
      l_mode = *l_modeList;

      demo2_ShowModeInfo(l_mode);

      l_modeList++;
    }
  }

int main( int argc, char* argv[] )
{
  uint16_t l_mode;

  if( vbe_Init() == 0 )
  {
    printf( "ERROR: VBE driver not supported.\n" );

    return 0;
  }

  demo2_ShowDriverInfo();

  if( argc < 2 )
  {
    // Display available 8/15/16/24/32-BPP modes by default
    demo2_ShowAvailableModes();

    goto Demo2_Exit;
  }

  if( strcmp(argv[1], "all") == 0 )
  {
    // Display mode information for each available mode
    demo2_ShowEachAvailableModeInfo();
  }
  else
  {
    // Display mode information for specific mode
    l_mode = strtoul(argv[1], NULL, 16);

    demo2_ShowModeInfo( l_mode );
  }

Demo2_Exit:
  printf( "\n" );

  vbe_Done( NULL );

  return 0;
}
