#ifndef __VBE_H
#define __VBE_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

  /*
   *  VBE Display Mode definitions
   */
  #define VBE_640x400x8    0x100

  #define VBE_640x480x8    0x101
  #define VBE_640x480x15   0x110
  #define VBE_640x480x16   0x111
  #define VBE_640x480x24   0x112

  #define VBE_800x600x8    0x103
  #define VBE_800x600x15   0x113
  #define VBE_800x600x16   0x114
  #define VBE_800x600x24   0x115

  #define VBE_1024x768x8   0x105
  #define VBE_1024x768x15  0x116
  #define VBE_1024x768x16  0x117
  #define VBE_1024x768x24  0x118

  #define VBE_1280x1024x8  0x107
  #define VBE_1280x1024x15 0x119
  #define VBE_1280x1024x16 0x11A
  #define VBE_1280x1024x24 0x11B

  /*
   *  Bit Field definitions
   */
  // VBEDriverInfo.Capabilities flags
  #define VBE_CAPS_DAC8      (((uint32_t)1) << 0)
  #define VBE_CAPS_NOTVGA    (((uint32_t)1) << 1)
  #define VBE_CAPS_BLANKFN9  (((uint32_t)1) << 2)
  #define VBE_CAPS_HWSTEREO  (((uint32_t)1) << 3)
  #define VBE_CAPS_STEREOEVC (((uint32_t)1) << 4)

  // VBEModeInfo.ModeAttributes flags
  #define VBE_MA_MODEHW (((uint32_t)1) << 0)
  #define VBE_MA_HASLFB (((uint32_t)1) << 7)
  #define VBE_MA_HW3BUF (((uint32_t)1) << 10)

  // VBEModeInfo.MemoryModel flags
  #define VBE_MM_PACKED 4 
  #define VBE_MM_DCOLOR 6
  #define VBE_MM_YUV    7

  /*
   *  Simulate Real Mode Interrupt Register Structure
   */
  #pragma pack( push, 1 )
  struct _rmiregs_t
  {
    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;
    uint32_t rsvd;
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;
    uint16_t flags;
    uint16_t es;
    uint16_t ds;
    uint16_t fs;
    uint16_t gs;
    uint16_t ip;
    uint16_t cs;
    uint16_t sp;
    uint16_t ss;
  };
  typedef struct _rmiregs_t rmiregs_t;
  #pragma pack( pop )

  /*
   *  VBE Driver Info Structure
   */
  #pragma pack( push, 1 )
  struct _VBEDriverInfo_t
  {
    uint8_t  VBESignature[4];
    uint16_t VBEVersion;
    uint32_t OEMStringPtr;
    uint32_t Capabilities;
    uint32_t VideoModePtr;
    uint16_t TotalMemory;

    // VBE 2.0 extensions
    uint16_t OemSoftwareRev;
    uint32_t OemVendorNamePtr;
    uint32_t OemProductNamePtr;
    uint32_t OemProductRevPtr;
    uint8_t  Reserved[222];
    uint8_t  OemDATA[256];
  };
  typedef struct _VBEDriverInfo_t VBEDriverInfo_t;
  #pragma pack( pop )

  /*
   *  VBE Mode Info Structure
   */
  #pragma pack( push, 1 )
  struct _VBEModeInfo_t
  {
    uint16_t ModeAttributes;
    uint8_t  WinAAttributes;
    uint8_t  WinBAttributes;
    uint16_t WinGranularity;
    uint16_t WinSize;
    uint16_t WinASegment;
    uint16_t WinBSegment;
    uint32_t WinFuncPtr;
    uint16_t BytesPerScanline;

    // VBE 1.2+
    uint16_t XResolution;
    uint16_t YResolution;
    uint8_t  XCharSize;
    uint8_t  YCharSize;
    uint8_t  NumberOfPlanes;
    uint8_t  BitsPerPixel;
    uint8_t  NumberOfBanks;
    uint8_t  MemoryModel;
    uint8_t  BankSize;
    uint8_t  NumberOfImagePages;
    uint8_t  Reserved1;

    // VBE 1.2+ Direct Color fields
    uint8_t  RedMaskSize;
    uint8_t  RedFieldPosition;
    uint8_t  GreenMaskSize;
    uint8_t  GreenFieldPosition;
    uint8_t  BlueMaskSize;
    uint8_t  BlueFieldPosition;
    uint8_t  RsvdMaskSize;
    uint8_t  RsvdFieldPosition;
    uint8_t  DirectColorModeInfo;

    // VBE 2.0+
    uint32_t PhysBasePtr;
    uint32_t Reserved2; // OffScreenMemOffset;
    uint16_t Reserved3; // OffScreenMemSize;

    // VBE 3.0+
    uint16_t LinBytesPerScanline;
    uint8_t  BnkNumberOfImagePages;
    uint8_t  LinNumberOfImagePages;
    uint8_t  LinRedMaskSize;
    uint8_t  LinRedFieldPosition;
    uint8_t  LinGreenMaskSize;
    uint8_t  LinGreenFieldPosition;
    uint8_t  LinBlueMaskSize;
    uint8_t  LinBlueFieldPosition;
    uint8_t  LinRsvdMaskSize;
    uint8_t  LinRsvdFieldPosition;
    uint32_t MaxPixelClock;

    uint8_t  Reserved4[189];
  };
  typedef struct _VBEModeInfo_t VBEModeInfo_t;
  #pragma pack( pop )

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

  /*
   *  32-bit RGB Structure and Palette
   */
  #pragma pack( push, 1 )
  struct _rgba_t
  {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
  };
  typedef struct _rgba_t rgba_t;
  #pragma pack( pop )

  typedef rgba_t pal4_t[256];

  /*
   *  Clip/General Rectangle
   */
  #pragma pack( push, 1 )
  struct _rect_t
  {
    union
    {
      int32_t x1;
      int32_t minx;
    };
    union
    {
      int32_t y1;
      int32_t miny;
    };
    union
    {
      int32_t x2;
      int32_t maxx;
    };
    union
    {
      int32_t y2;
      int32_t maxy;
    };
  };
  typedef struct _rect_t rect_t;
  #pragma pack( pop )

  /*
   *  VBE Selective Mode Information
   */
  #pragma pack(push, 1)
  struct _vbectx_t
  {
    uint8_t* lfb;
    uint32_t lfbsize;
    uint16_t lfbsel;
    uint8_t  bpp;
    uint8_t  rsvd;

    rect_t   clip;

    uint32_t width;
    uint32_t height;
    uint32_t linesize;

    uint32_t rmask;
    uint32_t gmask;
    uint32_t bmask;
    uint32_t amask;

    uint8_t  rshift;
    uint8_t  gshift;
    uint8_t  bshift;
    uint8_t  ashift;

    uint8_t  rpos;
    uint8_t  gpos;
    uint8_t  bpos;
    uint8_t  apos;
  };
  typedef struct _vbectx_t vbectx_t;
  #pragma pack(pop)

  /*
   *  DPMI Support Functions
   */
  uint32_t dpmi_AllocRealSeg( uint32_t p_size );

  void dpmi_FreeRealSeg( uint32_t* p_selseg );

  int dpmi_SimRealModeInt( uint8_t p_int, rmiregs_t* p_rmregs );

  uint16_t dpmi_AllocSel( void );

  void dpmi_FreeSel( uint16_t* p_sel );

  int dpmi_SetSelRights( uint16_t p_sel, uint16_t p_rights );

  int dpmi_SetSelBase( uint16_t p_sel, uint32_t p_linAddr );

  int dpmi_SetSelLimit( uint16_t p_sel, uint32_t p_limit );

  uint32_t dpmi_MapPhysicalAddress( uint32_t p_physAddr,
    uint32_t p_size );

  void dpmi_FreePhysicalAddress( uint32_t* p_linAddr );

  /*
   *  General System Support Functions
   */
  uint32_t sys_RM16ToFlat32( uint32_t p_RMSegOfs );

  /*
   *  VBE Support Functions
   */
  int vbe_Init( void );

  void vbe_Done( vbectx_t** p_ctx );

  int vbe_GetDriverInfo( VBEDriverInfo_t* p_drvInfo );

  int vbe_GetModeInfo( uint16_t p_mode, VBEModeInfo_t* p_modeInfo );

  /*
   *  VBE Display Functions
   */
  uint16_t vbe_FindMode( uint32_t p_width, uint32_t p_height,
    uint8_t p_bpp );

  vbectx_t* vbe_SetMode( uint16_t p_mode );

  void vbe_ReleaseCtx( vbectx_t** p_ctx );

  void vbe_CloseMode( vbectx_t** p_ctx );

  vbectx_t* vbe_CreateBuf( vbectx_t* p_ctx,
    uint32_t p_width, uint32_t p_height );

  void vbe_SetClip( vbectx_t* p_ctx,
    int32_t p_minx, int32_t p_miny,
    int32_t p_maxx, int32_t p_maxy );

  void vbe_Capture( vbectx_t* p_display, vbectx_t* p_buf );

  void vbe_Flip( vbectx_t* p_display, vbectx_t* p_buf );

  /*
   *  VBE Pixel Format Functions
   */
  void vbe_DecodePixel( vbectx_t* p_ctx, uint32_t p_pixel,
    uint8_t* p_r, uint8_t* p_g, uint8_t* p_b );

  uint32_t vbe_EncodePixel( vbectx_t* p_ctx,
    uint8_t p_r, uint8_t p_g, uint8_t p_b );

  /*
   *  8-BPP Graphics Functions
   */
  void vbe_GetPal( vbectx_t* p_ctx, rgba_t* p_pal );

  void vbe_SetPal( vbectx_t* p_ctx, rgba_t* p_pal );

#endif
