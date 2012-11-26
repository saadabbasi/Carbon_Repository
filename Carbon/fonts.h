#include <avr/pgmspace.h>

typedef struct
{
	const unsigned char widthBits;
	const unsigned int	offset;
} FONT_CHAR_INFO;

extern const unsigned char proFontWindows20ptBitmaps[] PROGMEM;
extern const unsigned char proFontWindows40ptBitmaps[] PROGMEM;
extern const FONT_CHAR_INFO proFontWindows20ptDescriptors[];
extern const FONT_CHAR_INFO proFontWindows40ptDescriptors[];
extern const char proFontWindows40ptFontInfo[];
