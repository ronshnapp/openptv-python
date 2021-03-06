/****************************************************************************

Routine:	       	imgcoord.c

Author/Copyright:      	Hans-Gerd Maas

Address:	       	Institute of Geodesy and Photogrammetry
                        ETH - Hoenggerberg
		       	CH - 8093 Zurich

Creation Date:	       	22.4.88

Description:	       	computes x', y' from given Point and orientation
	       		(see: Kraus)

Routines contained:

****************************************************************************/

#include "ptv.h"
#include <optv/parameters.h>
#include "imgcoord.h"

/* img_coord() calculates the coordinates on the iomage plane (metric) of a 3D
   point in the observed volume.
   
   Arguments:
   int cam - the camera number. This is needed only in cases where the LUT 
        structure is used, otherwise it is ignored.
   ...
 */
void img_coord (int cam, double X, double Y, double Z, 
    Exterior Ex, Interior I, Glass G, ap_52 ap, mm_np mm, double *x, double *y)
{
  double deno, r, dx, dy;
  Exterior Ex_t;
  double X_t,Y_t,Z_t,cross_p[3],cross_c[3];
  //trans
  trans_Cam_Point(Ex,mm,G,X,Y,Z,&Ex_t,&X_t,&Y_t,&Z_t,&cross_p,&cross_c);
  multimed_nlay_v2 (Ex_t, Ex, mm, X_t, Y_t, Z_t, &X_t, &Y_t, cam);
  back_trans_Point(X_t,Y_t,Z_t,mm, G,cross_p,cross_c,&X,&Y,&Z);

  X -= Ex.x0;  Y -= Ex.y0;  Z -= Ex.z0;

  deno = Ex.dm[0][2] * X + Ex.dm[1][2] * Y + Ex.dm[2][2] * Z;
  *x = I.xh - I.cc * (Ex.dm[0][0]*X + Ex.dm[1][0]*Y + Ex.dm[2][0]*Z) / deno;
  *y = I.yh - I.cc * (Ex.dm[0][1]*X + Ex.dm[1][1]*Y + Ex.dm[2][1]*Z) / deno;

  r = sqrt (*x * *x + *y * *y);

  dx = (*x) * (ap.k1*r*r + ap.k2*r*r*r*r + ap.k3*r*r*r*r*r*r)
    + ap.p1 * (r*r + 2*(*x)*(*x)) + 2*ap.p2*(*x)*(*y);
  dy = (*y) * (ap.k1*r*r + ap.k2*r*r*r*r + ap.k3*r*r*r*r*r*r)
    + ap.p2 * (r*r + 2*(*y)*(*y)) + 2*ap.p1*(*x)*(*y);

  *x += dx;
  *y += dy;

  *x = ap.scx * (*x) - sin(ap.she) * (*y);
  *y = cos(ap.she) * (*y);

}

void img_xy (X,Y,Z, Ex, I, G, x,y)

double 	X,Y,Z, *x,*y;
Exterior Ex;
Interior I;
Glass    G;

{
  double deno;

  deno = Ex.dm[0][2] * (X-Ex.x0)
    + Ex.dm[1][2] * (Y-Ex.y0)
    + Ex.dm[2][2] * (Z-Ex.z0);

  *x = I.xh - I.cc *  (Ex.dm[0][0] * (X-Ex.x0)
		       + Ex.dm[1][0] * (Y-Ex.y0)
		       + Ex.dm[2][0] * (Z-Ex.z0)) / deno;

  *y = I.yh - I.cc *  (Ex.dm[0][1] * (X-Ex.x0)
		       + Ex.dm[1][1] * (Y-Ex.y0)
		       + Ex.dm[2][1] * (Z-Ex.z0)) / deno;
}

void img_xy_mm_geo (int cam, double X, double Y, double Z, 
    Exterior Ex, Interior I, Glass G, mm_np mm, double *x, double *y)
{
  double deno;
  Exterior Ex_t;
  double X_t,Y_t,Z_t,cross_p[3],cross_c[3],Xh,Yh,Zh;

  trans_Cam_Point(Ex,mm,G,X,Y,Z,&Ex_t,&X_t,&Y_t,&Z_t,&cross_p,&cross_c);
  multimed_nlay_v2 (Ex_t, Ex, mm, X_t, Y_t, Z_t, &X_t, &Y_t, cam);
  back_trans_Point(X_t,Y_t,Z_t,mm, G,cross_p,cross_c,&X,&Y,&Z);

  deno = Ex.dm[0][2] * (X-Ex.x0)
    + Ex.dm[1][2] * (Y-Ex.y0)
    + Ex.dm[2][2] * (Z-Ex.z0);

  *x = - I.cc *  (Ex.dm[0][0] * (X-Ex.x0)
		  + Ex.dm[1][0] * (Y-Ex.y0)
		  + Ex.dm[2][0] * (Z-Ex.z0)) / deno;

  *y = - I.cc *  (Ex.dm[0][1] * (X-Ex.x0)
		  + Ex.dm[1][1] * (Y-Ex.y0)
		  + Ex.dm[2][1] * (Z-Ex.z0)) / deno;
}

