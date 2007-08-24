/* POSSOL.f -- translated by f2c (version 19970805).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#ifdef __cplusplus
extern "C" {
#endif

/* OTB patches: replace "f2c.h" by "otb_6S_f2c.h" */
/*#include "f2c.h"*/
#include "otb_6S_f2c.h"

/* Table of constant values */

static real c_b5 = (float)1.;

/*<    >*/
/* Subroutine */ int possol_(integer *month, integer *jday, real *tu, real *
	xlon, real *xlat, real *asol, real *phi0)
{
    integer ia;
    extern /* Subroutine */ int day_number__(integer *, integer *, integer *, 
	    integer *), print_error__(char *, ftnlen);
    integer nojour;
    extern /* Subroutine */ int pos_fft__(integer *, real *, real *, real *, 
	    real *, real *);

/*<       real    tu,xlon,xlat,asol,phi0 >*/
/*<       integer month,jday,ia,nojour >*/
/*     solar position (zenithal angle asol,azimuthal angle phi0 */
/*                     in degrees) */
/*     jday is the number of the day in the month */
/*<       ia = 0 >*/
    ia = 0;
/*<       call day_number(jday,month,ia,nojour) >*/
    day_number__(jday, month, &ia, &nojour);
/*<       call  pos_fft (nojour, tu, xlon, xlat, asol, phi0) >*/
    pos_fft__(&nojour, tu, xlon, xlat, asol, phi0);
/*<    >*/
    if (*asol > (float)90.) {
	print_error__("The sun is not raised", 21L);
    }
/*<       return >*/
    return 0;
/*<       end >*/
} /* possol_ */

/*<       subroutine day_number(jday,month,ia,j) >*/
/* Subroutine */ int day_number__(integer *jday, integer *month, integer *ia, 
	integer *j)
{
/*<       integer jday, month, ia, j >*/
/*<       if (month.le.2) then >*/
    if (*month <= 2) {
/*<                       j=31*(month-1)+jday >*/
	*j = (*month - 1) * 31 + *jday;
/*< 		      return >*/
	return 0;
/*< 		      endif >*/
    }
/*<       if (month.gt.8) then >*/
    if (*month > 8) {
/*<                       j=31*(month-1)-((month-2)/2)-2+jday >*/
	*j = (*month - 1) * 31 - (*month - 2) / 2 - 2 + *jday;
/*< 		      else >*/
    } else {
/*<                       j=31*(month-1)-((month-1)/2)-2+jday >*/
	*j = (*month - 1) * 31 - (*month - 1) / 2 - 2 + *jday;
/*< 		      endif >*/
    }
/*<       if(ia.ne.0 .and. mod(ia,4).eq.0) j=j+1 >*/
    if (*ia != 0 && *ia % 4 == 0) {
	++(*j);
    }
/*<       return >*/
    return 0;
/*<       end >*/
} /* day_number__ */

/*<       subroutine pos_fft (j,tu,xlon,xlat,asol,phi0) >*/
/* Subroutine */ int pos_fft__(integer *j, real *tu, real *xlon, real *xlat, 
	real *asol, real *phi0)
{
    /* Builtin functions */
    double cos(doublereal), sin(doublereal), asin(doublereal), r_sign(real *, 
	    real *);

    /* Local variables */
    real a1, a2, a3, a4, a5, b1, b2, b3, b4, b5, b6, b7, ah, et, az, xj, pi2, 
	    caz, xla, tet, tsm, tsv, elev, azim, delta, amuzero;

/*<    >*/
/*<       integer j >*/
/*<       parameter (pi=3.14159265,fac=pi/180.) >*/
/*     solar position (zenithal angle asol,azimuthal angle phi0 */
/*                     in degrees) */
/*     j is the day number in the year */

/*    mean solar time (heure decimale) */
/*<       tsm=tu+xlon/15. >*/
    tsm = *tu + *xlon / (float)15.;
/*<       xla=xlat*fac >*/
    xla = *xlat * (float).017453292500000002;
/*<       xj=float(j) >*/
    xj = (real) (*j);
/*<       tet=2.*pi*xj/365. >*/
    tet = xj * (float)6.2831853000000004 / (float)365.;
/*    time equation (in mn.dec) */
/*<       a1=.000075 >*/
    a1 = (float)7.5e-5;
/*<       a2=.001868 >*/
    a2 = (float).001868;
/*<       a3=.032077 >*/
    a3 = (float).032077;
/*<       a4=.014615 >*/
    a4 = (float).014615;
/*<       a5=.040849 >*/
    a5 = (float).040849;
/*<       et=a1+a2*cos(tet)-a3*sin(tet)-a4*cos(2.*tet)-a5*sin(2.*tet) >*/
    et = a1 + a2 * cos(tet) - a3 * sin(tet) - a4 * cos(tet * (float)2.) - a5 *
	     sin(tet * (float)2.);
/*<       et=et*12.*60./pi >*/
    et = et * (float)12. * (float)60. / (float)3.14159265;
/*     true solar time */
/*<       tsv=tsm+et/60. >*/
    tsv = tsm + et / (float)60.;
/*<       tsv=(tsv-12.) >*/
    tsv += (float)-12.;
/*     hour angle */
/*<       ah=tsv*15.*fac >*/
    ah = tsv * (float)15. * (float).017453292500000002;
/*     solar declination   (in radian) */
/*<       b1=.006918 >*/
    b1 = (float).006918;
/*<       b2=.399912 >*/
    b2 = (float).399912;
/*<       b3=.070257 >*/
    b3 = (float).070257;
/*<       b4=.006758 >*/
    b4 = (float).006758;
/*<       b5=.000907 >*/
    b5 = (float)9.07e-4;
/*<       b6=.002697 >*/
    b6 = (float).002697;
/*<       b7=.001480 >*/
    b7 = (float).00148;
/*<    >*/
    delta = b1 - b2 * cos(tet) + b3 * sin(tet) - b4 * cos(tet * (float)2.) + 
	    b5 * sin(tet * (float)2.) - b6 * cos(tet * (float)3.) + b7 * sin(
	    tet * (float)3.);
/*     elevation,azimuth */
/*<       amuzero=sin(xla)*sin(delta)+cos(xla)*cos(delta)*cos(ah) >*/
    amuzero = sin(xla) * sin(delta) + cos(xla) * cos(delta) * cos(ah);
/*<       elev=asin(amuzero) >*/
    elev = asin(amuzero);
/*<       az=cos(delta)*sin(ah)/cos(elev) >*/
    az = cos(delta) * sin(ah) / cos(elev);
/*<       if ( (abs(az)-1.000).gt.0.00000) az = sign(1.,az) >*/
    if (dabs(az) - (float)1. > (float)0.) {
	az = r_sign(&c_b5, &az);
    }
/*<       caz=(-cos(xla)*sin(delta)+sin(xla)*cos(delta)*cos(ah))/cos(elev) >*/
    caz = (-cos(xla) * sin(delta) + sin(xla) * cos(delta) * cos(ah)) / cos(
	    elev);
/*<       azim=asin(az) >*/
    azim = asin(az);
/*<       if(caz.le.0.) azim=pi-azim >*/
    if (caz <= (float)0.) {
	azim = (float)3.14159265 - azim;
    }
/*<       if(caz.gt.0.and.az.le.0) azim=2*pi+azim >*/
    if (caz > (float)0. && az <= (float)0.) {
	azim += (float)6.2831853000000004;
    }
/*<       azim=azim+pi >*/
    azim += (float)3.14159265;
/*<       pi2=2*pi >*/
    pi2 = (float)6.2831853000000004;
/*<       if(azim.gt.pi2) azim=azim-pi2 >*/
    if (azim > pi2) {
	azim -= pi2;
    }
/*<       elev=elev*180./pi >*/
    elev = elev * (float)180. / (float)3.14159265;
/*     conversion in degrees */
/*<       asol=90.-elev >*/
    *asol = (float)90. - elev;
/*<       phi0=azim/fac >*/
    *phi0 = azim / (float).017453292500000002;
/*<       return >*/
    return 0;
/*<       end >*/
} /* pos_fft__ */

#ifdef __cplusplus
	}
#endif
