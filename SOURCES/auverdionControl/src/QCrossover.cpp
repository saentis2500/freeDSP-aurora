#include <QDebug>

#include "QCrossover.hpp"
#include "ui_QCrossover.h"

#include "vektorraum.h"

using namespace Vektorraum;

QCrossover::QCrossover( tfilterdesign design, tfloat fc,
                      uint16_t addrB2_1, uint16_t addrB1_1, uint16_t addrB0_1,
                      uint16_t addrA2_1, uint16_t addrA1_1,
                      uint16_t addrB2_2, uint16_t addrB1_2, uint16_t addrB0_2,
                      uint16_t addrA2_2, uint16_t addrA1_2,
                      uint16_t addrB2_3, uint16_t addrB1_3, uint16_t addrB0_3,
                      uint16_t addrA2_3, uint16_t addrA1_3,
                      uint16_t addrB2_4, uint16_t addrB1_4, uint16_t addrB0_4,
                      uint16_t addrA2_4, uint16_t addrA1_4,
                      tfloat samplerate,
                      CFreeDspAurora* ptrdsp,
                      QWidget *parent ) :
  QDspBlock(parent), ui(new Ui::QCrossover)
{
  type = CROSSOVER;
  
  tfilterdesign designLp = kLinkwitzRiley24;

  #if defined( __MACOSX__ )
  setFixedSize( QSize( 200, 185 ) );
  #elif defined( __WIN__ )
  setFixedSize( QSize( 200, 165 ) );
  #endif

  addr[kParamB2_1] = addrB2_1;
  addr[kParamB1_1] = addrB1_1;
  addr[kParamB0_1] = addrB0_1;
  addr[kParamA2_1] = addrA2_1;
  addr[kParamA1_1] = addrA1_1;
  addr[kParamB2_2] = addrB2_2;
  addr[kParamB1_2] = addrB1_2;
  addr[kParamB0_2] = addrB0_2;
  addr[kParamA2_2] = addrA2_2;
  addr[kParamA1_2] = addrA1_2;
  addr[kParamB2_3] = addrB2_3;
  addr[kParamB1_3] = addrB1_3;
  addr[kParamB0_3] = addrB0_3;
  addr[kParamA2_3] = addrA2_3;
  addr[kParamA1_3] = addrA1_3;
  addr[kParamB2_4] = addrB2_4;
  addr[kParamB1_4] = addrB1_4;
  addr[kParamB0_4] = addrB0_4;
  addr[kParamA2_4] = addrA2_4;
  addr[kParamA1_4] = addrA1_4;
  fs = samplerate;
  dsp = ptrdsp;

  ui->setupUi(this);
  ui->comboBoxTypeHp->blockSignals( true );
  ui->comboBoxTypeHp->addItem( "Bes 6dB",  kBessel6 );
  ui->comboBoxTypeHp->addItem( "Bes 12dB", kBessel12 );
  ui->comboBoxTypeHp->addItem( "Bes 18dB", kBessel18 );
  ui->comboBoxTypeHp->addItem( "Bes 24dB", kBessel24 );
  ui->comboBoxTypeHp->addItem( "But 6dB",  kButterworth6 );
  ui->comboBoxTypeHp->addItem( "But 12dB", kButterworth12 );
  ui->comboBoxTypeHp->addItem( "But 18dB", kButterworth18 );
  ui->comboBoxTypeHp->addItem( "But 24dB", kButterworth24 );
  ui->comboBoxTypeHp->addItem( "LR 12dB",  kLinkwitzRiley12 );
  ui->comboBoxTypeHp->addItem( "LR 24dB",  kLinkwitzRiley24 );
  ui->comboBoxTypeHp->addItem( "LR 36dB",  kLinkwitzRiley36 );
  ui->comboBoxTypeHp->addItem( "LR 48dB",  kLinkwitzRiley48 );
  ui->comboBoxTypeHp->setCurrentIndex( design );
  ui->comboBoxTypeHp->blockSignals( false );
  ui->doubleSpinBoxFcHp->blockSignals( true );
  ui->doubleSpinBoxFcHp->setAttribute( Qt::WA_MacShowFocusRect, 0 );
  ui->doubleSpinBoxFcHp->setValue( fc );
  ui->doubleSpinBoxFcHp->blockSignals( false );

  ui->comboBoxTypeLp->blockSignals( true );
  ui->comboBoxTypeLp->addItem( "Bes 6dB",  kBessel6 );
  ui->comboBoxTypeLp->addItem( "Bes 12dB", kBessel12 );
  ui->comboBoxTypeLp->addItem( "Bes 18dB", kBessel18 );
  ui->comboBoxTypeLp->addItem( "Bes 24dB", kBessel24 );
  ui->comboBoxTypeLp->addItem( "But 6dB",  kButterworth6 );
  ui->comboBoxTypeLp->addItem( "But 12dB", kButterworth12 );
  ui->comboBoxTypeLp->addItem( "But 18dB", kButterworth18 );
  ui->comboBoxTypeLp->addItem( "But 24dB", kButterworth24 );
  ui->comboBoxTypeLp->addItem( "LR 12dB",  kLinkwitzRiley12 );
  ui->comboBoxTypeLp->addItem( "LR 24dB",  kLinkwitzRiley24 );
  ui->comboBoxTypeLp->addItem( "LR 36dB",  kLinkwitzRiley36 );
  ui->comboBoxTypeLp->addItem( "LR 48dB",  kLinkwitzRiley48 );
  ui->comboBoxTypeLp->setCurrentIndex( designLp );
  ui->comboBoxTypeLp->blockSignals( false );
  ui->doubleSpinBoxFcLp->blockSignals( true );
  ui->doubleSpinBoxFcLp->setAttribute( Qt::WA_MacShowFocusRect, 0 );
  ui->doubleSpinBoxFcLp->setValue( fc );
  ui->doubleSpinBoxFcLp->blockSignals( false );
  ui->doubleSpinBoxGainLp->blockSignals( true );
  ui->doubleSpinBoxGainLp->setAttribute( Qt::WA_MacShowFocusRect, 0 );
  ui->doubleSpinBoxGainLp->setValue( -10.0 );
  ui->doubleSpinBoxGainLp->blockSignals( false );

  //bypass = isbypassed;
  //ui->pushButtonBypass->setChecked( bypass );
}

QCrossover::~QCrossover()
{
  delete ui;
}

//------------------------------------------------------------------------------
/*! \brief Updates the filter.
 *
 */
void QCrossover::update( tvector<tfloat> f )
{
  #if !defined( __WIN__ )
  #warning QCrossover::update not fully implemented
  #endif
  qDebug()<<"QCrossover::update not fully implemented";

  updateCoeffs();
  tvector<tcomplex> z = exp( j * ( -2.0 * pi * f / fs ) );
  tvector<tcomplex> z2 = z*z;
  tfloat b0 = coeffs[kB0];
  tfloat b1 = coeffs[kB1];
  tfloat b2 = coeffs[kB2];
  tfloat a1 = coeffs[kA1];
  tfloat a2 = coeffs[kA2];
  tfloat a0 = 1.0;
  H = ( b0 + b1*z + b2*z2 ) / ( a0 - a1*z - a2*z2 );

  for( tuint ii = 1; ii < 4; ii++ )
  {
    b0 = coeffs[ii*5+kB0];
    b1 = coeffs[ii*5+kB1];
    b2 = coeffs[ii*5+kB2];
    a1 = coeffs[ii*5+kA1];
    a2 = coeffs[ii*5+kA2];
    a0 = 1.0;

    H = H * ( b0 + b1*z + b2*z2 ) / ( a0 - a1*z - a2*z2 );
  }
}

//------------------------------------------------------------------------------
/*!
 *
 */
void QCrossover::updateCoeffs( void )
{
  #if !defined( __WIN__ )
  #warning Add LP coeffs
  #endif

  for( tuint ii = 0; ii < 4; ii++ )
  {
    Q[ii] = -1.0;
    fc[ii] = -1.0;
  }
  filterDesign = static_cast<tfilterdesign>(ui->comboBoxTypeHp->currentData().toInt());
  

  for( tuint ii = 0; ii < 4; ii++ )
  {
    tfloat b0 = 1.0;
    tfloat b1 = 0.0;
    tfloat b2 = 0.0;
    tfloat a1 = 0.0;
    tfloat a2 = 0.0;
    coeffs[ii*5+kB0] = b0;
    coeffs[ii*5+kB1] = b1;
    coeffs[ii*5+kB2] = b2;
    coeffs[ii*5+kA1] = a1;
    coeffs[ii*5+kA2] = a2;
  }

  if( bypass )
  {
    for( tuint ii = 0; ii < 4; ii++ )
    {
      tfloat b0 = 1.0;
      tfloat b1 = 0.0;
      tfloat b2 = 0.0;
      tfloat a1 = 0.0;
      tfloat a2 = 0.0;
      coeffs[ii*5+kB0] = b0;
      coeffs[ii*5+kB1] = b1;
      coeffs[ii*5+kB2] = b2;
      coeffs[ii*5+kA1] = a1;
      coeffs[ii*5+kA2] = a2;
    }
  }
  else
  {
    tfloat w0, alpha;
    tfloat a0, a1, a2;
    tfloat b0, b1, b2;

    tfloat sa[3];
    tfloat sb[3];
    tfloat za[3];
    tfloat zb[3];
    tfloat Omega, T, T2, wn;

    switch( filterDesign )
    {
    case kLinkwitzRiley12:
      Q[0] = 0.5;
      fc[0] = ui->doubleSpinBoxFcHp->value();

      w0 = 2.0 * pi * fc[0] / fs;
      alpha = sin(w0) / (2.0 * Q[0]);
      a0 = 1.0 + alpha;
      b0 = ( ( 1.0 + cos(w0) ) * 0.5 ) / a0;
      b1 = ( -( 1.0 + cos(w0) ) ) / a0;
      b2 = ( ( 1.0 + cos(w0) ) * 0.5 ) / a0;
      a1 = ( -2.0 * cos(w0) ) / a0;
      a2 = ( 1.0 - alpha ) / a0;
      a0 = 1.0;
      coeffs[kB0] = b0;
      coeffs[kB1] = b1;
      coeffs[kB2] = b2;
      coeffs[kA1] = (-1.0)*a1;
      coeffs[kA2] = (-1.0)*a2;

      break;

    case kLinkwitzRiley24:
      Q[0] = 0.71;
      Q[1] = 0.71;
      fc[0] = ui->doubleSpinBoxFcHp->value();
      fc[1] = ui->doubleSpinBoxFcHp->value();

      w0 = 2.0 * pi * fc[0] / fs;
      alpha = sin(w0) / (2.0 * Q[0]);
      a0 = 1.0 + alpha;
      b0 = ( ( 1.0 + cos(w0) ) * 0.5 ) / a0;
      b1 = ( -( 1.0 + cos(w0) ) ) / a0;
      b2 = ( ( 1.0 + cos(w0) ) * 0.5 ) / a0;
      a1 = ( -2.0 * cos(w0) ) / a0;
      a2 = ( 1.0 - alpha ) / a0;
      a0 = 1.0;
      coeffs[kB0] = b0;
      coeffs[kB1] = b1;
      coeffs[kB2] = b2;
      coeffs[kA1] = (-1.0)*a1;
      coeffs[kA2] = (-1.0)*a2;

      w0 = 2.0 * pi * fc[1] / fs;
      alpha = sin(w0) / (2.0 * Q[1]);
      a0 = 1.0 + alpha;
      b0 = ( ( 1.0 + cos(w0) ) * 0.5 ) / a0;
      b1 = ( -( 1.0 + cos(w0) ) ) / a0;
      b2 = ( ( 1.0 + cos(w0) ) * 0.5 ) / a0;
      a1 = ( -2.0 * cos(w0) ) / a0;
      a2 = ( 1.0 - alpha ) / a0;
      a0 = 1.0;
      coeffs[5+kB0] = b0;
      coeffs[5+kB1] = b1;
      coeffs[5+kB2] = b2;
      coeffs[5+kA1] = (-1.0)*a1;
      coeffs[5+kA2] = (-1.0)*a2;

      break;

    case kLinkwitzRiley36:
      Q[0] = 0.50;
      Q[1] = 1.00;
      Q[2] = 1.00;
      fc[0] = ui->doubleSpinBoxFcHp->value();
      fc[1] = ui->doubleSpinBoxFcHp->value();
      fc[2] = ui->doubleSpinBoxFcHp->value();

      w0 = 2.0 * pi * fc[0] / fs;
      alpha = sin(w0) / (2.0 * Q[0]);
      a0 = 1.0 + alpha;
      b0 = ( ( 1.0 + cos(w0) ) * 0.5 ) / a0;
      b1 = ( -( 1.0 + cos(w0) ) ) / a0;
      b2 = ( ( 1.0 + cos(w0) ) * 0.5 ) / a0;
      a1 = ( -2.0 * cos(w0) ) / a0;
      a2 = ( 1.0 - alpha ) / a0;
      a0 = 1.0;
      coeffs[kB0] = b0;
      coeffs[kB1] = b1;
      coeffs[kB2] = b2;
      coeffs[kA1] = (-1.0)*a1;
      coeffs[kA2] = (-1.0)*a2;

      w0 = 2.0 * pi * fc[1] / fs;
      alpha = sin(w0) / (2.0 * Q[1]);
      a0 = 1.0 + alpha;
      b0 = ( ( 1.0 + cos(w0) ) * 0.5 ) / a0;
      b1 = ( -( 1.0 + cos(w0) ) ) / a0;
      b2 = ( ( 1.0 + cos(w0) ) * 0.5 ) / a0;
      a1 = ( -2.0 * cos(w0) ) / a0;
      a2 = ( 1.0 - alpha ) / a0;
      a0 = 1.0;
      coeffs[5+kB0] = b0;
      coeffs[5+kB1] = b1;
      coeffs[5+kB2] = b2;
      coeffs[5+kA1] = (-1.0)*a1;
      coeffs[5+kA2] = (-1.0)*a2;

      w0 = 2.0 * pi * fc[2] / fs;
      alpha = sin(w0) / (2.0 * Q[2]);
      a0 = 1.0 + alpha;
      b0 = ( ( 1.0 + cos(w0) ) * 0.5 ) / a0;
      b1 = ( -( 1.0 + cos(w0) ) ) / a0;
      b2 = ( ( 1.0 + cos(w0) ) * 0.5 ) / a0;
      a1 = ( -2.0 * cos(w0) ) / a0;
      a2 = ( 1.0 - alpha ) / a0;
      a0 = 1.0;
      coeffs[10+kB0] = b0;
      coeffs[10+kB1] = b1;
      coeffs[10+kB2] = b2;
      coeffs[10+kA1] = (-1.0)*a1;
      coeffs[10+kA2] = (-1.0)*a2;

      break;

    case kLinkwitzRiley48:
      Q[0] = 0.54;
      Q[1] = 1.34;
      Q[2] = 0.54;
      Q[3] = 1.34;
      fc[0] = ui->doubleSpinBoxFcHp->value();
      fc[1] = ui->doubleSpinBoxFcHp->value();
      fc[2] = ui->doubleSpinBoxFcHp->value();
      fc[3] = ui->doubleSpinBoxFcHp->value();

      w0 = 2.0 * pi * fc[0] / fs;
      alpha = sin(w0) / (2.0 * Q[0]);
      a0 = 1.0 + alpha;
      b0 = ( ( 1.0 + cos(w0) ) * 0.5 ) / a0;
      b1 = ( -( 1.0 + cos(w0) ) ) / a0;
      b2 = ( ( 1.0 + cos(w0) ) * 0.5 ) / a0;
      a1 = ( -2.0 * cos(w0) ) / a0;
      a2 = ( 1.0 - alpha ) / a0;
      a0 = 1.0;
      coeffs[kB0] = b0;
      coeffs[kB1] = b1;
      coeffs[kB2] = b2;
      coeffs[kA1] = (-1.0)*a1;
      coeffs[kA2] = (-1.0)*a2;

      w0 = 2.0 * pi * fc[1] / fs;
      alpha = sin(w0) / (2.0 * Q[1]);
      a0 = 1.0 + alpha;
      b0 = ( ( 1.0 + cos(w0) ) * 0.5 ) / a0;
      b1 = ( -( 1.0 + cos(w0) ) ) / a0;
      b2 = ( ( 1.0 + cos(w0) ) * 0.5 ) / a0;
      a1 = ( -2.0 * cos(w0) ) / a0;
      a2 = ( 1.0 - alpha ) / a0;
      a0 = 1.0;
      coeffs[5+kB0] = b0;
      coeffs[5+kB1] = b1;
      coeffs[5+kB2] = b2;
      coeffs[5+kA1] = (-1.0)*a1;
      coeffs[5+kA2] = (-1.0)*a2;

      w0 = 2.0 * pi * fc[2] / fs;
      alpha = sin(w0) / (2.0 * Q[2]);
      a0 = 1.0 + alpha;
      b0 = ( ( 1.0 + cos(w0) ) * 0.5 ) / a0;
      b1 = ( -( 1.0 + cos(w0) ) ) / a0;
      b2 = ( ( 1.0 + cos(w0) ) * 0.5 ) / a0;
      a1 = ( -2.0 * cos(w0) ) / a0;
      a2 = ( 1.0 - alpha ) / a0;
      a0 = 1.0;
      coeffs[10+kB0] = b0;
      coeffs[10+kB1] = b1;
      coeffs[10+kB2] = b2;
      coeffs[10+kA1] = (-1.0)*a1;
      coeffs[10+kA2] = (-1.0)*a2;

      w0 = 2.0 * pi * fc[3] / fs;
      alpha = sin(w0) / (2.0 * Q[3]);
      a0 = 1.0 + alpha;
      b0 = ( ( 1.0 + cos(w0) ) * 0.5 ) / a0;
      b1 = ( -( 1.0 + cos(w0) ) ) / a0;
      b2 = ( ( 1.0 + cos(w0) ) * 0.5 ) / a0;
      a1 = ( -2.0 * cos(w0) ) / a0;
      a2 = ( 1.0 - alpha ) / a0;
      a0 = 1.0;
      coeffs[15+kB0] = b0;
      coeffs[15+kB1] = b1;
      coeffs[15+kB2] = b2;
      coeffs[15+kA1] = (-1.0)*a1;
      coeffs[15+kA2] = (-1.0)*a2;

      break;

    case kBessel6:
      stages = 1;
      Q[0] = 0.0;
      fc[0] = ui->doubleSpinBoxFcHp->value();

      T = 1.0/fs;
      T2 = T*T;

      Omega = 2.0*pi*fc[0]/fs;
      wn  = 2.0/T * tan( Omega/2.0 );
      
      a0 = 0.0000; // bi
      a1 = 1.0000; // ai
      a2 = 1.0;
      b0 = 0.0;
      b1 = 0.0;
      b2 = 1.0;

      sa[0] = a2;
      sa[1] = a1 * wn;
      sa[2] = a0 * wn*wn;
      sb[0] = b2;
      sb[1] = b1;
      sb[2] = b0;

      zb[0] = 4.0*sb[0]    + 2.0*sb[1]*T + sb[2]*T2;
      zb[1] = 2.0*sb[2]*T2 - 8.0*sb[0];
      zb[2] = 4.0*sb[0]    - 2.0*sb[1]*T + sb[2]*T2;

      za[0] = 4.0*sa[0]    + 2.0*sa[1]*T + sa[2]*T2;
      za[1] = 2.0*sa[2]*T2 - 8.0*sa[0];
      za[2] = 4.0*sa[0]    - 2.0*sa[1]*T + sa[2]*T2;

      coeffs[ kB0 ] = zb[0] / za[0];
      coeffs[ kB1 ] = zb[1] / za[0];
      coeffs[ kB2 ] = zb[2] / za[0];
      coeffs[ kA1 ] = (-1.0)*za[1] / za[0];
      coeffs[ kA2 ] = (-1.0)*za[2] / za[0];

      break;

    case kBessel12:
      stages = 1;
      Q[0] = 0.577;
      fc[0] = ui->doubleSpinBoxFcHp->value();

      Omega = 2.0*pi*fc[0]/fs;
      T = 1.0/fs;
      T2 = T*T;
      wn  = 2.0/T * tan( Omega/2.0 );
      
      a0 = 0.6180;  // bi
      a1 = 1.3617;  // ai
      a2 = 1.0;
      b0 = 0.0;
      b1 = 0.0;
      b2 = 1.0;

      sa[0] = a2;
      sa[1] = a1 * wn;
      sa[2] = a0 * wn*wn;
      sb[0] = b2;
      sb[1] = b1;
      sb[2] = b0;

      zb[0] = 4.0*sb[0]    + 2.0*sb[1]*T + sb[2]*T2;
      zb[1] = 2.0*sb[2]*T2 - 8.0*sb[0];
      zb[2] = 4.0*sb[0]    - 2.0*sb[1]*T + sb[2]*T2;

      za[0] = 4.0*sa[0]    + 2.0*sa[1]*T + sa[2]*T2;
      za[1] = 2.0*sa[2]*T2 - 8.0*sa[0];
      za[2] = 4.0*sa[0]    - 2.0*sa[1]*T + sa[2]*T2;

      coeffs[ kB0 ] = zb[0] / za[0];
      coeffs[ kB1 ] = zb[1] / za[0];
      coeffs[ kB2 ] = zb[2] / za[0];
      coeffs[ kA1 ] = (-1.0)*za[1] / za[0];
      coeffs[ kA2 ] = (-1.0)*za[2] / za[0];

      break;

    case kBessel18:
      stages = 2;
      Q[0] = 0.0;
      Q[1] = 0.69;
      fc[0] = ui->doubleSpinBoxFcHp->value();
      fc[1] = ui->doubleSpinBoxFcHp->value();

      T = 1.0/fs;
      T2 = T*T;

      Omega = 2.0*pi*fc[0]/fs;
      wn  = 2.0/T * tan( Omega/2.0 );
      
      a0 = 0.0000; // bi
      a1 = 1.0000; // ai
      a2 = 1.0;
      b0 = 0.0;
      b1 = 0.0;
      b2 = 1.0;

      sa[0] = a2;
      sa[1] = a1 * wn;
      sa[2] = a0 * wn*wn;
      sb[0] = b2;
      sb[1] = b1;
      sb[2] = b0;

      zb[0] = 4.0*sb[0]    + 2.0*sb[1]*T + sb[2]*T2;
      zb[1] = 2.0*sb[2]*T2 - 8.0*sb[0];
      zb[2] = 4.0*sb[0]    - 2.0*sb[1]*T + sb[2]*T2;

      za[0] = 4.0*sa[0]    + 2.0*sa[1]*T + sa[2]*T2;
      za[1] = 2.0*sa[2]*T2 - 8.0*sa[0];
      za[2] = 4.0*sa[0]    - 2.0*sa[1]*T + sa[2]*T2;

      coeffs[ kB0 ] = zb[0] / za[0];
      coeffs[ kB1 ] = zb[1] / za[0];
      coeffs[ kB2 ] = zb[2] / za[0];
      coeffs[ kA1 ] = (-1.0)*za[1] / za[0];
      coeffs[ kA2 ] = (-1.0)*za[2] / za[0];
     
      a0 = 0.4772;  // bi
      a1 = 0.9996;  // ai
      a2 = 1.0;
      b0 = 0.0;
      b1 = 0.0;
      b2 = 1.0;

      sa[0] = a2;
      sa[1] = a1 * wn;
      sa[2] = a0 * wn*wn;
      sb[0] = b2;
      sb[1] = b1;
      sb[2] = b0;

      zb[0] = 4.0*sb[0]    + 2.0*sb[1]*T + sb[2]*T2;
      zb[1] = 2.0*sb[2]*T2 - 8.0*sb[0];
      zb[2] = 4.0*sb[0]    - 2.0*sb[1]*T + sb[2]*T2;

      za[0] = 4.0*sa[0]    + 2.0*sa[1]*T + sa[2]*T2;
      za[1] = 2.0*sa[2]*T2 - 8.0*sa[0];
      za[2] = 4.0*sa[0]    - 2.0*sa[1]*T + sa[2]*T2;

      coeffs[ 5 + kB0 ] = zb[0] / za[0];
      coeffs[ 5 + kB1 ] = zb[1] / za[0];
      coeffs[ 5 + kB2 ] = zb[2] / za[0];
      coeffs[ 5 + kA1 ] = (-1.0)*za[1] / za[0];
      coeffs[ 5 + kA2 ] = (-1.0)*za[2] / za[0];

      break;

    case kBessel24:
      stages = 2;
      Q[0] = 0.52;
      Q[1] = 0.81;
      fc[0] = ui->doubleSpinBoxFcHp->value();
      fc[1] = ui->doubleSpinBoxFcHp->value();
     
      T = 1.0/fs;
      T2 = T*T;

      Omega = 2.0*pi*fc[0]/fs;
      wn  = 2.0/T * tan( Omega/2.0 );
      
      a0 = 0.4889;  // bi
      a1 = 1.3397;  // ai
      a2 = 1.0;
      b0 = 0.0;
      b1 = 0.0;
      b2 = 1.0;

      sa[0] = a2;
      sa[1] = a1 * wn;
      sa[2] = a0 * wn*wn;
      sb[0] = b2;
      sb[1] = b1;
      sb[2] = b0;

      zb[0] = 4.0*sb[0]    + 2.0*sb[1]*T + sb[2]*T2;
      zb[1] = 2.0*sb[2]*T2 - 8.0*sb[0];
      zb[2] = 4.0*sb[0]    - 2.0*sb[1]*T + sb[2]*T2;

      za[0] = 4.0*sa[0]    + 2.0*sa[1]*T + sa[2]*T2;
      za[1] = 2.0*sa[2]*T2 - 8.0*sa[0];
      za[2] = 4.0*sa[0]    - 2.0*sa[1]*T + sa[2]*T2;

      coeffs[ kB0 ] = zb[0] / za[0];
      coeffs[ kB1 ] = zb[1] / za[0];
      coeffs[ kB2 ] = zb[2] / za[0];
      coeffs[ kA1 ] = (-1.0)*za[1] / za[0];
      coeffs[ kA2 ] = (-1.0)*za[2] / za[0];
     
      sa[0] = 0.3890 / (wn*wn);  // bi
      sa[1] = 0.7743 / wn;       // ai
      sa[2] = 1.0;
      sb[0] = 0.0;
      sb[1] = 0.0;
      sb[2] = 1.0;

      sa[0] = a2;
      sa[1] = a1 * wn;
      sa[2] = a0 * wn*wn;
      sb[0] = b2;
      sb[1] = b1;
      sb[2] = b0;

      zb[0] = 4.0*sb[0]    + 2.0*sb[1]*T + sb[2]*T2;
      zb[1] = 2.0*sb[2]*T2 - 8.0*sb[0];
      zb[2] = 4.0*sb[0]    - 2.0*sb[1]*T + sb[2]*T2;

      za[0] = 4.0*sa[0]    + 2.0*sa[1]*T + sa[2]*T2;
      za[1] = 2.0*sa[2]*T2 - 8.0*sa[0];
      za[2] = 4.0*sa[0]    - 2.0*sa[1]*T + sa[2]*T2;

      coeffs[ 5 + kB0 ] = zb[0] / za[0];
      coeffs[ 5 + kB1 ] = zb[1] / za[0];
      coeffs[ 5 + kB2 ] = zb[2] / za[0];
      coeffs[ 5 + kA1 ] = (-1.0)*za[1] / za[0];
      coeffs[ 5 + kA2 ] = (-1.0)*za[2] / za[0];

      break;

    case kButterworth6:
      stages = 1;
      Q[0] = 0.0;
      fc[0] = ui->doubleSpinBoxFcHp->value();

      T = 1.0/fs;
      T2 = T*T;

      Omega = 2.0*pi*fc[0]/fs;
      wn  = 2.0/T * tan( Omega/2.0 );
      
      a0 = 0.0000; // bi
      a1 = 1.0000; // ai
      a2 = 1.0;
      b0 = 0.0;
      b1 = 0.0;
      b2 = 1.0;

      sa[0] = a2;
      sa[1] = a1 * wn;
      sa[2] = a0 * wn*wn;
      sb[0] = b2;
      sb[1] = b1;
      sb[2] = b0;

      zb[0] = 4.0*sb[0]    + 2.0*sb[1]*T + sb[2]*T2;
      zb[1] = 2.0*sb[2]*T2 - 8.0*sb[0];
      zb[2] = 4.0*sb[0]    - 2.0*sb[1]*T + sb[2]*T2;

      za[0] = 4.0*sa[0]    + 2.0*sa[1]*T + sa[2]*T2;
      za[1] = 2.0*sa[2]*T2 - 8.0*sa[0];
      za[2] = 4.0*sa[0]    - 2.0*sa[1]*T + sa[2]*T2;

      coeffs[ kB0 ] = zb[0] / za[0];
      coeffs[ kB1 ] = zb[1] / za[0];
      coeffs[ kB2 ] = zb[2] / za[0];
      coeffs[ kA1 ] = (-1.0)*za[1] / za[0];
      coeffs[ kA2 ] = (-1.0)*za[2] / za[0];

      break;

    case kButterworth12:
      stages = 1;
      Q[0] = 0.71;
      fc[0] = ui->doubleSpinBoxFcHp->value();

      Omega = 2.0*pi*fc[0]/fs;
      T = 1.0/fs;
      T2 = T*T;
      wn  = 2.0/T * tan( Omega/2.0 );
      
      a0 = 1.0000;  // bi
      a1 = 1.4142;  // ai
      a2 = 1.0;
      b0 = 0.0;
      b1 = 0.0;
      b2 = 1.0;

      sa[0] = a2;
      sa[1] = a1 * wn;
      sa[2] = a0 * wn*wn;
      sb[0] = b2;
      sb[1] = b1;
      sb[2] = b0;

      zb[0] = 4.0*sb[0]    + 2.0*sb[1]*T + sb[2]*T2;
      zb[1] = 2.0*sb[2]*T2 - 8.0*sb[0];
      zb[2] = 4.0*sb[0]    - 2.0*sb[1]*T + sb[2]*T2;

      za[0] = 4.0*sa[0]    + 2.0*sa[1]*T + sa[2]*T2;
      za[1] = 2.0*sa[2]*T2 - 8.0*sa[0];
      za[2] = 4.0*sa[0]    - 2.0*sa[1]*T + sa[2]*T2;

      coeffs[ kB0 ] = zb[0] / za[0];
      coeffs[ kB1 ] = zb[1] / za[0];
      coeffs[ kB2 ] = zb[2] / za[0];
      coeffs[ kA1 ] = (-1.0)*za[1] / za[0];
      coeffs[ kA2 ] = (-1.0)*za[2] / za[0];

      break;

    case kButterworth18:
      stages = 2;
      Q[0] = 0.0;
      Q[1] = 1.0;
      fc[0] = ui->doubleSpinBoxFcHp->value();
      fc[1] = ui->doubleSpinBoxFcHp->value();

      T = 1.0/fs;
      T2 = T*T;

      Omega = 2.0*pi*fc[0]/fs;
      wn  = 2.0/T * tan( Omega/2.0 );
      
      a0 = 0.0000; // bi
      a1 = 1.0000; // ai
      a2 = 1.0;
      b0 = 0.0;
      b1 = 0.0;
      b2 = 1.0;

      sa[0] = a2;
      sa[1] = a1 * wn;
      sa[2] = a0 * wn*wn;
      sb[0] = b2;
      sb[1] = b1;
      sb[2] = b0;

      zb[0] = 4.0*sb[0]    + 2.0*sb[1]*T + sb[2]*T2;
      zb[1] = 2.0*sb[2]*T2 - 8.0*sb[0];
      zb[2] = 4.0*sb[0]    - 2.0*sb[1]*T + sb[2]*T2;

      za[0] = 4.0*sa[0]    + 2.0*sa[1]*T + sa[2]*T2;
      za[1] = 2.0*sa[2]*T2 - 8.0*sa[0];
      za[2] = 4.0*sa[0]    - 2.0*sa[1]*T + sa[2]*T2;

      coeffs[ kB0 ] = zb[0] / za[0];
      coeffs[ kB1 ] = zb[1] / za[0];
      coeffs[ kB2 ] = zb[2] / za[0];
      coeffs[ kA1 ] = (-1.0)*za[1] / za[0];
      coeffs[ kA2 ] = (-1.0)*za[2] / za[0];
     
      a0 = 1.0000;  // bi
      a1 = 1.0000;  // ai
      a2 = 1.0;
      b0 = 0.0;
      b1 = 0.0;
      b2 = 1.0;

      sa[0] = a2;
      sa[1] = a1 * wn;
      sa[2] = a0 * wn*wn;
      sb[0] = b2;
      sb[1] = b1;
      sb[2] = b0;

      zb[0] = 4.0*sb[0]    + 2.0*sb[1]*T + sb[2]*T2;
      zb[1] = 2.0*sb[2]*T2 - 8.0*sb[0];
      zb[2] = 4.0*sb[0]    - 2.0*sb[1]*T + sb[2]*T2;

      za[0] = 4.0*sa[0]    + 2.0*sa[1]*T + sa[2]*T2;
      za[1] = 2.0*sa[2]*T2 - 8.0*sa[0];
      za[2] = 4.0*sa[0]    - 2.0*sa[1]*T + sa[2]*T2;

      coeffs[ 5 + kB0 ] = zb[0] / za[0];
      coeffs[ 5 + kB1 ] = zb[1] / za[0];
      coeffs[ 5 + kB2 ] = zb[2] / za[0];
      coeffs[ 5 + kA1 ] = (-1.0)*za[1] / za[0];
      coeffs[ 5 + kA2 ] = (-1.0)*za[2] / za[0];

      break;

    case kButterworth24:
      stages = 2;
      Q[0] = 0.54;
      Q[1] = 1.31;
      fc[0] = ui->doubleSpinBoxFcHp->value();
      fc[1] = ui->doubleSpinBoxFcHp->value();

      T = 1.0/fs;
      T2 = T*T;

      Omega = 2.0*pi*fc[0]/fs;
      wn  = 2.0/T * tan( Omega/2.0 );
      
      a0 = 1.0000; // bi
      a1 = 1.8478; // ai
      a2 = 1.0;
      b0 = 0.0;
      b1 = 0.0;
      b2 = 1.0;

      sa[0] = a2;
      sa[1] = a1 * wn;
      sa[2] = a0 * wn*wn;
      sb[0] = b2;
      sb[1] = b1;
      sb[2] = b0;

      zb[0] = 4.0*sb[0]    + 2.0*sb[1]*T + sb[2]*T2;
      zb[1] = 2.0*sb[2]*T2 - 8.0*sb[0];
      zb[2] = 4.0*sb[0]    - 2.0*sb[1]*T + sb[2]*T2;

      za[0] = 4.0*sa[0]    + 2.0*sa[1]*T + sa[2]*T2;
      za[1] = 2.0*sa[2]*T2 - 8.0*sa[0];
      za[2] = 4.0*sa[0]    - 2.0*sa[1]*T + sa[2]*T2;

      coeffs[ kB0 ] = zb[0] / za[0];
      coeffs[ kB1 ] = zb[1] / za[0];
      coeffs[ kB2 ] = zb[2] / za[0];
      coeffs[ kA1 ] = (-1.0)*za[1] / za[0];
      coeffs[ kA2 ] = (-1.0)*za[2] / za[0];
     
      a0 = 1.0000;  // bi
      a1 = 0.7654;  // ai
      a2 = 1.0;
      b0 = 0.0;
      b1 = 0.0;
      b2 = 1.0;

      sa[0] = a2;
      sa[1] = a1 * wn;
      sa[2] = a0 * wn*wn;
      sb[0] = b2;
      sb[1] = b1;
      sb[2] = b0;

      zb[0] = 4.0*sb[0]    + 2.0*sb[1]*T + sb[2]*T2;
      zb[1] = 2.0*sb[2]*T2 - 8.0*sb[0];
      zb[2] = 4.0*sb[0]    - 2.0*sb[1]*T + sb[2]*T2;

      za[0] = 4.0*sa[0]    + 2.0*sa[1]*T + sa[2]*T2;
      za[1] = 2.0*sa[2]*T2 - 8.0*sa[0];
      za[2] = 4.0*sa[0]    - 2.0*sa[1]*T + sa[2]*T2;

      coeffs[ 5 + kB0 ] = zb[0] / za[0];
      coeffs[ 5 + kB1 ] = zb[1] / za[0];
      coeffs[ 5 + kB2 ] = zb[2] / za[0];
      coeffs[ 5 + kA1 ] = (-1.0)*za[1] / za[0];
      coeffs[ 5 + kA2 ] = (-1.0)*za[2] / za[0];

      break;

    }

  }
}

//------------------------------------------------------------------------------
/*!
 *
 */
void QCrossover::on_doubleSpinBoxFcHp_valueChanged( double  )
{
  updateCoeffs();
  //sendDspParameter();
  emit valueChanged();
  timerDspUpdate.stop();
  timerDspUpdate.start( DSPUPDATELATENCY );
}

//------------------------------------------------------------------------------
/*!
 *
 */
void QCrossover::on_comboBoxTypeHp_currentIndexChanged( int  )
{
  updateCoeffs();
  sendDspParameter();
  emit valueChanged();
}

//------------------------------------------------------------------------------
/*!
 *
 */
void QCrossover::on_pushButtonBypass_clicked()
{
  bypass = ui->pushButtonBypass->isChecked();
  updateCoeffs();
  sendDspParameter();
  emit valueChanged();
}

//------------------------------------------------------------------------------
/*!
 *
 */
void QCrossover::sendDspParameter( void )
{
  #if !defined( __WIN__ )
  #warning QCrossover::sendDspParameter not fully implemented
  #endif
  qDebug()<<"QCrossover::sendDspParameter not fully implemented";

  QByteArray content;

  content.append( dsp->makeParameterForWifi( addr[kParamB2_1], static_cast<float>(coeffs[kB2]) ) );
  content.append( dsp->makeParameterForWifi( addr[kParamB1_1], static_cast<float>(coeffs[kB1]) ) );
  content.append( dsp->makeParameterForWifi( addr[kParamB0_1], static_cast<float>(coeffs[kB0]) ) );
  content.append( dsp->makeParameterForWifi( addr[kParamA2_1], static_cast<float>(coeffs[kA2]) ) );
  content.append( dsp->makeParameterForWifi( addr[kParamA1_1], static_cast<float>(coeffs[kA1]) ) );
  dsp->sendParameterWifi( content );

  content.clear();
  content.append( dsp->makeParameterForWifi( addr[kParamB2_2], static_cast<float>(coeffs[5+kB2]) ) );
  content.append( dsp->makeParameterForWifi( addr[kParamB1_2], static_cast<float>(coeffs[5+kB1]) ) );
  content.append( dsp->makeParameterForWifi( addr[kParamB0_2], static_cast<float>(coeffs[5+kB0]) ) );
  content.append( dsp->makeParameterForWifi( addr[kParamA2_2], static_cast<float>(coeffs[5+kA2]) ) );
  content.append( dsp->makeParameterForWifi( addr[kParamA1_2], static_cast<float>(coeffs[5+kA1]) ) );
  dsp->sendParameterWifi( content );

  content.clear();
  content.append( dsp->makeParameterForWifi( addr[kParamB2_3], static_cast<float>(coeffs[2*5+kB2]) ) );
  content.append( dsp->makeParameterForWifi( addr[kParamB1_3], static_cast<float>(coeffs[2*5+kB1]) ) );
  content.append( dsp->makeParameterForWifi( addr[kParamB0_3], static_cast<float>(coeffs[2*5+kB0]) ) );
  content.append( dsp->makeParameterForWifi( addr[kParamA2_3], static_cast<float>(coeffs[2*5+kA2]) ) );
  content.append( dsp->makeParameterForWifi( addr[kParamA1_3], static_cast<float>(coeffs[2*5+kA1]) ) );
  dsp->sendParameterWifi( content );

  content.clear();
  content.append( dsp->makeParameterForWifi( addr[kParamB2_4], static_cast<float>(coeffs[3*5+kB2]) ) );
  content.append( dsp->makeParameterForWifi( addr[kParamB1_4], static_cast<float>(coeffs[3*5+kB1]) ) );
  content.append( dsp->makeParameterForWifi( addr[kParamB0_4], static_cast<float>(coeffs[3*5+kB0]) ) );
  content.append( dsp->makeParameterForWifi( addr[kParamA2_4], static_cast<float>(coeffs[3*5+kA2]) ) );
  content.append( dsp->makeParameterForWifi( addr[kParamA1_4], static_cast<float>(coeffs[3*5+kA1]) ) );
  dsp->sendParameterWifi( content );
}

//------------------------------------------------------------------------------
/*!
 *
 */
uint32_t QCrossover::getNumBytes( void )
{
  return 2*5*4 + 4*5*4;
}

//==============================================================================
/*!
 */
QByteArray QCrossover::getUserParams( void )
{
  QByteArray ret;
  #if !defined( __WIN__ )
  #warning QCrossover::getUserParams not implemented
  #endif
  qDebug()<<"QCrossover::getUserParams not implemented";
  return ret;
}

//==============================================================================
/*!
 */
void QCrossover::setUserParams( QByteArray& userParams, int& idx )
{
  #if !defined( __WIN__ )
  #warning QCrossover::setUserParams not implemented
  #endif
  qDebug()<<"QCrossover::setUserParams not implemented";
}

//==============================================================================
/*!
 */
QByteArray QCrossover::getDspParams( void )
{
  QByteArray ret;
  #if !defined( __WIN__ )
  #warning QCrossover::getDspParams not implemented
  #endif
  qDebug()<<"QCrossover::getDspParams not implemented";

  return ret;
}