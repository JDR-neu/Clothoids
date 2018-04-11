/*--------------------------------------------------------------------------*\
 |                                                                          |
 |  Copyright (C) 2017                                                      |
 |                                                                          |
 |         , __                 , __                                        |
 |        /|/  \               /|/  \                                       |
 |         | __/ _   ,_         | __/ _   ,_                                | 
 |         |   \|/  /  |  |   | |   \|/  /  |  |   |                        |
 |         |(__/|__/   |_/ \_/|/|(__/|__/   |_/ \_/|/                       |
 |                           /|                   /|                        |
 |                           \|                   \|                        |
 |                                                                          |
 |      Enrico Bertolazzi                                                   |
 |      Dipartimento di Ingegneria Industriale                              |
 |      Universita` degli Studi di Trento                                   |
 |      email: enrico.bertolazzi@unitn.it                                   |
 |                                                                          |
\*--------------------------------------------------------------------------*/

///
/// file: G2lib.hh
///

#ifndef G2LIB_HH
#define G2LIB_HH

#include <iostream>
#include <cmath>
#include <sstream>
#include <stdexcept>
#include <limits>

#ifndef G2LIB_ASSERT
  #define G2LIB_ASSERT(COND,MSG)         \
    if ( !(COND) ) {                        \
      std::ostringstream ost ;              \
      ost << "On line: " << __LINE__        \
          << " file: " << __FILE__          \
          << '\n' << MSG << '\n' ;          \
      throw std::runtime_error(ost.str()) ; \
    }
#endif

// select computer architecture
#if defined(__APPLE__) && defined(__MACH__)
  // osx architecture
  #define G2LIB_OS_OSX 1
  #if defined(__i386__)
    #define G2LIB_ARCH32 1
  #elif defined(__x86_64__)
    #define G2LIB_ARCH64 1
  #endif
#elif defined(__unix__)
  // linux architecture
  #define G2LIB_OS_LINUX 1
  #if defined(__i386__)
    #define G2LIB_ARCH32 1
  #elif defined(__x86_64__)
    #define G2LIB_ARCH64 1
  #endif
#elif defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
  // windows architecture
  #define G2LIB_OS_WINDOWS 1
  #if defined(_M_X64) || defined(_M_AMD64)
    #define G2LIB_ARCH64 1
  #else
    #define G2LIB_ARCH32 1
  #endif
  #ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
  #endif
  #include <windows.h>
#else
  #error "unsupported OS!"
#endif

// check if compiler is C++11
#if (defined(_MSC_VER) &&  _MSC_VER >= 1800) || \
    (defined(__cplusplus) && __cplusplus > 199711L)
  #ifndef G2LIB_DO_NOT_USE_CXX11
    #define G2LIB_USE_CXX11
  #endif
#else
  // not C++11 compiler
  #ifndef nullptr
    #define nullptr NULL
  #endif
#endif


//! Clothoid computations routine
namespace G2lib {

  typedef double valueType ;
  typedef int    indexType ;

  extern valueType const machepsi    ;
  extern valueType const m_pi        ; // pi
  extern valueType const m_pi_2      ; // pi/2
  extern valueType const m_2pi       ; // 2*pi
  extern valueType const m_1_pi      ; // 1/pi
  extern valueType const m_1_sqrt_pi ; // 1/sqrt(pi)

  /*
  // sin(x)/x
  */
  valueType Sinc( valueType x );
  valueType Sinc_D( valueType x );
  valueType Sinc_DD( valueType x );
  valueType Sinc_DDD( valueType x );

  /*
  // (1-cos(x))/x
  */
  valueType Cosc( valueType x );
  valueType Cosc_D( valueType x );
  valueType Cosc_DD( valueType x );
  valueType Cosc_DDD( valueType x );

  /*
  // atan(x)/x
  */
  valueType Atanc( valueType x );
  valueType Atanc_D( valueType x );
  valueType Atanc_DD( valueType x );
  valueType Atanc_DDD( valueType x );

  //! Add or remove multiple of \f$ 2\pi \f$ to an angle  in order to put it in the range \f$ [-\pi,\pi]\f$.
  void rangeSymm( valueType & ang ) ;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  inline
  valueType
  projectPointOnLine( valueType x0,
                      valueType y0,
                      valueType c0, //!< cos(theta0)
                      valueType s0, //!< sin(theta0)
                      valueType x,
                      valueType y ) {
    valueType dx = x - x0 ;
    valueType dy = y - y0 ;
    return (s0 * dy + c0 * dx) ;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  inline
  valueType
  projectPointOnCircle( valueType x0,
                        valueType y0,
                        valueType c0, //!< cos(theta0)
                        valueType s0, //!< sin(theta0)
                        valueType k,
                        valueType L,
                        valueType qx,
                        valueType qy ) {
    valueType dx  = x0 - qx ;
    valueType dy  = y0 - qy ;
    valueType a0  = c0 * dy - s0 * dx ;
    valueType b0  = s0 * dy + c0 * dx ;
    valueType tmp = a0*k ;

    if ( 1+2*tmp > 0 ) {

      tmp = b0/(1+tmp) ;
      tmp *= -Atanc(tmp*k) ; // lunghezza

      if ( tmp < 0 ) {
        valueType absk = std::abs(k) ;
        // if 2*pi*R + tmp <= L add 2*pi*R  to the solution
        if ( m_2pi <= absk*(L-tmp) ) tmp += m_2pi / absk ;
      }

      return tmp ;

    } else {

      valueType om = atan2( b0, a0+1/k ) ;
      if ( k < 0 ) om += m_pi ;
      valueType ss = -om/k ;
      valueType t  = m_2pi/std::abs(k);
      if      ( ss < 0 ) ss += t ;
      else if ( ss > t ) ss += t ;
      return ss ;
    }
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  inline
  bool
  pointInsideCircle( valueType x0,
                     valueType y0,
                     valueType c0, //!< cos(theta0)
                     valueType s0, //!< sin(theta0)
                     valueType k,
                     valueType qx,
                     valueType qy ) {
    valueType cx  = x0 - s0/k ;
    valueType cy  = y0 + c0/k ;
    valueType dst = hypot( qx - cx, qy - cy ) ;
    return dst*k <= 1 ;
  }

  /*\
   |   ____        _           ____       ____
   |  / ___|  ___ | |_   _____|___ \__  _|___ \
   |  \___ \ / _ \| \ \ / / _ \ __) \ \/ / __) |
   |   ___) | (_) | |\ V /  __// __/ >  < / __/
   |  |____/ \___/|_| \_/ \___|_____/_/\_\_____|
  \*/

  class Solve2x2 {
    indexType i[2], j[2] ;
    valueType LU[2][2] ;
    valueType epsi ;
    bool      singular ;

  public:
  
    Solve2x2() : epsi(1e-10) {}
    bool factorize( valueType A[2][2] ) ;
    bool solve( valueType const b[2], valueType x[2] ) const ;
  } ;

}

#endif

///
/// eof: G2lib.hh
///
