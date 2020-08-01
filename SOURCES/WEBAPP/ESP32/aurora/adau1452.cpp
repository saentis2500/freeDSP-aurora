#include <Arduino.h>
#include <Wire.h>
#include "AudioFilterFactory.h"
#include "hwconfig.h"
#include "adau1452.h"
#include "settings.h"

//==============================================================================
/*!
 */
void ADAU1452_WRITE_REGISTER( uint16_t reg, byte msb, byte lsb )
{
  Wire.beginTransmission( DSP_ADDR );
  Wire.write( (byte)( (reg >> 8) & 0xFF ) );
  Wire.write( (byte)(  reg       & 0xFF ) );
  Wire.write( msb );
  Wire.write( lsb );
  Wire.endTransmission( true );
}

//==============================================================================
/*!
 */
void ADAU1452_WRITE_BLOCK( uint16_t regaddr, byte val[], uint16_t len )
{
  for( uint16_t ii = 0; ii < len; ii = ii + 4 )
  {
    Wire.beginTransmission( DSP_ADDR );
    Wire.write( (byte)( (regaddr >> 8) & 0xFF ) );
    Wire.write( (byte)(  regaddr       & 0xFF ) );
    Wire.write( (byte)( val[ii] & 0xFF ) );
    Wire.write( (byte)( val[ii+1] & 0xFF ) );
    Wire.write( (byte)( val[ii+2] & 0xFF ) );
    Wire.write( (byte)( val[ii+3] & 0xFF ) );
    Wire.endTransmission( true );
  }
}

//==============================================================================
/*!
 *
 */
uint32_t convertTo824( double val )
{
  double fractpart, intpart;
  uint32_t ret;

  if( val < 0.0 )
    val += 256.0;

  intpart = floor( val );
  fractpart = val - intpart;

  ret = ((( static_cast<uint32_t>(static_cast<int8_t>(intpart)) ) << 24) & 0xff000000)
      + ((static_cast<uint32_t>(fractpart * 16777216.0)) & 0x00ffffff);

  return ret;
}

//==============================================================================
/*! Enables or disables the volume potentiometer
 *
 */
void enableVolPot( void )
{
  if( addrVPot )
  {
    uint32_t vpot;
    if( Settings.vpot == true )
      vpot = 0x00000000;
    else
      vpot = 0x00000001;

    byte val[4];
    val[0] = (vpot >> 24 ) & 0xFF;
    val[1] = (vpot >> 16 ) & 0xFF;
    val[2] = (vpot >> 8 ) & 0xFF;
    val[3] =  vpot & 0xFF;
    ADAU1452_WRITE_BLOCK( addrVPot, val, 4 );
  }
}

//==============================================================================
/*! Sets the values for a crossover block on DSP.
 *
 */
void setCrossover( int idx )
{
  if( (paramCrossover[idx].lp_fc > 0) && (paramCrossover[idx].lp_fc < 20000)
      && (paramCrossover[idx].lp_typ >= 0) && (paramCrossover[idx].lp_typ < AudioFilterFactory::kNumFilterDesigns) )
  {
    float a[12] = { 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0 };
    float b[12] = { 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0 };
    byte val[4];
    uint32_t floatval;

    if( !(paramCrossover[idx].lp_bypass) )
      AudioFilterFactory::makeLowPass( a, b, paramCrossover[idx].lp_typ, paramCrossover[idx].lp_fc, sampleRate );

    for( int ii = 0; ii < 4; ii++ )
    {
      uint16_t addr = paramCrossover[idx].lp_addr[ii];
      floatval = convertTo824(b[ 3*ii + 2 ]);
      val[0] = (floatval >> 24 ) & 0xFF;
      val[1] = (floatval >> 16 ) & 0xFF;
      val[2] = (floatval >> 8 ) & 0xFF;
      val[3] =  floatval & 0xFF;
      ADAU1452_WRITE_BLOCK( addr, val, 4 );  // B2
      addr++;

      floatval = convertTo824(b[ 3*ii + 1 ]);
      val[0] = (floatval >> 24 ) & 0xFF;
      val[1] = (floatval >> 16 ) & 0xFF;
      val[2] = (floatval >> 8 ) & 0xFF;
      val[3] =  floatval & 0xFF;
      ADAU1452_WRITE_BLOCK( addr, val, 4 );  // B1
      addr++;

      floatval = convertTo824(b[ 3*ii + 0 ]);
      val[0] = (floatval >> 24 ) & 0xFF;
      val[1] = (floatval >> 16 ) & 0xFF;
      val[2] = (floatval >> 8 ) & 0xFF;
      val[3] =  floatval & 0xFF;
      ADAU1452_WRITE_BLOCK( addr, val, 4 );  // B0
      addr++;

      floatval = convertTo824(a[ 3*ii + 2 ]);
      val[0] = (floatval >> 24 ) & 0xFF;
      val[1] = (floatval >> 16 ) & 0xFF;
      val[2] = (floatval >> 8 ) & 0xFF;
      val[3] =  floatval & 0xFF;
      ADAU1452_WRITE_BLOCK( addr, val, 4 );  // A2
      addr++;

      floatval = convertTo824(a[ 3*ii + 1 ]);
      val[0] = (floatval >> 24 ) & 0xFF;
      val[1] = (floatval >> 16 ) & 0xFF;
      val[2] = (floatval >> 8 ) & 0xFF;
      val[3] =  floatval & 0xFF;
      ADAU1452_WRITE_BLOCK( addr, val, 4 );  // A1
      addr++;
    }
  }

  if( (paramCrossover[idx].hp_fc > 0) && (paramCrossover[idx].hp_fc < 20000)
    && (paramCrossover[idx].hp_typ >= 0) && (paramCrossover[idx].hp_typ < AudioFilterFactory::kNumFilterDesigns) )
  {
    float a[12] = { 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0 };
    float b[12] = { 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0 };
    byte val[4];
    uint32_t floatval;
    if( !(paramCrossover[idx].hp_bypass) )
      AudioFilterFactory::makeHighPass( a, b, paramCrossover[idx].hp_typ, paramCrossover[idx].hp_fc, sampleRate );

    for( int ii = 0; ii < 4; ii++ )
    {
      uint16_t addr = paramCrossover[idx].hp_addr[ii];
      floatval = convertTo824(b[ 3*ii + 2 ]);
      val[0] = (floatval >> 24 ) & 0xFF;
      val[1] = (floatval >> 16 ) & 0xFF;
      val[2] = (floatval >> 8 ) & 0xFF;
      val[3] =  floatval & 0xFF;
      ADAU1452_WRITE_BLOCK( addr, val, 4 );  // B2
      addr++;

      floatval = convertTo824(b[ 3*ii + 1 ]);
      val[0] = (floatval >> 24 ) & 0xFF;
      val[1] = (floatval >> 16 ) & 0xFF;
      val[2] = (floatval >> 8 ) & 0xFF;
      val[3] =  floatval & 0xFF;
      ADAU1452_WRITE_BLOCK( addr, val, 4 );  // B1
      addr++;

      floatval = convertTo824(b[ 3*ii + 0 ]);
      val[0] = (floatval >> 24 ) & 0xFF;
      val[1] = (floatval >> 16 ) & 0xFF;
      val[2] = (floatval >> 8 ) & 0xFF;
      val[3] =  floatval & 0xFF;
      ADAU1452_WRITE_BLOCK( addr, val, 4 );  // B0
      addr++;

      floatval = convertTo824(a[ 3*ii + 2 ]);
      val[0] = (floatval >> 24 ) & 0xFF;
      val[1] = (floatval >> 16 ) & 0xFF;
      val[2] = (floatval >> 8 ) & 0xFF;
      val[3] =  floatval & 0xFF;
      ADAU1452_WRITE_BLOCK( addr, val, 4 );  // A2
      addr++;

      floatval = convertTo824(a[ 3*ii + 1 ]);
      val[0] = (floatval >> 24 ) & 0xFF;
      val[1] = (floatval >> 16 ) & 0xFF;
      val[2] = (floatval >> 8 ) & 0xFF;
      val[3] =  floatval & 0xFF;
      ADAU1452_WRITE_BLOCK( addr, val, 4 );  // A1
      addr++;
    }
  }

}

//==============================================================================
/*! Sets the values for a delay block on DSP.
 *
 */
void setDelay( int idx )
{
  if( (paramDelay[idx].delay >= 0) && (paramDelay[idx].delay <= 100) )
  {
    float dly = paramDelay[idx].delay/1000.0 * sampleRate;
    int32_t idly = static_cast<int32_t>(dly + 0.5);
    if( paramDelay[idx].bypass )
      idly = 0;

    uint16_t addr = paramDelay[idx].addr;
    byte val[4];
    val[0] = (idly >> 24 ) & 0xFF;
    val[1] = (idly >> 16 ) & 0xFF;
    val[2] = (idly >> 8 ) & 0xFF;
    val[3] =  idly & 0xFF;
    ADAU1452_WRITE_BLOCK( addr, val, 4 );
  }
}

//==============================================================================
/*! Sets the ir for a fir block on DSP.
 *
 */
void setFir( int idx )
{
  byte val[4];
  uint32_t floatval;

  for( uint16_t kk = 0; kk < MAX_LENGTH_IR; kk++ )
  {
    uint16_t addr = paramFir[idx].addr + kk;
    if( paramFir[idx].bypass )
    {
      if( kk == 0 )
        floatval = convertTo824( 1.0 );
      else
        floatval = convertTo824( 0.0 );
    }
    else
      floatval = convertTo824( paramFir[idx].ir[kk] );

    val[0] = (floatval >> 24 ) & 0xFF;
    val[1] = (floatval >> 16 ) & 0xFF;
    val[2] = (floatval >> 8 ) & 0xFF;
    val[3] =  floatval & 0xFF;
    ADAU1452_WRITE_BLOCK( addr, val, 4 );
  }
}

//==============================================================================
/*! Sets the values for a gain block on DSP.
 *
 */
void setGain( int idx )
{
  if( (paramGain[idx].gain >= -120.0) && (paramGain[idx].gain <= 0.0) )
  {
    uint32_t float824val;
    if( paramGain[idx].mute )
      float824val = convertTo824( 0.0 );
    else
      float824val = convertTo824( pow( 10.0, paramGain[idx].gain / 20.0 ) );

    byte val[4];
    val[0] = (float824val >> 24 ) & 0xFF;
    val[1] = (float824val >> 16 ) & 0xFF;
    val[2] = (float824val >> 8 ) & 0xFF;
    val[3] = float824val & 0xFF;
    ADAU1452_WRITE_BLOCK( paramGain[idx].addr, val, 4 );
  }
}

//==============================================================================
/*! Sets the values for a highpass block on DSP.
 *
 */
void setHighPass( int idx )
{
  if( (paramHP[idx].fc > 0.0) && (paramHP[idx].fc < 20000.0)
      && (paramHP[idx].typ >= 0) && (paramHP[idx].typ < AudioFilterFactory::kNumFilterDesigns) )
  {
    float a[12] = { 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0 };
    float b[12] = { 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0 };
    byte val[4];
    uint32_t floatval;
    if( !(paramHP[idx].bypass) )
      AudioFilterFactory::makeHighPass( a, b, paramHP[idx].typ, paramHP[idx].fc, sampleRate );

    for( int ii = 0; ii < 4; ii++ )
    {
      uint16_t addr = paramHP[idx].addr[ii];
      floatval = convertTo824(b[ 3*ii + 2 ]);
      val[0] = (floatval >> 24 ) & 0xFF;
      val[1] = (floatval >> 16 ) & 0xFF;
      val[2] = (floatval >> 8 ) & 0xFF;
      val[3] =  floatval & 0xFF;
      ADAU1452_WRITE_BLOCK( addr, val, 4 );  // B2
      addr++;

      floatval = convertTo824(b[ 3*ii + 1 ]);
      val[0] = (floatval >> 24 ) & 0xFF;
      val[1] = (floatval >> 16 ) & 0xFF;
      val[2] = (floatval >> 8 ) & 0xFF;
      val[3] =  floatval & 0xFF;
      ADAU1452_WRITE_BLOCK( addr, val, 4 );  // B1
      addr++;

      floatval = convertTo824(b[ 3*ii + 0 ]);
      val[0] = (floatval >> 24 ) & 0xFF;
      val[1] = (floatval >> 16 ) & 0xFF;
      val[2] = (floatval >> 8 ) & 0xFF;
      val[3] =  floatval & 0xFF;
      ADAU1452_WRITE_BLOCK( addr, val, 4 );  // B0
      addr++;

      floatval = convertTo824(a[ 3*ii + 2 ]);
      val[0] = (floatval >> 24 ) & 0xFF;
      val[1] = (floatval >> 16 ) & 0xFF;
      val[2] = (floatval >> 8 ) & 0xFF;
      val[3] =  floatval & 0xFF;
      ADAU1452_WRITE_BLOCK( addr, val, 4 );  // A2
      addr++;

      floatval = convertTo824(a[ 3*ii + 1 ]);
      val[0] = (floatval >> 24 ) & 0xFF;
      val[1] = (floatval >> 16 ) & 0xFF;
      val[2] = (floatval >> 8 ) & 0xFF;
      val[3] =  floatval & 0xFF;
      ADAU1452_WRITE_BLOCK( addr, val, 4 );  // A1
      addr++;
    }
  }
}

//==============================================================================
/*! Sets the values for a high shelving block on DSP.
 *
 */
void setHighShelving( int idx )
{
  if( (paramHshelv[idx].fc > 0.0) && (paramHshelv[idx].fc < 20000.0)
      && (paramHshelv[idx].gain >= -24.0) && (paramHshelv[idx].gain <= 24.0)
      && (paramHshelv[idx].slope >= 0.1) && (paramHshelv[idx].slope <= 2.0) )
  {
    float a[3] = { 1.0, 0.0, 0.0 };
    float b[3] = { 1.0, 0.0, 0.0 };
    byte val[4];
    uint32_t floatval;
    if( !(paramHshelv[idx].bypass) )
      AudioFilterFactory::makeHighShelv( a, b, paramHshelv[idx].gain, paramHshelv[idx].fc, paramHshelv[idx].slope, sampleRate );

    uint16_t addr = paramHshelv[idx].addr;
    floatval = convertTo824(b[2]);
    val[0] = (floatval >> 24 ) & 0xFF;
    val[1] = (floatval >> 16 ) & 0xFF;
    val[2] = (floatval >> 8 ) & 0xFF;
    val[3] =  floatval & 0xFF;
    ADAU1452_WRITE_BLOCK( addr, val, 4 );  // B2
    addr++;

    floatval = convertTo824(b[1]);
    val[0] = (floatval >> 24 ) & 0xFF;
    val[1] = (floatval >> 16 ) & 0xFF;
    val[2] = (floatval >> 8 ) & 0xFF;
    val[3] =  floatval & 0xFF;
    ADAU1452_WRITE_BLOCK( addr, val, 4 );  // B1
    addr++;

    floatval = convertTo824(b[0]);
    val[0] = (floatval >> 24 ) & 0xFF;
    val[1] = (floatval >> 16 ) & 0xFF;
    val[2] = (floatval >> 8 ) & 0xFF;
    val[3] =  floatval & 0xFF;
    ADAU1452_WRITE_BLOCK( addr, val, 4 );  // B0
    addr++;

    floatval = convertTo824(a[2]);
    val[0] = (floatval >> 24 ) & 0xFF;
    val[1] = (floatval >> 16 ) & 0xFF;
    val[2] = (floatval >> 8 ) & 0xFF;
    val[3] =  floatval & 0xFF;
    ADAU1452_WRITE_BLOCK( addr, val, 4 );  // A2
    addr++;

    floatval = convertTo824(a[1]);
    val[0] = (floatval >> 24 ) & 0xFF;
    val[1] = (floatval >> 16 ) & 0xFF;
    val[2] = (floatval >> 8 ) & 0xFF;
    val[3] =  floatval & 0xFF;
    ADAU1452_WRITE_BLOCK( addr, val, 4 );  // A1
  }
}


//==============================================================================
/*! Sets a new input selection on DSP.
 *
 */
void setInput( const int idx )
{
  uint32_t sel = (paramInputs[idx].sel >> 16) & 0x0000ffff;
  uint16_t addrChn = paramInputs[idx].addrChn[sel];
  uint16_t addrPort = paramInputs[idx].addrPort;
  sel = paramInputs[idx].sel;

  byte val[4];
  uint32_t intval = sel & 0x0000ffff;
  val[0] = (intval >> 24 ) & 0xFF;
  val[1] = (intval >> 16 ) & 0xFF;
  val[2] = (intval >> 8 ) & 0xFF;
  val[3] =  intval & 0xFF;
  ADAU1452_WRITE_BLOCK( addrChn, val, 4 );

  intval = (sel >> 16) & 0x0000ffff;
  val[0] = (intval >> 24 ) & 0xFF;
  val[1] = (intval >> 16 ) & 0xFF;
  val[2] = (intval >> 8 ) & 0xFF;
  val[3] =  intval & 0xFF;
  ADAU1452_WRITE_BLOCK( addrPort, val, 4 );
}

//==============================================================================
/*! Sets the values for a low pass block on DSP.
 *
 */
void setLowPass( int idx )
{
  if( (paramLP[idx].fc > 0.0) && (paramLP[idx].fc < 20000.0)
      && (paramLP[idx].typ >= 0) && (paramLP[idx].typ < AudioFilterFactory::kNumFilterDesigns) )
  {
    float a[12] = { 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0 };
    float b[12] = { 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0 };
    byte val[4];
    uint32_t floatval;
    if( !(paramLP[idx].bypass) )
      AudioFilterFactory::makeLowPass( a, b, paramLP[idx].typ, paramLP[idx].fc, sampleRate );

    for( int ii = 0; ii < 4; ii++ )
    {
      uint16_t addr = paramLP[idx].addr[ii];
      floatval = convertTo824(b[ 3*ii + 2 ]);
      val[0] = (floatval >> 24 ) & 0xFF;
      val[1] = (floatval >> 16 ) & 0xFF;
      val[2] = (floatval >> 8 ) & 0xFF;
      val[3] =  floatval & 0xFF;
      ADAU1452_WRITE_BLOCK( addr, val, 4 );  // B2
      addr++;

      floatval = convertTo824(b[ 3*ii + 1 ]);
      val[0] = (floatval >> 24 ) & 0xFF;
      val[1] = (floatval >> 16 ) & 0xFF;
      val[2] = (floatval >> 8 ) & 0xFF;
      val[3] =  floatval & 0xFF;
      ADAU1452_WRITE_BLOCK( addr, val, 4 );  // B1
      addr++;

      floatval = convertTo824(b[ 3*ii + 0 ]);
      val[0] = (floatval >> 24 ) & 0xFF;
      val[1] = (floatval >> 16 ) & 0xFF;
      val[2] = (floatval >> 8 ) & 0xFF;
      val[3] =  floatval & 0xFF;
      ADAU1452_WRITE_BLOCK( addr, val, 4 );  // B0
      addr++;

      floatval = convertTo824(a[ 3*ii + 2 ]);
      val[0] = (floatval >> 24 ) & 0xFF;
      val[1] = (floatval >> 16 ) & 0xFF;
      val[2] = (floatval >> 8 ) & 0xFF;
      val[3] =  floatval & 0xFF;
      ADAU1452_WRITE_BLOCK( addr, val, 4 );  // A2
      addr++;

      floatval = convertTo824(a[ 3*ii + 1 ]);
      val[0] = (floatval >> 24 ) & 0xFF;
      val[1] = (floatval >> 16 ) & 0xFF;
      val[2] = (floatval >> 8 ) & 0xFF;
      val[3] =  floatval & 0xFF;
      ADAU1452_WRITE_BLOCK( addr, val, 4 );  // A1
      addr++;
    }
  }
}

//==============================================================================
/*! Sets the values for a low shelving block on DSP.
 *
 */
void setLowShelving( int idx )
{
  if( (paramLshelv[idx].fc > 0) && (paramLshelv[idx].fc < 20000)
      && (paramLshelv[idx].gain >= -24.0) && (paramLshelv[idx].gain <= 24.0)
      && (paramLshelv[idx].slope >= 0.1) && (paramLshelv[idx].slope <= 2.0) )
  {
    float a[3] = { 1.0, 0.0, 0.0 };
    float b[3] = { 1.0, 0.0, 0.0 };
    byte val[4];
    uint32_t floatval;
    if( !(paramLshelv[idx].bypass) )
      AudioFilterFactory::makeLowShelv( a, b, paramLshelv[idx].gain, paramLshelv[idx].fc, paramLshelv[idx].slope, sampleRate );

    uint16_t addr = paramLshelv[idx].addr;
    floatval = convertTo824(b[2]);
    val[0] = (floatval >> 24 ) & 0xFF;
    val[1] = (floatval >> 16 ) & 0xFF;
    val[2] = (floatval >> 8 ) & 0xFF;
    val[3] =  floatval & 0xFF;
    ADAU1452_WRITE_BLOCK( addr, val, 4 );  // B2
    addr++;

    floatval = convertTo824(b[1]);
    val[0] = (floatval >> 24 ) & 0xFF;
    val[1] = (floatval >> 16 ) & 0xFF;
    val[2] = (floatval >> 8 ) & 0xFF;
    val[3] =  floatval & 0xFF;
    ADAU1452_WRITE_BLOCK( addr, val, 4 );  // B1
    addr++;

    floatval = convertTo824(b[0]);
    val[0] = (floatval >> 24 ) & 0xFF;
    val[1] = (floatval >> 16 ) & 0xFF;
    val[2] = (floatval >> 8 ) & 0xFF;
    val[3] =  floatval & 0xFF;
    ADAU1452_WRITE_BLOCK( addr, val, 4 );  // B0
    addr++;

    floatval = convertTo824(a[2]);
    val[0] = (floatval >> 24 ) & 0xFF;
    val[1] = (floatval >> 16 ) & 0xFF;
    val[2] = (floatval >> 8 ) & 0xFF;
    val[3] =  floatval & 0xFF;
    ADAU1452_WRITE_BLOCK( addr, val, 4 );  // A2
    addr++;

    floatval = convertTo824(a[1]);
    val[0] = (floatval >> 24 ) & 0xFF;
    val[1] = (floatval >> 16 ) & 0xFF;
    val[2] = (floatval >> 8 ) & 0xFF;
    val[3] =  floatval & 0xFF;
    ADAU1452_WRITE_BLOCK( addr, val, 4 );  // A1
  }
}

//==============================================================================
/*! Sets the value for master volume on DSP.
 *
 */
void setMasterVolume( void )
{
  if( masterVolume.addr != 0x0000 )
  {
    if( (masterVolume.val >= -120.0) && (masterVolume.val <= 0.0) )
    {
      uint16_t reg = masterVolume.addr;
      uint32_t rxval = convertTo824( pow( 10.0, masterVolume.val / 20.0 ) );

      byte val[4];
      val[0] = (rxval >> 24 ) & 0xFF;
      val[1] = (rxval >> 16 ) & 0xFF;
      val[2] = (rxval >> 8 ) & 0xFF;
      val[3] = rxval & 0xFF;
      ADAU1452_WRITE_BLOCK( reg, val, 4 );

    }
  }
}

//==============================================================================
/*! Sets the values for a peq block on DSP.
 *
 */
void setPEQ( int idx )
{
  if( (paramPeq[idx].fc > 0.0) && (paramPeq[idx].fc < 20000.0)
    && (paramPeq[idx].gain >= -24.0) && (paramPeq[idx].gain <= 24.0)
    && (paramPeq[idx].Q >= 0.1) && (paramPeq[idx].Q <= 100.0) )
  {
    float a[3] = { 1.0, 0.0, 0.0 };
    float b[3] = { 1.0, 0.0, 0.0 };
    byte val[4];
    uint32_t floatval;
    if( !(paramPeq[idx].bypass) )
      AudioFilterFactory::makeParametricEQ( a, b, paramPeq[idx].gain, paramPeq[idx].fc, paramPeq[idx].Q, sampleRate );

    uint32_t addr = paramPeq[idx].addr;
    floatval = convertTo824(b[2]);
    val[0] = (floatval >> 24 ) & 0xFF;
    val[1] = (floatval >> 16 ) & 0xFF;
    val[2] = (floatval >> 8 ) & 0xFF;
    val[3] =  floatval & 0xFF;
    ADAU1452_WRITE_BLOCK( addr, val, 4 );  // B2
    addr++;

    floatval = convertTo824(b[1]);
    val[0] = (floatval >> 24 ) & 0xFF;
    val[1] = (floatval >> 16 ) & 0xFF;
    val[2] = (floatval >> 8 ) & 0xFF;
    val[3] =  floatval & 0xFF;
    ADAU1452_WRITE_BLOCK( addr, val, 4 );  // B1
    addr++;

    floatval = convertTo824(b[0]);
    val[0] = (floatval >> 24 ) & 0xFF;
    val[1] = (floatval >> 16 ) & 0xFF;
    val[2] = (floatval >> 8 ) & 0xFF;
    val[3] =  floatval & 0xFF;
    ADAU1452_WRITE_BLOCK( addr, val, 4 );  // B0
    addr++;

    floatval = convertTo824(a[2]);
    val[0] = (floatval >> 24 ) & 0xFF;
    val[1] = (floatval >> 16 ) & 0xFF;
    val[2] = (floatval >> 8 ) & 0xFF;
    val[3] =  floatval & 0xFF;
    ADAU1452_WRITE_BLOCK( addr, val, 4 );  // A2
    addr++;

    floatval = convertTo824(a[1]);
    val[0] = (floatval >> 24 ) & 0xFF;
    val[1] = (floatval >> 16 ) & 0xFF;
    val[2] = (floatval >> 8 ) & 0xFF;
    val[3] =  floatval & 0xFF;
    ADAU1452_WRITE_BLOCK( addr, val, 4 );  // A1
  }
}


//==============================================================================
/*! Sets the values for a phase block on DSP.
 *
 */
void setPhase( int idx )
{
  if( (paramPhase[idx].fc > 0.0) && (paramPhase[idx].fc < 20000.0)
    && (paramPhase[idx].Q >= 0.1) && (paramPhase[idx].Q <= 100.0) )
  {
    float a[3] = { 1.0, 0.0, 0.0 };
    float b[3] = { 1.0, 0.0, 0.0 };
    byte val[4];
    uint32_t floatval;
    if( !paramPhase[idx].bypass )
      AudioFilterFactory::makeAllpass( a, b, paramPhase[idx].fc, paramPhase[idx].Q, sampleRate );

    if( paramPhase[idx].inv == true )
    {
      Serial.println("Inverting");
      b[0] *= -1.0;
      b[1] *= -1.0;
      b[2] *= -1.0;
    }

    uint16_t addr = paramPhase[idx].addr;
    floatval = convertTo824(b[2]);
    val[0] = (floatval >> 24 ) & 0xFF;
    val[1] = (floatval >> 16 ) & 0xFF;
    val[2] = (floatval >> 8 ) & 0xFF;
    val[3] =  floatval & 0xFF;
    ADAU1452_WRITE_BLOCK( addr, val, 4 );  // B2
    addr++;

    floatval = convertTo824(b[1]);
    val[0] = (floatval >> 24 ) & 0xFF;
    val[1] = (floatval >> 16 ) & 0xFF;
    val[2] = (floatval >> 8 ) & 0xFF;
    val[3] =  floatval & 0xFF;
    ADAU1452_WRITE_BLOCK( addr, val, 4 );  // B1
    addr++;

    floatval = convertTo824(b[0]);
    val[0] = (floatval >> 24 ) & 0xFF;
    val[1] = (floatval >> 16 ) & 0xFF;
    val[2] = (floatval >> 8 ) & 0xFF;
    val[3] =  floatval & 0xFF;
    ADAU1452_WRITE_BLOCK( addr, val, 4 );  // B0
    addr++;

    floatval = convertTo824(a[2]);
    val[0] = (floatval >> 24 ) & 0xFF;
    val[1] = (floatval >> 16 ) & 0xFF;
    val[2] = (floatval >> 8 ) & 0xFF;
    val[3] =  floatval & 0xFF;
    ADAU1452_WRITE_BLOCK( addr, val, 4 );  // A2
    addr++;

    floatval = convertTo824(a[1]);
    val[0] = (floatval >> 24 ) & 0xFF;
    val[1] = (floatval >> 16 ) & 0xFF;
    val[2] = (floatval >> 8 ) & 0xFF;
    val[3] =  floatval & 0xFF;
    ADAU1452_WRITE_BLOCK( addr, val, 4 );  // A1
  }
}

//==============================================================================
/*! Changes the routing for the SPDIF output
 */
void setSpdifOutputRouting( void )
{
  uint32_t sel = (spdifOutput.selectionLeft >> 16) & 0x0000ffff;
  uint16_t addrChn = spdifOutputSelector.addrChnLeft[sel];
  uint16_t addrPort = spdifOutputSelector.addrPortLeft;
  sel = spdifOutput.selectionLeft;

  byte val[4];
  uint32_t intval = sel & 0x0000ffff;
  val[0] = (intval >> 24 ) & 0xFF;
  val[1] = (intval >> 16 ) & 0xFF;
  val[2] = (intval >> 8 ) & 0xFF;
  val[3] =  intval & 0xFF;
  ADAU1452_WRITE_BLOCK( addrChn, val, 4 );

  intval = (sel >> 16) & 0x0000ffff;
  val[0] = (intval >> 24 ) & 0xFF;
  val[1] = (intval >> 16 ) & 0xFF;
  val[2] = (intval >> 8 ) & 0xFF;
  val[3] =  intval & 0xFF;
  ADAU1452_WRITE_BLOCK( addrPort, val, 4 );

  sel = (spdifOutput.selectionRight >> 16) & 0x0000ffff;
  addrChn = spdifOutputSelector.addrChnRight[sel];
  addrPort = spdifOutputSelector.addrPortRight;
  sel = spdifOutput.selectionRight;

  intval = sel & 0x0000ffff;
  val[0] = (intval >> 24 ) & 0xFF;
  val[1] = (intval >> 16 ) & 0xFF;
  val[2] = (intval >> 8 ) & 0xFF;
  val[3] =  intval & 0xFF;
  ADAU1452_WRITE_BLOCK( addrChn, val, 4 );

  intval = (sel >> 16) & 0x0000ffff;
  val[0] = (intval >> 24 ) & 0xFF;
  val[1] = (intval >> 16 ) & 0xFF;
  val[2] = (intval >> 8 ) & 0xFF;
  val[3] =  intval & 0xFF;
  ADAU1452_WRITE_BLOCK( addrPort, val, 4 );

}

