#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <string>
#include <Windows.h>
#include <plplot/plplot.h>
#include <plplot/plplotP.h>



#if defined(_WIN32) || defined(_WIN64)
#include <Windows.h>
#elif __linux__
//TODO: add here code for linux header for finding resolution
#endif


class cxxplplot {

public:

	enum class Color {
		black,
		red,
		yellow,
		green,
		aquamarine,
		pink,
		wheat,
		grey,
		brown,
		blue,
		blue_violet,
		cyan,
		turquoise,
		magenta,
		salmon,
		white
	};

	enum class Line2dStyle {
		contiguous = 1,		//contiguous line	
		s_d_s_p = 2,		//short dashed and short spaced
		l_d_l_s = 3,		//long dashed and long spaced
		l_d_s_s = 4,		//long dashed and short spaced
		unk1 = 5,
		unk2 = 6,
		unk3 = 7,
		unk4 = 8,
		custom = 9
	};


	typedef struct Box2DOptions {

		cxxplplot::Color axis_col = cxxplplot::Color::red;
		double xmin;
		double xmax;
		double ymin;
		double ymax;

		cxxplplot::Color label_col = cxxplplot::Color::green;
		std::string plot_label;
		std::string x_label;
		std::string y_label;

		std::string x_axis_options = "bnstu";
		std::string y_axis_options = "bnstu";

	} Box2DOptions;

	cxxplplot(const std::string& output_driver, const int x_ratio, const int y_ratio);
	cxxplplot(const std::string& output_filename_type, const std::string& filename);
	cxxplplot(void);

	void SetPage(int page); //put 0 for creating a new one, put a number to access it
	int GetTotalPages(void);

	void BuildBox2D(const cxxplplot::Box2DOptions& options);
	void Plot2DLine(cxxplplot::Color line_color, cxxplplot::Line2dStyle style, double *x, double *y, int points, std::string custom_characters = "");

	~cxxplplot();

private:
	int CountDigits(int arg);
	int pages_;

};

