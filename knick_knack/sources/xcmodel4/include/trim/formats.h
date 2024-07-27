/* formats.h */

#define	REMARK(fd)	while( fgetc(fd)!='\n' && !feof(fd) ); if( feof(fd) ) _ErrorSet(_TRIM_ERR_FREAD,"")

/*--------------- file format .db ( surfaces )  -----
*
*	SURFACE FILENAME
*	REMARK
*	DEGREE(u,v)
*	REMARK
*	CONTROL POINT NUMBER(u,v)
*	REMARK
*	KNOTS NUMBER(u,v)
*	REMARK
*	for every control point (u)
*		for every control point (v)
*			POINTS
*	REMARK
*	for every knot (u)
*		POINT
*	REMARK
*	for every knot (v)
*		POINT
*/

#define	DBS_FILENAMEREAD			"FILENAME : %s\n"
#define	DBS_DEGREESTR				"DEGREE_U_V\n"
#define	DBS_CPNSTR				"N.C.P._U_V\n"
#define	DBS_KNOTNSTR				"KNOTS_U_V\n"
#define	DBS_DEGREE				"%10ld%10ld\n"
#define	DBS_CPN					"%10ld%10ld\n"
#define	DBS_KNOTS_NO				"%10ld%10ld\n"
#define	DBS_COORD_CP				"COORD.C.P.(X,Y,Z,W)\n"
#define	DBS_COORD_KNOTS_U			"KNOTS_U\n"
#define	DBS_COORD_KNOTS_V			"KNOTS_V\n"

#define	DBS_READ_POINT				"%le%le%le%le\n"
#define DBS_READ_KNOT				"%le\n"

#define DBS_GET_FILENAME(fd,name)   	    	if( fscanf( (fd), DBS_FILENAMEREAD,name)<1) _ErrorSet(_TRIM_ERR_FREAD,"")
#define	DBS_GET_DEGREE(fd,du,dv)		if( fscanf( (fd), DBS_DEGREE, &(du), &(dv))<2) _ErrorSet(_TRIM_ERR_FREAD,"")
#define	DBS_GET_CONTROL_POINT_NUMBER(fd,nu,nv)	if( fscanf( (fd), DBS_CPN, &(nu), &(nv))<2) _ErrorSet(_TRIM_ERR_FREAD,"")
#define	DBS_GET_KNOT_NUMBER(fd,nu,nv)		if( fscanf( (fd), DBS_KNOTS_NO, &(nu), &(nv))<2) _ErrorSet(_TRIM_ERR_FREAD,"")
#define	DBS_GET_POINT(fd,x,y,z,w)		if( fscanf( (fd), DBS_READ_POINT,&(x),&(y),&(z),&(w))<4) _ErrorSet(_TRIM_ERR_FREAD,"")
#define DBS_GET_KNOT(fd,k)			if( fscanf( (fd), DBS_READ_KNOT, &(k))<1) _ErrorSet(_TRIM_ERR_FREAD,"")

