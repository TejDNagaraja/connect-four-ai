#include<cstdlib>
#include<iostream>
#include<getopt.h>
#include<string>
#include<vector>
#include<sstream>
#include<utility>


using namespace std;

int minimax(vector<vector<int> > & board, pair<int, int> last_move, int me, int depth, int & my_best_depth, int & opp_best_depth, int & move, bool myTurn);
bool finished(vector<vector<int> > & board, pair<int, int> last_move, int & winner);
int fastMove(vector<vector<int> > & board);

int main(int argc, char * argv[])
{
	vector< vector<int> > board(6,vector<int>(7,0));
	istringstream s(argv[2]);
	string player = argv[4];
	int me;
	int move = 0;
	int my_best_depth = 1000;
	int opp_best_depth = 1000;
	char waste_char;
	
	int count = 0;
	
	for(int i = 0; i < 6; i++)
	{
		s >> waste_char >> waste_char;
		for(int j = 0; j < 7; j++)
		{
			s >> board[i][j] >> waste_char;
			if(i == 5)
			{
				if(board[i][j] == 0)
				{
					count++;
				}
			}
		}
	}
	
	if(count == 7)
		exit(3);
		
	if(!player.compare("player-one"))
		me = 1;
	else
		me = 2;
	
	minimax(board, make_pair(0,0), me, 0, my_best_depth, opp_best_depth, move, true);
	
	cout << move << "\n";
	exit(move);
}

int minimax(vector<vector<int> > & board, pair<int, int> last_move, int me, int depth, int & my_best_depth, int & opp_best_depth, int & move, bool myTurn)
{
	
	vector<int> scores;
	vector<int> moves;
	int winner;
	
	if(finished(board, last_move, winner))
	{
		//I win
		if(me == winner)
		{
			if(depth < my_best_depth)
				my_best_depth = depth;
			return 100 - depth;
		}
		//Tie
		if(winner == 0)
			return 0;
		//Opponent Wins
		if(depth < opp_best_depth)
			opp_best_depth = depth;
   		 	return depth - 100;
	}
		
	if(myTurn)
	{
		if(depth > my_best_depth)
			return -101;
		
		vector<bool> moveFound(7, false);
		for(int i = 5; i >= 0; i--)
		{
			for(int j = 0; j < 7; j++)
			{
				if(!moveFound[j] && board[i][j] == 0)
				{	
					board[i][j] = me;
					moveFound[j] = true;
					scores.push_back(minimax(board, make_pair(i,j), me, depth + 1, my_best_depth, opp_best_depth, move, !myTurn));
					moves.push_back(j);					
					board[i][j] = 0;
				}
			}
		}
		
		//find max score
		int max_index = 0;
		int max = -101;
		for(int i = 0; i < scores.size(); i++)
		{
			if(scores[i] > max)
			{
				max = scores[i];
				max_index = i;
			}
		}
				
		move = moves[max_index];
		return scores[max_index];
	}
	else
	{
		if(depth > opp_best_depth)
			return 101;
		
		vector<bool> moveFound(7, false);
		for(int i = 5; i >= 0; i--)
		{
			for(int j = 0; j < 7; j++)
			{
				if(!moveFound[j] && board[i][j] == 0)
				{
					board[i][j] = -1 * (me - 3);
					moveFound[j] = true;
					scores.push_back(minimax(board, make_pair(i,j), me, depth + 1, my_best_depth, opp_best_depth, move, !myTurn));
					moves.push_back(j);
					board[i][j] = 0;
				}
			}
		}
		
		//find min score
		int min_index = 0;
		int min = 101;
		for(int i = 0; i < scores.size(); i++)
		{
			if(scores[i] < min)
			{
				min = scores[i];
				min_index = i;
			}
		}
		
		move = moves[min_index];
		return scores[min_index];
	}
	return fastMove(board);	
}

bool finished(vector<vector<int> > & board, pair<int, int> last_move, int & winner)
{
	int streak = 1;
	//check horizontal
	for(int i = 0; i < board[0].size() - 1; i++)
	{
		if(board[last_move.first][i] != 0 && board[last_move.first][i] == board[last_move.first][i+1])
		{
			streak++;
			if(streak == 4)
			{
				winner = board[last_move.first][last_move.second];
				return true;
			}
		}
		else
			streak = 1;
	}
	streak = 1;
	//check vertical
	for(int i = board.size() - 1; i > 0; i--)
	{
		if(board[i][last_move.second] != 0 && board[i-1][last_move.second] == board[i][last_move.second])
		{
			streak++;
			if(streak == 4)
			{
				winner = board[last_move.first][last_move.second];
				return true;
			}
		}
		else
			streak = 1;
	}
	streak = 1;
	
	//check negative diagonals
	int i = last_move.first;
	int k = last_move.first;
	int j = last_move.second;
	int l = last_move.second;

	while(i > 0 && j > 0)
	{
		i--; 
		j--;
	
	}
	
	while(i < board.size() - 1 && j < board[0].size() - 1)
	{
		if(board[i][j] != 0 && board[i+1][j+1] == board[i][j])
		{
			streak++;
			if(streak==4)
			{
				winner = board[last_move.first][last_move.second];
				return true;
			}
		}
		else
			streak = 1;
		i++;
		j++;
	}

	streak = 1;
	while(k < board.size() - 1 && l > 0)
	{
		k++;
		l--;
	}
	while(k > 0 && l < board[0].size() - 1)
	{
		if(board[k][l] != 0 && board[k-1][l+1] == board[k][l])
		{
			streak++;
			if(streak==4)
			{
				winner = board[last_move.first][last_move.second];
				return true;
			}
		}
		else
			streak = 1;
		k--;
		l++;
	}

	return false;
}

int fastMove(vector<vector<int> > & board)
{
	for(int j = 0; j < 7; j++)
	{
		if(board[0][j] == 0)
			return j;
	}
	return 0;
}