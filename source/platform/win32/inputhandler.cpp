#include <platform/system.hpp>
#include <platform/win32/inputhandler.hpp>

static input_state states[2];
static input_state *current_state;
static input_state *previous_state;
static u8           virtual_map[256];

input_state*    
get_current_input_state()
{

    return current_state;

}

input_state*    
get_previous_input_state()
{

    return previous_state;

}

void            
swap_input_states()
{

    input_state *temp = current_state;
    current_state = previous_state;
    previous_state = temp;

    *current_state = *previous_state;
    return;

}

u32             
convert_keycode(u32 code)
{

    u32 result = virtual_map[code];
    return result;

}

void            
initialize_input_states()
{

    static b32 virtual_initialized = false;

    // Only needs to initialize once.
    if (virtual_initialized) return;
    virtual_initialized = true;

    current_state = &states[0];
    previous_state = &states[1];

    // Set the timestamps to a known initial value.
    for (size_t idx = 0; idx < 256; ++idx)
    {
        current_state->keyboard[idx].time = system_timestamp();
        previous_state->keyboard[idx].time = system_timestamp();
    }

    for (size_t idx = 0; idx < 8; ++idx)
    {
        current_state->mouse[idx].time = system_timestamp();
        previous_state->mouse[idx].time = system_timestamp();
    }

    virtual_map[0x00] = MagKeyNull;
    virtual_map[0x01] = MagKeyNull;
    virtual_map[0x02] = MagKeyNull;
    virtual_map[0x03] = MagKeyNull;
    virtual_map[0x04] = MagKeyNull;
    virtual_map[0x05] = MagKeyNull;
    virtual_map[0x06] = MagKeyNull;
    virtual_map[0x07] = MagKeyNull;
    virtual_map[0x08]       = MagKeyBack;
    virtual_map[0x09]       = MagKeyTab;
    virtual_map[0x0a] = MagKeyNull;
    virtual_map[0x0b] = MagKeyNull;
    virtual_map[0x0c] = MagKeyNull;
    virtual_map[0x0d]       = MagKeyEnter;
    virtual_map[0x0e] = MagKeyNull;
    virtual_map[0x0f] = MagKeyNull;
    virtual_map[0x10]       = MagKeyShift;
    virtual_map[0x11]       = MagKeyControl;
    virtual_map[0x12]       = MagKeyAlt;
    virtual_map[0x13] = MagKeyNull;
    virtual_map[0x14] = MagKeyNull;
    virtual_map[0x15] = MagKeyNull;
    virtual_map[0x16] = MagKeyNull;
    virtual_map[0x17] = MagKeyNull;
    virtual_map[0x18] = MagKeyNull;
    virtual_map[0x19] = MagKeyNull;
    virtual_map[0x1a] = MagKeyNull;
    virtual_map[0x1b]       = MagKeyEscape;
    virtual_map[0x1c] = MagKeyNull;
    virtual_map[0x1d] = MagKeyNull;
    virtual_map[0x1e] = MagKeyNull;
    virtual_map[0x1f] = MagKeyNull;
    virtual_map[0x20]       = MagKeySpace;
    virtual_map[0x21] = MagKeyNull;
    virtual_map[0x22] = MagKeyNull;
    virtual_map[0x23] = MagKeyNull;
    virtual_map[0x24] = MagKeyNull;
    virtual_map[0x25]       = MagKeyLeft;
    virtual_map[0x26]       = MagKeyUp;
    virtual_map[0x27]       = MagKeyRight;
    virtual_map[0x28]       = MagKeyDown;
    virtual_map[0x29] = MagKeyNull;
    virtual_map[0x2a] = MagKeyNull;
    virtual_map[0x2b] = MagKeyNull;
    virtual_map[0x2c] = MagKeyNull;
    virtual_map[0x2d] = MagKeyNull;
    virtual_map[0x2e]       = MagKeyDelete;
    virtual_map[0x2f] = MagKeyNull;
    virtual_map[0x30]       = MagKey0;
    virtual_map[0x31]       = MagKey1;
    virtual_map[0x32]       = MagKey2;
    virtual_map[0x33]       = MagKey3;
    virtual_map[0x34]       = MagKey4;
    virtual_map[0x35]       = MagKey5;
    virtual_map[0x36]       = MagKey6;
    virtual_map[0x37]       = MagKey7;
    virtual_map[0x38]       = MagKey8;
    virtual_map[0x39]       = MagKey9;
    virtual_map[0x3a] = MagKeyNull;
    virtual_map[0x3b] = MagKeyNull;
    virtual_map[0x3c] = MagKeyNull;
    virtual_map[0x3d] = MagKeyNull;
    virtual_map[0x3e] = MagKeyNull;
    virtual_map[0x3f] = MagKeyNull;
    virtual_map[0x40] = MagKeyNull;
    virtual_map[0x41]       = MagKeyA;
    virtual_map[0x42]       = MagKeyB;
    virtual_map[0x43]       = MagKeyC;
    virtual_map[0x44]       = MagKeyD;
    virtual_map[0x45]       = MagKeyE;
    virtual_map[0x46]       = MagKeyF;
    virtual_map[0x47]       = MagKeyG;
    virtual_map[0x48]       = MagKeyH;
    virtual_map[0x49]       = MagKeyI;
    virtual_map[0x4a]       = MagKeyJ;
    virtual_map[0x4b]       = MagKeyK;
    virtual_map[0x4c]       = MagKeyL;
    virtual_map[0x4d]       = MagKeyM;
    virtual_map[0x4e]       = MagKeyN;
    virtual_map[0x4f]       = MagKeyO;
    virtual_map[0x50]       = MagKeyP;
    virtual_map[0x51]       = MagKeyQ;
    virtual_map[0x52]       = MagKeyR;
    virtual_map[0x53]       = MagKeyS;
    virtual_map[0x54]       = MagKeyT;
    virtual_map[0x55]       = MagKeyU;
    virtual_map[0x56]       = MagKeyV;
    virtual_map[0x57]       = MagKeyW;
    virtual_map[0x58]       = MagKeyX;
    virtual_map[0x59]       = MagKeyY;
    virtual_map[0x5a]       = MagKeyZ;
    virtual_map[0x5b] = MagKeyNull;
    virtual_map[0x5c] = MagKeyNull;
    virtual_map[0x5d] = MagKeyNull;
    virtual_map[0x5e] = MagKeyNull;
    virtual_map[0x5f] = MagKeyNull;
    virtual_map[0x60]       = MagKey0;
    virtual_map[0x61]       = MagKey1;
    virtual_map[0x62]       = MagKey2;
    virtual_map[0x63]       = MagKey3;
    virtual_map[0x64]       = MagKey4;
    virtual_map[0x65]       = MagKey5;
    virtual_map[0x66]       = MagKey6;
    virtual_map[0x67]       = MagKey7;
    virtual_map[0x68]       = MagKey8;
    virtual_map[0x69]       = MagKey9;
    virtual_map[0x6a] = MagKeyNull;
    virtual_map[0x6b] = MagKeyNull;
    virtual_map[0x6c] = MagKeyNull;
    virtual_map[0x6d] = MagKeyNull;
    virtual_map[0x6e] = MagKeyNull;
    virtual_map[0x6f] = MagKeyNull;
    virtual_map[0x70]       = MagKeyF1;
    virtual_map[0x71]       = MagKeyF2;
    virtual_map[0x72]       = MagKeyF3;
    virtual_map[0x73]       = MagKeyF4;
    virtual_map[0x74]       = MagKeyF5;
    virtual_map[0x75]       = MagKeyF6;
    virtual_map[0x76]       = MagKeyF7;
    virtual_map[0x77]       = MagKeyF8;
    virtual_map[0x78]       = MagKeyF9;
    virtual_map[0x79]       = MagKeyF10;
    virtual_map[0x7a]       = MagKeyF11;
    virtual_map[0x7b]       = MagKeyF12;
    virtual_map[0x7c] = MagKeyNull;
    virtual_map[0x7d] = MagKeyNull;
    virtual_map[0x7e] = MagKeyNull;
    virtual_map[0x7f] = MagKeyNull;
    virtual_map[0x80] = MagKeyNull;
    virtual_map[0x81] = MagKeyNull;
    virtual_map[0x82] = MagKeyNull;
    virtual_map[0x83] = MagKeyNull;
    virtual_map[0x84] = MagKeyNull;
    virtual_map[0x85] = MagKeyNull;
    virtual_map[0x86] = MagKeyNull;
    virtual_map[0x87] = MagKeyNull;
    virtual_map[0x88] = MagKeyNull;
    virtual_map[0x89] = MagKeyNull;
    virtual_map[0x8a] = MagKeyNull;
    virtual_map[0x8b] = MagKeyNull;
    virtual_map[0x8c] = MagKeyNull;
    virtual_map[0x8d] = MagKeyNull;
    virtual_map[0x8e] = MagKeyNull;
    virtual_map[0x8f] = MagKeyNull;
    virtual_map[0x90] = MagKeyNull;
    virtual_map[0x91] = MagKeyNull;
    virtual_map[0x92] = MagKeyNull;
    virtual_map[0x93] = MagKeyNull;
    virtual_map[0x94] = MagKeyNull;
    virtual_map[0x95] = MagKeyNull;
    virtual_map[0x96] = MagKeyNull;
    virtual_map[0x97] = MagKeyNull;
    virtual_map[0x98] = MagKeyNull;
    virtual_map[0x99] = MagKeyNull;
    virtual_map[0x9a] = MagKeyNull;
    virtual_map[0x9b] = MagKeyNull;
    virtual_map[0x9c] = MagKeyNull;
    virtual_map[0x9d] = MagKeyNull;
    virtual_map[0x9e] = MagKeyNull;
    virtual_map[0x9f] = MagKeyNull;
    virtual_map[0xa0]       = MagKeyShift; // Left shift
    virtual_map[0xa1]       = MagKeyShift; // Right shift
    virtual_map[0xa2]       = MagKeyControl; // Left control
    virtual_map[0xa3]       = MagKeyControl; // Right control
    virtual_map[0xa4]       = MagKeyAlt; // Left alt
    virtual_map[0xa5]       = MagKeyAlt; // Right alt
    virtual_map[0xa6] = MagKeyNull;
    virtual_map[0xa7] = MagKeyNull;
    virtual_map[0xa8] = MagKeyNull;
    virtual_map[0xa9] = MagKeyNull;
    virtual_map[0xaa] = MagKeyNull;
    virtual_map[0xab] = MagKeyNull;
    virtual_map[0xac] = MagKeyNull;
    virtual_map[0xad] = MagKeyNull;
    virtual_map[0xae] = MagKeyNull;
    virtual_map[0xaf] = MagKeyNull;
    virtual_map[0xb0] = MagKeyNull;
    virtual_map[0xb1] = MagKeyNull;
    virtual_map[0xb2] = MagKeyNull;
    virtual_map[0xb3] = MagKeyNull;
    virtual_map[0xb4] = MagKeyNull;
    virtual_map[0xb5] = MagKeyNull;
    virtual_map[0xb6] = MagKeyNull;
    virtual_map[0xb7] = MagKeyNull;
    virtual_map[0xb8] = MagKeyNull;
    virtual_map[0xb9] = MagKeyNull;
    virtual_map[0xba]       = MagKeySemicolon;
    virtual_map[0xbb]       = MagKeyEqual;
    virtual_map[0xbc]       = MagKeyComma;
    virtual_map[0xbd]       = MagKeyMinus;
    virtual_map[0xbe]       = MagKeyPeriod;
    virtual_map[0xbf]       = MagKeyRightSlash;
    virtual_map[0xc0]       = MagKeyTilde;
    virtual_map[0xc1] = MagKeyNull;
    virtual_map[0xc2] = MagKeyNull;
    virtual_map[0xc3] = MagKeyNull;
    virtual_map[0xc4] = MagKeyNull;
    virtual_map[0xc5] = MagKeyNull;
    virtual_map[0xc6] = MagKeyNull;
    virtual_map[0xc7] = MagKeyNull;
    virtual_map[0xc8] = MagKeyNull;
    virtual_map[0xc9] = MagKeyNull;
    virtual_map[0xca] = MagKeyNull;
    virtual_map[0xcb] = MagKeyNull;
    virtual_map[0xcc] = MagKeyNull;
    virtual_map[0xcd] = MagKeyNull;
    virtual_map[0xce] = MagKeyNull;
    virtual_map[0xcf] = MagKeyNull;
    virtual_map[0xd0] = MagKeyNull;
    virtual_map[0xd1] = MagKeyNull;
    virtual_map[0xd2] = MagKeyNull;
    virtual_map[0xd3] = MagKeyNull;
    virtual_map[0xd4] = MagKeyNull;
    virtual_map[0xd5] = MagKeyNull;
    virtual_map[0xd6] = MagKeyNull;
    virtual_map[0xd7] = MagKeyNull;
    virtual_map[0xd8] = MagKeyNull;
    virtual_map[0xd9] = MagKeyNull;
    virtual_map[0xda] = MagKeyNull;
    virtual_map[0xdb]       = MagKeyLeftBracket;
    virtual_map[0xdc]       = MagKeyLeftSlash;
    virtual_map[0xdd]       = MagKeyRightBracket;
    virtual_map[0xde]       = MagKeyQuote;
    virtual_map[0xdf] = MagKeyNull;
    virtual_map[0xe0] = MagKeyNull;
    virtual_map[0xe1] = MagKeyNull;
    virtual_map[0xe2] = MagKeyNull;
    virtual_map[0xe3] = MagKeyNull;
    virtual_map[0xe4] = MagKeyNull;
    virtual_map[0xe5] = MagKeyNull;
    virtual_map[0xe6] = MagKeyNull;
    virtual_map[0xe7] = MagKeyNull;
    virtual_map[0xe8] = MagKeyNull;
    virtual_map[0xe9] = MagKeyNull;
    virtual_map[0xea] = MagKeyNull;
    virtual_map[0xeb] = MagKeyNull;
    virtual_map[0xec] = MagKeyNull;
    virtual_map[0xed] = MagKeyNull;
    virtual_map[0xee] = MagKeyNull;
    virtual_map[0xef] = MagKeyNull;
    virtual_map[0xf0] = MagKeyNull;
    virtual_map[0xf1] = MagKeyNull;
    virtual_map[0xf2] = MagKeyNull;
    virtual_map[0xf3] = MagKeyNull;
    virtual_map[0xf4] = MagKeyNull;
    virtual_map[0xf5] = MagKeyNull;
    virtual_map[0xf6] = MagKeyNull;
    virtual_map[0xf7] = MagKeyNull;
    virtual_map[0xf8] = MagKeyNull;
    virtual_map[0xf9] = MagKeyNull;
    virtual_map[0xfa] = MagKeyNull;
    virtual_map[0xfb] = MagKeyNull;
    virtual_map[0xfc] = MagKeyNull;
    virtual_map[0xfd] = MagKeyNull;
    virtual_map[0xfe] = MagKeyNull;
    virtual_map[0xff] = MagKeyNull;

}
