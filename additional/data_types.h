#include <stdint.h>

typedef uint8_t  BYTE;
typedef uint32_t DWORD;
typedef int32_t  LONG;
typedef uint16_t WORD;

//header of GIF file
struct gifHeader
{
    //header
    BYTE signature[3];  //header signature (always "GIF")
    BYTE version[3];    //gif version ie. 87a or 89a
    //logical screen descriptor
    WORD screenWidth;   
    WORD screenHeight;  //display parameters in pixels
    BYTE packed;    //screen & color map info
    BYTE backgroundColor;   //backgnd color index
    BYTE aspectRatio;   //pixel aspect ratio = (aspectRatio + 15)/64  if this is zero then no aspectRatio is specified
}__attribute__((__packed__));

struct graphicsControlExtensionBlock
{
    BYTE introducer;    //always 0x21
    BYTE label;     //always 0xf9
    BYTE blockSize;     //always 0x04
    BYTE packed;
    WORD delayTime;     //in 1/100 secs
    BYTE colorIndex;    //if transparent color index is 1 else 0
    BYTE terminator;    //always 0x00
}__attribute__((__packed__));

struct RGBTRIPLE
{
    BYTE red;
    BYTE green;
    BYTE blue;
} __attribute__((__packed__));