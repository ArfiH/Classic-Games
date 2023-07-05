#include <iostream>
#include <vector>
#include <cstdlib>
#include <time.h>
#include <iomanip>

using namespace std;

void printBoard(vector<vector<int>> v, int box) {
	for (int i = 0; i < box; i++) {
		cout << "|";
		for (int j = 0; j < box; j++) {
			if (v[i][j] == -1) {
				cout << "* |";
			} else {
				cout << v[i][j] << " |";
			}
		}
		cout << '\n';
	}
}

vector<int> flattenBoard(int box, vector<vector<int>> &v, vector<int> &flatV) {
	for (int i = 0; i < box; i++) {
		for (int j = 0; j < box; j++) {
			flatV.push_back(v[i][j]);			
		}
	}
	return flatV;
}


vector<int> getNeighbour(int n, int box) {
	vector<int> temp;
	int r = n / box;
	int c = n % box;
	if (r - 1 >= 0 and c - 1 >= 0) {
		temp.push_back((r - 1) * box + (c - 1));
	}
	if (c - 1 >= 0) {
		temp.push_back(r * box + (c - 1));
	}
	if (r + 1 < box and c - 1 >= 0) {
		temp.push_back((r + 1) * box + (c - 1));
	}
	if (r - 1 >= 0) {
		temp.push_back((r - 1) * box + c);
	}
	if (r + 1 < box) {
		temp.push_back((r + 1) * box + c);
	}
	if (r - 1 >= 0 and c + 1 < box) {
		temp.push_back((r - 1) * box + (c + 1));
	}
	if (c + 1 < box) {
		temp.push_back(r * box + (c + 1));
	}
	if (r + 1 < box and c + 1 < box) {
		temp.push_back((r + 1) * box + (c + 1));
	}
	return temp;
}


void getBound(int row, int col, int row_upd, int col_upd, int box, vector<vector<int>> &v, vector<vector<int>> &visible) {
	int newrow = row + row_upd;
	int newcol = col + col_upd;
	while (newrow >= 0 and newcol >= 0 and newrow < box and newcol < box and v[newrow][newcol] != -1) {
		visible[newrow][newcol] = v[newrow][newcol];
		newrow += row_upd;
		newcol += col_upd;
	}
}


int main() {
	cout << "Welcome to Minesweeper\n";
	cout << "Choose difficulty\n1 to play Easy.\n2 to play medium.\n3 to play hard.\n";

	int box = 5;
	int bombsnum = 2;
	
	int difficulty;
	do {
		cin >> difficulty;
	} while(difficulty < 1 or difficulty > 3);
	
	if (difficulty == 1) {
		box = 6;
		bombsnum = 5;
	}
	else if (difficulty == 2) {
		box = 10;
		bombsnum = 15;
	}
	else if (difficulty == 3) {
		box = 13;
		bombsnum = 35;
	}
	vector<int> temp(box);
	vector<vector<int>> v(box, temp);
	vector<int> temp2(box, -2);
	vector<vector<int>> visible(box, temp2);

	vector<int> flatV;
	flatV = flattenBoard(box, v, flatV);

	// generate bombs at random locations
	srand(time(0));
	int bombsCreated = 0;
	vector<int> t(box * box);
	while (bombsCreated < bombsnum) {
		int loc = rand() % (box * box);
		if (t.at(loc) == 0) {
			t.at(loc)++;
			// cout << loc << "  ";
			bombsCreated++; 
			flatV[loc] = -1;
			int row = loc / box;
			int col = loc % box;
			v[row][col] = -1;

			// Increment neighbouring cell of the bomb
			vector<int> temp = getNeighbour(loc, box);
			// cout << "Neighbours of " << loc << ":\n";
			for (int i = 0; i < temp.size(); i++) {
				cout << temp.at(i) << ' ';
				flatV.at(temp.at(i))++;
				int row = temp.at(i) / box;
				int col = temp.at(i) % box;
				if (v[row][col] != -1)
					v[row][col]++;
			}
			cout << '\n';
		}
	}
	cout << '\n';

	// printBoard(v, box);
	cout << "Choose a cell to dig\n";
	bool triggered = false;
	while (triggered == false) {
		int spacesLeft = 0;
		
		cout << "  ";
		for (int i = 1; i <= box; i++) {
			cout << setw(2) << i << " ";
		}
		cout << "\n";
		for (int i = 0; i < box; i++) {
			cout << setw(2) << i + 1 << "|";
			for (int j = 0; j < box; j++) {
				if (visible[i][j] == -2) {
					cout << "  |";
					spacesLeft++;
				} else if (visible[i][j] == -1) {
					cout << " * |";
				}
				else {
					cout << setw(2) << visible[i][j] << "|";
				}
			}
			cout << '\n';
		}

		int r = -1;
		int c = -1;
		
		if (spacesLeft == bombsnum) {
			cout << "You won!\n";
			break;
		}

		bool wrong = false;
		while (r <= 0 or c <= 0 or r > box or c > box or wrong) {
			cout << "Usage:row col\n";
			cin >> r >> c;
			if (visible[r - 1][c - 1] >= 0) {
				cout << "This cell is already dug.\n";
				wrong = true;
			} else {
				wrong = false;
			}
		}
		r--;
		c--;
		if (v[r][c] == -1) {
			triggered = true;
			cout << "You lost!\n";
			printBoard(v, box);
			break;
		} else if (v[r][c] >= 0) {
			visible[r][c] = v[r][c];
			getBound(r, c, 1, 0, box, v, visible);
			getBound(r, c, 0, 1, box, v, visible);
			getBound(r, c, 1, 1, box, v, visible);
			getBound(r, c, -1, 0, box, v, visible);
			getBound(r, c, 0, -1, box, v, visible);
			getBound(r, c, -1, -1, box, v, visible);
			getBound(r, c, 1, -1, box, v, visible);
			getBound(r, c, -1, 1, box, v, visible);
		}
		
		cout << '\n';

	}
}