/*GO game
Written by Matthew Russell, Last Updated Jan 11, 2015
Ai can somewhat reliably get ~2/5 total points

::TO DO::
doesn't prevent previous board states
*/
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cmath>
using namespace std;

int size_d = 10;
const char piece_blank = '_';
const char piece_black = 'O';
const char piece_white = '#';
const char skip_turn   = '.';

void display(int board[19][19]){ // displays the 2x2 matrix board
	char refboard[] = {piece_black, piece_blank, piece_white};
	cout << endl;
	for(int i = 0; i < size_d; i++){ // rows
		cout << char(i + 97) << " "; // display row declaration
		for(int j = 0; j < size_d; j++){ // columns
			cout << refboard[board[i][j]] << "  "; // display rest of characters in row
		}
		cout << endl;
		if(i < (size_d - 1)){
			cout << endl;
		}else{
			cout << " ";
			for(int j = 0; j < size_d; j++){ // columns 2nd line
				cout << " " << (j + 1);
				if(j < 9){
					cout << " ";
				}
			}
			cout << endl << endl;
		}
	}
}

void start_pos(int board[19][19], int group[19][19], unsigned int & group_num){
	string size_str;
	bool check = true;
	while(check){ // get board size, set to global integer size_d
		cout << "Enter Board size(3 -> 19): ";
		cin >> size_str;
		if((size_str[0] == '1') && (size_str.length() == 2)){ // combine string characters
			size_d = (10 * (size_str[0] - 48)) + (size_str[1] - 48);
		}else{
			size_d = size_str[0] - 48;
		}
		if((size_d > 19) || (size_d < 3)){
			cout << "ERROR: Invalid input\n";
		}else{
			check = false;
		}
	}
	for(int i = 0; i < size_d; i++){ // reset board and group to blank
		for(int j = 0; j < size_d; j++){
			board[i][j] = 1;
			group[i][j] = 0;
		}
	}
	group_num = 0;
}

void input(char zero, char one, char two, char three, int out[2]){ // input 2 characters as refrence to board
  string instr;
  cout << "Input " << skip_turn << " to skip your turn\n";
  while(true){
  	cout << "Please input coordinates for piece as <row><column>: ";
    cin >> instr;
    if(instr[0] == skip_turn){ // if no data input, assume player is skipping turn
    	out[0] = size_d; // indicates skipped turn in function player
    	return;
    }else if(((instr[0] >= zero) && (instr[0] <= one)) && ((instr[1] >= two) && (instr[1] <= three))){ // compare instr[] to parameters
      out[0] = instr[0] - 97;
      if((instr[1] == '1') && (instr.length() == 3)){
      	out[1] = (10 * (instr[1] - 48)) + (instr[2] - 49) ;
      }else{
      	out[1] = instr[1] - 49; // convert char to int, see above
      }
      return;
    }else{
      cout << "Error: invalid input\n";
    }
  }
}

bool find_space(int group_t[19][19], unsigned int read_group){ // find any open space for read_group
	for(int k = 0; k < size_d; k++){ // for each position on board
		for(int l = 0; l < size_d; l++){
			if(group_t[k][l] == read_group){ // if right group is at that position
				for(int m = -1; m <= 1; m++){ // for each position adjacent to piece
					for(int n = -1; n <= 1; n++){
						if((m * m) != (n * n)){
							if((((k + m) < size_d) && ((k + m) >= 0)) && (((l + n) < size_d) && ((l + n) >= 0))){ // if within bounds
								if(group_t[k+m][l+n] == 0){ // if piece adjacent to position is blank(has at least one blank spot)
									return true; 
								}
							}
						}
					}
				}
			}
		}
	}
	return false;
}

void delete_group_t(int board[19][19], int group_t[19][19], int read_group){ // set readgroup to zero
	for(int a = 0; a < size_d; a++){
		for(int b = 0; b < size_d; b++){
			if(group_t[a][b] == read_group){
				group_t[a][b] = 0;
				board[a][b] = 1;
			}
		}
	}
}

bool update(int board[19][19], int group[19][19], int pos[2], int turn, unsigned int & group_num){ // returns true only if pos has been updated, updates position
	int read_group;
	if(turn){
		board[pos[0]][pos[1]] = 2; // if white's turn, place white piece, etc
	}else{
		board[pos[0]][pos[1]] = 0;
	}
	int group_t[19][19];
	for(int i = 0; i < size_d; i++){ // set group_t to regular group
		for(int j = 0; j < size_d; j++){
			group_t[i][j] = group[i][j];
		}
	}
	group_t[pos[0]][pos[1]] = group_num;
	for(int a = -1; a <= 1; a++){ // for each adjacent pos
		for(int b = -1; b <= 1; b++){
			if((a * a) != (b * b)){ 
				if((((pos[0] + a) < size_d) && ((pos[0] + a) >= 0)) && (((pos[1] + b) < size_d) && ((pos[1] + b) >= 0))){ // if within bounds
					read_group = group_t[pos[0]+a][pos[1]+b]; // set read_group to adjacent group number 
					if(board[pos[0]][pos[1]] == board[pos[0] + a][pos[1] + b]){ // if adjacent piece is same color
						for(int c = 0; c < size_d; c++){ // for entire board
							for(int d = 0; d < size_d; d++){
								if(group_t[c][d] == read_group){ 
									group_t[c][d] = group_num; // set old group_num to new group_num
								}
							}
						}
					}else if(board[pos[0]+a][pos[1]+b] != 1){ // if adjacent piece is enemy color
						if(!find_space(group_t, read_group)){ // if no open spots for enemy
							delete_group_t(board, group_t, read_group); // delete enemy group
						}
					}
				}
			}
		}
	}
	if(find_space(group_t, group_num)){ // if open spots for group_num after checking everything else
		for(int a = 0; a < size_d; a++){
			for(int b = 0; b < size_d; b++){
				group[a][b] = group_t[a][b];
			}
		}
		return true; // exit
	}else{ // if no open spaces for new group, reset board and ask for new variables
		board[pos[0]][pos[1]] = 1;
	}
	return false;
}

bool player(int board[19][19], bool turn, int group[19][19], unsigned int & group_num){ // returns true if player skipped turn
	int out[2];
	bool verify;
	int read_group;
	int group_t[19][19]; // make temporary group
	group_num++;
	while(true){
		if(turn){
			cout << "White's Turn:\n";
		}else{
			cout << "Black's Turn:\n";
		}
		verify = false;
		while(!verify){ // get a valid input
			input(char(97), char(97 + size_d), char(49), char(49 + size_d), out); // get input
			if(out[0] == size_d){ // if player skips turn
				group_num--;
				cout << "Player is skipping Turn\n";
				return true;
			}else if(board[out[0]][out[1]] == 1){ // if blank spot, exit loop;
				verify = true;
			}else{
				cout << "ERROR: cannot replace piece\n";
			}
		}
		if(update(board, group, out, turn, group_num)){ // update board and group
			return false;	// exit		
		}
		cout << "ERROR: piece suicide, please try again\n";
	}
	return false; // exit, shouldn't ever happen
}

void set_board(int group_set[19][19], int set_num, int x_pos, int y_pos, int & white, int & black, int & blank){ // recursively find each piece in the group
	group_set[x_pos][y_pos] = set_num;
	for(int i = -1; i <= 1; i++){ // for each adjacent position
		for(int j = -1; j <= 1; j++){
			if((i * i) != (j * j)){
				if(((i + x_pos < size_d) && (i + x_pos >= 0)) && ((j + y_pos < size_d) && (j + y_pos >= 0))){ // if within bounds
					if(group_set[i + x_pos][j + y_pos] == 0){ // if black is found
						black++;
					}else if(group_set[i + x_pos][j + y_pos] == 2){ // if white is found
						white++;
					}else if(group_set[i + x_pos][j + y_pos] == 1){ // if blank is found
						set_board(group_set, set_num, i + x_pos, j + y_pos, white, black, blank);
						blank++;
					}

				}
			}
		}
	}
}

void evaluate(int board[19][19], int total[2]){ // totals point values for both white and black
	total[0] = 0;
	total[1] = 0;
	int white, black, blank;
	unsigned int blank_num = 2;
	for(int a = 0; a < size_d; a++){ // for entire board
		for(int b = 0; b < size_d; b++){
			white = 0;
			black = 0;
			blank = 1;
			if(board[a][b] == 1){ // if board pos = empty
				blank_num++;
				set_board(board, blank_num, a, b, white, black, blank);
				if(blank < (pow(static_cast<double>(size_d), 2) - 1)){ // if there are more than 1 pieces on board
					if(white == 0){ // if no white, add blank vals to black 
						total[0] += blank;
					}else if(black == 0){
						total[1] += blank;
					}
				}
			}else if(board[a][b] == 0){ // if board pos = black
				total[0]++;
			}else if(board[a][b] == 2){ // if board pos = white
				total[1]++;
			}
		}
	}
}

void win_game(int board[19][19], char & n_game){
	int total[2] = {0, 0};
	string instr;
	evaluate(board, total);
	if(total[0] < total[1]){
		cout << "White wins with " << total[1] << " points to Black's " << total[0] << " points!\n";
	}else if(total[0] > total[1]){
		cout << "Black wins with " << total[0] << " points to White's " << total[1] << " points!\n";
	}else{
		cout << "Tie Game! [" << total[1] << "] points.\n";
	}
	n_game = '0';
	while(n_game == '0'){
		cout << "Do you want to play again?(y/n): ";
		cin >> instr;
		if(instr[0] == 'y'){
			n_game = 'y';
		}else if(instr[0] == 'n'){
			n_game = 'n';
		}else{
			cout << "ERROR: invalid input\n";
		}
	}
}

bool surrounded(int board[19][19], int pos[2], int turn){ // returns true if there is only 1 space open for specified group
	int spaces = 0;
	int occupied = 0;
	for(int c = -1; c <= 1; c++){ // for each adjacent pos to open space
		for(int d = -1; d <= 1; d++){
			if(c * c != d * d){
				if((((pos[0] + c) >= 0) && ((pos[0] + c) < size_d)) && (((pos[1] + d) >= 0) && ((pos[1] + d) < size_d))){ // if on board
					spaces++;
					if(((turn) && (board[pos[0] + c][pos[1] + d] == 2)) || ((!turn) && (board[pos[0] + c][pos[1] + d] == 0))){ //if color's turn, and adjacent == -adjacent
						occupied++;
					}
				}
			}
		}
	}
	if(occupied == spaces){ // if all adjacent spaces around position are filled with friendlies
		return true;
	}
	return false;
}

bool ai(int board[19][19], int group[19][19], bool turn, unsigned int & group_num){ // returns true if skipping turn
	int pos[2];
	int b_temp[19][19];
	int g_temp[19][19];
	double point[19][19];
	group_num++;
	for(int a = 0; a < size_d; a++){ // for entire board, initialize point
		for(int b = 0; b < size_d; b++){
			point[a][b] = 0.0;
		}
	}
	int total[2]; // white is [1]
	/*POINTS SCORECARD
	+1 point 					for each point scored
	+1 point 					for each point lost for opponent
	+radius/points 		for each position not in diagonl from center
	+0.04							for each friendly in 5x5 box around pos
	+0.4 							for each enemy open spot taken
	*/
	for(pos[0] = 0; pos[0] < size_d; pos[0]++){ // for entire board
		for(pos[1] = 0; pos[1] < size_d; pos[1]++){
			if(board[pos[0]][pos[1]] == 1){ // if comp moves there, add diffrence in totals from evaluating to point(pos)
				for(int a = 0; a < size_d; a++){ // for entire board, initialize b_temp and g_temp
					for(int b = 0; b < size_d; b++){
						g_temp[a][b] = group[a][b];
						b_temp[a][b] = board[a][b];
					}
				}
				if(update(b_temp, g_temp, pos, turn, group_num)){ // if its a possible move
					if(surrounded(board, pos, turn)){ // if only 1 space left in group, don't place there
						point[pos[0]][pos[1]] = 0;
					}else{                                               // if more than one space left in group
						evaluate(b_temp, total); // get point totals
						if((turn) && ((total[1] - total[0]) > 0)){ // true = white
							point[pos[0]][pos[1]] += total[1] - total[0];
						}else if((!turn) && ((total[0] - total[1]) > 0)){
							point[pos[0]][pos[1]] += total[0] - total[1];
						}else{
							point[pos[0]][pos[1]] = 0;
						}
						if((pos[0] + pos[1] != size_d - 1) && (pos[0] != pos[1])){ // less likely to choose positions in straight diagonal from center
							point[pos[0]][pos[1]] += sqrt(pow((pos[0] + 0.5) - (size_d / 2.0), 2) + pow((pos[1] + 0.5) - (size_d / 2.0), 2) + 1) / (1.0 * (total[0] + total[1] + 1.0)); // more likely to choose positions on edge of board earlier in game
						}
						for(int i = -2; i <= 2; i++){ // if same color piece is nearby, increace likelyhood of moving there
							for(int j = -2; j <= 2; j++){
								if((i * i != 4) && ( j * j != 4)){ // exclude corners
									if(((pos[0] + i < size_d) && (pos[0] + i >= 0)) && ((pos[1] + j < size_d) && (pos[1] + j >= 0))){
										if(b_temp[pos[0] + i][pos[1] + j] == b_temp[pos[0]][pos[1]]){ // if friendly is close(slightly more likley to move closer to itself)
											point[pos[0]][pos[1]] += 0.04;
										}else if(b_temp[pos[0] + i][pos[1] + j] != 1){ // if enemy adjacent(for removing enemy open spots)
											if((i * i == 1) && (j * j == 1)){
												point[pos[0]][pos[1]] += 0.25;
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	for(int a = 0; a < size_d; a++){ // for entire board, find largest value, for move position
		for(int b = 0; b < size_d; b++){
			if((point[pos[0]][pos[1]] < point[a][b]) && (point[a][b] != 0)){
				pos[0] = a;
				pos[1] = b;
			}else if(point[pos[0]][pos[1]] == point[a][b]){
				if(rand()%2){ // randomly choose one if find 2 equal
					pos[0] = a;
					pos[1] = b;
				}
			}
		}
	}
	/*cout << "Point Values:\n";//***
	for(int a = 0; a < size_d; a++){ // for entire board, print point value
		for(int b = 0; b < size_d; b++){
			cout << point[a][b] << "   ";
		}
		cout << endl;
	}
	cout << endl;               //****/
	if(point[pos[0]][pos[1]] != 0){ // if valid move, update
		update(board, group, pos, turn, group_num);
		cout << "Computer is moving to " << char(pos[0]+97) << pos[1]+1 << endl;
		return false; // output move
	}
	group_num--;
	cout << "Computer is skipping Turn\n";
	return true; // skip turn if no good point values
}

void c_initial(bool & single, bool & c_first){ // sets up if computer is beign used, and whether computer goes first
	string instr;
	while(true){
		cout << "Singleplayer or Multiplayer (s/m): ";
		cin >> instr;
		if(instr[0] == 's'){
			single = true;
			while(true){
				cout << "Computer goes first (y/n): ";
				cin >> instr;
				if(instr[0] == 'y'){
					c_first = true;
					return;
				}else if(instr[0] == 'n'){
					c_first = false;
					return;
				}
				cout << "ERROR: invalid input\n";
			}
		}else if(instr[0] == 'm'){
			single = false;
			return;
		}
		cout << "ERROR: invalid input\n";
	}
}

int main(){
	int board[19][19];
	int group[19][19];
	char n_game = 'y';
	bool turn; // false = black, true = white->2
	bool single; // is the game singleplayer
	bool c_first; // does computer go first
	int end; // stores number of players that have skipped turn that round
	unsigned int group_num = 0;
	srand(time(0));
	cout << "\n::  T E R M I N A l   G O  ::\n     By: Matthew Russell\n\nVersion: 1.7\n";
	//Rules ->
	cout << "\nGO RULES:\nGo is a turn based game where players alternately place their pieces on a square grid.\nThe goal of that game is to completely surround or occupy space on the board.\nThe game ends when both players skip their turn consecutively.\nThe player with the most space surrounded or occupied completely by their color wins.\nHowever, pieces of your color completely surrounded by the opposing color with no\n adjacent blank spots are deleted!\n\n";
	while(n_game == 'y'){
		c_initial(single, c_first);
		start_pos(board, group, group_num);
		cout << "Black goes first, and loses ties.\n";
		cout << "Piece Characters: " << piece_black << " = Black, " << piece_white << " = White, " << piece_blank << " = Blank\n";
		display(board);
		end = 0;
		turn = false;
		while(end < 2){
			//Determine who gets to play:
			if((turn != c_first) && (single)){ // turn false is black(goes first), c_first true is comp goes first, and not singleplayer
				if(ai(board, group, turn, group_num)){ // if ai skipping turn
					end++;
				}else{
					end = 0;
				}
			}else{
				if(player(board, turn, group, group_num)){ // if player skipping turn
					end++;
				}else{
					end = 0;
				}
			}
			display(board);
			if(end >= 2){
				win_game(board, n_game);
			}
			turn = !turn;
		}
	}
	return 0;
}





