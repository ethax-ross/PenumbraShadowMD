#ifdef ESP32

#define SDA_PIN 21
#define SCL_PIN 22
#define RX_PIN 3

#define MAIN_PIN 27
#define RIGHT_PIN 25
#define LEFT_PIN 16
#define BOTTOM_PIN 17
#define TOP_PIN 33
#define BACK_PIN 26

#define LIGHTKIT_PIN_A RX_PIN
#define LIGHTKIT_PIN_B SDA_PIN
#define LIGHTKIT_PIN_C SCL_PIN

#elif defined(__AVR__)

#define SDA_PIN SDA
#define SCL_PIN SCL
#define RX_PIN 0

#define MAIN_PIN 5
#define RIGHT_PIN 10
#define LEFT_PIN 3
#define BOTTOM_PIN 2
#define TOP_PIN 6
#define BACK_PIN 9

#define LIGHTKIT_PIN_A RX_PIN
#define LIGHTKIT_PIN_B SDA_PIN
#define LIGHTKIT_PIN_C SCL_PIN

#else

#error Unsupported board

#endif

// Enable this define and connect pins RX, SDA, and SCL to the Uppity Spinner
// Rotary Board. Connect RX to A, SDA, to B, and SCL to C. Connect ground and 5V
#define UPPITY_SPINNER_MODE
#ifdef UPPITY_SPINNER_MODE
#define __ERROR(a)    _Pragma(#a)
#define _ERROR( a) __ERROR(GCC error #a)

// 
#if LIGHTKIT_PIN_A == RX_PIN
#define Serial _ERROR(Cannot use Serial in Uppity Spinner Mode. RX pins used as pin A)
#endif

#endif
