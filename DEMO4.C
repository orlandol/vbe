#include <stdio.h>
#include <string.h>

#include "vbe.h"
#include "gfx.h"

int main( int argc, char* argv[] )
{
  VBEModeInfo_t l_minfo;

  if( vbe_Init() == 0 )
  {
    printf( "ERROR: VBE driver not supported.\n" );

    return 0;
  }

  vbe_Done( NULL );

  return 0;
}
