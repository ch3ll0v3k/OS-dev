#include "kernel.h"

// ====================================================================
#define VBE_DISPI_BANK_ADDRESS          0xA0000
#define VBE_DISPI_LFB_PHYSICAL_ADDRESS  0xE0000000
#define VBE_DISPI_BANK_SIZE_KB          64

#define VBE_DISPI_ID0                   0xB0C0 // setting X and Y resolution and bit depth (8 BPP only), banked mode
#define VBE_DISPI_ID1                   0xB0C1 // virtual width and height, X and Y offset
#define VBE_DISPI_ID2                   0xB0C2 // 15, 16, 24 and 32 BPP modes, support for linear frame buffer, support for retaining memory contents on mode switching
#define VBE_DISPI_ID3                   0xB0C3 // support for getting capabilities, support for using 8 bit DAC
#define VBE_DISPI_ID4                   0xB0C4 // VRAM increased to 8 MB
#define VBE_DISPI_ID5                   0xB0C5 // VRAM increased to 16 MB? [TODO: verify and check for other changes] 

#define VBE_DISPI_IOPORT_INDEX          0x01CE
#define VBE_DISPI_IOPORT_DATA           0x01CF

#define VBE_DISPI_INDEX_ID              0x0

#define VBE_DISPI_INDEX_XRES            0x1
#define VBE_DISPI_INDEX_YRES            0x2

#define VBE_DISPI_MAX_XRES              1024
#define VBE_DISPI_MAX_YRES              768

#define VBE_DISPI_INDEX_BPP             0x3
#define VBE_DISPI_INDEX_ENABLE          0x4
#define VBE_DISPI_INDEX_BANK            0x5
#define VBE_DISPI_INDEX_VIRT_WIDTH      0x6
#define VBE_DISPI_INDEX_VIRT_HEIGHT     0x7
#define VBE_DISPI_INDEX_X_OFFSET        0x8
#define VBE_DISPI_INDEX_Y_OFFSET        0x9

#define VBE_DISPI_DISABLED              0x00
#define VBE_DISPI_ENABLED               0x01
#define VBE_DISPI_GETCAPS               0x02
#define VBE_DISPI_8BIT_DAC              0x20
#define VBE_DISPI_LFB_ENABLED           0x40
#define VBE_DISPI_NOCLEARMEM            0x80

// ====================================================================
void _vid_reg(uint16_t i_val, uint16_t i_data);
void set_vid_mod(uint32_t width, uint32_t height, uint32_t bpp, uint8_t use_lineare );

uint16_t bga_reg(uint16_t index_val);
int is_bga_available();
void bga_set_bank(uint16_t bank_number);

// ====================================================================
/*
uint16_t vbe_mode_best_match(int x, int y, int depth) {

    struct VbeInfoBlock *vbeInfo = (VbeInfoBlock *)0x2000;
    struct ModeInfoBlock *modeInfo = (ModeInfoBlock *)0x3000;
    uint16_t *modes;
    int i;
    uint16_t best_match = 0x13;
    int pixdiff, bestpixdiff = DIFF(320 * 200, x * y);
    int depthdiff, bestdepthdiff = 8 >= depth ? 8 - depth : (depth - 8) * 2;

    strncpy(vbeInfo->VbeSignature, "VBE2", 4);
    intV86(0x10, "ax,es:di", 0x4F00, 0, vbeInfo);       // Query VBE information block.

    if ((uint16_t)v86.tss.eax != 0x004F) return best_match;

    modes = (uint16_t *)REALPTR(vbeInfo->VideoModePtr); // get the modes array, returned by the call above.
    for (i = 0 ; modes[i] != 0xFFFF ; i++) {
        intV86(0x10, "ax,cx,es:di", 0x4F01, modes[i], 0, modeInfo); // Acquire current mode Information.

        if ((uint16_t)v86.tss.eax != 0x004F ) continue;

        // Does the current mode supports linear frame buffer, if not skip it.
        if ((modeInfo->attributes & 0x90) != 0x90) continue;

        // Not a packed pixel nor a direct color mode...
        if (modeInfo->memory_model != 4 && modeInfo->memory_model != 6) continue;

        // Check if this is the exact mode we were looking for, and return directly if it's exact match.
        if (x == modeInfo->XResolution && y == modeInfo->YResolution && depth == modeInfo->BitsPerPixel)
            return modes[i];

        // Otherwise, compare to the closest match so far, set it as best if needed.
        pixdiff = DIFF(modeInfo->Xres * modeInfo->Yres, x * y);
        depthdiff = (modeInfo->bpp >= depth)? modeInfo->bpp - depth : (depth - modeInfo->bpp) * 2;

        if (bestpixdiff > pixdiff || (bestpixdiff == pixdiff && bestdepthdiff > depthdiff)) {
            best_match = modes[i];
            bestpixdiff = pixdiff;
            bestdepthdiff = depthdiff;
        }
    }
    if ( x == 640 && y == 480 && depth == 1 ) return 0x11;
    return best_match;
}
*/
// ====================================================================
/*
VBE Display API
-------------------------------------------------------------------------------------------------------------
  This document is part of the Bochs/VBEBios documentation,
  it specifies the bochs host <-> vbebios client communication.

  That means, the display code implementation and the vbebios code depend
  very heavily on each other. As such, this documents needs be synchronised
  between bochs CVS and the vgabios CVS.

  This document does not describe how the VBEBios implements the VBE2/3 spec.
  This document does not describe how the Bochs display code will display gfx based upon this spec.


API History
-----------
0xb0c0            supports the following VBE_DISPI_ interfaces (present in Bochs 1.4):
                  VBE_DISPI_INDEX_ID
                  VBE_DISPI_INDEX_XRES
                  VBE_DISPI_INDEX_YRES
                  VBE_DISPI_INDEX_BPP
                  VBE_DISPI_INDEX_ENABLE
                  VBE_DISPI_INDEX_BANK

                  Bpp format supported is:
                  VBE_DISPI_BPP_8

0xb0c1            supports 0xb0c0 VBE_DISPI_ interfaces, additional interfaces (present in Bochs 2.0):
                  VBE_DISPI_INDEX_VIRT_WIDTH
                  VBE_DISPI_INDEX_VIRT_HEIGHT
                  VBE_DISPI_INDEX_X_OFFSET
                  VBE_DISPI_INDEX_Y_OFFSET

0xb0c2            supports 0xb0c1 VBE_DISPI_ interfaces, interfaces updated for
                  additional features (present in Bochs 2.1):
                  VBE_DISPI_INDEX_BPP supports >8bpp color depth (value = bits)
                  VBE_DISPI_INDEX_ENABLE supports new flags VBE_DISPI_NOCLEARMEM and VBE_DISPI_LFB_ENABLED
                  VBE i/o registers changed from 0xFF80/81 to 0x01CE/CF

0xb0c3            supports 0xb0c2 VBE_DISPI_ interfaces, interfaces updated for
                  additional features:
                  VBE_DISPI_INDEX_ENABLE supports new flags VBE_DISPI_GETCAPS and VBE_DISPI_8BIT_DAC

0xb0c4            VBE video memory increased to 8 MB


History
-------
  Version 0.6     2002 Nov 23  Jeroen Janssen
                  - Added LFB support
                  - Added Virt width, height and x,y offset

  Version 0.5     2002 March 08   Jeroen Janssen
                  - Added documentation about panic behaviour / current limits of the data values.
                  - Changed BPP API (in order to include future (A)RGB formats)
                  - Initial version (based upon extended display text of the vbe bochs display patch)


Todo
----
  Version 0.6+    [random order]
                  - Add lots of different (A)RGB formats

References
----------
  [VBE3]          VBE 3 Specification at
                  http://www.vesa.org/vbe3.pdf

  [BOCHS]         Bochs Open Source IA-32 Emulator at
                  http://bochs.sourceforge.net

  [VBEBIOS]       VBE Bios for Bochs at
                  http://savannah.gnu.org/projects/vgabios/

  [Screenshots]   Screenshots of programs using the VBE Bios at
                  http://japj.org/projects/bochs_plex86/screenshots.html

Abbreviations
-------------
  VBE             Vesa Bios Extension
  DISPI           (Bochs) Display Interface
  BPP             Bits Per Pixel
  LFB             Linear Frame Buffer


#defines
--------
vbetables-gen.c
  #define VBE_DISPI_TOTAL_VIDEO_MEMORY_MB 8

vbe.h
  #define VBE_DISPI_BANK_ADDRESS          0xA0000
  #define VBE_DISPI_BANK_SIZE_KB          64

  #define VBE_DISPI_MAX_XRES              1024
  #define VBE_DISPI_MAX_YRES              768

  #define VBE_DISPI_IOPORT_INDEX          0x01CE
  #define VBE_DISPI_IOPORT_DATA           0x01CF

  #define VBE_DISPI_INDEX_ID              0x0
  #define VBE_DISPI_INDEX_XRES            0x1
  #define VBE_DISPI_INDEX_YRES            0x2
  #define VBE_DISPI_INDEX_BPP             0x3
  #define VBE_DISPI_INDEX_ENABLE          0x4
  #define VBE_DISPI_INDEX_BANK            0x5
  #define VBE_DISPI_INDEX_VIRT_WIDTH      0x6
  #define VBE_DISPI_INDEX_VIRT_HEIGHT     0x7
  #define VBE_DISPI_INDEX_X_OFFSET        0x8
  #define VBE_DISPI_INDEX_Y_OFFSET        0x9

  #define VBE_DISPI_ID0                   0xB0C0
  #define VBE_DISPI_ID1                   0xB0C1
  #define VBE_DISPI_ID2                   0xB0C2
  #define VBE_DISPI_ID3                   0xB0C3
  #define VBE_DISPI_ID4                   0xB0C4

  #define VBE_DISPI_DISABLED              0x00
  #define VBE_DISPI_ENABLED               0x01
  #define VBE_DISPI_VBE_ENABLED           0x40
  #define VBE_DISPI_NOCLEARMEM            0x80

  #define VBE_DISPI_LFB_PHYSICAL_ADDRESS  0xE0000000

API
---
  The display api works by using a index (VBE_DISPI_IOPORT_INDEX) and
  data (VBE_DISPI_IOPORT_DATA) ioport. One writes the index of the parameter to the index port.
  Next, the parameter value can be read or written.

[0xb0c0]
  * VBE_DISPI_INDEX_ID  : WORD {R,W}
    This parameter can be used to detect the current display API (both bochs & vbebios).
    The bios writes VBE_DISPI_ID0 to the dataport and reads it back again.
    This way, the display code knows the vbebios 'ID' and the vbebios can check if the correct
    display code is present.
    As a result, a PANIC can be generated if an incompatible vbebios/display code combination is detected.
    This panic can be generated from the bochs display code (NOT the bios, see Notes).

    Example values: VBE_DISPI_ID0

  * VBE_DISPI_INDEX_XRES : WORD {R,W}
    This parameter can be used to read/write the vbe display X resolution (in pixels).
    It's illegal to set the XRES when the VBE is enabled (display code should generate PANIC).

    If the value written exceeds VBE_DISPI_MAX_XRES, the display code needs to generate a PANIC.

    Example values:   320,640,800,1024

  * VBE_DISPI_INDEX_YRES : WORD {R,W}
    This parameter can be used to read/write the vbe display Y resolution (in pixels).
    It's illegal to set the YRES when the VBE is enabled (display code should generate PANIC).

    If the value written exceeds VBE_DISPI_MAX_YRES, the display code needs to generate a PANIC.

    Example values:   200,400,480,600,768

  * VBE_DISPI_INDEX_BPP : WORD {R,W}
    This parameter can be used to read/write the vbe display BPP.
    It's illegal to set the BPP when the VBE is enabled (display code should generate PANIC).

    If the value written is an incompatible BPP, the display code needs to generate a PANIC.

    Example values:   VBE_DISPI_BPP_8

  * VBE_DISPI_INDEX_ENABLE : WORD {R,W}
    This parameter can be used to read/write the vbe ENABLED state.
    If the bios writes VBE_DISPI_ENABLED then the display code will setup a hostside display mode
    with the current XRES, YRES and BPP settings.
    If the bios write VBE_DISPI_DISABLED then the display code will switch back to normal vga mode behaviour.

    Example values: VBE_DISPI_ENABLED, VBE_DISPI_DISABLED

  * VBE_DISPI_INDEX_BANK : WORD {R,W}
    This parameter can be used to read/write the current selected BANK (at 0xA0000).
    This can be used for switching banks in banked mode.

[0xb0c1]
  * VBE_DISPI_INDEX_VIRT_WIDTH : WORD {R,W}
    This parameter can be used to read/write the current virtual width.
    Upon enabling a mode, this will be set to the current xres
    Setting this field during enabled mode will result in the virtual width to be changed.
    Value will be adjusted if current setting is not possible.

  * VBE_DISPI_INDEX_VIRT_HEIGHT : WORD {R}
    This parameter can be read in order to obtain the current virtual height.
    This setting will be adjusted after setting a virtual width in order to stay within limit of video memory.

  * VBE_DISPI_INDEX_X_OFFSET : WORD {R,W}
    The current X offset (in pixels!) of the visible screen part.
    Writing a new offset will also result in a complete screen refresh.

  * VBE_DISPI_INDEX_Y_OFFSET : WORD {R,W}
    The current Y offset (in pixels!) of the visible screen part.
    Writing a new offset will also result in a complete screen refresh.


[0xb0c2]
  * VBE_DISPI_INDEX_BPP : WORD {R,W}
    The value written is now the number of bits per pixel. A value of 0 is treated
    the same as 8 for backward compatibilty. These values are supported: 8, 15,
    16, 24 and 32. The value of 4 is not yet handled in the VBE code.
  * VBE_DISPI_INDEX_ENABLE : WORD {R,W}
    The new flag VBE_DISPI_NOCLEARMEM allows to preserve the VBE video memory.
    The new flag VBE_DISPI_LFB_ENABLED indicates the usage of the LFB.

[0xb0c3]
  * VBE_DISPI_INDEX_ENABLE : WORD {R,W}
    If the new flag VBE_DISPI_GETCAPS is enabled, the xres, yres and bpp registers
    return the gui capabilities.
    The new flag VBE_DISPI_8BIT_DAC switches the DAC to 8 bit mode.

[0xb0c4]
  * VBE_DISPI_TOTAL_VIDEO_MEMORY_MB set to 8 (moved to auto-generated vbetables.h)

Displaying GFX (banked mode)
--------------
  What happens is that the total screen is devided in banks of 'VBE_DISPI_BANK_SIZE_KB' KiloByte in size.
  If you want to set a pixel you can calculate its bank by doing:

    offset = pixel_x + pixel_y * resolution_x;
    bank = offset / 64 Kb (rounded 1.9999 -> 1)

    bank_pixel_pos = offset - bank * 64Kb

  Now you can set the current bank and put the pixel at VBE_DISPI_BANK_ADDRESS + bank_pixel_pos

Displaying GFX (linear frame buffer mode)
--------------
  NOT WRITTEN YET

Notes
-----
  * Since the XRES/YRES/BPP may not be written when VBE is enabled, if you want to switch from one VBE mode
    to another, you will need to disable VBE first.

  * Note when the bios doesn't find a valid DISPI_ID, it can disable the VBE functions. This allows people to
    use the same bios for both vbe enabled and disabled bochs executables.

*/



