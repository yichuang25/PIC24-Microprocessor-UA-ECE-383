/** \file
 Original code based on Microchip application note AN643, it used
16-bit signed ADC samples and the standard IMA ADPCM compression.
This version has been modified to use Dialogic ADPCM, which
uses 12-bit samples and a smaller table.
Specification is at:
http://multimedia.cx/mirror/dialogic-adpcm.pdf

*/

#include "pic24_all.h"

struct ADPCMstate {
  int16_t i16_prevsample;/* Predicted sample */
  int16_t i16_previndex;/* Index into step size table */
};

static struct ADPCMstate state;

/* Table of index changes */
static const int8_t ai8_IndexTable[16] = {
  -1, -1, -1, -1, 2, 4, 6, 8,
  -1, -1, -1, -1, 2, 4, 6, 8
};


#define MAX_POS 2047
#define MAX_NEG -2048
#define MAX_INDEX 48

static const uint16_t au16_StepSizeTable[49] = {
  16, 17, 19, 21, 23, 25, 28, 31, 34, 37, 41, 45,
  50, 55, 60, 66, 73, 80, 88, 97, 107, 118, 130, 143,
  157, 173, 190, 209, 230, 253, 279, 307, 337, 371, 408, 449,
  494, 544, 598, 658, 724, 796, 876, 963, 1060, 1166, 1282, 1411, 1552
};

void ADPCMEncoderInit(void) {
  state.i16_prevsample=0;
  state.i16_previndex=0;
}

static int16_t i16_diff; /* Difference between sample and predicted sample */
static int16_t i16_step; /* Quantizer step size */
static int16_t i16_predsample; /* Output of ADPCM predictor */
static int16_t i16_diffq; /* Dequantized predicted difference */
static int16_t i16_index; /* Index into step size table */
/*****************************************************************************
* ADPCMEncoder - ADPCM encoder routine *
******************************************************************************
* Input Variables: *
* int16_t sample - 16-bit signed speech sample *
* Return Variable: *
* char - 8-bit number containing the 4-bit ADPCM code *
*****************************************************************************/
uint8_t ADPCMEncoder( int16_t i16_sample) {
  uint8_t code; /* ADPCM output value */
  int16_t tempstep; /* Temporary step size */
  /* Restore previous values of predicted sample and quantizer step
  size index
  */
  i16_predsample = state.i16_prevsample;
  i16_index = state.i16_previndex;
  i16_step = au16_StepSizeTable[i16_index];
  /* Compute the difference between the actual sample (sample) and the
  the predicted sample (predsample)
  */
  i16_diff = i16_sample - i16_predsample;
  if (i16_diff >= 0)
    code = 0;
  else {
    code = 8;
    i16_diff = -i16_diff;
  }
  /* Quantize the difference into the 4-bit ADPCM code using the
  the quantizer step size
  */
  tempstep = i16_step;
  if ( i16_diff >= tempstep ) {
    code |= 4;
    i16_diff -= tempstep;
  }
  tempstep >>= 1;
  if ( i16_diff >= tempstep ) {
    code |= 2;
    i16_diff -= tempstep;
  }
  tempstep >>= 1;
  if ( i16_diff >= tempstep )
    code |= 1;
  /* Inverse quantize the ADPCM code into a predicted difference
  using the quantizer step size
  */
  i16_diffq = i16_step >> 3;
  if ( code & 4 )
    i16_diffq += i16_step;
  if ( code & 2 )
    i16_diffq += i16_step >> 1;
  if ( code & 1 )
    i16_diffq += i16_step >> 2;
  /* Fixed predictor computes new predicted sample by adding the
  old predicted sample to predicted difference
  */
  if ( code & 8 )
    i16_predsample -= i16_diffq;
  else
    i16_predsample += i16_diffq;
  /* Check for overflow of the new predicted sample
  */
  if ( i16_predsample > MAX_POS )
    i16_predsample = MAX_POS;
  else if ( i16_predsample < MAX_NEG )
    i16_predsample = MAX_NEG;
  /* Find new quantizer stepsize index by adding the old index
  to a table lookup using the ADPCM code
  */
  i16_index += ai8_IndexTable[code];
  /* Check for overflow of the new quantizer step size index
  */
  if ( i16_index < 0 )
    i16_index = 0;
  if ( i16_index > MAX_INDEX )
    i16_index = MAX_INDEX;
  /* Save the predicted sample and quantizer step size index for
  next iteration
  */
  state.i16_prevsample = i16_predsample;
  state.i16_previndex = i16_index;
  /* Return the new ADPCM code */
  return ( code & 0x0f );
}

void ADPCMDecoderInit(void) {
  state.i16_prevsample= 0;
  state.i16_previndex=0;
}

/*****************************************************************************
* ADPCMDecoder - ADPCM decoder routine *
******************************************************************************
* Input Variables: *
* char code - 8-bit number containing the 4-bit ADPCM code *
* Return Variable: *
* int16_t - 16-bit signed speech sample *
*****************************************************************************/
int16_t ADPCMDecoder(uint8_t u8_code) {
  /* Restore previous values of predicted sample and quantizer step
  size index
  */
  i16_predsample = state.i16_prevsample;
  i16_index = state.i16_previndex;
  /* Find quantizer step size from lookup table using index
  */
  i16_step = au16_StepSizeTable[i16_index];
  /* Inverse quantize the ADPCM code into a difference using the
  quantizer step size
  */
  i16_diffq = i16_step >> 3;
  if ( u8_code & 4 )
    i16_diffq += i16_step;
  if ( u8_code & 2 )
    i16_diffq += i16_step >> 1;
  if ( u8_code & 1 )
    i16_diffq += i16_step >> 2;
  /* Add the difference to the predicted sample
  */
  if ( u8_code & 8 )
    i16_predsample -= i16_diffq;
  else
    i16_predsample += i16_diffq;
  /* Check for overflow of the new predicted sample
  */
  if ( i16_predsample > MAX_POS )
    i16_predsample = MAX_POS;
  else if ( i16_predsample < MAX_NEG )
    i16_predsample = MAX_NEG;
  /* Find new quantizer step size by adding the old index and a
  table lookup using the ADPCM code
  */
  i16_index += ai8_IndexTable[u8_code];
  /* Check for overflow of the new quantizer step size index
  */
  if ( i16_index < 0 )
    i16_index = 0;
  if ( i16_index > MAX_INDEX )
    i16_index = MAX_INDEX;
  /* Save predicted sample and quantizer step size index for next
  iteration
  */
  state.i16_prevsample = i16_predsample;
  state.i16_previndex = i16_index;
  /* Return the new speech sample */
  return( i16_predsample );
}
