/* OCEABRDF.f -- translated by f2c (version 19970805).
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

static doublereal c_b2 = 3.52;
static integer c__24 = 24;
static integer c__48 = 48;
static real c_b6 = (float)0.;

/*<    >*/
/* Subroutine */ int oceabrdf_(real *pws, real *paw, real *xsal, real *pcl, 
	real *pwl, real *rfoam, real *rwat, real *rglit, integer *mu, integer 
	*np, real *rm, real *rp, real *brdfint)
{
    /* Initialized data */

    static real ref[39] = { (float).22,(float).22,(float).22,(float).22,(
	    float).22,(float).22,(float).215,(float).21,(float).2,(float).19,(
	    float).175,(float).155,(float).13,(float).08,(float).1,(float)
	    .105,(float).1,(float).08,(float).045,(float).055,(float).065,(
	    float).06,(float).055,(float).04,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0. };

    /* System generated locals */
    integer rm_offset, brdfint_dim1, brdfint_offset, i__1, i__2;
    real r__1;
    doublereal d__1;

    /* Builtin functions */
    double atan(doublereal), acos(doublereal), sqrt(doublereal), pow_dd(
	    doublereal *, doublereal *), cos(doublereal), sin(doublereal), 
	    asin(doublereal);

    /* Local variables */
    extern /* Subroutine */ int sunglint_(real *, real *, real *, real *, 
	    real *, real *, real *, real *);
    real a, c__;
    integer j, k, m, n;
    real w, fa[48], fi, n12, ta[24], ni, pi, nr, wl, tp, rw, tw;
    extern /* Subroutine */ int morcasiwat_(real *, real *, real *);
    real fac, wfa[48], fip, rog, tds;
    integer iwl;
    real wta[24], rwc, tdv, rwb, azw, wlp, phi1, phi2, pond, rogp, wspd, summ,
	     teta1, teta2, ref_i__, tetas;
    extern /* Subroutine */ int gauss_(real *, real *, real *, real *, 
	    integer *);
    real tetav;
    extern /* Subroutine */ int indwat_(real *, real *, real *, real *);


/* INPUT:  pws=wind speed (in m/s) */
/*         paw= azim. of sun - azim. of wind (in deg.) */
/* 	  xsal=salinity (in ppt) */
/* 	  pcl=pigment concentration (in mg.m-3) */
/*         pwl=wavelength of the computation (in micrometer) */
/*         mu=number of zenith angle */
/*         np=number of azimuth */
/*        rm=cosine of Gauss's angles for angles between -PI/2 and PI/2 de
g*/
/*         rp=Gauss's angles for angles between 0 and 2*PI */
/* OUTPUT: brdfint(j,k)=the total reflectance of the sea water */
/*        rfoam= the effective reflectance of the foam (foam coverage x fo
am reflectance)*/
/* 	  rwat= the reflectance of the water (just above the surface) */
/*         rglit= the reflectance of the sunglint */

/*<       parameter (nta=24,nfa=48) >*/
/*<       integer np,mu,k,j,m,n,iwl >*/
/*<       real rm(-mu:mu),rp(np),brdfint(-mu:mu,np) >*/
/*<       real teta1,teta2,phi1,phi2,ta(nta),fa(nfa),wta(nta),wfa(nfa) >*/
/*<       real Ref(39) >*/
/*<       real pwl,paw,pcl,pws,wl,wspd,C,azw,xsal >*/
/*<       real rfoam,rwat,rglit >*/
/*<       real pi,fac,nr,ni,n12 >*/
/*<       real tetas,w,wlp,ref_i,rwc,rw,tds,summ,tp,fip >*/
/*<       real rogp,pond,tetav,tw,tdv,fi,rog,a,rwb >*/
/* effective reflectance of the whitecaps (Koepke, 1984) */
/*<    >*/
    /* Parameter adjustments */
    rm_offset = -(*mu);
    rm -= rm_offset;
    brdfint_dim1 = *mu - (-(*mu)) + 1;
    brdfint_offset = -(*mu) + brdfint_dim1;
    brdfint -= brdfint_offset;
    --rp;

    /* Function Body */
/* conversion of parameter */
/*<       C=pcl >*/
    c__ = *pcl;
/*<       wspd=pws >*/
    wspd = *pws;
/*<       azw=paw >*/
    azw = *paw;
/*<       wl=pwl >*/
    wl = *pwl;
/*<       pi=atan(1.)*4. >*/
    pi = atan((float)1.) * (float)4.;
/*<       fac=pi/180. >*/
    fac = pi / (float)180.;
/*<       tetas=acos(rm(0))/fac >*/
    tetas = acos(rm[0]) / fac;
/* COMPUTE INDEX OF WATER */
/*<       call indwat(wl,xsal,nr,ni) >*/
    indwat_(&wl, xsal, &nr, &ni);
/*      write(6,'(1A,1X,2(F10.5,1X))') "nr,ni",nr,ni */
/*<       n12=sqrt(nr*nr+ni*ni) >*/
    n12 = sqrt(nr * nr + ni * ni);
/* COMPUTE WHITECAPS REFLECTANCE (LAMBERTIAN) */
/*<       W=2.95e-06*(wspd**3.52) >*/
    d__1 = (doublereal) wspd;
    w = pow_dd(&d__1, &c_b2) * (float)2.95e-6;
/*<       iwl=1+int((wl-0.2)/0.1) >*/
    iwl = (integer) ((wl - (float).2) / (float).1) + 1;
/*<       wlp=0.5+(iwl-1)*0.1 >*/
    wlp = (iwl - 1) * (float).1 + (float).5;
/*<       Ref_i=ref(iwl+1)+(wl-wlp)/0.1*(ref(iwl)-ref(iwl+1)) >*/
    ref_i__ = ref[iwl] + (wl - wlp) / (float).1 * (ref[iwl - 1] - ref[iwl]);
/*<       Rwc=W*Ref_i >*/
    rwc = w * ref_i__;
/* COMPUTE BACKSCATTERED REFLECTANCE FROM THE SEA WATER (LAMBERTIAN) */
/*  water reflectance below the sea surface */
/*<       call MORCASIWAT(wl,C,Rw) >*/
    morcasiwat_(&wl, &c__, &rw);
/* call gauss quadrature */
/*<       tds=1.0 >*/
    tds = (float)1.;
/*<       tdv=1.0 >*/
    tdv = (float)1.;
/*<       if (Rw.gt.0.0001) then >*/
    if (rw > (float)1e-4) {
/*<         teta1=0. >*/
	teta1 = (float)0.;
/*<         teta2=pi/2. >*/
	teta2 = pi / (float)2.;
/*<         call gauss(teta1,teta2,ta,wta,nta) >*/
	gauss_(&teta1, &teta2, ta, wta, &c__24);
/*<         phi1=0. >*/
	phi1 = (float)0.;
/*<         phi2=2.*pi >*/
	phi2 = pi * (float)2.;
/*<         call gauss(phi1,phi2,fa,wfa,nfa) >*/
	gauss_(&phi1, &phi2, fa, wfa, &c__48);
/* COMPUTE DOWNWARD TRANSMISSION FUNCTION */
/*<         tds=0. >*/
	tds = (float)0.;
/*<         summ=0. >*/
	summ = (float)0.;
/*<         do k=1,nfa >*/
	for (k = 1; k <= 48; ++k) {
/*<         do j=1,nta >*/
	    for (j = 1; j <= 24; ++j) {
/*<           tp=ta(j)/fac >*/
		tp = ta[j - 1] / fac;
/*<           fip=fa(k)/fac >*/
		fip = fa[k - 1] / fac;
/*<           call sunglint(wspd,nr,ni,azw,tetas,tp,fip,rogp) >*/
		sunglint_(&wspd, &nr, &ni, &azw, &tetas, &tp, &fip, &rogp);
/*<           pond=cos(ta(j))*sin(ta(j))*wfa(k)*wta(j) >*/
		pond = cos(ta[j - 1]) * sin(ta[j - 1]) * wfa[k - 1] * wta[j - 
			1];
/*<           summ=summ+pond >*/
		summ += pond;
/*<           tds=tds+rogp*pond >*/
		tds += rogp * pond;
/*<         enddo >*/
	    }
/*<         enddo >*/
	}
/*<         tds=1.-tds/summ >*/
	tds = (float)1. - tds / summ;
/*<       endif >*/
    }
/* Compute glint contribution,transmission terms and total reflectance */
/*<       Do 1 j=1,mu >*/
    i__1 = *mu;
    for (j = 1; j <= i__1; ++j) {
/*<         tetav=acos(rm(j))/fac >*/
	tetav = acos(rm[j]) / fac;
/* compute upward transmission */
/*<         if (Rw.gt.0.0001) then >*/
	if (rw > (float)1e-4) {
/*<           tw=asin(sin(tetav*fac)/nr)/fac >*/
	    tw = asin(sin(tetav * fac) / nr) / fac;
/*<           tdv=0. >*/
	    tdv = (float)0.;
/*<           summ=0. >*/
	    summ = (float)0.;
/*<           do n=1,nfa >*/
	    for (n = 1; n <= 48; ++n) {
/*<           do m=1,nta >*/
		for (m = 1; m <= 24; ++m) {
/*<             tp=ta(m)/fac >*/
		    tp = ta[m - 1] / fac;
/*<             fip=fa(n)/fac >*/
		    fip = fa[n - 1] / fac;
/* here we assume no aborption */
/*<             call sunglint(wspd,1./nr,0.0,azw,tw,tp,fip,rogp) >*/
		    r__1 = (float)1. / nr;
		    sunglint_(&wspd, &r__1, &c_b6, &azw, &tw, &tp, &fip, &
			    rogp);
/*<             pond=cos(ta(m))*sin(ta(m))*wfa(n)*wta(m) >*/
		    pond = cos(ta[m - 1]) * sin(ta[m - 1]) * wfa[n - 1] * wta[
			    m - 1];
/*<             summ=summ+pond >*/
		    summ += pond;
/*<             tdv=tdv+rogp*pond >*/
		    tdv += rogp * pond;
/*<           enddo >*/
		}
/*<           enddo >*/
	    }
/*<           tdv=1.-tdv/summ >*/
	    tdv = (float)1. - tdv / summ;
/*<         endif >*/
	}
/*<         Do 2 k=1,np >*/
	i__2 = *np;
	for (k = 1; k <= i__2; ++k) {
/*<            if (j.eq.mu) then >*/
	    if (j == *mu) {
/*<              fi=rm(-mu) >*/
		fi = rm[-(*mu)];
/*<            else >*/
	    } else {
/*<              fi=(rp(k)+rm(-mu)) >*/
		fi = rp[k] + rm[-(*mu)];
/*<            endif >*/
	    }
/*<            if (fi.lt.0.) fi=fi+2.*pi >*/
	    if (fi < (float)0.) {
		fi += pi * (float)2.;
	    }
/*<            if (fi.gt.(2.*pi)) fi=fi-2.*pi >*/
	    if (fi > pi * (float)2.) {
		fi -= pi * (float)2.;
	    }
/*<            fi=fi/fac >*/
	    fi /= fac;
/* SUNGLINT REFLECTANCE */
/*<            call sunglint(wspd,nr,ni,azw,tetas,tetav,fi,rog) >*/
	    sunglint_(&wspd, &nr, &ni, &azw, &tetas, &tetav, &fi, &rog);
/*  water reflectance above the sea surface */
/* for explanation on value of a see OCEAALBE.f */
/*<            a=0.485 >*/
	    a = (float).485;
/* add change in solid angle from under to above to surface */
/* that account for 1/(n12*n12) decrease in sea water directional 
*/
/* reflectance */
/*<            Rwb=(1/(n12*n12))*tds*tdv*Rw/(1-a*Rw) >*/
	    rwb = 1 / (n12 * n12) * tds * tdv * rw / (1 - a * rw);
/* TOTAL REFLECTANCE OF SEA WATER */
/*<            brdfint(j,k)=Rwc+(1-W)*Rog+(1-Rwc)*Rwb >*/
	    brdfint[j + k * brdfint_dim1] = rwc + (1 - w) * rog + (1 - rwc) * 
		    rwb;
/*< 	   rfoam=rwc >*/
	    *rfoam = rwc;
/*< 	   rwat=(1-Rwc)*Rwb >*/
	    *rwat = (1 - rwc) * rwb;
/*< 	   rglit=(1-W)*Rog >*/
	    *rglit = (1 - w) * rog;
/*<  2      continue >*/
/* L2: */
	}
/*<  1    continue >*/
/* L1: */
    }
/*<       return >*/
    return 0;
/*<       end >*/
} /* oceabrdf_ */

#ifdef __cplusplus
	}
#endif
