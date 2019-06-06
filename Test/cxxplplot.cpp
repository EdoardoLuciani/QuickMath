#include "cxxplplot.h"

cxxplplot::cxxplplot(const std::string& output_driver, const int x_ratio, const int y_ratio) {

	#ifdef _WINDOWS_
	int width = (GetSystemMetrics(SM_CXSCREEN) * x_ratio) / 100;
	int height = (GetSystemMetrics(SM_CYSCREEN) * y_ratio) / 100;
	#elif
	//TODO: code here for finding resolution on linux, cannot test it right now HOOOOOOOOOOLP
	#endif

	int digits = CountDigits(width) + CountDigits(height) + 2;

	char *buf = new char[digits];
	memset(buf,0,digits);

	_itoa(width, buf, 10);
	buf[strlen(buf)] = 'x';
	_itoa(height, (buf + strlen(buf)), 10);

	plsdev(output_driver.c_str());
	plsetopt("geometry", buf);
	delete[] buf;
	plinit();
}

cxxplplot::cxxplplot(const std::string& output_filename_type, const std::string& filename) {
	plsdev(output_filename_type.c_str());
	plsfnam(filename.c_str());
	plinit();
}

cxxplplot::cxxplplot(void) {
	plinit();
}

cxxplplot::~cxxplplot() {
	plend();
}

void cxxplplot::SetPage(int page) { 
	pladv(page); 
	pages_++; 
}

int cxxplplot::GetTotalPages(void) { return pages_; };

void cxxplplot::BuildBox2D(const cxxplplot::Box2DOptions& options) {

	double sx_v = 0, ex_v = 1, sy_v = 0, ey_v = 1;

	if (!options.x_label.empty()) {
		sy_v = 0.1;
	}
	if (!options.y_label.empty()) {
		sx_v = 0.1;
	}
	if (!options.plot_label.empty()) {
		ey_v = 0.9;
	}
	
	c_plvpor(sx_v, ex_v, sy_v, ey_v);
	c_plwind(options.xmin, options.xmax, options.ymin, options.ymax);

	c_plcol0((PLINT)options.label_col);
	c_pllab(options.x_label.c_str(),options.x_label.c_str(), options.plot_label.c_str());
	
	c_plcol0((PLINT)options.axis_col);
	c_plbox(options.x_axis_options.c_str(), 0.0, 0, options.y_axis_options.c_str(), 0.0, 0);
}

void cxxplplot::Plot2DLine(cxxplplot::Color line_color, cxxplplot::Line2dStyle style, double *x, double *y, int points, std::string custom_characters) {
	c_plcol0((PLINT)line_color);

	if (style == cxxplplot::Line2dStyle::custom) {
		c_plstring(points, x, y, custom_characters.c_str());
	}
	else {
		c_pllsty((PLINT)style);
		c_plline(points, x, y);
	}
}

int cxxplplot::CountDigits(int arg) {
	return snprintf(NULL, 0, "%d", arg) - (arg < 0);
}
