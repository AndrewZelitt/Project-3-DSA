﻿//this will be the gui cpp file for our project
//written by Andrew Zelitt
#include "GUI.h"


using namespace std;

//Most of this code has been adapted from the example Code given by FLTK
class output_table : public Fl_Table {
private:
	vector<vector<const char *>> datastore;

public:

	output_table(int x, int y, int w, int h, const char* l = 0) : Fl_Table(x, y, w, h, l) {
		vector<const char*> temp = { "Title","Album","Artists","Duration","Release Year" };
		datastore.push_back(temp);
		// Rows
		rows(datastore.size());                    // how many rows
		row_header(1);              // enable row headers (along left)
		row_height_all(20);         // default height of rows
		row_resize(0);              // disable row resizing
		// Cols
		cols(5);             // how many columns
		col_header(1);              // enable column headers (along top)
		//col_width_all(w/5-50);          // default width of columns
		col_width_all(100);
		col_resize(1);              // enable column resizing
		end();
	}
	void add_item(vector<const char*> column);

	//void autowidth(int pad);
	void DrawData(const char* s, int X, int Y, int W, int H) {
		fl_push_clip(X, Y, W, H);
		// Draw cell bg
		fl_color(FL_WHITE); fl_rectf(X, Y, W, H);
		// Draw cell data
		fl_color(FL_GRAY0); fl_draw(s, X, Y, W, H, FL_ALIGN_CENTER);
		// Draw box border
		fl_color(color()); fl_rect(X, Y, W, H);
		fl_pop_clip();
	}
	
	void draw_cell(TableContext context, int ROW = 0, int COL = 0, int X = 0, int Y = 0, int W = 0, int H = 0) FL_OVERRIDE {
		static char s[40];
		switch (context) {
		case CONTEXT_STARTPAGE:                   // before page is drawn..
			fl_font(FL_HELVETICA, 16);              // set the font for our drawing operations
			return;
		case CONTEXT_COL_HEADER:                  // Draw column headers
			sprintf(s, "%c", 'A' + COL);                // "A", "B", "C", etc.
			DrawHeader(s, X, Y, W, H);
			return;
		case CONTEXT_ROW_HEADER:                  // Draw row headers
			sprintf(s, "%03d:", ROW);                 // "001:", "002:", etc
			DrawHeader(s, X, Y, W, H);
			return;
		case CONTEXT_CELL:                        // Draw data in cells
			DrawData(datastore[ROW][COL], X, Y, W, H);
			return;
		default:
			return;
		}
	}
	void DrawHeader(const char* s, int X, int Y, int W, int H) {
		fl_push_clip(X, Y, W, H);
		fl_draw_box(FL_THIN_UP_BOX, X, Y, W, H, row_header_color());
		fl_color(FL_BLACK);
		fl_draw(s, X, Y, W, H, FL_ALIGN_CENTER);
		fl_pop_clip();
	}
};

void output_table::add_item(vector<const char*> column) {
	this->datastore.push_back(column);
	rows(datastore.size());
	int w = 0;
	int h = 0;
	int pad = 10;
	for (int r = 0; r < (int)datastore.size(); r++) {
		for (int c = 0; c < (int)datastore[r].size(); c++) {
			w = 0; fl_measure(datastore[r][c], w, h, 0);       // pixel width of row text
			if ((w + pad) > col_width(c)) col_width(c, w + pad);
		}
	}
	
	table_resized();
	redraw();
}

output_table* result_table = new output_table(10, 2 * 768 / 6, 1366 - 20, 768 - 20);

//this is the function that will take in the value of each of the given items when pressed.
void button_pressed( Fl_Choice* tree_type, Fl_Choice* operation_choice, Fl_Choice* sorting_option, Fl_Input* search_option, Fl_Scroll* scroller) {
	//decide which inputs to use based on all of the options.
	//and do the operations that are necessary;
	vector<const char*> temp = {"Testify","The Battle Of Los Angeles","Rage Against The Machine","210133","1999"};
	result_table->add_item(temp);
}

void updated_operation(Fl_Choice* operation_choice, Fl_Choice* sort_choice, Fl_Input* search_option) {
	if (operation_choice->value() == 1) {
		//search option
		sort_choice->show();
		search_option->show();
	}
	else if(operation_choice->value() == 2 ){
		//sort option
		sort_choice->show();
	}
	else {
		sort_choice->hide();
	}
}


/*
//adapted from the example code for autowidth for a table from fltk.
void output_table::autowidth(int pad) {
	int w, h;
	static const char *G_header[] = { "Name", "Album","Artists","Duration","Release Year" };
	// Initialize all column widths to header width
	fl_font(FL_HELVETICA, 12);
	for (int c = 0; G_header[c]; c++) {
		w = 0; fl_measure(G_header[c], w, h, 0);                   // pixel width of header text
		col_width(c, w + pad);
	}
	fl_font(FL_HELVETICA, 12);
	for (int r = 0; r < (int)datastore.size(); r++) {
		for (int c = 0; c < (int)datastore[r].size(); c++) {
			w = 0; fl_measure(datastore[r][c], w, h, 0);       // pixel width of row text
			if ((w + pad) > col_width(c)) col_width(c, w + pad);
		}
	}
	table_resized();
	redraw();
}
*/



int main(int argc, char** argv) {


	Fl_Window* window = new Fl_Window(1366, 768);
	Fl_Box* title = new Fl_Box(0, 0, window->w(), window->h()/10, "Music Men");
	int x_size_factor = 6;
	int border_offset = 25;
	Fl_Button* button = new Fl_Button(window->w() - 1.1*window->w() / x_size_factor, window->h() / 6, window->w() / x_size_factor, window->h() / 11, "Go"); //this button will take the arguments from the various drop downs and text boxes.
	Fl_Choice* tree_type = new Fl_Choice(window->w() / border_offset, window->h() / 6, window->w() / x_size_factor, window->h() / 11, "");
	Fl_Choice* operation_choice = new Fl_Choice(tree_type->w() + window->w() / border_offset, window->h() / 6, window->w() / x_size_factor, window->h() / 11, "");
	Fl_Choice* sorting_option = new Fl_Choice(tree_type->w() + window->w() / border_offset + operation_choice->w(), window->h() / 6, window->w() / x_size_factor, window->h() / 11, "");
	Fl_Input* search_option = new Fl_Input(tree_type->w() + window->w() / border_offset + operation_choice->w() + sorting_option->w(), window->h() / 6, window->w() / x_size_factor, window->h() / 11, "");
	Fl_Scroll* scroller = new Fl_Scroll(10, 2 * window->h() / 6, window->w() - 20, window->h() - 10 - window->h() / 6);
	result_table = new output_table(10, 2 * window->h() / 6, window->w() - 30, window->h() - 275);

	
	//formatting
	title->box(FL_UP_BOX);
	title->labelfont(FL_HELVETICA );
	title->labelsize(36);

	button->labelsize(30);

	tree_type->add("Data Structure Choice | B+ Tree | Red Black Tree");
	tree_type->value(0);

	operation_choice->add("Operations | Search | Sort");
	operation_choice->value(0);

	sorting_option->add("Sort By: | Album | Title | Artist name | Duration | Release Year");
	sorting_option->value(0);
	sorting_option->hide();

	search_option->value("Search Argument");
	search_option->hide();


	scroller->type(Fl_Scroll::VERTICAL);
	scroller->resizable();

	scroller->add(result_table);
	scroller->show();


	result_table->show();

	//callback functions
	//when the go button is pressed
	FL_FUNCTION_CALLBACK_5( button, button_pressed, Fl_Choice*, tree_type, Fl_Choice*, operation_choice, Fl_Choice*, sorting_option, Fl_Input*, search_option, Fl_Scroll*, scroller);
	//when the operation choice is changed
	FL_FUNCTION_CALLBACK_3(operation_choice, updated_operation, Fl_Choice* , operation_choice, Fl_Choice*, sorting_option, Fl_Input*,  search_option);
	//when the choice of how to search for something is changed. might not be necessary
	FL_FUNCTION_CALLBACK_3(sorting_option, updated_operation, Fl_Choice*, operation_choice, Fl_Choice*, sorting_option, Fl_Input*, search_option);

	//FL_FUNCTION_CALLBACK_4(button, button_pressed, Fl_Choice*, tree_type, Fl_Choice*, operation_choice, Fl_Choice*, sorting_option, Fl_Input*, search_option);
	//window settings
	window->end();
	window->resizable(window);
	window->show(argc, argv);
	return Fl::run();
}
