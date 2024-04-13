//this will be the gui cpp file for our project
//written by Andrew Zelitt
#include "GUI.h"


using namespace std;

//this is the function that will take in the value of each of the given items when pressed.
void button_pressed(Fl_Choice* tree_type, Fl_Choice* operation_choice, Fl_Choice* sorting_option, Fl_Input* search_option) {
	//decide which inputs to use based on all of the options.
	//and do the operations that are necessary;

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

class Row {
public:
	vector<string> cols;
};

class output_table : public Fl_Table_Row {
private: 
	vector<Row> datastore;
	static void event_callback(Fl_Widget*, void*);

public:

	output_table(int x, int y, int w, int h, const char* l = 0) : Fl_Table_Row(x, y, w, h, l) {
		callback(event_callback, (void*)this);
	}
	void autowidth(int pad);
};

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
		for (int c = 0; c < (int)datastore[r].cols.size(); c++) {
			w = 0; fl_measure(datastore[r].cols[c].c_str(), w, h, 0);       // pixel width of row text
			if ((w + pad) > col_width(c)) col_width(c, w + pad);
		}
	}
	table_resized();
	redraw();
}



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
	
	output_table result_table(10, 2 * window->h() / 6, window->w() - 20, window->h() - 20);
	
	
	
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



	//callback functions
	//when the go button is pressed
	FL_FUNCTION_CALLBACK_4(button, button_pressed, Fl_Choice *, tree_type, Fl_Choice* , operation_choice , Fl_Choice* , sorting_option, Fl_Input* , search_option);
	//when the operation choice is changed
	FL_FUNCTION_CALLBACK_3(operation_choice, updated_operation, Fl_Choice* , operation_choice, Fl_Choice*, sorting_option, Fl_Input*,  search_option);
	//when the choice of how to search for something is changed. might not be necessary
	FL_FUNCTION_CALLBACK_3(sorting_option, updated_operation, Fl_Choice*, operation_choice, Fl_Choice*, sorting_option, Fl_Input*, search_option);


	//window settings
	window->end();
	window->resizable(window);
	window->show(argc, argv);
	return Fl::run();
}
