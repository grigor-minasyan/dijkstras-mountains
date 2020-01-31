#include "Bridges.h"
#include "ColorGrid.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cassert>
#include <cmath>
#include <vector>
#include <queue>
using namespace std;
using namespace bridges;

// This program implements the mountain paths assignment in C++. See the README.md for a
// detailed description

// function prototypes
ColorGrid *getImage (int *img_arr, int imgWidth, int imgHeight, int maxVal, int minVal);
int *readData (int *imgWidth, int *imgHeight, int *maxVal, int *minVal, string file_name);
void findPath(int *img_arr, int imgWidth, int imgHeight, int startRow, int maxVal);
void dij_path(int *img_arr, int imgWidth, int imgHeight, int start_row, int start_col, int end_row, int end_col);

const int coloring_code = -250000;
const int coloring_code_2 = -260000;
const int coloring_code_3 = -270000;

struct Tile {
	int col,row; //the location of the tile, x=col, y=row, access[y][x]
	int from_col,from_row; // the location of the place we came from to get here
	int distance;
	Tile() {
		col = -2;
	}
	Tile(int col, int row, int from_col, int from_row, int distance) : col(col), row(row), from_col(from_col), from_row(from_row), distance(distance) {}
	bool operator<(const Tile &rhs) const  {
		return distance < rhs.distance;
	}
	bool operator>(const Tile &rhs) const  {
		return distance > rhs.distance;
	}
	void set_distance(int d) {
		this->distance = d;
	}
	bool operator==(const Tile &rhs) const {
		return col==rhs.col && row==rhs.row;
	}
};

int main(int argc, char **argv) {
	const int bridges_project_id = 1003;
	const string bridges_username = "gogminsam";
	const string bridges_api_key  = "1523056821236";

	//static_assert(bridges_username != "username","You must register on BRIDGES and get your own usernsme and API key.");

	// bridges object initialization
	Bridges bridges(bridges_project_id, bridges_username, bridges_api_key);

	// read input elevation data
	int width, height, maxVal, minVal;
	int *elev_data;

	// parse command line arguments
	// argument 2 is input file
	string filename = "./colorado1.dat";
	if (argc >= 2) filename = argv[1]; //This is how you read a command line parameter
	elev_data = readData(&width, &height, &maxVal, &minVal, filename);

	bridges.setTitle("Mountain Paths "s + filename + " - Dijkstra's");

	int startRow = 0, startCol = 0, endRow = 0, endCol = 0;
	cout << "Please enter starting row and col (row < "<< height <<" and col < "<< width << "):\n"; //Only starting row is used for greedy mode
	cin >> startRow >> startCol;
	cout << "Please enter ending row and col:\n"; //Neither of these is used in greedy mode
	cin >> endRow >> endCol;

	dij_path(elev_data, width, height, startRow, startCol, endRow, endCol);

	ColorGrid *cg = getImage(elev_data, width, height, maxVal, minVal);

	// visualize
	bridges.setDataStructure(cg);
	bridges.visualize();

	delete cg; //WRK Fix memory leak in BRIDGES
	delete[] elev_data; //WRK Ditto
}

int *readData(int *width, int *height, int *maxVal, int *minVal, string file_name) {

	ifstream infile(file_name.c_str());

	// read header
	int max = 0, min = 0, w, h, val;
	infile >> w >> h;

	*width = w; *height = h;

	// allocate space for the input data
	// we will use a 1D array
	int size = w*h;
	int *elev_data = new int[size];
	assert(elev_data); //WRK

	// read the elevation data
	for (int k = 0; k < size; k++) {
		infile >>  val;

		// update the max val
		max = (val > max) ? val : max;
		min = (val < min) ? val : min;

		// store  in elevation array
		elev_data[k] = val;
	}
	// record the max value
	*maxVal = max;
	*minVal = min;

	return elev_data;
}

// takes in the processed elevation data and returns a color grid for
// visualization using BRIDGES
ColorGrid *getImage(int *elev_data, int width, int height, int maxVal, int minVal) {
	// create color grid
	ColorGrid *cg = new ColorGrid(height, width);

	float pixel_val;
	int n = 0, gray, red;

	// load the elevation data
	for (int j = 0; j < height; j++)
	for (int k = 0; k < width; k++) {
		pixel_val = (float) elev_data[n++];
		if (pixel_val == coloring_code) {
			// this is the path drawn in blue across the image
			cg->set(j, k, Color(0, 0, 255));
		} else if (pixel_val == coloring_code_2) {
			cg->set(j, k, Color(68, 45, 72));
		} else if (pixel_val == coloring_code_3) {
			cg->set(j, k, Color(255, 255, 0));
		}	else {
			// scale value to be within 0-255, for r,g,b range
			if (pixel_val >= 0) {
				gray = (int) (pixel_val*255./maxVal);
				int red = 2550000/((gray-255)*(gray-255)+10000);
				int green = 200-0.7*red;
				int blue = 250000/((gray-125)*(gray-125)+1000);
				cg->set(j, k, Color(red, green, blue));
			} else {
				red = (int) (pixel_val*255./(minVal));
				cg->set(j, k, Color(red, red, red));
			}

		}
	}
	return cg;
}


void dij_path(int *img_arr, int imgWidth, int imgHeight, int start_row, int start_col, int end_row, int end_col) {

	if (start_row < 0 || start_row >= imgHeight || start_col < 0 || start_col >= imgWidth || end_row < 0 || end_row >= imgHeight || end_col < 0 || end_col >= imgWidth) {
		cout << "Invalid starting row or column.\n";
		return;
	}

	//data holders for pixels
	vector<vector<Tile>> visited(imgHeight, vector <Tile> (imgWidth));
	vector<vector<Tile>> to_visit_vec(imgHeight, vector <Tile> (imgWidth));
	priority_queue <Tile, vector<Tile>, greater<Tile>> to_visit;

	//add the first pixel to to_visit data holders
	to_visit.push(Tile(start_col, start_row, -1, -1, 0));
	to_visit_vec[start_row][start_col] = Tile(start_col, start_row, -1, -1, 0);

	//proccess all te nodes
	while (to_visit.size()) {

		Tile start_tile = to_visit.top();

		//if start_tile is not repeated since to_process queue is allowed to have duplicates
		if (visited[start_tile.row][start_tile.col].col == -2) {

			//visit the nighbors of start_tile
			for (int i = -1; i <= 1; i++) {
				for (int j = -1; j<= 1; j++) {

					//skip itself
					if (i == 0 && j == 0) continue;

					//assign the new coordinates for the new tile and the distance_to_be
					int new_col = start_tile.col+j , new_row = start_tile.row+i;
					int distance_to_be = start_tile.distance+((i*j)?2601:1840)+20*abs(img_arr[(new_row)*imgWidth + new_col]-img_arr[(start_tile.row)*imgWidth + start_tile.col]);

					//check if not going out of bounds with neighbors
					if (new_col >= 0 && new_col < imgWidth && new_row >= 0 && new_row < imgHeight) {
						//if not in the visited and if not in the to_visit, insert or check and update the distance and parent pixel
						//note to_visit priority queue keeps the old Tile and adds the duplicate with shorter distance
						if (visited[new_row][new_col].col == -2 && (to_visit_vec[new_row][new_col].col == -2 || to_visit_vec[new_row][new_col].distance > distance_to_be)) {
							to_visit_vec[new_row][new_col] = Tile(new_col, new_row, start_tile.col, start_tile.row, distance_to_be);
							to_visit.push(to_visit_vec[new_row][new_col]);
						}
					}
				}
			}
		} else {
			to_visit.pop();
			to_visit_vec[start_tile.row][start_tile.col].col = -2;
			continue;
		}
		visited[start_tile.row][start_tile.col] = start_tile;
		to_visit.pop();
		to_visit_vec[start_tile.row][start_tile.col].col = -2;
	}
	//color array
	while (!(end_col == -1 && end_row == -1)) {
		Tile end_tile_it = visited[end_row][end_col];
		img_arr[(end_tile_it.row)*imgWidth + end_tile_it.col] = coloring_code_3;
		end_col = end_tile_it.from_col;
		end_row = end_tile_it.from_row;
	}
}
