#include <math.h>

#define RED 50
#define GREEN 48
#define BLUE 26
#define MATRIX_HEIGHT 20
#define MATRIX_WIDTH 19

#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))

double angle_r = 0, frequency = 0.5;
uint8_t index = 0, i = 0;
uint16_t deltaTime = 0;
unsigned long lastTrigger = 0;
unsigned long currentTime = 0, lastTime = 0;
bool prevRead = 0, red = 0, green = 0, digRead = 0, blue = 0;
unsigned long matrix[MATRIX_HEIGHT][MATRIX_WIDTH] = {
{0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000 },
{0x000000, 0xff0000, 0x000000, 0xff0000, 0x000000, 0xff0000, 0xff0000, 0x000000, 0xff0000, 0x000000, 0x000000, 0xff0000, 0x000000, 0x000000, 0xff0000, 0xff0000, 0xff0000, 0x000000, 0x000000 },
{0x000000, 0xff0000, 0x000000, 0xff0000, 0x000000, 0xff0000, 0x000000, 0x000000, 0xff0000, 0x000000, 0x000000, 0xff0000, 0x000000, 0x000000, 0xff0000, 0x000000, 0xff0000, 0x000000, 0x000000 },
{0x000000, 0xff0000, 0xff0000, 0xff0000, 0x000000, 0xff0000, 0xff0000, 0x000000, 0xff0000, 0x000000, 0x000000, 0xff0000, 0x000000, 0x000000, 0xff0000, 0x000000, 0xff0000, 0x000000, 0x000000 },
{0x000000, 0xff0000, 0x000000, 0xff0000, 0x000000, 0xff0000, 0x000000, 0x000000, 0xff0000, 0x000000, 0x000000, 0xff0000, 0x000000, 0x000000, 0xff0000, 0x000000, 0xff0000, 0x000000, 0x000000 },
{0x000000, 0xff0000, 0x000000, 0xff0000, 0x000000, 0xff0000, 0xff0000, 0x000000, 0xff0000, 0xff0000, 0x000000, 0xff0000, 0xff0000, 0x000000, 0xff0000, 0xff0000, 0xff0000, 0x000000, 0x000000 },
{0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000 },
{0x000000, 0x00ff00, 0x000000, 0x00ff00, 0x000000, 0x00ff00, 0x00ff00, 0x000000, 0x00ff00, 0x000000, 0x000000, 0x00ff00, 0x000000, 0x000000, 0x00ff00, 0x00ff00, 0x00ff00, 0x000000, 0x000000 },
{0x000000, 0x00ff00, 0x000000, 0x00ff00, 0x000000, 0x00ff00, 0x000000, 0x000000, 0x00ff00, 0x000000, 0x000000, 0x00ff00, 0x000000, 0x000000, 0x00ff00, 0x000000, 0x00ff00, 0x000000, 0x000000 },
{0x000000, 0x00ff00, 0x00ff00, 0x00ff00, 0x000000, 0x00ff00, 0x00ff00, 0x000000, 0x00ff00, 0x000000, 0x000000, 0x00ff00, 0x000000, 0x000000, 0x00ff00, 0x000000, 0x00ff00, 0x000000, 0x000000 },
{0x000000, 0x00ff00, 0x000000, 0x00ff00, 0x000000, 0x00ff00, 0x000000, 0x000000, 0x00ff00, 0x000000, 0x000000, 0x00ff00, 0x000000, 0x000000, 0x00ff00, 0x000000, 0x00ff00, 0x000000, 0x000000 },
{0x000000, 0x00ff00, 0x000000, 0x00ff00, 0x000000, 0x00ff00, 0x00ff00, 0x000000, 0x00ff00, 0x00ff00, 0x000000, 0x00ff00, 0x00ff00, 0x000000, 0x00ff00, 0x00ff00, 0x00ff00, 0x000000, 0x000000 },
{0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000 },
{0x000000, 0x0000ff, 0x000000, 0x0000ff, 0x000000, 0x0000ff, 0x0000ff, 0x000000, 0x0000ff, 0x000000, 0x000000, 0x0000ff, 0x000000, 0x000000, 0x0000ff, 0x0000ff, 0x0000ff, 0x000000, 0x000000 },
{0x000000, 0x0000ff, 0x000000, 0x0000ff, 0x000000, 0x0000ff, 0x000000, 0x000000, 0x0000ff, 0x000000, 0x000000, 0x0000ff, 0x000000, 0x000000, 0x0000ff, 0x000000, 0x0000ff, 0x000000, 0x000000 },
{0x000000, 0x0000ff, 0x0000ff, 0x0000ff, 0x000000, 0x0000ff, 0x0000ff, 0x000000, 0x0000ff, 0x000000, 0x000000, 0x0000ff, 0x000000, 0x000000, 0x0000ff, 0x000000, 0x0000ff, 0x000000, 0x000000 },
{0x000000, 0x0000ff, 0x000000, 0x0000ff, 0x000000, 0x0000ff, 0x000000, 0x000000, 0x0000ff, 0x000000, 0x000000, 0x0000ff, 0x000000, 0x000000, 0x0000ff, 0x000000, 0x0000ff, 0x000000, 0x000000 },
{0x000000, 0x0000ff, 0x000000, 0x0000ff, 0x000000, 0x0000ff, 0x0000ff, 0x000000, 0x0000ff, 0x0000ff, 0x000000, 0x0000ff, 0x0000ff, 0x000000, 0x0000ff, 0x0000ff, 0x0000ff, 0x000000, 0x000000 },
{0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000 },
{0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000 }
};
volatile uint8_t *out;
uint8_t red_matrix[MATRIX_HEIGHT][MATRIX_WIDTH],
green_matrix[MATRIX_HEIGHT][MATRIX_WIDTH],
blue_matrix[MATRIX_HEIGHT][MATRIX_WIDTH];

const double _30 = PI / 6, _150 = 5 * _30, _3_2PI = 3.0 / (2.0 * PI),
             _270 = 3 * PI / 2, full_circle = 2 * PI / 1000000, _360 = 2 * PI, _6_PI = 6 / PI;
uint8_t nOverflows = 0, timer_ticks = 0, pwm_resolution = 128;
const uint8_t pins[MATRIX_HEIGHT] = {43, 42, 41, 40, 47, 45, 44, 2, 5, 3, 4, 13, 46, 12, 11, 8, 10, 9, 6, 7};//Tabela pinova

void DigitalWrite(uint8_t pin, bool val)
{
    out = portOutputRegister(digitalPinToPort(pin));
    if (val) *out |= digitalPinToBitMask(pin);
    else *out &= ~digitalPinToBitMask(pin);
}

bool DigitalRead(uint8_t pin)
{
  return *portInputRegister(digitalPinToPort(pin)) & digitalPinToBitMask(pin);
}

void AnalogWrite(uint8_t pin, int val)
{
    if (val == 0)
        digitalWrite(pin, LOW);
    else if (val == 255)
        digitalWrite(pin, HIGH);
    else
    {
      switch(digitalPinToTimer(pin))
      {
        #if defined(TCCR0) && defined(COM00) && !defined(__AVR_ATmega8__)
        case TIMER0A: 
        sbi(TCCR0, COM00); OCR0 = val; break;
        #endif
        #if defined(TCCR0A) && defined(COM0A1)
        case TIMER0A: sbi(TCCR0A, COM0A1); OCR0A = val; break;
        #endif
        #if defined(TCCR0A) && defined(COM0B1)
        case TIMER0B: sbi(TCCR0A, COM0B1); OCR0B = val; break;
        #endif
        #if defined(TCCR1A) && defined(COM1A1)
        case TIMER1A: sbi(TCCR1A, COM1A1); OCR1A = val; break;
        #endif
        #if defined(TCCR1A) && defined(COM1B1)
        case TIMER1B: sbi(TCCR1A, COM1B1); OCR1B = val; break;
        #endif
        #if defined(TCCR1A) && defined(COM1C1)
        case TIMER1C: sbi(TCCR1A, COM1C1); OCR1C = val; break;
        #endif
        #if defined(TCCR2) && defined(COM21)
        case TIMER2: sbi(TCCR2, COM21); OCR2 = val; break;
        #endif
        #if defined(TCCR2A) && defined(COM2A1)
        case TIMER2A: sbi(TCCR2A, COM2A1); OCR2A = val; break;
        #endif
        #if defined(TCCR2A) && defined(COM2B1)
        case TIMER2B: sbi(TCCR2A, COM2B1); OCR2B = val; break;
        #endif
        #if defined(TCCR3A) && defined(COM3A1)
        case TIMER3A: sbi(TCCR3A, COM3A1); OCR3A = val; break;
        #endif
        #if defined(TCCR3A) && defined(COM3B1)
       case TIMER3B: sbi(TCCR3A, COM3B1); OCR3B = val; break;
        #endif
        #if defined(TCCR3A) && defined(COM3C1)
        case TIMER3C: sbi(TCCR3A, COM3C1); OCR3C = val; break;
        #endif
        #if defined(TCCR4A)
        case TIMER4A: sbi(TCCR4A, COM4A1);
            #if defined(COM4A0)
            cbi(TCCR4A, COM4A0);
            #endif 
            OCR4A = val; break;
        #endif
        #if defined(TCCR4A) && defined(COM4B1)
        case TIMER4B: sbi(TCCR4A, COM4B1); OCR4B = val; break;
        #endif
        #if defined(TCCR4A) && defined(COM4C1)
        case TIMER4C: sbi(TCCR4A, COM4C1); OCR4C = val; break;
        #endif
        #if defined(TCCR4C) && defined(COM4D1)
        case TIMER4D: sbi(TCCR4C, COM4D1);
            #if defined(COM4D0)
            cbi(TCCR4C, COM4D0);
            #endif
            OCR4D = val; break;
        #endif
        #if defined(TCCR5A) && defined(COM5A1)
        case TIMER5A: sbi(TCCR5A, COM5A1); OCR5A = val; break;
        #endif
        #if defined(TCCR5A) && defined(COM5B1)
        case TIMER5B: sbi(TCCR5A, COM5B1); OCR5B = val; break;
        #endif
        #if defined(TCCR5A) && defined(COM5C1)
        case TIMER5C: sbi(TCCR5A, COM5C1); OCR5C = val; break;
        #endif
      }
    }
}

void _AnalogWrite(uint8_t pin, uint8_t val)
{
    DigitalWrite(pin, ((timer_ticks + nOverflows <= val * pwm_resolution / 256) || val == 255) && val != 0);
}

void setup()
{
    for (int i = 0; i <= 53; i++) 
    {
        pinMode(i, OUTPUT);
        digitalWrite(i, LOW);
    }
    pinMode(52, INPUT);
    digitalWrite(53, HIGH);
    pinMode(49, INPUT);
    //Serial.begin(250000);
    for (i = 0; i < MATRIX_HEIGHT; i++)
        for (uint8_t j = 0; j < MATRIX_WIDTH; j++)
        {
            red_matrix[i][j] =(matrix[i][j] >> 16) & 0xff;
        }
    for (i = 0; i < MATRIX_HEIGHT; i++)
        for (uint8_t j = 0; j < MATRIX_WIDTH; j++)
        {
            green_matrix[i][j] = (matrix[i][j] >> 8) & 0xff;
        }
    for (i = 0; i < MATRIX_HEIGHT; i++)
        for (int j = 0; j < MATRIX_WIDTH; j++)
        {
            blue_matrix[i][j] = (uint8_t)((matrix[i][j] & 0xff));
        }
}

void loop()
{
    if ((currentTime = 4 * TCNT0) > lastTime)
        deltaTime = (uint16_t)(currentTime - lastTime);
    lastTime = currentTime;
        
    angle_r += full_circle * frequency * deltaTime;
    if (angle_r >= _360)
        angle_r = 0;
    red = _30 < angle_r && angle_r < _150;
    green = _150 < angle_r && angle_r < _270;
    blue =  (_270 < angle_r && angle_r < _360) || angle_r < _30;

    //Serial.println(deltaTime);
    
    if (!red)
        DigitalWrite(RED, HIGH);
    if (!green)
        DigitalWrite(GREEN, HIGH);
    if (red || green)
        DigitalWrite(BLUE, HIGH);
    
    if (timer_ticks > TCNT0)
        nOverflows = nOverflows < 255 ? nOverflows + 256 : 0;
    timer_ticks = TCNT0;
    
    if (red)
    {
        index = (uint8_t)((MATRIX_WIDTH - 1) * (1.25 - _3_2PI * angle_r));
        for (i = 0; i < 5; i++)              
            _AnalogWrite(pins[i], red_matrix[i][index]);
        for (i = 5; i < MATRIX_HEIGHT; i++)
            AnalogWrite(pins[i], red_matrix[i][index]);
        DigitalWrite(RED, LOW);
    }
    else if (green)
    {
        index = (uint8_t)((MATRIX_WIDTH - 1) * (2.25 - _3_2PI * angle_r));
        for (i = 0; i < 5; i++)
            _AnalogWrite(pins[i], green_matrix[i][index]);
        for (i = 5; i < MATRIX_HEIGHT; i++)
            AnalogWrite(pins[i], green_matrix[i][index]);
        DigitalWrite(GREEN, LOW);
    }
    else if (blue)
    {
        if (angle_r < _30)
            index = (uint8_t)((MATRIX_WIDTH - 1) * (3.25 - _3_2PI * (angle_r + _360)));
        else
            index = (uint8_t)((MATRIX_WIDTH - 1) * (3.25 - _3_2PI * angle_r));
        for (i = 0; i < 5; i++)
            _AnalogWrite(pins[i], blue_matrix[i][index]);
        for (i = 5; i < MATRIX_HEIGHT; i++)
            AnalogWrite(pins[i], blue_matrix[i][index]);
        DigitalWrite(BLUE, LOW);
                    /*Serial.print(angle_r * 180 / PI);
            Serial.write(" ");
            Serial.println(index);*/
    }
    if (timer_ticks + nOverflows > pwm_resolution)
        nOverflows = 0;

    digRead = DigitalRead(52);
    if (prevRead == 0 && digRead)
    {
          if ((currentTime = micros()) > lastTrigger)
         {
              angle_r = 0;
              frequency = 1000000.0 / (currentTime - lastTrigger);
              lastTrigger = currentTime;
              prevRead = 1;
          }
    }
    else if (!digRead)
        prevRead = 0;
}
