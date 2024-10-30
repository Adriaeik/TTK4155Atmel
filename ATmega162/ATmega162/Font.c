/*
 * Font.c
 *
 * Created: 18.09.2024 00:51:09
 *  Author: eikel
 */ 

#include "Font.h"
// Lagrar fontdata i PROGMEM
const uint8_t font8x8_basic[] PROGMEM = {
	// ASCII 0x20 (Space)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	
	// ASCII 0x21 (!)
	0x00, 0x00, 0x5F, 0x00, 0x00, 0x00, 0x00, 0x00,
	
	// ASCII 0x22 (")
	0x00, 0x07, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00,
	
	// ASCII 0x23 (#)
	0x14, 0x7F, 0x14, 0x7F, 0x14, 0x00, 0x00, 0x00,
	
	// ASCII 0x24 ($)
	0x24, 0x2A, 0x7F, 0x2A, 0x12, 0x00, 0x00, 0x00,
	
	// ASCII 0x25 (%)
	0x23, 0x13, 0x08, 0x64, 0x62, 0x00, 0x00, 0x00,
	
	// ASCII 0x26 (&)
	0x36, 0x49, 0x56, 0x20, 0x50, 0x00, 0x00, 0x00,
	
	// ASCII 0x27 (')
	0x00, 0x08, 0x07, 0x03, 0x00, 0x00, 0x00, 0x00,
	
	// ASCII 0x28 (()
	0x00, 0x1C, 0x22, 0x41, 0x00, 0x00, 0x00, 0x00,
	
	// ASCII 0x29 ())
	0x00, 0x41, 0x22, 0x1C, 0x00, 0x00, 0x00, 0x00,
	
	// ASCII 0x2A (*)
	0x14, 0x08, 0x3E, 0x08, 0x14, 0x00, 0x00, 0x00,
	
	// ASCII 0x2B (+)
	0x08, 0x08, 0x3E, 0x08, 0x08, 0x00, 0x00, 0x00,
	
	// ASCII 0x2C (,)
	0x00, 0x50, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00,
	
	// ASCII 0x2D (-)
	0x08, 0x08, 0x08, 0x08, 0x08, 0x00, 0x00, 0x00,
	
	// ASCII 0x2E (.)
	0x00, 0x60, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00,
	
	// ASCII 0x2F (/)
	0x20, 0x10, 0x08, 0x04, 0x02, 0x00, 0x00, 0x00,
	
	// ASCII 0x30 (0)
	0x3E, 0x51, 0x49, 0x45, 0x3E, 0x00, 0x00, 0x00,
	
	// ASCII 0x31 (1)
	0x00, 0x42, 0x7F, 0x40, 0x00, 0x00, 0x00, 0x00,
	
	// ASCII 0x32 (2)
	0x42, 0x61, 0x51, 0x49, 0x46, 0x00, 0x00, 0x00,
	
	// ASCII 0x33 (3)
	0x21, 0x41, 0x45, 0x4B, 0x31, 0x00, 0x00, 0x00,
	
	// ASCII 0x34 (4)
	0x18, 0x14, 0x12, 0x7F, 0x10, 0x00, 0x00, 0x00,
	
	// ASCII 0x35 (5)
	0x27, 0x45, 0x45, 0x45, 0x39, 0x00, 0x00, 0x00,
	
	// ASCII 0x36 (6)
	0x3C, 0x4A, 0x49, 0x49, 0x30, 0x00, 0x00, 0x00,
	
	// ASCII 0x37 (7)
	0x01, 0x71, 0x09, 0x05, 0x03, 0x00, 0x00, 0x00,
	
	// ASCII 0x38 (8)
	0x36, 0x49, 0x49, 0x49, 0x36, 0x00, 0x00, 0x00,
	
	// ASCII 0x39 (9)
	0x06, 0x49, 0x49, 0x29, 0x1E, 0x00, 0x00, 0x00,
	
	// ASCII 0x3A (:)
	0x00, 0x36, 0x36, 0x00, 0x00, 0x00, 0x00, 0x00,
	
	// ASCII 0x3B (;)
	0x00, 0x56, 0x36, 0x00, 0x00, 0x00, 0x00, 0x00,
	
	// ASCII 0x3C ('<')
	0x08, 0x14, 0x22, 0x41, 0x00, 0x00, 0x00, 0x00,

	// ASCII 0x3D ('=')
	0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x00, 0x00,

	// ASCII 0x3E ('>')
	0x41, 0x22, 0x14, 0x08, 0x00, 0x00, 0x00, 0x00,

	// ASCII 0x3F ('?')
	0x02, 0x01, 0x51, 0x09, 0x06, 0x00, 0x00, 0x00,

	// ASCII 0x40 ('@')
	0x3E, 0x41, 0x5D, 0x5D, 0x1E, 0x00, 0x00, 0x00,
	
	// ASCII 0x41 (A)
	0x7E, 0x11, 0x11, 0x11, 0x7E, 0x00, 0x00, 0x00,
	
	// ASCII 0x42 (B)
	0x7F, 0x49, 0x49, 0x49, 0x36, 0x00, 0x00, 0x00,
	
	// ASCII 0x43 (C)
	0x3E, 0x41, 0x41, 0x41, 0x22, 0x00, 0x00, 0x00,
	
	// ASCII 0x44 (D)
	0x7F, 0x41, 0x41, 0x22, 0x1C, 0x00, 0x00, 0x00,
	
	// ASCII 0x45 (E)
	0x7F, 0x49, 0x49, 0x49, 0x41, 0x00, 0x00, 0x00,
	
	// ASCII 0x46 (F)
	0x7F, 0x09, 0x09, 0x09, 0x01, 0x00, 0x00, 0x00,
	
	// ASCII 0x47 (G)
	0x3E, 0x41, 0x49, 0x49, 0x7A, 0x00, 0x00, 0x00,
	
	// ASCII 0x48 (H)
	0x7F, 0x08, 0x08, 0x08, 0x7F, 0x00, 0x00, 0x00,
	
	// ASCII 0x49 (I)
	0x00, 0x41, 0x7F, 0x41, 0x00, 0x00, 0x00, 0x00,
	
	// ASCII 0x4A (J)
	0x20, 0x40, 0x41, 0x3F, 0x01, 0x00, 0x00, 0x00,
	
	// ASCII 0x4B (K)
	0x7F, 0x08, 0x14, 0x22, 0x41, 0x00, 0x00, 0x00,
	
	// ASCII 0x4C (L)
	0x7F, 0x40, 0x40, 0x40, 0x40, 0x00, 0x00, 0x00,
	
	// ASCII 0x4D (M)
	0x7F, 0x02, 0x04, 0x02, 0x7F, 0x00, 0x00, 0x00,
	
	// ASCII 0x4E (N)
	0x7F, 0x02, 0x04, 0x08, 0x7F, 0x00, 0x00, 0x00,
	
	// ASCII 0x4F (O)
	0x3E, 0x41, 0x41, 0x41, 0x3E, 0x00, 0x00, 0x00,
	
	// ASCII 0x50 (P)
	0x7F, 0x09, 0x09, 0x09, 0x06, 0x00, 0x00, 0x00,
	
	// ASCII 0x51 (Q)
	0x3E, 0x41, 0x51, 0x21, 0x5E, 0x00, 0x00, 0x00,
	
	// ASCII 0x52 (R)
	0x7F, 0x09, 0x19, 0x29, 0x46, 0x00, 0x00, 0x00,
	
	// ASCII 0x53 (S)
	0x46, 0x49, 0x49, 0x49, 0x31, 0x00, 0x00, 0x00,
	
	// ASCII 0x54 (T)
	0x01, 0x01, 0x7F, 0x01, 0x01, 0x00, 0x00, 0x00,
	
	// ASCII 0x55 (U)
	0x3F, 0x40, 0x40, 0x40, 0x3F, 0x00, 0x00, 0x00,
	
	// ASCII 0x56 (V)
	0x1F, 0x20, 0x40, 0x20, 0x1F, 0x00, 0x00, 0x00,
	
	// ASCII 0x57 (W)
	0x7F, 0x20, 0x18, 0x20, 0x7F, 0x00, 0x00, 0x00,
	
	// ASCII 0x58 (X)
	0x63, 0x14, 0x08, 0x14, 0x63, 0x00, 0x00, 0x00,
	
	// ASCII 0x59 (Y)
	0x03, 0x04, 0x78, 0x04, 0x03, 0x00, 0x00, 0x00,
	
	// ASCII 0x5A (Z)
	0x61, 0x51, 0x49, 0x45, 0x43, 0x00, 0x00, 0x00,
	
	// ASCII 0x5B ('[')
	0x00, 0x3E, 0x41, 0x41, 0x00, 0x00, 0x00, 0x00,

	// ASCII 0x5C ('\')
	0x02, 0x04, 0x08, 0x10, 0x20, 0x00, 0x00, 0x00,

	// ASCII 0x5D (']')
	0x00, 0x41, 0x41, 0x3E, 0x00, 0x00, 0x00, 0x00,

	// ASCII 0x5E ('^')
	0x04, 0x02, 0x01, 0x02, 0x04, 0x00, 0x00, 0x00,

	// ASCII 0x5F ('_')
	0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x00, 0x00,

	// ASCII 0x60 ('`')
	0x00, 0x00, 0x03, 0x07, 0x00, 0x00, 0x00, 0x00,

	// ASCII 0x61 (a)
	0x20, 0x54, 0x54, 0x54, 0x78, 0x00, 0x00, 0x00,
	
	// ASCII 0x62 (b)
	0x7F, 0x48, 0x44, 0x44, 0x38, 0x00, 0x00, 0x00,
	
	// ASCII 0x63 (c)
	0x38, 0x44, 0x44, 0x44, 0x20, 0x00, 0x00, 0x00,
	
	// ASCII 0x64 (d)
	0x38, 0x44, 0x44, 0x48, 0x7F, 0x00, 0x00, 0x00,
	
	// ASCII 0x65 (e)
	0x38, 0x54, 0x54, 0x54, 0x18, 0x00, 0x00, 0x00,
	
	// ASCII 0x66 (f)
	0x08, 0x7E, 0x09, 0x01, 0x02, 0x00, 0x00, 0x00,
	
	// ASCII 0x67 (g)
	0x08, 0x14, 0x54, 0x54, 0x3C, 0x00, 0x00, 0x00,
	
	// ASCII 0x68 (h)
	0x7F, 0x08, 0x04, 0x04, 0x78, 0x00, 0x00, 0x00,
	
	// ASCII 0x69 (i)
	0x00, 0x44, 0x7D, 0x40, 0x00, 0x00, 0x00, 0x00,
	
	// ASCII 0x6A (j)
	0x20, 0x40, 0x44, 0x3D, 0x00, 0x00, 0x00, 0x00,
	
	// ASCII 0x6B (k)
	0x00, 0x7F, 0x10, 0x28, 0x44, 0x00, 0x00, 0x00,
	
	// ASCII 0x6C (l)
	0x00, 0x41, 0x7F, 0x40, 0x00, 0x00, 0x00, 0x00,
	
	// ASCII 0x6D (m)
	0x7C, 0x04, 0x18, 0x04, 0x78, 0x00, 0x00, 0x00,
	
	// ASCII 0x6E (n)
	0x7C, 0x08, 0x04, 0x04, 0x78, 0x00, 0x00, 0x00,
	
	// ASCII 0x6F (o)
	0x38, 0x44, 0x44, 0x44, 0x38, 0x00, 0x00, 0x00,
	
	// ASCII 0x70 (p)
	0x7C, 0x14, 0x14, 0x14, 0x08, 0x00, 0x00, 0x00,
	
	// ASCII 0x71 (q)
	0x08, 0x14, 0x14, 0x18, 0x7C, 0x00, 0x00, 0x00,
	
	// ASCII 0x72 (r)
	0x7C, 0x08, 0x04, 0x04, 0x08, 0x00, 0x00, 0x00,
	
	// ASCII 0x73 (s)
	0x48, 0x54, 0x54, 0x54, 0x20, 0x00, 0x00, 0x00,
	
	// ASCII 0x74 (t)
	0x04, 0x3F, 0x44, 0x40, 0x20, 0x00, 0x00, 0x00,
	
	// ASCII 0x75 (u)
	0x3C, 0x40, 0x40, 0x20, 0x7C, 0x00, 0x00, 0x00,
	
	// ASCII 0x76 (v)
	0x1C, 0x20, 0x40, 0x20, 0x1C, 0x00, 0x00, 0x00,
	
	// ASCII 0x77 (w)
	0x3C, 0x40, 0x30, 0x40, 0x3C, 0x00, 0x00, 0x00,
	
	// ASCII 0x78 (x)
	0x44, 0x28, 0x10, 0x28, 0x44, 0x00, 0x00, 0x00,
	
	// ASCII 0x79 (y)
	0x0C, 0x50, 0x50, 0x50, 0x3C, 0x00, 0x00, 0x00,
	
	// ASCII 0x7A (z)
	0x44, 0x64, 0x54, 0x4C, 0x44, 0x00, 0x00, 0x00,
	
	// ASCII 0x7B ({)
	0x00, 0x08, 0x36, 0x41, 0x00, 0x00, 0x00, 0x00,
	
	// ASCII 0x7C (|)
	0x00, 0x00, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x00,
	
	// ASCII 0x7D (})
	0x00, 0x41, 0x36, 0x08, 0x00, 0x00, 0x00, 0x00,
	
	// ASCII 0x7E (~)
	0x08, 0x04, 0x08, 0x10, 0x08, 0x00, 0x00, 0x00,
	
	/*Egendefinerte*/
	
};
/*___________________ HARDKODA SKJERMBILDER MED CHAR ___________________*/

const char solkors[128] PROGMEM = 
"   yap@@@*      "
"   #@@@#   *#   "
"  #@@@*  *@@@@# "
"*  +#@@@@@@  @@@"
"@@   +@@@@*   @@"
"@@@%%@@@@@@ +  *"
" #@@@@*  *@@@#  "
"   ##   #@@@#   ";

const char smiley[128] PROGMEM =
"  _____\  /_____"  // linje 1 (tom linje)
"  @@@@@   @@@@@ "  // linje 2 (�yne)
" @     @ @     @"  // linje 3 (kontur av hode)
" @  /\\  @  /\\  @"  // linje 4 (venstre og h�yre �ye) m� bruke \\ for � f� \ (trur eg)
" @  \\/  @  \\/  @"  // linje 5 (tom hodekontur)
"  @    @ @    @ "  // linje 6 (tom linje)
"   @@@@   @@@@  "  // linje 7 (bunnen av hodet)
" \\_____________/"; // linje 8 (smil)






/*___________________ HARDKODA SKJERMBILDER MED BYTES ___________________*/
const uint8_t emoji_sunglasses_1024[1024] PROGMEM = {
	// F�rste del av hovudet (�verste radene)
	0b00000000, 0b00000000, 0b00011110, 0b00011110, 0b00000000, 0b00000000, 0b00011110, 0b00011110,
	0b00000000, 0b00011110, 0b00011110, 0b00000000, 0b00000000, 0b00000000, 0b00011110, 0b00011110,
	0b00000000, 0b00011110, 0b00011110, 0b00000000, 0b00000000, 0b00000000, 0b00011110, 0b00011110,
	0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00011110, 0b00011110,
	
	// Solbriller (midtpartiet)
	0b00111111, 0b00111111, 0b00111111, 0b00111111, 0b00111111, 0b00111111, 0b11111111, 0b11111111,
	0b00111111, 0b00111111, 0b00111111, 0b00111111, 0b00111111, 0b00111111, 0b11111111, 0b11111111,
	0b00011111, 0b00011111, 0b00011111, 0b00011111, 0b00011111, 0b00011111, 0b11111111, 0b11111111,
	0b00011111, 0b00011111, 0b00011111, 0b00011111, 0b00011111, 0b00011111, 0b11111111, 0b11111111,

	// Smil og hovudet sin nedre del
	0b00000000, 0b00011110, 0b00011110, 0b00000000, 0b00000000, 0b00000000, 0b00011110, 0b00011110,
	0b00000000, 0b00011110, 0b00011110, 0b00000000, 0b00000000, 0b00000000, 0b00011110, 0b00011110,
	0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00011110, 0b00011110,
	0b00000000, 0b00011110, 0b00011110, 0b00000000, 0b00000000, 0b00000000, 0b00011110, 0b00011110,

	// Fyll resten med nuller for � dekke heile skjermen
	[160 ... 1023] = 0x00
};






/*___________________ MENYA ___________________*/
// Definer menyane og lagre dei i flash med PROGMEM
const char mainMenuItems[8*16] PROGMEM = 
" Start Game     "
" Settings       "
" High Scores    "
" Credits        "
" Exit           ";

const char scrollMenuItems[20*16] PROGMEM =
" BACK           " " Nr. 1:         " " Nr. 2:         " " Nr. 3:         " " Nr. 4:         "
" Nr. 5:         " " Nr. 6:         " " Nr. 7:         " " Nr. 8:         " " Nr. 9:         "
" Nr. 10:        " " Nr. 11:        " " Nr. 12:        " " Nr. 13:        " " Nr. 14:        "
" Nr. 15:        " " Nr. 16:        " " Nr. 17:        " " Nr. 18:        " " Nr. 19:        ";



const char settingsMenuItems[5*16] PROGMEM = 
	" Reset settings "
	" Lives          "
	" Sensitivity    "
	" Difficulty     "
	" Back           ";