//this will be the gui cpp file for our project
//written by Andrew Zelitt
#include "GUI.h"


using namespace std;

int winsize = 1380;

//Some of this code has been adapted from the various example Code given by FLTK
class output_table : public Fl_Table {
private:
	vector<vector<const char*>> datastore;
	int max_width = 0;
public:
	int prev_width = 1380;
	Fl_Scrollbar* scroll;
	output_table(int x, int y, int w, int h, const char* l = 0) : Fl_Table(x, y, w, h, l) {
		vector<const char*> temp = { "Title","Album","Artists","Duration","Year" };
		datastore.push_back(temp);
		// Rows
		rows(datastore.size());                    // how many rows
		row_header(1);              // enable row headers (along left)
		row_header_width(90);
		row_height_all(20);         // default height of rows
		row_resize(1);              // enable row resizing
		// Cols
		cols(5);             // how many columns
		col_header(1);              // enable column headers (along top)
		//col_width_all(w/5-50);          // default width of columns
		col_width_all(75);
		col_resize(1);              // enable column resizing
		end();
		table_resized();
		update_list();
		redraw();
		max_width = w;
		scroll = vscrollbar;
	}
	
	void add_item(vector<const char*> column);


	void resetArray() {
		datastore.clear();
	}

	
	void update_list() {
		rows(datastore.size());

		int w = 0;
		int h = 0;
		int pad = 0;
		for (int c = 0; c < 3; c++) {
			col_width(c, ((this->wiw - row_header_width() - col_width(4) - col_width(3) - 15) / 3));
		}
			int r1;
			int r2;
			int c1;
			int c2;
			this->visible_cells(r1, r2, c1, c2);
			int wi = 75;
			int max_wi = 0;

			for (int r = r1; r < r2; r++) {
				max_wi = 0;
				for (int c = 0; c < 3; c++) {
					w = 0; fl_measure(datastore[r][c], w, h, 0);       // pixel width of row text
					wi = col_width(c);
					if (w > max_wi) {
						max_wi = w;
					}
				}
					if (max_wi > (9*wi)/10) {
						row_height(r, h*(ceil((float)max_wi/wi)) + 25);
					
					}
					else if (max_wi < wi + 20 + wi/10 ) {
						row_height(r, (h * (floor((float)wi/max_wi)) + 10));
						
					}
			}
			prev_width = this->wiw;
	}
	void DrawData(const char* s, int X, int Y, int W, int H) {
		fl_push_clip(X, Y, W, H);
		// Draw cell bg
		char r = 15;
		char g = 52;
		char b = 128;
		fl_color(r, g, b);
		fl_color(fl_color_average(FL_WHITE,fl_color(), 0.20)); fl_rectf(X, Y, W, H);
		// Draw cell data
		fl_color(FL_GRAY0); fl_draw(s, X, Y, W, H, FL_ALIGN_WRAP);
		// Draw box border
		fl_color(color()); fl_rect(X, Y, W, H);
		fl_pop_clip();
	}
	int init = 0;
	void draw_cell(TableContext context, int ROW = 0, int COL = 0, int X = 0, int Y = 0, int W = 0, int H = 0) FL_OVERRIDE {
			vector<const char*> temp = { "Title","Album","Artists","Duration","Year" };
			static char s[40];
			switch (context) {
			case CONTEXT_STARTPAGE:                   // before page is drawn..
				fl_font(FL_HELVETICA, 16);              // set the font for our drawing operations
				return;
			case CONTEXT_COL_HEADER:                  // Draw column headers
				//sprintf(s, "%c", 'A' + COL);
				//DrawHeader(s, X, Y, W, H);
				DrawHeader(temp[COL], X, Y, W, H);
				return;
			case CONTEXT_ROW_HEADER:                  // Draw row headers
				sprintf(s, "%06d:", ROW + 1);                 // "001:", "002:", etc
				DrawHeader(s, X, Y, W, H);
				return;
			case CONTEXT_CELL:                        // Draw data in cells
				if (COL < 3) {
					fl_font(FL_HELVETICA, 16);
				}
				else {
					fl_font(FL_HELVETICA, 16);
				}
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

}
class my_input : public Fl_Input {
public:
	Fl_Multiline_Output* stopwatch;
	my_input(int X, int Y, int W, int H, const char* L = 0) : Fl_Input(X, Y, W, H, L = 0) {
		stopwatch = new Fl_Multiline_Output(X, Y - H / 2 - (Y - 50) / 2, W, H + 10, "");
	}
	void update_timer(float load_time, float search_time, int choice) {
		string send_to_stopwatch = "";
		string load = to_string(load_time);
		string search = to_string(search_time);
		//sprintf(load, "%04f:", load_time);
		//sprintf(search, "%04f:", search_time);
		if (choice == 2) {
			

			 send_to_stopwatch = "Time for creating Tree: " + load + "s\n" + "Time for traversal " + search + "s";
		}
		else {
			send_to_stopwatch = "Time for creating Tree: " + load + "s\n" + "Time for searching " + search + "s";
		}
		//cout << send_to_stopwatch << endl;
		stopwatch->value(send_to_stopwatch.c_str());
	}
};

output_table* result_table = new output_table(10, 2 * 768 / 6, 1366 - 20, 768 - 20);
vector<vector<const char*>> * input_data = new vector<vector<const char*>> ;

int status = 0;
//this function does most of the things.
//this is the function that will take in the value of each of the given items when pressed.
void button_pressed(Fl_Choice* tree_type, Fl_Choice* operation_choice, Fl_Choice* sorting_option, my_input* search_option, Fl_Choice* sort_dir) {
	//decide which inputs to use based on all of the options.
	//and do the operations that are necessary;
		int op_choice = operation_choice->value();
		int arg = sorting_option->value();
		int arg1 = sort_dir->value();
		int insertion_arg = 4;
		if (arg == 0) {
			arg = 5;
		}
		float load_time = 0;
		float search_time = 0;

		if (tree_type->value() == 1) {
			if (arg != 6) {
				if (op_choice == 1) {
					insertion_arg = 5;
				}
				else if (op_choice == 2) {
					insertion_arg = arg;
				}
				//Create the new Tree
				AVLtree* avl = new AVLtree;
				//Set the data pointer
				avl->raw_database = input_data;

				//Start Timer
				auto start = std::chrono::high_resolution_clock::now();

				//insert root value
				avl->root = avl->insert(nullptr, 0, insertion_arg - 1, arg1);

				//insert all of the datavalues into the tree based on the given choices.
				result_table->hide();
				for (int x = 1; x < input_data->size(); x++) {
					avl->insert(avl->root, x, insertion_arg - 1, arg1);
				}

				//get time output for how long it took to insert everything.
				auto finish = std::chrono::high_resolution_clock::now();
				auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(finish - start);

				//start second timer
				auto start1 = std::chrono::high_resolution_clock::now();

				//if just sorting then do traversal, if searching do search 
				if (op_choice == 2) {
					//just output the sorted data
					avl->inorder(avl->root);
				}
				else if (op_choice == 1) {

					//do search
					string input = "Rage Against the Machine";
					if (search_option->value() != "") {
						input = search_option->value();
					}

					avl->search(avl->root, input, sorting_option->value() - 1, 0);
				}
				//Stop timer.
				auto finish1 = std::chrono::high_resolution_clock::now();
				auto microseconds1 = std::chrono::duration_cast<std::chrono::microseconds>(finish1 - start1);

				//clear the current values in the table
				result_table->resetArray();
				//add new items into the table
				for (int x = 0; x < avl->output_for_search.size(); x++) {
					result_table->add_item(input_data->at(avl->output_for_search[x]));
				}

				cout << "Num items parsed: " << avl->num_items << "\n";
				cout << "Height of tree: " << avl->root->height << "\n";
				//delete the output array from the tree
				vector<int>().swap(avl->output_for_search);
				//avl->output_for_search.clear();
				//delete each root from the tree.
				avl->deleteAll(avl->root);
				delete avl;
				result_table->show();
				result_table->update_list();
				load_time = (float)microseconds.count() / 1000000;
				search_time = (float)microseconds1.count() / 1000000;
				cout << "load_time: " << load_time << "s \n";
				cout << "search_time: " << search_time << "s\n";
			}
			else { //this is for testing purposes
				//Times all operations to get average timing for testing. Also can be used to see memory efficiency.
				float avg_time = 0;
				//tests all combinations of actions that can be done. 
				for (int i = 0; i < 3; i++) {
					vector<string> searching = { "Hypnotize","Hypnotize","System Of A Down","191936","2005" };
					vector<string> out = { "Title", "Album", "Artist", "Duration", "Release Year" };
					vector<string> ascent = { "Ascending", "Descending" };
					float total_time = 0;
					float combined_time = 0;
					float longest_time = 0;
					int longest_s = 0;
					int longest_a = 0;
					int longest_se = 0;
					for (int a = 0; a < 2; a++) {
						for (int s = 4; s >= 0; s--) {
							for (int se = 4; se >= 0; se--) {
								AVLtree* avl = new AVLtree;
								avl->raw_database = input_data;
								auto start = std::chrono::high_resolution_clock::now();
								avl->root = avl->insert(nullptr, 0, s, a);
								result_table->hide();
								for (int x = 1; x < input_data->size(); x++) {
									avl->insert(avl->root, x, se, a);
								}
								auto finish = std::chrono::high_resolution_clock::now();
								auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(finish - start);
								auto start1 = std::chrono::high_resolution_clock::now();
								string input = "Rage Against the Machine";
								input = searching[se];
								avl->search(avl->root, input, se, a);
								result_table->resetArray();
								auto finish1 = std::chrono::high_resolution_clock::now();
								auto microseconds1 = std::chrono::duration_cast<std::chrono::microseconds>(finish1 - start1);
								for (int x = 0; x < avl->output_for_search.size(); x++) {
									result_table->add_item(input_data->at(avl->output_for_search[x]));
								}
								cout << "Sorted by: " << out[s] << " Searching with: " << out[se] << "\n";
								cout << ascent[a] << "\n";
								cout << "Number of items parsed: " << avl->num_items << "\n";
								cout << "Height of tree: " << avl->root->height << "\n";
								vector<int>().swap(avl->output_for_search);
								avl->deleteAll(avl->root);
								delete avl;
								result_table->show();
								result_table->update_list();
								load_time = (float)microseconds.count() / 1000000;
								search_time = (float)microseconds1.count() / 1000000;
								combined_time = search_time + load_time;
								total_time += combined_time;
								if (combined_time > longest_time) {
									longest_time = combined_time;
									longest_a = a;
									longest_s = s;
									longest_se = se;
								}
								cout << "loading time: " << load_time << "s \n";
								cout << "searching time: " << search_time << "s\n\n";
							}
						}
					}
					cout << "Longest_time: " << longest_time << "\n";
					cout << "Longest Combination: " << ascent[longest_a] << " Sorted By: " << out[longest_s] << " Looking for: " << out[longest_se] << "\n";
					cout << "Total Time: " << total_time << "\n";
					cout << "Average individual time: " << (total_time / (2 * 5 * 5)) << "\n\n\n";
					avg_time += total_time;
				}
				cout << "Average Time: " << (avg_time / 3) << "\n";
			}
		} else if (tree_type->value() == 2) {

		    //put the b+ Tree testing code here
		    BPlusTree* BPlus = new BPlusTree;
		    vector<string>* song = new vector<string>;
		    auto start2 = std::chrono::high_resolution_clock::now();
		    for (int i = 0; i < input_data->size(); i++) {
                song->emplace_back(input_data->at(i)[0]);
                song->emplace_back(input_data->at(i)[1]);
                song->emplace_back(input_data->at(i)[2]);
                song->emplace_back(input_data->at(i)[3]);
                song->emplace_back(input_data->at(i)[4]);
                BPlus->insert(*song, 0);
		    }
		    auto finish2 = std::chrono::high_resolution_clock::now();
		    auto microseconds2 = std::chrono::duration_cast<std::chrono::microseconds>(finish2 - start2);
		    cout << "B+ load time: " << (float) microseconds2.count() / 1000000 << endl;
		    auto start3 = std::chrono::high_resolution_clock::now();
            //attribute is 0 for title 1 fdr album 2 for artist 3 for length 4 for year
		    vector<vector<string>> searchResult = BPlus->search(search_option->value(), arg - 1);
            auto finish3 = std::chrono::high_resolution_clock::now();
		    auto microseconds3 = std::chrono::duration_cast<std::chrono::microseconds>(finish3 - start3);
		    cout << "B+ search time: " << (float)microseconds3.count() / 1000000 << endl;
            vector<const char*> * songout = new vector<const char*>;
            string* song_item = new string;
            result_table->resetArray();
            for (auto songs : searchResult) {
                *song_item = songs[0];
                songout->push_back(song_item->c_str());
                song_item = new string;
                *song_item = songs[1];
                songout->push_back(song_item->c_str());
                song_item = new string;
                *song_item = songs[2];
                songout->push_back(song_item->c_str());
                song_item = new string;
                *song_item = songs[3];
                songout->push_back(song_item->c_str());
                song_item = new string;
                *song_item = songs[4];
                songout->push_back(song_item->c_str());
                song_item = new string;
                result_table->add_item(*songout);
                songout = new vector<const char*>;
                //vector<const char>().swap(songout);
            }
            vector<vector<string>>().swap(searchResult);
		    //searchResult.clear();
		    result_table->show();
		    result_table->update_list();
	    }
		search_option->update_timer(load_time, search_time, op_choice);
}

void updated_operation(Fl_Choice* operation_choice, Fl_Choice* sort_choice, Fl_Input* search_option, Fl_Choice* sort_dir) {
	if (operation_choice->value() == 1) {
		//search option
		//sort_choice->remove(0);
		sort_choice->add("Search type");
		sort_choice->value(6);
		sort_choice->show();
		search_option->show();
		sort_dir->hide();


	}
	else if(operation_choice->value() == 2 ){
		//sort option
		sort_choice->show();
		search_option->hide();
		sort_dir->show();
	}
	else {
		sort_choice->hide();
		sort_dir->hide();

	}
}


class my_window : public Fl_Window {
	
public:
	
	my_window(int w, int h) : Fl_Window (w, h) {
		
	}
	//resizes the table when the window size changes. 
	void resize(int X, int Y, int W, int H) {
		this->Fl_Window::resize(X, Y, W, H);
		result_table->update_list();
	}
};

int scrollpos;
void scrolled(output_table* result_table){

	if(abs(result_table->scroll->value() -  scrollpos) > 75){
		
	}
	result_table->update_list();
}





/*
//adapted from the example code for autowidth for a table from fltk.
void output_table::autowidth(int pad) {
	int w, h;
	static const char *G_header[] = { "Name", "Album","Artists","Duration","Year" };
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
	
	my_window* window = new my_window(1380, 768);
	Fl_Box* title = new Fl_Box(10, 0, window->w()-20, window->h()/10, "Music Men");
	int x_size_factor = 6;
	int border_offset = 25;
	Fl_Button* button =   		  new Fl_Button(window->w() - 1.1*window->w() / x_size_factor, window->h() / 6, window->w() / x_size_factor, window->h() / 11, "Go"); //this button will take the arguments from the various drop downs and text boxes.
	Fl_Choice* tree_type =        new Fl_Choice(window->w() / border_offset, window->h() / 6, window->w() / x_size_factor, window->h() / 11, "");
	Fl_Choice* operation_choice = new Fl_Choice(tree_type->w() + window->w() / border_offset, window->h() / 6, window->w() / x_size_factor, window->h() / 11, "");
	Fl_Choice* sorting_option =   new Fl_Choice(tree_type->w() + window->w() / border_offset + operation_choice->w(), window->h() / 6, window->w() / x_size_factor, window->h() / 11, "");
	my_input* search_option =     new my_input( tree_type->w() + window->w() / border_offset + operation_choice->w() + sorting_option->w(), window->h() / 6, window->w() / x_size_factor, window->h() / 11, "");
	Fl_Choice* sort_dir =         new Fl_Choice(tree_type->w() + window->w() / border_offset + operation_choice->w() + sorting_option->w() +  search_option->w() , window->h()/6, window->w()/10, window->h()/11, "");
	result_table = new output_table(10, 2 * window->h() / 6, window->w() - 20, window->h() - 275);
	search_option->stopwatch->resize(sort_dir->x(), button->y() - button->h() / 2 - (button->y() - title->y()) / 2, button->w() + sort_dir->w(), search_option->stopwatch->textsize()*3 + 15 );

	winsize = window->w();
	
	//formatting
	title->box(FL_UP_BOX);
	title->labelfont(FL_HELVETICA );
	title->labelsize(36);

	button->labelsize(30);

	tree_type->add("Data Structure Choice | AVLtree | B+ Tree");
	tree_type->value(0);

	operation_choice->add("Operations | Search | Sort");
	operation_choice->value(0);

	sorting_option->add("Sort By: | Title | Album | Artist Name | Duration | Release Year");
	sorting_option->value(0);
	sorting_option->hide();

	search_option->value("Search Argument");
	search_option->hide();

	search_option->stopwatch->value("Time Taken: 0s");
	search_option->stopwatch->wrap(FL_ALIGN_TOP);
	search_option->stopwatch->textsize(search_option->stopwatch->textsize()*1.5);


	result_table->hide();

	sort_dir->add("Ascending | Descending");
	sort_dir->value(0);
	sort_dir->hide();

	char r = 15;
	char g = 52;
	char b = 128;

	window->color(fl_rgb_color(r, g, b));
	title->color(fl_rgb_color(r, g, b));
	title->box(FL_NO_BOX);
	
	
	button->box(FL_UP_BOX);
	tree_type->box(FL_NO_BOX);
	operation_choice->box(FL_SHADOW_BOX);
	search_option->stopwatch->box(FL_FLAT_BOX);
	sort_dir->box(FL_SHADOW_BOX);
	

	//reading from the data files.
    {
	ifstream inputFile;
	inputFile.open("800k1artist.csv");

	if (!inputFile.is_open()) {
		cout << "error, could not read file :(";
		return 1;
	}
	string buf;
	stringstream buff;
	string* temp = new string;
	//ignore the first line;
	vector<const char*> row;
	getline(inputFile, buf, '\n');
	for(int i = 0; i < 100; i++){
		getline(inputFile, buf, '\n');
	//while (getline(inputFile, buf, '\n')) {
		buff.str(buf);
		while (getline(buff, *temp, ',')) {
			if ((*temp)[0] == '[') {
				(*temp).erase(0, 2);
				(*temp).pop_back();
				(*temp).pop_back();
				row.push_back((*temp).c_str());
			}
			else {
			
				if(temp->size() == 5 && ((*temp)[0] == '1' || ((*temp)[0]) == '2')){
					(*temp).pop_back();
				}
				while ((*temp)[0] == '\"') {
					(*temp).pop_back();
					(*temp).erase(0, 1);
				}
				row.push_back((*temp).c_str());
			}
			//cout << temp.c_str() << "\n";
			temp = new string;
		}

		buff.clear();
		if (row.size() == 5) {
			string duration = row[3];
			string date = row[4];
			//makes sure a valid row of data is being added.
			if (date.size() == 4 && duration.size() <= 8) {
				input_data->push_back(row);
			}
		}
		vector<const char*>().swap(row);
		//row.clear();
	}
	inputFile.close();
	//for smaller datasets for ease of testing.
	if(0){
	inputFile.open("800k1artist2.csv");
	int line = 0;
	getline(inputFile, buf, '\n');
	while (getline(inputFile, buf, '\n')) {
		buff.str(buf);
		while (getline(buff, *temp, ',')) {
			if ((*temp)[0] == '[') {
				(*temp).erase(0, 2);
				(*temp).pop_back();
				(*temp).pop_back();
				row.push_back((*temp).c_str());
			}
			else {
			//there are some bad values that mess up the reading of csv file, this fixes those.
				if(temp->size() == 5 && ((*temp)[0] == '1' || ((*temp)[0]) == '2')){
					(*temp).pop_back();
				}
				while ((*temp)[0] == '\"') {
					(*temp).pop_back();
					(*temp).erase(0, 1);
				}
				row.push_back((*temp).c_str());
			}
			//cout << temp.c_str() << "\n";
			temp = new string;
		}

		buff.clear();
		if (row.size() == 5) {
			string duration = row[3];
			string date = row[4];
			//makes sure a valid row of data is being added.
			if (date.size() == 4 && duration.size() <= 8) {
				input_data->push_back(row);
			}
		}
		vector<const char*>().swap(row);
		//row.clear();
	}
	inputFile.close();
	}
	}
	cout << input_data->size() << "\n";
	//callback functions
	//when the go button is pressed
	FL_FUNCTION_CALLBACK_5( button, button_pressed, Fl_Choice*, tree_type, Fl_Choice*, operation_choice, Fl_Choice*, sorting_option, my_input*, search_option, Fl_Choice* , sort_dir );
	//when the operation choice is changed
	FL_FUNCTION_CALLBACK_4(operation_choice, updated_operation, Fl_Choice* , operation_choice, Fl_Choice*, sorting_option, Fl_Input*,  search_option, Fl_Choice* , sort_dir);
	//when the choice of how to search for something is changed. might not be necessary
	FL_FUNCTION_CALLBACK_4(sorting_option, updated_operation, Fl_Choice*, operation_choice, Fl_Choice*, sorting_option, Fl_Input*, search_option, Fl_Choice* , sort_dir);
	//int scrollbar_pos = 
	FL_FUNCTION_CALLBACK_1(result_table, scrolled , output_table*, result_table);
	



	window->end();
	window->resizable(window);
	window->show(argc, argv);
	return Fl::run();
}
