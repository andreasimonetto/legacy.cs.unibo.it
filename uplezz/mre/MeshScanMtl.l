/* Scanner for Wavefront .mtl files */

%option noyywrap
%option prefix="__mreMeshScanMtl"

%{
#include "mre/Mesh"
#include "mre/SceneFactory"
#include <stdio.h>

#define YY_DECL int mre::Mesh::yyScanMtl()

using namespace mre;

static char rdbuf[128];
static string curMtl;
static string objPath;

%}

DIGIT   [0-9]
UINT    {DIGIT}+
INT     "-"?{UINT}
FLOAT   "-"?{UINT}("."{UINT})?(e[ \t]*{INT})?

%%

newmtl[ \t]+.*	{
	sscanf(yytext, "newmtl %s\n", rdbuf);
	curMtl = rdbuf;
}

Ns[ \t]+{FLOAT}	{
	float sh;
	sscanf(yytext, "Ns %f\n", &sh);
	sh *= 0.128;
	materials[curMtl].setShininess(sh);
}

Kd[ \t]+{FLOAT}[ \t]+{FLOAT}[ \t]+{FLOAT}	{
	float r, g, b;
	sscanf(yytext, "Kd %f %f %f\n", &r, &g, &b);
	materials[curMtl].setDiffuse(r, g, b);
}

Ka[ \t]+{FLOAT}[ \t]+{FLOAT}[ \t]+{FLOAT}	{
	float r, g, b;
	sscanf(yytext, "Ka %f %f %f\n", &r, &g, &b);
	materials[curMtl].setAmbient(r, g, b);
}

Ks[ \t]+{FLOAT}[ \t]+{FLOAT}[ \t]+{FLOAT}	{
	float r, g, b;
	sscanf(yytext, "Ks %f %f %f\n", &r, &g, &b);
	materials[curMtl].setSpecular(r, g, b);
}

map_Kd[ \t]+.*	{
	int j = 0, i = 6;
	while(yytext[i] == ' ' || yytext[i] == '\t') 
		i++;
	while(yytext[i] && yytext[i] != '\n' && yytext[i] != '\r')
		rdbuf[j++] = yytext[i++];
	rdbuf[j] = 0;

	materials[curMtl].attachTexture(SceneFactory::getInstance()->loadTexture2D(objPath + string(rdbuf)));
}

Tf[ \t]+{FLOAT}[ \t]+{FLOAT}[ \t]+{FLOAT}	{
	//sscanf(yytext, "Tf %f %f %f\n", &(materials[curMtl].emission[0]),
	//	&(materials[curMtl].emission[1]), &(materials[curMtl].emission[2]));
}

#.*"\r"?"\n"	/* eat up comments */

[ \t\r\n]+	/* eat up whitespace */

.	{
//	fprintf(stderr, "Unrecognized character: %s\n", yytext);
}


%%

int mre::Mesh::loadMtl(const string &path)
{
FILE *fp;
int retval = -1;

	objPath = meshPath.substr(0, meshPath.find_last_of('/') + 1);
	fp = fopen((objPath + path).c_str(), "rt");	
	if(fp) {
		yyin = fp;
		retval = yyScanMtl();
		fclose(fp);
	}

	return retval;
}

