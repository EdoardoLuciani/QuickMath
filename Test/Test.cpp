#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <stdio.h>
#include <stdarg.h>
#include <Windows.h>
#include <codecvt>
#include <locale>
#include <string.h>

#include <plplot/plplot.h>
#include <plplot/plplotP.h>
#include "cxxplplot.h"

#define NSIZE    300

int count_digits(int arg);
void PLOutput(const char *driver, int x_ratio, int y_ratio, const char *filename);
void linspace(double *v, int min, int max, int n_nums);
void cmap1_init(int);

/*int main(int argc, char *argv[]) {

	PLFLT x[NSIZE], y[NSIZE];
	
	linspace(x, -50, -50,NSIZE);

	for (int i = 0; i < NSIZE; i++) {
		y[i] = cos(x[i]);
	}

	cxxplplot plot("ntk", 70, 70);
	plot.SetPage(0);

	cxxplplot::Box2DOptions setting;
	setting.xmin=-50;
	setting.xmax=50;
	setting.ymin = -10;
	setting.ymax = 10;
	plot.BuildBox2D(setting);
	plot.Plot2DLine(cxxplplot::Color::magenta,cxxplplot::Line2dStyle::contiguous ,x, y, NSIZE);
	
}*/

#define XPTS    50          // Data points in x
#define YPTS    50             // Data points in y


int main(int argc, char *argv[]) {

	PLOutput("svg", 70, 70, "ciao.svg");

	PLFLT    *x, *y, **z;
	PLFLT    dx = 20 / (PLFLT)(XPTS - 1);
	PLFLT    dy = 20 / (PLFLT)(YPTS - 1);
	PLFLT    zmin, zmax;

	// Parse and process command line arguments 
	// Initialize plplot

	plinit();
	x = (PLFLT *)calloc(XPTS, sizeof(PLFLT));
	y = (PLFLT *)calloc(YPTS, sizeof(PLFLT));
	plAlloc2dGrid(&z, XPTS, YPTS);

	linspace(x, -1, 1, XPTS);
	linspace(y, -1, 1, YPTS);

	for (int i = 0; i < XPTS; i++) {
		for (int j = 0; j < YPTS; j++) {
			z[i][j] = 1 / (15*( pow(x[i], 2) + pow(y[j], 2)));

			//1 / (15*( pow(x[i], 2) + pow(y[j], 2)));
			//sin(10 * (pow(x[i], 2) + pow(y[j], 2))) / 10;
			//sin(5 * x[i])*cos(5 * y[j]) / 5;
			//pow(y[j], 2) + pow(x[i], 2);

			//sign(xx) * atan(xx * 80) / 6 * sign(-yy - xx + 1) * sign(-yy + xx + 1) * 5 - 1.01;
		}
	}

	plMinMax2dGrid((PLFLT_MATRIX)z, XPTS, YPTS, &zmax, &zmin);

	c_pllightsource(1., 1., 1.);

	pladv(0);
	plvpor(0.0, 1.0, 0.0, 0.9);
	plwind(-1.0, 1.0, -0.9, 1.1);

	plcol0(3);
	plmtex("t", 1.0, 0.5, 0.5, "Ripple");

	plcol0(1);
	plw3d(1.0, 1.0, 1.0, x[0], x[XPTS - 1], y[0], y[YPTS - 1], zmin, zmax, 40, 45);

	plbox3("bnstu", "x axis", 0.0, 0,
		"bnstu", "y axis", 0.0, 0,
		"bcdmnstuv", "z axis", 0.0, 0);

	cmap1_init(0);
	plsurf3d(x, y, (PLFLT_MATRIX)z, XPTS, YPTS, MAG_COLOR + FACETED, NULL, 0);

	free((void *)x);
	free((void *)y);
	plFree2dGrid(z, XPTS, YPTS);

	plend();

	exit(0);
}


void linspace(double *v, int min, int max, int n_nums) {
	double dv = (max - min) / ((double)n_nums-1);

		for (int i = 0; i < n_nums; i++) {
			v[i] = min + i * dv;
		}
}

void PLOutput(const char *driver, int x_ratio, int y_ratio, const char *filename) {

	int width = (GetSystemMetrics(SM_CXSCREEN) * x_ratio) / 100;
	int height = (GetSystemMetrics(SM_CYSCREEN) * y_ratio) / 100;

	char *buf = new char[count_digits(width) + count_digits(height) + 2];
	SecureZeroMemory(buf, count_digits(width) + count_digits(height) + 2);

	_itoa(width, buf, 10);
	buf[strlen(buf)] = 'x';
	_itoa(height, (buf + strlen(buf)), 10);

	plsdev(driver);

	if (!strcmp(driver, "ntk")) {
		plsetopt("geometry", buf);
	}
	else if (!strcmp(driver, "svg")) {
		plsfnam(filename);
	}

	delete[] buf;
}

int count_digits(int arg) {
	return snprintf(NULL, 0, "%d", arg) - (arg < 0);
}

static void cmap1_init(int gray) {
	PLFLT i[2], h[2], l[2], s[2];

	i[0] = 0.0;         // left boundary
	i[1] = 1.0;         // right boundary

	if (gray)
	{
		h[0] = 0.0;     // hue -- low: red (arbitrary if s=0)
		h[1] = 0.0;     // hue -- high: red (arbitrary if s=0)

		l[0] = 0.5;     // lightness -- low: half-dark
		l[1] = 1.0;     // lightness -- high: light

		s[0] = 0.0;     // minimum saturation
		s[1] = 0.0;     // minimum saturation
	}
	else
	{
		h[0] = 240; // blue -> green -> yellow ->
		h[1] = 0;   // -> red

		l[0] = 0.6;
		l[1] = 0.6;

		s[0] = 0.8;
		s[1] = 0.8;
	}

	plscmap1n(256);
	c_plscmap1l(0, 2, i, h, l, s, NULL);
}