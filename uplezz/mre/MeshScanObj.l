/* Scanner for Wavefront .obj files */

%option noyywrap
%option prefix="__mreMeshScanObj"

%{
#include "mre/Mesh"
#include <stdio.h>

#define YY_DECL int mre::Mesh::yyScanObj()

using namespace mre;

static char rdbuf[128];

static bool getNextUnsigned(int &n, char *yytext, int &i)
{
	while(yytext[i] && (yytext[i] < '0' || yytext[i] > '9') && yytext[i] != '-')
		i++;

	if(!yytext[i] || sscanf(yytext + i, "%d", &n) != 1) 
		return false;

	while(yytext[i] && ((yytext[i] >= '0' && yytext[i] <= '9') || yytext[i] == '-'))
		i++;

	return true;
}

%}

DIGIT   [0-9]
UINT    {DIGIT}+
INT     "-"?{UINT}
FLOAT   "-"?{UINT}("."{UINT})?(e[ \t]*{INT})?

%%

mtllib[ \t]+.*	{
	sscanf(yytext, "mtllib %s\n", rdbuf);
	mtllibPath = rdbuf;
}

usemtl[ \t]+.*	{
	if(curGroup) {
		sscanf(yytext, "usemtl %s\n", rdbuf);
		string matName = rdbuf;
		curGroup->material = &(materials[matName]);
	}
}

v[ \t]+{FLOAT}[ \t]+{FLOAT}[ \t]+{FLOAT}	{
	Vector3f v;
	sscanf(yytext, "v %f %f %f", &v[0], &v[1], &v[2]);
	vertices.push_back(v);
}

vn[ \t]+{FLOAT}[ \t]+{FLOAT}[ \t]+{FLOAT}	{
	Vector3f vn;
	sscanf(yytext, "vn %f %f %f", &vn[0], &vn[1], &vn[2]);
	normals.push_back(vn);
}

vt[ \t]+{FLOAT}[ \t]+{FLOAT}	{
	Vector2f vt;
	sscanf(yytext, "vt %f %f", &vt[0], &vt[1]);
	texcoords.push_back(vt);
}

g[ \t]+.*	{
	int i = 2, j = 0;
	while(yytext[i] == ' ' || yytext[i] == '\t')
		i++;
	while(yytext[i] && yytext[i] != '\n' && yytext[i] != '\r')
		rdbuf[j++] = yytext[i++];
	rdbuf[j] = 0;
//	sscanf(yytext, "g %s\n", rdbuf);
	string grpName = rdbuf;
	curGroup = &(groups[grpName]);
}

f[ \t]+{INT}[ \t]+{INT}([ \t]+{INT})+	{
	// v
	int v, i = 1;
	Triangle *a = new Triangle();

	getNextUnsigned(a->vindices[0], yytext, i);
	getNextUnsigned(a->vindices[1], yytext, i);
	getNextUnsigned(a->vindices[2], yytext, i);
	curGroup->triangles.push_back(triangles.size());
	triangles.push_back(a);

	while(getNextUnsigned(v, yytext, i)) {
		Triangle *b = a;
		a = new Triangle();
		a->vindices[0] = b->vindices[0];
		a->vindices[1] = b->vindices[2];
		a->vindices[2] = v;
		curGroup->triangles.push_back(triangles.size());
		triangles.push_back(a);
	}
}

f[ \t]+{INT}"/"{INT}[ \t]+{INT}"/"{INT}([ \t]+{INT}"/"{INT})+	{
	// v/t
	int v, t, i = 1;
	Triangle *a = new Triangle();

	getNextUnsigned(a->vindices[0], yytext, i);
	getNextUnsigned(a->tindices[0], yytext, i);
	getNextUnsigned(a->vindices[1], yytext, i);
	getNextUnsigned(a->tindices[1], yytext, i);
	getNextUnsigned(a->vindices[2], yytext, i);
	getNextUnsigned(a->tindices[2], yytext, i);
	curGroup->triangles.push_back(triangles.size());
	triangles.push_back(a);

	while(getNextUnsigned(v, yytext, i) && getNextUnsigned(t, yytext, i)) {
		Triangle *b = a;
		a = new Triangle();
		a->vindices[0] = b->vindices[0];
		a->tindices[0] = b->tindices[0];
		a->vindices[1] = b->vindices[2];
		a->tindices[1] = b->tindices[2];
		a->vindices[2] = v;
		a->tindices[2] = t;
		curGroup->triangles.push_back(triangles.size());
		triangles.push_back(a);
	}
}

f[ \t]+{INT}"//"{INT}[ \t]+{INT}"//"{INT}([ \t]+{INT}"//"{INT})+	{
	// v//n
	int v, n, i = 1;
	Triangle *a = new Triangle();

	getNextUnsigned(a->vindices[0], yytext, i);
	getNextUnsigned(a->nindices[0], yytext, i);
	getNextUnsigned(a->vindices[1], yytext, i);
	getNextUnsigned(a->nindices[1], yytext, i);
	getNextUnsigned(a->vindices[2], yytext, i);
	getNextUnsigned(a->nindices[2], yytext, i);
	curGroup->triangles.push_back(triangles.size());
	triangles.push_back(a);

	while(getNextUnsigned(v, yytext, i) && getNextUnsigned(n, yytext, i)) {
		Triangle *b = a;
		a = new Triangle();
		a->vindices[0] = b->vindices[0];
		a->nindices[0] = b->nindices[0];
		a->vindices[1] = b->vindices[2];
		a->nindices[1] = b->nindices[2];
		a->vindices[2] = v;
		a->nindices[2] = n;
		curGroup->triangles.push_back(triangles.size());
		triangles.push_back(a);
	}
}


f[ \t]+{INT}"/"{INT}"/"{INT}[ \t]+{INT}"/"{INT}"/"{INT}([ \t]+{INT}"/"{INT}"/"{INT})+	{
	// v/t/n
	int v, t, n, i = 1;
	Triangle *a = new Triangle();

	getNextUnsigned(a->vindices[0], yytext, i);
	getNextUnsigned(a->tindices[0], yytext, i);
	getNextUnsigned(a->nindices[0], yytext, i);
	getNextUnsigned(a->vindices[1], yytext, i);
	getNextUnsigned(a->tindices[1], yytext, i);
	getNextUnsigned(a->nindices[1], yytext, i);
	getNextUnsigned(a->vindices[2], yytext, i);
	getNextUnsigned(a->tindices[2], yytext, i);
	getNextUnsigned(a->nindices[2], yytext, i);
	curGroup->triangles.push_back(triangles.size());
	triangles.push_back(a);

	while(getNextUnsigned(v, yytext, i) && getNextUnsigned(t, yytext, i) && getNextUnsigned(n, yytext, i)) {
		Triangle *b = a;
		a = new Triangle();
		a->vindices[0] = b->vindices[0];
		a->tindices[0] = b->tindices[0];
		a->nindices[0] = b->nindices[0];
		a->vindices[1] = b->vindices[2];
		a->tindices[1] = b->tindices[2];
		a->nindices[1] = b->nindices[2];
		a->vindices[2] = v;
		a->tindices[2] = t;
		a->nindices[2] = n;
		curGroup->triangles.push_back(triangles.size());
		triangles.push_back(a);
	}
}

#.*"\r"?"\n"	/* eat up comments */

[ \t\r\n]+	/* eat up whitespace */

.	{
//	fprintf(stderr, "Unrecognized character: %s\n", yytext);
}


%%

int mre::Mesh::loadObj(const string &path)
{
FILE *fp = fopen(path.c_str(), "rt");
int retval = -1;

	if(fp) {
		yyin = fp;
		curGroup = 0;
		retval = yyScanObj();
		fclose(fp);
	}

	return retval;
}


