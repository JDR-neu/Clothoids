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
/// file: Clothoid.hh
///

#ifndef CLOTHOID_HH
#define CLOTHOID_HH

#include "Fresnel.hh"
#include "Line.hh"
#include "Circle.hh"
#include "Triangle2D.hh"

#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <iostream>

//! Clothoid computations routine
namespace G2lib {

  /*\
   |    ____ _       _   _           _     _
   |   / ___| | ___ | |_| |__   ___ (_) __| |
   |  | |   | |/ _ \| __| '_ \ / _ \| |/ _` |
   |  | |___| | (_) | |_| | | | (_) | | (_| |
   |   \____|_|\___/ \__|_| |_|\___/|_|\__,_|
  \*/

  //! Compute Lommel function
  real_type
  LommelReduced( real_type mu, real_type nu, real_type z );

  /*\
   |    ____ _       _   _           _     _  ____
   |   / ___| | ___ | |_| |__   ___ (_) __| |/ ___|   _ _ ____   _____
   |  | |   | |/ _ \| __| '_ \ / _ \| |/ _` | |  | | | | '__\ \ / / _ \
   |  | |___| | (_) | |_| | | | (_) | | (_| | |__| |_| | |   \ V /  __/
   |   \____|_|\___/ \__|_| |_|\___/|_|\__,_|\____\__,_|_|    \_/ \___|
  \*/
  //! \brief Class to manage Clothoid Curve
  class ClothoidCurve : public BaseCurve {
  public:

    typedef struct {
      real_type    s0;
      real_type    L;
      ClothoidData cd;
      Triangle2D   t;
    } bbData;

    typedef struct {
      real_type    split_angle;
      real_type    split_size;
      real_type    split_offs;
      real_type    s0;
      real_type    L;
      ClothoidData cd;
    } bbData2;

    typedef struct {
      real_type s0, theta0, x0, y0;
      real_type s1, theta1, x1, y1;
    } bbDataForSplit;

  private:

    ClothoidData CD; //!< clothoid data
    real_type    L;  //!< lenght of clothoid segment

    void
    bbSplit_internal( bbData2 const & data, std::vector<bbData> & bbV ) const;

    //! Use newton and bisection to intersect two small clothoid segment
    bool
    intersect_internal( bbData const & c1, real_type c1_offs, real_type & s1,
                        bbData const & c2, real_type c2_offs, real_type & s2,
                        int_type max_iter,
                        real_type tolerance ) const;

    void
    bbTriangles( real_type                 offs,
                 std::vector<Triangle2D> & tvec,
                 bbDataForSplit const    & data,
                 real_type                 max_angle,
                 real_type                 max_size,
                 int_type                  level ) const;

    void
    bbTriangles( std::vector<Triangle2D> & tvec,
                 bbDataForSplit const    & data,
                 real_type                 max_angle,
                 real_type                 max_size,
                 int_type                  level ) const {
      bbTriangles( 0, tvec, data, max_angle, max_size, level );
    }

  public:

    using BaseCurve::thetaBegin;
    using BaseCurve::thetaEnd;

    using BaseCurve::xBegin;
    using BaseCurve::yBegin;
    using BaseCurve::xEnd;
    using BaseCurve::yEnd;

    using BaseCurve::tx_Begin;
    using BaseCurve::ty_Begin;
    using BaseCurve::tx_End;
    using BaseCurve::ty_End;

    using BaseCurve::nx_Begin;
    using BaseCurve::ny_Begin;
    using BaseCurve::nx_End;
    using BaseCurve::ny_End;

    using BaseCurve::X;
    using BaseCurve::X_D;
    using BaseCurve::X_DD;
    using BaseCurve::X_DDD;

    using BaseCurve::Y;
    using BaseCurve::Y_D;
    using BaseCurve::Y_DD;
    using BaseCurve::Y_DDD;

    using BaseCurve::evaluate;

    using BaseCurve::eval;
    using BaseCurve::eval_D;
    using BaseCurve::eval_DD;
    using BaseCurve::eval_DDD;

    using BaseCurve::closestPoint;
    using BaseCurve::distance;

    ClothoidCurve()
    : BaseCurve(G2LIB_CLOTHOID)
    {
      CD.x0     = 0;
      CD.y0     = 0;
      CD.theta0 = 0;
      CD.kappa0 = 0;
      CD.dk     = 0;
      L         = 0;
    }

    //! construct a clothoid with the standard parameters
    ClothoidCurve( real_type _x0,
                   real_type _y0,
                   real_type _theta0,
                   real_type _k,
                   real_type _dk,
                   real_type _L )
    : BaseCurve(G2LIB_CLOTHOID)
    {
      CD.x0     = _x0;
      CD.y0     = _y0;
      CD.theta0 = _theta0;
      CD.kappa0 = _k;
      CD.dk     = _dk;
      L         = _L;
    }

    //! construct a clothoid by solving the hermite G1 problem
    ClothoidCurve( real_type const P0[],
                   real_type       theta0,
                   real_type const P1[],
                   real_type       theta1 )
    : BaseCurve(G2LIB_CLOTHOID)
    {
      build_G1( P0[0], P0[1], theta0, P1[0], P1[1], theta1 );
    }

    void
    copy( ClothoidCurve const & c ) {
      CD = c.CD;
      L  = c.L;
    }

    ClothoidCurve( ClothoidCurve const & s )
    : BaseCurve(G2LIB_CLOTHOID)
    { copy(s); }

    ClothoidCurve( LineSegment const & LS )
    : BaseCurve(G2LIB_CLOTHOID)
    {
      CD.x0     = LS.x0;
      CD.y0     = LS.y0;
      CD.theta0 = LS.theta0;
      CD.kappa0 = 0;
      CD.dk     = 0;
      L         = LS.L;
    }

    ClothoidCurve( CircleArc const & C )
    : BaseCurve(G2LIB_CLOTHOID)
    {
      CD.x0     = C.x0;
      CD.y0     = C.y0;
      CD.theta0 = C.theta0;
      CD.kappa0 = C.k;
      CD.dk     = 0;
      L         = C.L;
    }

    ClothoidCurve const & operator = ( ClothoidCurve const & s )
    { copy(s); return *this; }

    /*\
     |  _         _ _    _
     | | |__ _  _(_) |__| |
     | | '_ \ || | | / _` |
     | |_.__/\_,_|_|_\__,_|
    \*/
    //! construct a clothoid with the standard parameters
    void
    build( real_type _x0,
           real_type _y0,
           real_type _theta0,
           real_type _k,
           real_type _dk,
           real_type _L ) {
      CD.x0     = _x0;
      CD.y0     = _y0;
      CD.theta0 = _theta0;
      CD.kappa0 = _k;
      CD.dk     = _dk;
      L         = _L;
    }

    /*!
     | \brief build a clothoid by solving the hermite G1 problem
     |
     | \param x0     initial x position \f$ x_0      \f$
     | \param y0     initial y position \f$ y_0      \f$
     | \param theta0 initial angle      \f$ \theta_0 \f$
     | \param x1     final x position   \f$ x_1      \f$
     | \param y1     final y position   \f$ y_1      \f$
     | \param theta1 final angle        \f$ \theta_1 \f$
     | \return number of iteration performed
    \*/
    int
    build_G1( real_type x0,
              real_type y0,
              real_type theta0,
              real_type x1,
              real_type y1,
              real_type theta1,
              real_type tol = 1e-12 ) {
      return CD.build_G1( x0, y0, theta0, x1, y1, theta1, tol, L );
    }

    /*!
     | \brief build a clothoid by solving the hermite G1 problem
     |
     | \param x0     initial x position \f$ x_0      \f$
     | \param y0     initial y position \f$ y_0      \f$
     | \param theta0 initial angle      \f$ \theta_0 \f$
     | \param x1     final x position   \f$ x_1      \f$
     | \param y1     final y position   \f$ y_1      \f$
     | \param theta1 final angle        \f$ \theta_1 \f$
     | \return number of iteration performed
    \*/
    int
    build_G1_D( real_type x0,
                real_type y0,
                real_type theta0,
                real_type x1,
                real_type y1,
                real_type theta1,
                real_type L_D[2],
                real_type k_D[2],
                real_type dk_D[2],
                real_type tol = 1e-12 ) {
      return CD.build_G1( x0, y0, theta0, x1, y1, theta1, tol, L,
                          true, L_D, k_D, dk_D );
    }

    /*!
     | \brief build a clothoid by solving the forward problem
     |
     | \param x0     initial x position \f$ x_0      \f$
     | \param y0     initial y position \f$ y_0      \f$
     | \param theta0 initial angle      \f$ \theta_0 \f$
     | \param kappa0 initial curvature  \f$ \kappa_0 \f$
     | \param x1     final x position   \f$ x_1      \f$
     | \param y1     final y position   \f$ y_1      \f$
    \*/
    bool
    build_forward( real_type x0,
                   real_type y0,
                   real_type theta0,
                   real_type kappa0,
                   real_type x1,
                   real_type y1,
                   real_type tol = 1e-12 ) {
      return CD.build_forward( x0, y0, theta0, kappa0, x1, y1, tol, L );
    }

    /*!
     | \brief build a clothoid from a line segment
     |
     | \param LS line segment object
    \*/
    void
    build( LineSegment const & LS ) {
      CD.x0     = LS.x0;
      CD.y0     = LS.y0;
      CD.theta0 = LS.theta0;
      CD.kappa0 = 0;
      CD.dk     = 0;
      L         = LS.L;
    }

    /*!
     | \brief build a clothoid from a line segment
     |
     | \param C line segment object
    \*/
    void
    build( CircleArc const & C ) {
      CD.x0     = C.x0;
      CD.y0     = C.y0;
      CD.theta0 = C.theta0;
      CD.kappa0 = C.k;
      CD.dk     = 0;
      L         = C.L;
    }

    void
    Pinfinity( real_type & x, real_type & y, bool plus = true ) const
    { CD.Pinfinity( x, y, plus ); }

    /*!
     | \brief get clothoid curvature at curvilinear cooordinate `s`
     |
     | \param  s curvilinear cooordinate
     | \return curvature at curvilinear cooordinate `s`
    \*/
    real_type
    kappa( real_type s ) const
    { return CD.kappa(s); }

    /*!
     | \brief get clothoid curvature derivative at curvilinear cooordinate `s`
     |
     | \return curvature derivative (radiant/s) at curvilinear cooordinate `s`
    \*/
    real_type
    kappa_D( real_type ) const
    { return CD.dk; }

    /*!
     | \brief get clothoid curvature second derivative at curvilinear cooordinate `s`
     |
     | \return curvature second derivative (radiant/s^2) at curvilinear cooordinate `s`
    \*/
    real_type
    kappa_DD( real_type ) const
    { return 0; }

    /*!
     | \brief get clothoid curvature third derivative at curvilinear cooordinate `s`
     |
     | \return angle third derivative (radiant/s^3) at curvilinear cooordinate `s`
    \*/
    real_type
    kappa_DDD( real_type ) const
    { return 0; }

    real_type
    dkappa() const
    { return CD.dk; }

    /*!
     | \return clothoid total variation
    \*/
    real_type
    thetaTotalVariation() const;

    real_type
    thetaMinMax( real_type & thMin, real_type & thMax ) const;

    /*!
     | \return clothoid angle range
    \*/
    real_type
    deltaTheta() const
    { real_type thMin, thMax; return thetaMinMax( thMin, thMax ); }

    real_type
    curvatureMinMax( real_type & kMin, real_type & kMax ) const;

    /*!
     | \return clothoid total curvature variation
    \*/
    real_type curvatureTotalVariation() const;

    real_type integralCurvature2() const;

    real_type integralJerk2() const;

    real_type integralSnap2() const;

    /*\
     |     _ _    _
     |  __| (_)__| |_ __ _ _ _  __ ___
     | / _` | (_-<  _/ _` | ' \/ _/ -_)
     | \__,_|_/__/\__\__,_|_||_\__\___|
    \*/
    /*!
     | \brief Compute the point on clothoid at minimal distance from a given point
     |
     | \param  qx x-coordinate of the given point
     | \param  qy y-coordinate of the given point
     | \param  X  x-coordinate of the point on clothoid at minimal distance
     | \param  Y  y-coordinate of the point on clothoid at minimal distance
     | \param  S  curvilinear coordinate of the point (X,Y) on the clothoid
     | \return the distance of the
     |
    \*/
    real_type
    closestPointBySample( real_type   ds,
                          real_type   qx,
                          real_type   qy,
                          real_type & X,
                          real_type & Y,
                          real_type & S ) const;

    real_type
    distanceBySample( real_type   ds,
                      real_type   qx,
                      real_type   qy,
                      real_type & S ) const {
      real_type X, Y;
      return closestPointBySample( ds, qx, qy, X, Y, S );
    }

    real_type
    distanceBySample( real_type ds,
                      real_type qx,
                      real_type qy ) const {
      real_type X, Y, S;
      return closestPointBySample( ds, qx, qy, X, Y, S );
    }

    /*\
     |  _    _   _____    _                _
     | | |__| |_|_   _| _(_)__ _ _ _  __ _| |___
     | | '_ \ '_ \| || '_| / _` | ' \/ _` | / -_)
     | |_.__/_.__/|_||_| |_\__,_|_||_\__, |_\___|
     |                               |___/
    \*/

    //! get the triangle bounding box (if angle variation less that pi/2)
    bool
    bbTriangle( real_type & xx0, real_type & yy0,
                real_type & xx1, real_type & yy1,
                real_type & xx2, real_type & yy2 ) const {
      return CD.bbTriangle( L,
                            xx0, yy0,
                            xx1, yy1,
                            xx2, yy2 );
    }

    //! get the triangle bounding box (if angle variation less that pi/2)
    bool
    bbTriangle( real_type offs,
                real_type & xx0, real_type & yy0,
                real_type & xx1, real_type & yy1,
                real_type & xx2, real_type & yy2 ) const {
      return CD.bbTriangle( L, offs,
                            xx0, yy0,
                            xx1, yy1,
                            xx2, yy2 );
    }

    bool
    bbTriangle( Triangle2D & t ) const {
      real_type x0, y0, x1, y1, x2, y2;
      bool ok = CD.bbTriangle( L, x0, y0, x1, y1, x2, y2 );
      if ( ok ) t.build( x0, y0, x1, y1, x2, y2 );
      return ok;
    }

    bool
    bbTriangle( real_type offs, Triangle2D & t ) const {
      real_type x0, y0, x1, y1, x2, y2;
      bool ok = CD.bbTriangle( L, offs, x0, y0, x1, y1, x2, y2 );
      if ( ok ) t.build( x0, y0, x1, y1, x2, y2 );
      return ok;
    }

    void
    bbTriangles( real_type                 offs,
                 std::vector<Triangle2D> & tvec,
                 real_type                 max_angle = m_pi/6, // 30 degree
                 real_type                 max_size  = 1e100 ) const;

    void
    bbTriangles( std::vector<Triangle2D> & tvec,
                 real_type                 max_angle = m_pi/6, // 30 degree
                 real_type                 max_size  = 1e100 ) const {
      bbTriangles( 0, tvec, max_angle, max_size );
    }

    /*!
     | \brief split clothoids in smaller segments
     |
     | \param split_angle maximum angle variation
     | \param split_size  maximum height of the triangle
     | \param split_offs  curve offset
     | \param bb          splitting data structures vector
    \*/
    void
    bbSplit( real_type             split_angle,
             real_type             split_size,
             real_type             split_offs,
             std::vector<bbData> & bb,
             bool                  reset_bb = true ) const;

    /*\
     |  _     _                      _
     | (_)_ _| |_ ___ _ _ ___ ___ __| |_
     | | | ' \  _/ -_) '_(_-</ -_) _|  _|
     | |_|_||_\__\___|_| /__/\___\__|\__|
     |
    \*/
    /*!
     | \brief intersect two clothoid arcs
     |
     | \param offs      offset of the first arc
     | \param c         the second clothoid arc
     | \param c_offs    offset of the second arc
     | \param s1        intersection parameters of the first arc
     | \param s2        intersection parameters of the second arc
     | \param max_iter  max allowed iteration
     | \param tolerance admitted tolerance
    \*/
    void
    intersect( real_type                offs,
               ClothoidCurve const    & c,
               real_type                c_offs,
               std::vector<real_type> & s1,
               std::vector<real_type> & s2,
               int_type                 max_iter,
               real_type                tolerance ) const;

    /*!
     | \brief intersect two clothoid arcs
     |
     | \param c         the second clothoid arc
     | \param s1        intersection parameters of the first arc
     | \param s2        intersection parameters of the second arc
     | \param max_iter  max allowed iteration
     | \param tolerance admitted tolerance
    \*/
    void
    intersect( ClothoidCurve const    & c,
               std::vector<real_type> & s1,
               std::vector<real_type> & s2,
               int_type                 max_iter,
               real_type                tolerance ) const {
      intersect( 0, c, 0, s1, s2, max_iter, tolerance );
    }

    /*!
     | \brief intersect a clothoid with a circle arc
     |
     | \param offs      offset of the first arc
     | \param c_in      the circle arc
     | \param c_offs    offset of the circle arc
     | \param s1        intersection parameters of the first arc
     | \param s2        intersection parameters of the second arc
     | \param max_iter  max allowed iteration
     | \param tolerance admitted tolerance
    \*/
    void
    intersect( real_type                offs,
               CircleArc const        & c_in,
               real_type                c_offs,
               std::vector<real_type> & s1,
               std::vector<real_type> & s2,
               int_type                 max_iter,
               real_type                tolerance ) const {
      ClothoidCurve c(c_in);
      intersect( offs, c, c_offs, s1, s2, max_iter, tolerance );
    }

    /*!
     | \brief intersect a clothoid with a circle arc
     |
     | \param c_in      the circle arc
     | \param s1        intersection parameters of the first arc
     | \param s2        intersection parameters of the second arc
     | \param max_iter  max allowed iteration
     | \param tolerance admitted tolerance
    \*/
    void
    intersect( CircleArc const        & c_in,
               std::vector<real_type> & s1,
               std::vector<real_type> & s2,
               int_type                 max_iter,
               real_type                tolerance ) const {
      ClothoidCurve c(c_in);
      intersect( 0, c, 0, s1, s2, max_iter, tolerance );
    }

    /*!
     | \brief intersect a clothoid with a line segment
     |
     | \param offs      offset of the first arc
     | \param c_in      the line segment
     | \param c_offs    offset of the line segment
     | \param s1        intersection parameters of the first arc
     | \param s2        intersection parameters of the second arc
     | \param max_iter  max allowed iteration
     | \param tolerance admitted tolerance
    \*/
    void
    intersect( real_type                offs,
               LineSegment const      & c_in,
               real_type                c_offs,
               std::vector<real_type> & s1,
               std::vector<real_type> & s2,
               int_type                 max_iter,
               real_type                tolerance ) const {
      ClothoidCurve c(c_in);
      intersect( offs, c, c_offs, s1, s2, max_iter, tolerance );
    }

    /*!
     | \brief intersect a clothoid with a line segment
     |
     | \param c_in      the line segment
     | \param s1        intersection parameters of the first arc
     | \param s2        intersection parameters of the second arc
     | \param max_iter  max allowed iteration
     | \param tolerance admitted tolerance
    \*/
    void
    intersect( LineSegment const      & c_in,
               std::vector<real_type> & s1,
               std::vector<real_type> & s2,
               int_type                 max_iter,
               real_type                tolerance ) const {
      ClothoidCurve c(c_in);
      intersect( 0, c, 0, s1, s2, max_iter, tolerance );
    }

    // collision detection
    bool
    approximate_collision(
      real_type             offs,
      ClothoidCurve const & c,
      real_type             c_offs,
      real_type             max_angle, //!< maximum angle variation
      real_type             max_size   //!< curve offset
    ) const;

    // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

    virtual
    void
    bbox( real_type & xmin,
          real_type & ymin,
          real_type & xmax,
          real_type & ymax ) const G2LIB_OVERRIDE;

    virtual
    void
    bbox( real_type   offs,
          real_type & xmin,
          real_type & ymin,
          real_type & xmax,
          real_type & ymax ) const G2LIB_OVERRIDE;

    // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

    virtual
    real_type
    length() const G2LIB_OVERRIDE
    { return L; }

    virtual
    real_type
    length( real_type ) const G2LIB_OVERRIDE {
      G2LIB_ASSERT( false, "Offset length not available for Clothoids" );
      return 0;
    }

    real_type kappaBegin() const { return CD.kappa0; }
    real_type kappaEnd()   const { return CD.kappa(L); }

    virtual
    real_type
    thetaBegin() const G2LIB_OVERRIDE
    { return CD.theta0; }

    virtual
    real_type
    thetaEnd() const G2LIB_OVERRIDE
    { return CD.theta(L); }

    virtual
    real_type
    xBegin() const G2LIB_OVERRIDE
    { return CD.x0; }

    virtual
    real_type
    xEnd() const G2LIB_OVERRIDE
    { return CD.X(L); }

    virtual
    real_type
    yBegin() const G2LIB_OVERRIDE
    { return CD.y0; }

    virtual
    real_type
    yEnd() const G2LIB_OVERRIDE
    { return CD.Y(L); }

    virtual
    real_type
    tx_Begin() const G2LIB_OVERRIDE
    { return CD.tg0_x(); }

    virtual
    real_type
    ty_Begin() const G2LIB_OVERRIDE
    { return CD.tg0_y(); }

    virtual
    real_type
    nx_Begin() const G2LIB_OVERRIDE
    { return CD.nor0_x(); }

    virtual
    real_type
    ny_Begin() const G2LIB_OVERRIDE
    { return CD.nor0_y(); }

    // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

    /*\
     |  _____                   _   _   _
     | |_   _|   __ _ _ __   __| | | \ | |
     |   | |    / _` | '_ \ / _` | |  \| |
     |   | |   | (_| | | | | (_| | | |\  |
     |   |_|    \__,_|_| |_|\__,_| |_| \_|
    \*/

    virtual
    real_type
    tx( real_type s ) const G2LIB_OVERRIDE
    { return CD.tg_x( s ); }

    virtual
    real_type
    ty( real_type s ) const G2LIB_OVERRIDE
    { return CD.tg_y( s ); }

    virtual
    real_type
    tx_D( real_type s ) const G2LIB_OVERRIDE
    { return CD.tg_x_D( s ); }

    virtual
    real_type
    ty_D( real_type s ) const G2LIB_OVERRIDE
    { return CD.tg_y_D( s ); }

    virtual
    real_type
    tx_DD( real_type s ) const G2LIB_OVERRIDE
    { return CD.tg_x_DD( s ); }

    virtual
    real_type
    ty_DD( real_type s ) const G2LIB_OVERRIDE
    { return CD.tg_y_DD( s ); }

    virtual
    real_type
    tx_DDD( real_type s ) const G2LIB_OVERRIDE
    { return CD.tg_x_DDD( s ); }

    virtual
    real_type
    ty_DDD( real_type s ) const G2LIB_OVERRIDE
    { return CD.tg_y_DDD( s ); }

    // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

    virtual
    void
    tg( real_type   s,
        real_type & tx,
        real_type & ty ) const G2LIB_OVERRIDE
    { CD.tg( s, tx, ty ); }

    virtual
    void
    tg_D( real_type   s,
          real_type & tx_D,
          real_type & ty_D ) const G2LIB_OVERRIDE
    { CD.tg_D( s, tx_D, ty_D ); }

    virtual
    void
    tg_DD( real_type   s,
           real_type & tx_DD,
           real_type & ty_DD ) const G2LIB_OVERRIDE
    { CD.tg_DD( s, tx_DD, ty_DD ); }

    virtual
    void
    tg_DDD( real_type   s,
            real_type & tx_DDD,
            real_type & ty_DDD ) const G2LIB_OVERRIDE
    { CD.tg_DDD( s, tx_DDD, ty_DDD ); }

    // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

    /*!
     * \brief get clothoid angle at curvilinear cooordinate `s`
     *
     * \param  s curvilinear cooordinate
     * \return angle (radiant) at curvilinear cooordinate `s`
     */
    real_type
    theta( real_type s ) const G2LIB_OVERRIDE
    { return CD.theta(s); }

    /*!
     * \brief get clothoid angle derivative (=curvature) at curvilinear cooordinate `s`
     *
     * \param  s curvilinear cooordinate
     * \return angle derivative (radiant/s) at curvilinear cooordinate `s`
     */
    real_type
    theta_D( real_type s ) const G2LIB_OVERRIDE
    { return CD.kappa(s); }

    /*!
     * \brief get clothoid angle second derivative at curvilinear cooordinate `s`
     *
     * \return angle second derivative (radiant/s^2) at curvilinear cooordinate `s`
     */
    real_type
    theta_DD( real_type ) const G2LIB_OVERRIDE
    { return CD.dk; }

    /*!
     * \brief get clothoid angle third derivative at curvilinear cooordinate `s`
     *
     * \return angle third derivative (radiant/s^3) at curvilinear cooordinate `s`
     */
    real_type
    theta_DDD( real_type ) const G2LIB_OVERRIDE
    { return 0; }

    // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

    virtual
    void
    evaluate( real_type   s,
              real_type & th,
              real_type & k,
              real_type & x,
              real_type & y ) const G2LIB_OVERRIDE
    { CD.evaluate( s, th, k, x, y ); }

    // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

    /*!
     * \brief clothoid X coordinate at curvilinear coordinate `s`
     * \param s curvilinear coordinate
     * \return clothoid X coordinate
     */
    real_type
    X( real_type s ) const G2LIB_OVERRIDE
    { return CD.X(s); }

    real_type
    X_D( real_type s ) const G2LIB_OVERRIDE
    { return CD.X_D(s); }

    real_type
    X_DD( real_type s ) const G2LIB_OVERRIDE
    { return CD.X_DD(s); }

    real_type
    X_DDD( real_type s ) const G2LIB_OVERRIDE
    { return CD.X_DDD(s); }

    /*!
     * \brief clothoid Y coordinate at curvilinear coordinate `s`
     * \param s curvilinear coordinate
     * \return clothoid Y coordinate
     */
    real_type
    Y( real_type s ) const G2LIB_OVERRIDE
    { return CD.Y(s); }

    real_type
    Y_D( real_type s ) const G2LIB_OVERRIDE
    { return CD.Y_D(s); }

    real_type
    Y_DD ( real_type s ) const G2LIB_OVERRIDE
    { return CD.Y_DD(s); }

    real_type
    Y_DDD( real_type s ) const G2LIB_OVERRIDE
    { return CD.Y_DDD(s); }

    /*!
     * \brief clothoid X coordinate at curvilinear coordinate `s`
     * \param s curvilinear coordinate
     * \param t lateral offset
     * \return clothoid X coordinate
     */
    real_type
    X( real_type s, real_type t ) const G2LIB_OVERRIDE
    { return CD.X(s,t); }

    real_type
    X_D( real_type s, real_type t ) const G2LIB_OVERRIDE
    { return CD.X_D(s,t); }

    real_type
    X_DD( real_type s, real_type t ) const G2LIB_OVERRIDE
    { return CD.X_DD(s,t); }

    real_type
    X_DDD( real_type s, real_type t ) const G2LIB_OVERRIDE
    { return CD.X_DDD(s,t); }

    /*!
     * \brief clothoid Y coordinate at curvilinear coordinate `s`
     * \param s curvilinear coordinate
     * \param t lateral offset
     * \return clothoid Y coordinate
     */
    real_type
    Y( real_type s, real_type t ) const G2LIB_OVERRIDE
    { return CD.Y(s,t); }

    real_type
    Y_D( real_type s, real_type t ) const G2LIB_OVERRIDE
    { return CD.Y_D(s,t); }

    real_type
    Y_DD( real_type s, real_type t ) const G2LIB_OVERRIDE
    { return CD.Y_DD(s,t); }

    real_type
    Y_DDD( real_type s, real_type t ) const G2LIB_OVERRIDE
    { return CD.Y_DDD(s,t); }

    // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

    virtual
    void
    eval( real_type   s,
          real_type & x,
          real_type & y ) const G2LIB_OVERRIDE
    { CD.eval( s, x, y ); }

    virtual
    void
    eval_D( real_type   s,
            real_type & x_D,
            real_type & y_D ) const G2LIB_OVERRIDE
    { CD.eval_D( s, x_D, y_D ); }

    virtual
    void
    eval_DD( real_type   s,
             real_type & x_DD,
             real_type & y_DD ) const G2LIB_OVERRIDE
    { CD.eval_DD( s, x_DD, y_DD ); }

    virtual
    void
    eval_DDD( real_type   s,
              real_type & x_DDD,
              real_type & y_DDD ) const G2LIB_OVERRIDE
    { CD.eval_DDD( s, x_DDD, y_DDD ); }

    virtual
    void
    eval( real_type   s,
          real_type   offs,
          real_type & x,
          real_type & y ) const G2LIB_OVERRIDE
    { CD.eval( s, offs, x, y ); }

    virtual
    void
    eval_D( real_type   s,
            real_type   offs,
            real_type & x_D,
            real_type & y_D ) const G2LIB_OVERRIDE
    { CD.eval_D( s, offs, x_D, y_D ); }

    virtual
    void
    eval_DD( real_type   s,
             real_type   offs,
             real_type & x_DD,
             real_type & y_DD ) const G2LIB_OVERRIDE
    { CD.eval_DD( s, offs, x_DD, y_DD ); }

    virtual
    void
    eval_DDD( real_type   s,
              real_type   offs,
              real_type & x_DDD,
              real_type & y_DDD ) const G2LIB_OVERRIDE
    { CD.eval_DDD( s, offs, x_DDD, y_DDD ); }

    /*\
     |  _                        __
     | | |_ _ __ __ _ _ __  ___ / _| ___  _ __ _ __ ___
     | | __| '__/ _` | '_ \/ __| |_ / _ \| '__| '_ ` _ \
     | | |_| | | (_| | | | \__ \  _| (_) | |  | | | | | |
     |  \__|_|  \__,_|_| |_|___/_|  \___/|_|  |_| |_| |_|
    \*/

    virtual
    void
    translate( real_type tx, real_type ty ) G2LIB_OVERRIDE
    { CD.x0 += tx; CD.y0 += ty; }

    virtual
    void
    rotate( real_type angle, real_type cx, real_type cy ) G2LIB_OVERRIDE
    { CD.rotate( angle, cx, cy ); }

    virtual
    void
    scale( real_type s ) G2LIB_OVERRIDE {
      CD.kappa0 /= s;
      CD.dk     /= s*s;
      L         *= s;
    }

    virtual
    void
    reverse() G2LIB_OVERRIDE
    { CD.reverse(L); }

    virtual
    void
    changeOrigin( real_type newx0, real_type newy0 ) G2LIB_OVERRIDE
    { CD.x0 = newx0; CD.y0 = newy0; }

    virtual
    void
    trim( real_type s_begin, real_type s_end ) G2LIB_OVERRIDE {
      CD.origin_at( s_begin );
      L = s_end - s_begin;
    }

    void
    changeCurvilinearOrigin( real_type s0, real_type newL ) {
      CD.origin_at( s0 );
      L = newL;
    }

    /*\
     |   _       _                          _
     |  (_)_ __ | |_ ___ _ __ ___  ___  ___| |_
     |  | | '_ \| __/ _ \ '__/ __|/ _ \/ __| __|
     |  | | | | | ||  __/ |  \__ \  __/ (__| |_
     |  |_|_| |_|\__\___|_|  |___/\___|\___|\__|
    \*/

    virtual
    bool
    collision( BaseCurve const & obj ) const G2LIB_OVERRIDE;

    virtual
    bool
    collision( real_type         offs,
               BaseCurve const & obj,
               real_type         offs_obj ) const G2LIB_OVERRIDE;

    virtual
    void
    intersect( BaseCurve const & obj,
               IntersectList   & ilist,
               bool              swap_s_vals ) const G2LIB_OVERRIDE;

    virtual
    void
    intersect( real_type         offs,
               BaseCurve const & obj,
               real_type         offs_obj,
               IntersectList   & ilist,
               bool              swap_s_vals ) const G2LIB_OVERRIDE;

    bool
    collision( ClothoidCurve const & C ) const;

    bool
    collision( real_type             offs,
               ClothoidCurve const & C,
               real_type             offs_C ) const;

    void
    intersect( ClothoidCurve const & C,
               IntersectList       & ilist,
               bool                  swap_s_vals ) const;

    void
    intersect( real_type               offs,
               ClothoidCurve const   & C,
               real_type               offs_C,
               IntersectList         & ilist,
               bool                    swap_s_vals ) const;

    /*\
     |                  _           _   _
     |  _ __  _ __ ___ (_) ___  ___| |_(_) ___  _ __
     | | '_ \| '__/ _ \| |/ _ \/ __| __| |/ _ \| '_ \
     | | |_) | | | (_) | |  __/ (__| |_| | (_) | | | |
     | | .__/|_|  \___// |\___|\___|\__|_|\___/|_| |_|
     | |_|           |__/
    \*/

    /*!
     | \param  qx  x-coordinate of the point
     | \param  qy  y-coordinate of the point
     | \param  x   x-coordinate of the projected point on the curve
     | \param  y   y-coordinate of the projected point on the curve
     | \param  s   parameter on the curve of the projection
     | \return 1  = unique orthogonal projection
     |         0  = more than one projection (first returned)
     |         -1 = projection line not othogonal to curve
     |         -2 = projection line not othogonal andnot unique
    \*/
    virtual
    int_type
    projection( real_type   qx,
                real_type   qy,
                real_type & x,
                real_type & y,
                real_type & s ) const G2LIB_OVERRIDE;

    /*!
     | \param  qx   x-coordinate of the point
     | \param  qy   y-coordinate of the point
     | \param  offs offset of the curve
     | \param  x    x-coordinate of the projected point on the curve
     | \param  y    y-coordinate of the projected point on the curve
     | \param  s    parameter on teh curve of the projection
     | \return 1  = unique orthogonal projection
     |         0  = more than one projection (first returned)
     |         -1 = projection line not othogonal to curve
     |         -2 = projection line not othogonal andnot unique
    \*/
    virtual
    int_type // true if projection is unique and orthogonal
    projection( real_type   qx,
                real_type   qy,
                real_type   offs,
                real_type & x,
                real_type & y,
                real_type & s ) const G2LIB_OVERRIDE;

    virtual
    real_type
    closestPoint( real_type   qx,
                  real_type   qy,
                  real_type & x,
                  real_type & y,
                  real_type & s ) const G2LIB_OVERRIDE;

    virtual
    real_type
    closestPoint( real_type   qx,
                  real_type   qy,
                  real_type   offs,
                  real_type & x,
                  real_type & y,
                  real_type & s ) const G2LIB_OVERRIDE;

    /*\
     |    __ _           _ ____ _____
     |   / _(_)_ __   __| / ___|_   _|
     |  | |_| | '_ \ / _` \___ \ | |
     |  |  _| | | | | (_| |___) || |
     |  |_| |_|_| |_|\__,_|____/ |_|
    \*/

    /*! \brief Find parametric coordinate.
     *
     * We consider the line passing to the point \f$ P \f$
     * with tangent \f$ T \f$ and a point \f$ Q \f$
     * compute the coordinte \f$ s \f$ and \f$ t \f$ such that
     * \f$ Q = P + T s + N t \f$
     * where \f$ P + T s \f$ is the point on the line at coordinate
     * \f$ s \f$ and \f$ N \f$ is the normal to the line obtained by
     * rotating by `90` degree counterclockwise the tangent \f$ T \f$.
     *
     * \param x x-coordinate point
     * \param y y-coordinate point
     * \param s value \f$ s \f$
     * \param t value \f$ t \f$
     */

    virtual
    bool
    findST( real_type   x,
            real_type   y,
            real_type & s,
            real_type & t ) const G2LIB_OVERRIDE;

    // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
    // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

    void
    info( ostream_type & stream ) const G2LIB_OVERRIDE
    { stream << "Clothoid\n" << *this << '\n'; }

    friend
    ostream_type &
    operator << ( ostream_type & stream, ClothoidCurve const & c );

  };

}

#endif

///
/// eof: Clothoid.hh
///
