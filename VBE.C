#include <stdio.h>

#include "vbe.h"

  uint32_t vbe_selseg = 0;

  /*
   *  uint32_t dpmi_AllocRealSeg( p_size )
   *
   *  Purpose:
   *    Allocates p_size bytes of low memory,
   *    padded to a 16 byte boundary.
   *
   *  Returns:
   *    0 on error
   *    ((PMSelector << 16) | RMSegment) on success
   */
  uint32_t dpmi_AllocRealSeg( uint32_t p_size );
  #pragma aux dpmi_AllocRealSeg=\
  "  test    ebx, ebx"\
  "  jz      AllocRealSeg_Error"\
  "  cmp     ebx, 65535"\
  "  ja      AllocRealSeg_Error"\
  "  add     ebx, 15"\
  "  shr     ebx, 4"\
  "  mov     eax, 0x100"\
  "  int     0x31"\
  "  jc      AllocRealSeg_Error"\
  "  and     eax, 0x0000FFFF"\
  "  shl     edx, 16"\
  "  add     eax, edx"\
  "  jmp     AllocRealSeg_Exit"\
  "AllocRealSeg_Error:"\
  "  xor     eax, eax"\
  "AllocRealSeg_Exit:"\
  parm[ebx]\
  modify[ebx edx]\
  value[eax];

  /*
   *  void dpmi_FreeRealSeg( *p_selseg )
   *
   *  Purpose:
   *    Releases previously allocated low memory.
   *
   *  Returns:
   *    Nothing
   *
   */
  void dpmi_FreeRealSeg( uint32_t* p_selseg );
  #pragma aux dpmi_FreeRealSeg=\
  "  test    edi, edi"\
  "  jz      FreeRealSeg_Error"\
  "  mov     edx, [edi]"\
  "  push    edi"\
  "  shr     edx, 16"\
  "  mov     eax, 0x101"\
  "  int     0x31"\
  "  pop     edi"\
  "  mov     dword ptr [edi], 0"\
  "FreeRealSeg_Error:"\
  parm[edi]\
  modify[eax edx edi];

  /*
   *  int dpmi_SimRealModeInt( p_int, p_rmregs )
   *
   *  Purpose:
   *    Simulates a Real Mode Interrupt.
   *
   *  Returns:
   *    0 on error
   *    1 on success
   */
  int dpmi_SimRealModeInt( uint8_t p_int, rmiregs_t* p_rmregs );
  #pragma aux dpmi_SimRealModeInt=\
  "  push    es"\
  "  test    edi, edi"\
  "  jz      SimRealModeInt_Error"\
  "  and     ebx, 0x000000FF"\
  "  xor     ecx, ecx"\
  "  mov     eax, 0x300"\
  "  int     0x31"\
  "  jc      SimRealModeInt_Error"\
  "  mov     eax, 1"\
  "  jmp     SimRealModeInt_Exit"\
  "SimRealModeInt_Error:"\
  "  xor     eax, eax"\
  "SimRealModeInt_Exit:"\
  "  pop     es"\
  parm[bl][edi]\
  modify[ebx ecx edx edi esi]\
  value[eax];

  /*
   *  uint16_t dpmi_AllocSel()
   *
   *  Purpose:
   *    Allocates one descriptor.
   *
   *  Returns:
   *    0 on error
   *    Selector on success
   */
  uint16_t dpmi_AllocSel( void );
  #pragma aux dpmi_AllocSel=\
  "  mov     eax, 0"\
  "  mov     ecx, 1"\
  "  int     0x31"\
  "  jnc     AllocSel_Exit"\
  "  xor     ax, ax"\
  "AllocSel_Exit:"\
  modify[ecx]\
  value[ax];

  /*
   *  void dpmi_FreeSel( *p_sel )
   *
   *  Purpose:
   *    Releases a descriptor.
   *
   *  Returns:
   *    Nothing
   */
  void dpmi_FreeSel( uint16_t* p_sel );
  #pragma aux dpmi_FreeSel=\
  "  test    edi, edi"\
  "  jz      FreeSel_Error"\
  "  mov     bx, [edi]"\
  "  mov     dword ptr [edi], 0"\
  "  mov     eax, 1"\
  "  int     0x31"\
  "FreeSel_Error:"\
  parm[edi]\
  modify[eax ebx];

  /*
   *  int dpmi_SetSelRights( p_sel, p_rights )
   *
   *  Purpose:
   *    Modifies access rights and type fields for the
   *    specified selector.
   *
   *  Returns:
   *    0 on error
   *    1 on success
   */
  int dpmi_SetSelRights( uint16_t p_sel, uint16_t p_rights );
  #pragma aux dpmi_SetSelRights=\
  "  mov     eax, 9"\
  "  int     0x31"\
  "  mov     eax, 0"\
  "  jc      SetSelRights_Error"\
  "  mov     eax, 1"\
  "SetSelRights_Error:"\
  parm[bx][cx]\
  value[eax];

  /*
   *  int dpmi_SetSelBase( p_sel, p_linAddr )
   *
   *  Purpose:
   *    Sets the base address for the specified selector.
   *
   *  Returns:
   *    0 on error
   *    1 on success
   */
  int dpmi_SetSelBase( uint16_t p_sel, uint32_t p_linAddr );
  #pragma aux dpmi_SetSelBase=\
  "  mov     edx, ecx"\
  "  shr     ecx, 16"\
  "  and     edx, 0xFFFF"\
  "  mov     eax, 7"\
  "  int     0x31"\
  "  jc      SetSelBase_Error"\
  "  mov     eax, 1"\
  "  jmp     SetSelBase_Exit"\
  "SetSelBase_Error:"\
  "  xor     eax, eax"\
  "SetSelBase_Exit:"\
  parm[bx][ecx]\
  modify[ecx edx]\
  value[eax];

  /*
   *  int dpmi_SetSelLimit( p_sel, p_limit )
   *
   *  Purpose:
   *    Sets the limit for the specified selector.
   *
   *  Returns:
   *    0 on error
   *    1 on success
   */
  int dpmi_SetSelLimit( uint16_t p_sel, uint32_t p_limit );
  #pragma aux dpmi_SetSelLimit=\
  "  mov     edx, ecx"\
  "  shr     ecx, 16"\
  "  and     edx, 0xFFFF"\
  "  mov     eax, 8"\
  "  int     0x31"\
  "  jc      SetSelLimit_Error"\
  "  mov     eax, 1"\
  "  jmp     SetSelLimit_Exit"\
  "SetSelLimit_Error:"\
  "  xor     eax, eax"\
  "SetSelLimit_Exit:"\
  parm[bx][ecx]\
  modify[ecx edx]\
  value[eax];

  /*
   *  uint32_t dpmi_MapPhysicalAddress( p_physAddr, p_size )
   *
   *  Purpose:
   *    Converts a physical address to a linear address.
   *
   *  Returns:
   *    0 on error
   *    Linear Address on success
   */
  uint32_t dpmi_MapPhysicalAddress( uint32_t p_physAddr,
    uint32_t p_size );
  #pragma aux dpmi_MapPhysicalAddress=\
  "  mov     ecx, ebx"\
  "  mov     edi, esi"\
  "  shr     ebx, 16"\
  "  and     ecx, 0xFFFF"\
  "  shr     esi, 16"\
  "  and     edi, 0xFFFF"\
  "  mov     eax, 0x800"\
  "  int     0x31"\
  "  jc      MapPhysicalAddress_Error"\
  "  shl     ebx, 16"\
  "  and     ecx, 0xFFFF"\
  "  mov     eax, ebx"\
  "  or      eax, ecx"\
  "  jmp     MapPhysicalAddress_Exit"\
  "MapPhysicalAddress_Error:"\
  "  xor     eax, eax"\
  "MapPhysicalAddress_Exit:"\
  parm[ebx][esi]\
  modify[ebx ecx edx esi edi]\
  value[eax];

  /*
   *  uint32_t dpmi_FreePhysicalAddress( *p_linAddr )
   *
   *  Purpose:
   *    Releases a previously mapped linear address.
   *
   *  Returns:
   *    Nothing
   */
  void dpmi_FreePhysicalAddress( uint32_t* p_linAddr );
  #pragma aux dpmi_FreePhysicalAddress=\
  "  test    edi, edi"\
  "  jz      FreePhysicalAddress_Error"\
  "  mov     bx, [edi + 2]"\
  "  mov     cx, [edi + 0]"\
  "  mov     dword ptr [edi], 0"\
  "  mov     eax, 0x0801"\
  "  int     0x31"\
  "FreePhysicalAddress_Error:"\
  parm[edi]\
  modify[eax];

  /*
   *  uint32_t sysRM16ToFlat32( p_RMSegOfs )
   *
   *  Purpose:
   *    Converts a Real Mode Seg16:Ofs16 address
   *    into a Flat 32-Bit address.
   *
   *  Returns:
   *    (Seg16 << 4) + Ofs16
   */
  uint32_t sys_RM16ToFlat32( uint32_t p_RMSegOfs );
  #pragma aux sys_RM16ToFlat32=\
  "  mov     edx, eax"\
  "  and     eax, 0xFFFF0000"\
  "  and     edx, 0x0000FFFF"\
  "  shr     eax, 12"\
  "  add     eax, edx"\
  parm[eax]\
  modify[edx]\
  value[eax];

  /*
   *  int vbe_Init()
   *
   *  Purpose:
   *    Allocates low memory for VBE driver info, low memory
   *    for VBE mode info, and tests for VBE 2.0+.
   *
   *  Returns:
   *    0 on error
   *    1 on success
   */
  int vbe_Init( void )
  {
    VBEDriverInfo_t l_drv;

    if( vbe_selseg == 0 )
    {
      vbe_selseg = dpmi_AllocRealSeg(2048);
    }

    // Indirect revalidation of vbe_selseg
    return vbe_GetDriverInfo(&l_drv);
  }

  /*
   *  void vbe_Done( **p_ctx )
   *
   *  Purpose:
   *    Releases low memory, releases system resources used
   *    by VBE Context, and resets display mode.
   *
   *  Returns:
   *    Nothing
   */
  void vbe_Done( vbectx_t** p_ctx )
  {
    // Already checks for/sets to NULL
    vbe_CloseMode( p_ctx );

    // Already checks for/sets to NULL
    dpmi_FreeRealSeg( &vbe_selseg );
  }

  /*
   *  int vbe_GetDriverInfo( *p_drvInfo )
   *
   *  Purpose:
   *    Returns VBE driver info. Handles low memory.
   *    Validates for version VBE 2.0+.
   *
   *  Returns:
   *    0 on error
   *    1 on success
   */
  int vbe_GetDriverInfo( VBEDriverInfo_t* p_drvInfo )
  {
    char             l_vbe2Sig[4] = "VBE2";
    rmiregs_t        l_regs;
    VBEDriverInfo_t* l_di;
    char             l_drvName[128];

    if( vbe_selseg && p_drvInfo )
    {
      // 2K DOS memory is divided in half. First half is for driver info.
      l_di = (VBEDriverInfo_t*)((vbe_selseg & 0x0000FFFF) << 4);
      memset( l_di, 0, 1024 );
      memcpy( l_di, l_vbe2Sig, 4 );

      memset( &l_regs, 0, sizeof(l_regs) );
      l_regs.eax = 0x4F00;
      l_regs.es  = (uint16_t)vbe_selseg;
      l_regs.edi = 0;
      dpmi_SimRealModeInt( 0x10, &l_regs );

      if( (((uint16_t)l_regs.eax) == 0x004F) &&
        (strncmp(l_di->VBESignature, "VESA", 4) == 0) &&
        (l_di->VBEVersion >= 0x200) )
      {
        l_di->OEMStringPtr =
          sys_RM16ToFlat32(l_di->OEMStringPtr);
        l_di->VideoModePtr =
          sys_RM16ToFlat32(l_di->VideoModePtr);

        l_di->OemVendorNamePtr =
          sys_RM16ToFlat32(l_di->OemVendorNamePtr);
        l_di->OemProductNamePtr =
          sys_RM16ToFlat32(l_di->OemProductNamePtr);
        l_di->OemProductRevPtr =
          sys_RM16ToFlat32(l_di->OemProductRevPtr);

        memcpy( p_drvInfo, l_di, sizeof(VBEDriverInfo_t) );

        return 1;
      }
    }

    return 0;
  }

  /*
   *  int vbe_GetModeInfo( p_mode, *p_modeInfo )
   *
   *  Purpose:
   *    Returns specified VBE mode info. Handles low memory.
   *    Validates for LFB and 8/15/16/24/32-BPP.
   *
   *  Returns:
   *    0 on error
   *    1 on success
   */
  int vbe_GetModeInfo( uint16_t p_mode, VBEModeInfo_t* p_modeInfo )
  {
    rmiregs_t      l_regs;
    VBEModeInfo_t* l_mi;

    if( !(p_mode && vbe_selseg && p_modeInfo) )
    {
      return 0;
    }

    // Check if mode list terminator was passed as parameter
    if( p_mode == 0xFFFF )
    {
      return 0;
    }

    // 2K DOS memory is divided in half. Second half is for mode info.
    l_mi = (VBEModeInfo_t*)(((vbe_selseg & 0x0000FFFF) << 4) + 1024);
    memset( l_mi, 0, 1024 );

    memset( &l_regs, 0, sizeof(l_regs) );
    l_regs.eax = 0x4F01;
    l_regs.ecx = p_mode;
    l_regs.es  = (uint16_t)vbe_selseg;
    l_regs.edi = 1024;
    dpmi_SimRealModeInt( 0x10, &l_regs );

    if( ((uint16_t)l_regs.eax) != 0x004F )
    {
      return 0;
    }

    // Mode must be supported
    if( (l_mi->ModeAttributes & VBE_MA_MODEHW) == 0 )
    {
      return 0;
    }

    // LFB must be present
    if( (l_mi->ModeAttributes & VBE_MA_HASLFB) == 0 )
    {
      return 0;
    }

    memcpy( p_modeInfo, l_mi, sizeof(VBEModeInfo_t) );

    return 1;
  }

  /*
   *  uint16_t vbe_FindMode( p_width, p_height, p_bpp )
   *
   *  Purpose:
   *    Searches VBE Driver mode list for a specific
   *    VBE or OEM LFB mode. BPP must be an exact match.
   *    Ignores banked display modes.
   *
   *  Returns:
   *    0 on error
   *    VBE or OEM mode on success
   */
  uint16_t vbe_FindMode( uint32_t p_width, uint32_t p_height,
    uint8_t p_bpp )
  {
    VBEDriverInfo_t l_drv;
    VBEModeInfo_t   l_mi;
    uint16_t*       l_modeList;
    uint16_t        l_mode;

    // Requested width, height, and BPP must not be 0
    if( !(p_width && p_height && p_bpp) )
    {
      return 0;
    }

    // Get fresh copy of VBE driver info
    if( vbe_GetDriverInfo(&l_drv) == 0 )
    {
      return 0;
    }

    // Set and validate mode list pointer
    l_modeList = (uint16_t*)l_drv.VideoModePtr;
    if( l_modeList == NULL )
    {
      return 0;
    }

    // Get first mode
    l_mode = *l_modeList;

    // Loop through all modes. Stop on first exact match.
    while( l_mode != 0xFFFF )
    {
      // Get next mode
      l_mode = *l_modeList;

      // Initialize mode information
      memset( &l_mi, 0, sizeof(l_mi) );

      // Ignore return result since defaults are 0
      vbe_GetModeInfo( l_mode, &l_mi );

      // If exact match...
      if( (l_mi.XResolution == p_width) &&
          (l_mi.YResolution == p_height) &&
          (l_mi.BitsPerPixel == p_bpp) )
      {
        // ...then return the display mode
        return l_mode;
      }

      l_modeList++;
    }

    return 0;
  }

  /*
   *  uint16_t vbe_NearestMode( p_width, p_height, p_bpp )
   *
   *  Purpose:
   *    Searches VBE Driver mode list for a VBE or OEM LFB mode
   *    at or above the requested width, height, and BPP.
   *    BPP must be an exact match.
   *    Ignores banked display modes.
   *
   *  Returns:
   *    0 on error
   *    VBE or OEM mode on success
   */
  uint16_t vbe_NearestMode( uint32_t p_width, uint32_t p_height,
    uint8_t p_bpp )
  {
    return 0;
  }

  /*
   *  vbectx_t* vbe_SetMode( p_mode )
   *
   *  Purpose:
   *    Sets the display mode to specified VBE LFB display mode.
   *    Ignores banked display modes.
   *
   *  Returns:
   *    NULL on error
   *    VBE Context on success
   */
  vbectx_t* vbe_SetMode( uint16_t p_mode )
  {
    rmiregs_t     l_regs;
    vbectx_t*     l_ctx;
    VBEModeInfo_t l_mi;
    uint32_t      l_size;

    // Already validates Mode and LFB support
    memset( &l_mi, 0, sizeof(l_mi) );
    if( vbe_GetModeInfo(p_mode, &l_mi) == 0 )
    {
      return NULL;
    }

    l_ctx = NULL;
    switch( l_mi.BitsPerPixel )
    {
    case 8:
      // EXTRA: Set default palette, and fall through

    case 15:
    case 16:
    case 24:
    case 32:
      if( l_mi.PhysBasePtr )
      {
        l_ctx = (vbectx_t*)calloc(1, sizeof(VBEModeInfo_t));
      }
      break;

    default:
      // Unsupported video mode
      return NULL;
    }

    if( l_ctx == NULL )
    {
      return NULL;
    }

    // Set VBE display mode
    memset( &l_regs, 0, sizeof(l_regs) );
    l_regs.eax = 0x4F02;
    l_regs.ebx = p_mode | 0x4000; // 0x4000 = LFB flag
    dpmi_SimRealModeInt( 0x10, &l_regs );
    if( ((uint16_t)l_regs.eax) != 0x004F )
    {
      vbe_ReleaseCtx( &l_ctx );
      return NULL;
    }

    // Allocate LFB selector
    l_ctx->lfbsel = dpmi_AllocSel();
    if( l_ctx->lfbsel == 0 )
    {
      vbe_ReleaseCtx( &l_ctx );
      return NULL;
    }

    if( dpmi_SetSelRights(l_ctx->lfbsel, 0x8092) == 0 )
    {
      vbe_ReleaseCtx( &l_ctx );
      return NULL;
    }

    // Map Linear Frame Buffer
    l_size = l_mi.YResolution * l_mi.BytesPerScanline;
    l_ctx->lfb = (uint8_t*)dpmi_MapPhysicalAddress(
      l_mi.PhysBasePtr, l_size);
    if( l_ctx->lfb == NULL )
    {
      vbe_ReleaseCtx( &l_ctx );
      return NULL;
    }

    // Initialize LFB selector
    if( dpmi_SetSelBase(l_ctx->lfbsel, (uint32_t)l_ctx->lfb) == 0 )
    {
      vbe_ReleaseCtx( &l_ctx );
      return NULL;
    }

    if( dpmi_SetSelLimit(l_ctx->lfbsel, l_size) == 0 )
    {
      vbe_ReleaseCtx( &l_ctx );
      return NULL;
    }

    // Initialize context with required information
    l_ctx->lfbsize   = l_size;
    l_ctx->bpp       = l_mi.BitsPerPixel;

    l_ctx->clip.minx = 0;
    l_ctx->clip.miny = 0;
    l_ctx->clip.maxx = (l_mi.XResolution - 1);
    l_ctx->clip.maxy = (l_mi.YResolution - 1);

    l_ctx->width     = l_mi.XResolution;
    l_ctx->height    = l_mi.YResolution;
    l_ctx->linesize  = l_mi.BytesPerScanline;

    if( l_mi.MemoryModel == VBE_MM_DCOLOR )
    {
      l_ctx->rmask = ((1UL << l_mi.RedMaskSize) - 1)
        << l_mi.RedFieldPosition;
      l_ctx->gmask = ((1UL << l_mi.GreenMaskSize) - 1)
        << l_mi.GreenFieldPosition;
      l_ctx->bmask = ((1UL << l_mi.BlueMaskSize) - 1)
        << l_mi.BlueFieldPosition;
      l_ctx->amask = ((1UL << l_mi.RsvdMaskSize) - 1)
        << l_mi.RsvdFieldPosition;

      l_ctx->rshift = 8 - l_mi.RedMaskSize;
      l_ctx->gshift = 8 - l_mi.GreenMaskSize;
      l_ctx->bshift = 8 - l_mi.BlueMaskSize;
      l_ctx->ashift = 8 - l_mi.RsvdMaskSize;

      l_ctx->rpos = l_mi.RedFieldPosition;
      l_ctx->gpos = l_mi.GreenFieldPosition;
      l_ctx->bpos = l_mi.BlueFieldPosition;
      l_ctx->apos = l_mi.RsvdFieldPosition;
    }

    return l_ctx;
  }

  /*
   *  void vbe_ReleaseCtx( **p_ctx )
   *
   *  Purpose:
   *    Releases any/all system resources.
   *
   *  Returns:
   *    Nothing
   */
  void vbe_ReleaseCtx( vbectx_t** p_ctx )
  {
    vbectx_t* l_ctx;

    if( p_ctx )
    {
      l_ctx = *p_ctx;

      if( l_ctx )
      {

        if( l_ctx->lfbsel )
        {
          dpmi_FreePhysicalAddress( (uint32_t*)&(l_ctx->lfb) );

          dpmi_FreeSel( &(l_ctx->lfbsel) );
        }
        else
        {
          free( l_ctx->lfb );
        }

        free( *p_ctx );
      }
      *p_ctx = NULL;
    }
  }

  /*
   *  void vbe_CloseMode( **p_ctx )
   *
   *  Purpose:
   *    Releases system resources, and resets display mode.
   *
   *  Returns:
   *    Nothing
   */
  void vbe_CloseMode( vbectx_t** p_ctx )
  {
    rmiregs_t l_regs;

    if( p_ctx )
    {
      if( (*p_ctx)->lfbsel )
      {
        memset( &l_regs, 0, sizeof(l_regs) );
        l_regs.eax = 0x03;
        dpmi_SimRealModeInt( 0x10, &l_regs );
      }

      vbe_ReleaseCtx( p_ctx );
    }
  }

  /*
   *  vbectx_t* vbe_CreateBuf( *p_ctx, p_width, p_height )
   *
   *  Purpose:
   *    Allocates an off-screen buffer context.
   *
   *  Returns:
   *    NULL on error
   *    Valid Off-screen Context on success
   */
  vbectx_t* vbe_CreateBuf( vbectx_t* p_ctx,
    uint32_t p_width, uint32_t p_height )
  {
    vbectx_t* l_newctx;

    if( p_ctx )
    {
      l_newctx = (vbectx_t*)malloc(sizeof(vbectx_t));
      if( l_newctx )
      {
        memcpy( l_newctx, p_ctx, sizeof(vbectx_t) );

        if( p_width )
        {
          l_newctx->width = p_width;
        }

        if( p_height )
        {
          l_newctx->height = p_height;
        }

        l_newctx->lfbsel    = 0;

        l_newctx->clip.minx = 0;
        l_newctx->clip.miny = 0;
        l_newctx->clip.maxx = (l_newctx->width - 1);
        l_newctx->clip.maxy = (l_newctx->height - 1);

        l_newctx->lfbsize = l_newctx->height * l_newctx->linesize;
        l_newctx->lfb     = (uint8_t*)calloc(1, l_newctx->lfbsize);
        if( l_newctx->lfb )
        {
          return l_newctx;
        }

        free( l_newctx );
      }
    }

    return NULL;
  }

  /*
   *  void vbe_SetClip( *p_ctx, p_minx, p_miny, p_maxx, p_maxy )
   *
   *  Purpose:
   *    Sets requested drawing rectangle to within valid
   *    boundaries: from (0, 0) to (width - 1, height - 1).
   *
   *  Returns:
   *    Nothing
   */
  void vbe_SetClip( vbectx_t* p_ctx, int32_t p_minx,
    int32_t p_miny, int32_t p_maxx, int32_t p_maxy )
  {
    if( p_ctx )
    {
      // Validate minx
      if( p_minx < 0 )
      {
        p_minx = 0;
      }

      // Validate miny
      if( p_miny < 0 )
      {
        p_miny = 0;
      }

      // Validate maxx
      if( p_maxx >= p_ctx->width )
      {
        p_maxx = p_ctx->width - 1;
      }

      // Validate maxy
      if( p_maxy >= p_ctx->height )
      {
        p_maxy = p_ctx->height - 1;
      }

      // Update context clip boundaries
      p_ctx->clip.minx = p_minx;
      p_ctx->clip.miny = p_miny;
      p_ctx->clip.maxx = p_maxx;
      p_ctx->clip.maxy = p_maxy;
    }
  }

  /*
   *  void vbe_Capture( *p_display, *p_buf )
   *
   *  Purpose:
   *    Copies display to off-screen buffer.
   *
   *  Returns:
   *    Nothing
   */
  void vbe_Capture( vbectx_t* p_display, vbectx_t* p_buf )
  {
    if( p_display && p_buf )
    {
      if( p_display->lfb && p_buf->lfb
        && (p_display->lfbsize == p_buf->lfbsize) )
      {
        memcpy( p_buf->lfb, p_display->lfb, p_buf->lfbsize );
      }
    }
  }

  /*
   *  void vbe_Flip( *p_display, *p_buf )
   *
   *  Purpose:
   *    Copies off-screen buffer to display.
   *
   *  Returns:
   *    Nothing
   */
  void vbe_Flip( vbectx_t* p_display, vbectx_t* p_buf )
  {
    if( p_display && p_buf )
    {
      if( p_display->lfb && p_buf->lfb
        && (p_display->lfbsize == p_buf->lfbsize) )
      {
        memcpy( p_display->lfb, p_buf->lfb, p_display->lfbsize );
      }
    }
  }

  /*
   *  void vbe_DecodePixel( *p_ctx, p_pixel, *p_r, *p_g, *p_b )
   *
   *  Purpose:
   *    Converts 32-Bit pixel into R, G, B triplet.
   *    Intended for 15/16/24/32-BPP, 8-BPP needs a palette.
   *
   *  Returns:
   *    Nothing
   */
  void vbe_DecodePixel( vbectx_t* p_ctx, uint32_t p_pixel,
    uint8_t* p_r, uint8_t* p_g, uint8_t* p_b )
  {
    if( p_ctx && p_r && p_g && p_b )
    {
      *p_r = ((p_pixel & p_ctx->rmask) >> p_ctx->rpos) << p_ctx->rshift;
      *p_g = ((p_pixel & p_ctx->gmask) >> p_ctx->gpos) << p_ctx->gshift;
      *p_b = ((p_pixel & p_ctx->bmask) >> p_ctx->bpos) << p_ctx->bshift;
    }
  }

  /*
   *  uint32_t vbe_EncodePixel( *p_ctx, p_r, p_g, p_b )
   *
   *  Purpose:
   *    Encodes R, G, B triplet into generic 32-bit pixel format.
   *    Intended for 15/16/24/32-BPP. 8-BPP needs color matching.
   *
   *  Returns:
   *    Encoded pixel on success
   */
  uint32_t vbe_EncodePixel( vbectx_t* p_ctx,
    uint8_t p_r, uint8_t p_g, uint8_t p_b )
  {
    uint32_t l_r;
    uint32_t l_g;
    uint32_t l_b;

    if( p_ctx )
    {
      l_r = ((p_r >> p_ctx->rshift) << p_ctx->rpos) & p_ctx->rmask;
      l_g = ((p_g >> p_ctx->gshift) << p_ctx->gpos) & p_ctx->gmask;
      l_b = ((p_b >> p_ctx->bshift) << p_ctx->bpos) & p_ctx->bmask;

      return (l_r | l_g | l_b);
    }

    return 0;
  }

  /*
   *  void vbe_GetPal( *p_ctx, *p_pal );
   *
   *  Purpose:
   *    Copies hardware palette into an array of R,G,B,A values.
   *
   *  Returns:
   *    Nothing
   */
  void vbe_GetPalASM( rgba_t* p_pal );
  #pragma aux vbe_GetPalASM=\
  "  test    edi, edi"\
  "  jz      GetPal_Error"\
  ""\
  "  mov     dx, 0x3C7"\
  "  xor     al, al"\
  "  mov     ecx, 256"\
  "  out     dx, al"\
  ""\
  "  mov     dx, 0x3C9"\
  ""\
  "GetPal_Loop:"\
  "  in      al, dx"\
  "  shl     al, 2"\
  "  mov     [edi], al"\
  "  inc     edi"\
  ""\
  "  in      al, dx"\
  "  shl     al, 2"\
  "  mov     [edi], al"\
  "  inc     edi"\
  ""\
  "  in      al, dx"\
  "  shl     al, 2"\
  "  mov     [edi], al"\
  "  add     edi, 2"\
  ""\
  "  dec     ecx"\
  "  jnz     GetPal_Loop"\
  ""\
  "GetPal_Error:"\
  parm[edi]\
  modify[eax ecx edx edi];

  void vbe_GetPal( vbectx_t* p_ctx, rgba_t* p_pal )
  {
    if( p_ctx )
    {
      if( p_ctx->bpp == 8 )
      {
        vbe_GetPalASM( p_pal );
      }
    }
  }

  /*
   *  void vbe_SetPal( *p_ctx, *p_pal );
   *
   *  Purpose:
   *    Sets internal palette from an array of R,G,B,A values.
   *
   *  Returns:
   *    Nothing
   */
  void vbe_SetPalASM( rgba_t* p_pal );
  #pragma aux vbe_SetPalASM=\
  "  test    esi, esi"\
  "  jz      SetPal_Error"\
  ""\
  "  mov     dx, 0x3C8"\
  "  xor     al, al"\
  "  mov     ecx, 256"\
  "  out     dx, al"\
  ""\
  "  mov     dx, 0x3C9"\
  ""\
  "SetPal_Loop:"\
  "  mov     al, [esi]"\
  "  inc     esi"\
  "  shr     al, 2"\
  "  out     dx, al"\
  ""\
  "  mov     al, [esi]"\
  "  inc     esi"\
  "  shr     al, 2"\
  "  out     dx, al"\
  ""\
  "  mov     al, [esi]"\
  "  add     esi, 2"\
  "  shr     al, 2"\
  "  out     dx, al"\
  ""\
  "  dec     ecx"\
  "  jnz     SetPal_Loop"\
  ""\
  "SetPal_Error:"\
  parm[esi]\
  modify[eax ecx edx esi];

  void vbe_SetPal( vbectx_t* p_ctx, rgba_t* p_pal )
  {
    if( p_ctx )
    {
      if( p_ctx->bpp == 8 )
      {
        vbe_SetPalASM( p_pal );
      }
    }
  }
