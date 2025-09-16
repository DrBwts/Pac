#include <FastLED.h>
#include <stdint.h>

#define LED_PIN       7
#define NUM_LEDS      256
#define INT_BITS      16
#define INT_BITS_32   32 

CRGB leds[NUM_LEDS];

int eye_dir_flg = 0;

uint16_t ghost_bk[16] = {
                          0b0000000000000000,
                          0b0000001111000000,
                          0b0000111111110000,
                          0b0001111111111000,
                          0b0001111111111000,
                          0b0011111111111100,
                          0b0011111111111100,
                          0b0111111111111110,
                          0b0111111111111110,
                          0b0111111111111110,
                          0b0111111111111110,
                          0b0111111111111110,
                          0b0110111001110110,
                          0b0100011001100010,
                          0b0000000000000000,
                          0b0000000000000000
                        };

uint16_t eyes_lt[16] = {
                        0b0000000000000000,
                        0b0000000000000000,
                        0b0000000000000000,
                        0b0000000000000000,
                        0b0000011000011000,
                        0b0000111100111100,
                        0b0000111100111100,
                        0b0000111100111100,
                        0b0000011000011000,
                        0b0000000000000000,
                        0b0000000000000000,
                        0b0000000000000000,
                        0b0000000000000000,
                        0b0000000000000000,
                        0b0000000000000000,
                        0b0000000000000000
                        };

uint16_t pup_lt[16] = {
                        0b0000000000000000,
                        0b0000000000000000,
                        0b0000000000000000,
                        0b0000000000000000,
                        0b0000000000000000,
                        0b0000000000000000,
                        0b0000001100001100,
                        0b0000001100001100,
                        0b0000000000000000,
                        0b0000000000000000,
                        0b0000000000000000,
                        0b0000000000000000,
                        0b0000000000000000,
                        0b0000000000000000,
                        0b0000000000000000,
                        0b0000000000000000
                        };
                        
uint16_t eyes_rt[16] = {
                        0b0000000000000000,
                        0b0000000000000000,
                        0b0000000000000000,
                        0b0000000000000000,
                        0b0001100001100000,
                        0b0011110011110000,
                        0b0011110011110000,
                        0b0011110011110000,
                        0b0001100001100000,
                        0b0000000000000000,
                        0b0000000000000000,
                        0b0000000000000000,
                        0b0000000000000000,
                        0b0000000000000000,
                        0b0000000000000000,
                        0b0000000000000000
                        };

uint16_t pup_rt[16] = {
                        0b0000000000000000,
                        0b0000000000000000,
                        0b0000000000000000,
                        0b0000000000000000,
                        0b0000000000000000,
                        0b0000000000000000,
                        0b0011000011000000,
                        0b0011000011000000,
                        0b0000000000000000,
                        0b0000000000000000,
                        0b0000000000000000,
                        0b0000000000000000,
                        0b0000000000000000,
                        0b0000000000000000,
                        0b0000000000000000,
                        0b0000000000000000
                        };

uint16_t temp_ghost_bk[16];
uint16_t temp_eyes_lt[16];
uint16_t temp_pup_lt[16];
uint16_t temp_eyes_rt[16];
uint16_t temp_pup_rt[16];

int col_seq[64] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                   2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
                   3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3};

int n_col_seq = sizeof(col_seq) / sizeof(col_seq[0]);

uint8_t panel[16][16] = {
                        {0,   1,   2,   3,   4,   5,   6,   7,   8,   9,   10,  11,  12,  13,  14,  15},
                        {31,  30,  29,  28,  27,  26,  25,  24,  23,  22,  21,  20,  19,  18,  17,  16},
                        {32,  33,  34,  35,  36,  37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47},
                        {63,  62,  61,  60,  59,  58,  57,  56,  55,  54,  53,  52,  51,  50,  49,  48},
                        {64,  65,  66,  67,  68,  69,  70,  71,  72,  73,  74,  75,  76,  77,  78,  79},
                        {95,  94,  93,  92,  91,  90,  89,  88,  87,  86,  85,  84,  83,  82,  81,  80},
                        {96,  97,  98,  99,  100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111},
                        {127, 126, 125, 124, 123, 122, 121, 120, 119, 118, 117, 116, 115, 114, 113, 112},
                        {128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143},
                        {159, 158, 157, 156, 155, 154, 153, 152, 151, 150, 149, 148, 147, 146, 145, 144},
                        {160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175},
                        {191, 190, 189, 188, 187, 186, 185, 184, 183, 182, 181, 180, 179, 178, 177, 176},
                        {192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207},
                        {223, 222, 221, 220, 219, 218, 217, 216, 215, 214, 213, 212, 211, 210, 209, 208},
                        {224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239},
                        {255, 254, 253, 252, 251, 250, 249, 248, 247, 246, 245, 244, 243, 242, 241, 240}
                      };

void setup() 
{
  Serial.begin(9600);
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  clear_all();

  for(int i = 0; i < 16; i++)
  {
    temp_ghost_bk[i] = ghost_bk[i];
    temp_eyes_lt[i]  = eyes_lt[i];
    temp_pup_lt[i]   = pup_lt[i];
    temp_eyes_rt[i]  = eyes_rt[i];
    temp_pup_rt[i]   = pup_rt[i];
  } 
}

void loop() 
{
  // Clear all
  for(int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = CRGB(0, 0, 0);
  }

  for(int i = 0; i < 16; i++)
  {
    for(int j = 0; j < 16; j++)
    {
      if(temp_ghost_bk[i] & (1U << j))
      { 
        switch(col_seq[j])
        {
          case 0:
            leds[panel[i][j]] = CRGB(0, 120, 0);
            break;
          case 1:
            leds[panel[i][j]] = CRGB(120, 120, 0);
            break;
          case 2:
            leds[panel[i][j]] = CRGB(120, 0, 0);
            break;
          case 3:
            leds[panel[i][j]] = CRGB(0, 120, 120);
            break;
        }
      }
      
      if(eye_dir_flg == 0)
      {
        if(temp_eyes_lt[i] & (1U << j))
        {
          leds[panel[i][j]] = CRGB(30, 30, 30);
        }
        if(temp_pup_lt[i] & (1U << j))
        {
          leds[panel[i][j]] = CRGB(0, 0, 0);
        }
      }
      else 
      {
        if(temp_eyes_rt[i] & (1U << j))
        {
          leds[panel[i][j]] = CRGB(30, 30, 30);
        }
        if(temp_pup_rt[i] & (1U << j))
        {
          leds[panel[i][j]] = CRGB(0, 0, 0);
        }
      }
      
    }

    temp_ghost_bk[i] = leftRotate(temp_ghost_bk[i], 1);
    temp_eyes_lt[i]  = leftRotate(temp_eyes_lt[i], 1);
    temp_pup_lt[i]   = leftRotate(temp_pup_lt[i], 1);
    temp_eyes_rt[i]  = leftRotate(temp_eyes_rt[i], 1);
    temp_pup_rt[i]   = leftRotate(temp_pup_rt[i], 1);

  }

  rotateArrRight(col_seq, n_col_seq, 1);

  FastLED.show();
  delay(200);

  if(eye_dir_flg == 0)
  {
    eye_dir_flg = 1;
  }
  else
  {
    eye_dir_flg = 0;
  }
}

void clear_all()
{
  for(int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = CRGB(0, 0, 0);
  }
  FastLED.show();
}

/* rotate n by d bits*/
int leftRotate(uint16_t n, uint16_t d)
{
    return (n << d) | (n >> (INT_BITS - d));
}

int rightRotate(uint16_t n, uint16_t d)
{
    return (n >> d) | (n << (INT_BITS - d));
}

uint32_t leftRotate_uint32(uint32_t n, uint32_t d)
{
    return (n << d) | (n >> (INT_BITS_32 - d));
}

//  return (val << 1) | (val >> 31);

uint32_t rightRotate_int32(uint32_t n, uint32_t d)
{
    return (n >> d) | (n << (INT_BITS_32 - d));
}


// Function to reverse a portion of the array
void reverse(int* arr, int start, int end) 
{
    while (start < end) 
    {
        int temp = arr[start];
        arr[start] = arr[end];
        arr[end] = temp;
        start++;
        end--;
    }
}

// Function to rotate an array by d elements to the right
void rotateArrRight(int arr[], int n, int d) {
    
    // Handle the case where d > size of array
    d %= n;

    // Reverse the entire array
    reverse(arr, 0, n - 1);

    // Reverse the first d elements
    reverse(arr, 0, d - 1);

    // Reverse the remaining n-d elements
    reverse(arr, d, n - 1);
}

// Function to rotate an array by d elements to the left
void rotateArrLeft(int* arr, int n, int d) 
{
    
    // Handle the case where d > size of array
    d %= n;

    // Reverse the first d elements
    reverse(arr, 0, d - 1);

    // Reverse the remaining n-d elements
    reverse(arr, d, n - 1);

    // Reverse the entire array
    reverse(arr, 0, n - 1);
}
