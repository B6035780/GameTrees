#include "TicTacToe.h"
#include <cstdio>
#include <iterator>
#include <stdio.h>

int TicTacToe::win_counter[N_POS + 1] = {},
    TicTacToe::lose_counter[N_POS + 1] = {},
    TicTacToe::draw_counter = 0,
    TicTacToe::leaf_counter = 0,
    TicTacToe::node_counter = 0;

TicTacToe *TicTacToe::get_child(smallint move) {
	
	if (!children[move])
		children[move] = new TicTacToe(this, move, -INF, +INF);

	return children[move];
}

bool TicTacToe::is_win() const {
    const smallint pt = parent->turn;
    switch (move) {
    case 0:
        return (s[1] == pt && s[2] == pt) ||
               (s[3] == pt && s[6] == pt) ||
               (s[4] == pt && s[8] == pt);
    case 1:
        return (s[0] == pt && s[2] == pt) ||
               (s[4] == pt && s[7] == pt);
    case 2:
        return (s[1] == pt && s[0] == pt) ||
               (s[5] == pt && s[8] == pt) ||
               (s[4] == pt && s[6] == pt);
    case 3:
        return (s[4] == pt && s[5] == pt) ||
               (s[0] == pt && s[6] == pt);
    case 4:
        return (s[3] == pt && s[5] == pt) ||
               (s[1] == pt && s[7] == pt) ||
               (s[0] == pt && s[8] == pt) ||
               (s[2] == pt && s[6] == pt);
    case 5:
        return (s[4] == pt && s[3] == pt) ||
               (s[2] == pt && s[8] == pt);
    case 6:
        return (s[7] == pt && s[8] == pt) ||
               (s[3] == pt && s[0] == pt) ||
               (s[4] == pt && s[2] == pt);
    case 7:
        return (s[6] == pt && s[8] == pt) ||
               (s[4] == pt && s[1] == pt);
    case 8:
        return (s[7] == pt && s[6] == pt) ||
               (s[5] == pt && s[2] == pt) ||
               (s[4] == pt && s[0] == pt);
    default:
        return false;
    }
}

TicTacToe::TicTacToe():
        turn(MAX), move(-1), depth(0), alpha(-INF), beta(+INF), s(), parent(nullptr) {
    ++node_counter;
    search();
}

TicTacToe::TicTacToe(const TicTacToe *parent, smallint move, smallint alpha, smallint beta):
        turn(-parent->turn), move(move), depth(parent->depth + 1),
        alpha(alpha), beta(beta), parent(parent) {

	++node_counter;
	std::copy(std::begin(parent->s), std::end(parent->s), s);
	s[move] = parent->turn;

	bool isWin = is_win();
	bool isFull = (depth == N_POS);

	if (isWin || isFull)
	{
		++leaf_counter;
		if (isWin)
		{
			if (parent->turn == MAX)
				++win_counter[depth];
			else
				++lose_counter[depth];

			v = parent->turn * (10 - depth);
		}
		else
		{
			++draw_counter;
			v = ZERO;
		}
	}
	else
	{
		search();
	}

    }

void TicTacToe::search() {
	if (turn == MAX)
	{
		smallint max = -INF;
		for (smallint p = 0; p < N_POS; ++p)
		{
			if (s[p] == ZERO)
			{
				children[p] = new TicTacToe(this, p, alpha, beta);
				if (children[p]->v > max)
				{
					max = children[p]->v;

					if (max >= beta)	//pruning
					{
						v = max;
						return;
					}
				}
			}
		}
		v = max;
	}
	else
	{
		smallint min = +INF;
		for (smallint p = 0; p < N_POS; ++p)
		{
			if (s[p] == ZERO)
			{
				children[p] = new TicTacToe(this, p, alpha, beta);
				if (children[p]->v < min)
				{
					min = children[p]->v;
					if (min <= alpha)
					{
						v = min;
						return;
					}
				}
			}
		}
		v = min;
	}
}

TicTacToe::~TicTacToe() {
    for (TicTacToe *child : children)
        delete child;
}

// Visualize a stone
char v(int s) {
    const char X = 'x', O = 'o', S = ' ';
    if (s == TicTacToe::MAX)
        return X;
    else if (s == TicTacToe::MIN)
        return O;
    else // if (s == TicTacToe::ZERO)
        return S;
}

std::ostream &operator<<(std::ostream &out, const TicTacToe &t) {
    const int BUFSIZE = 256;
    char buffer[BUFSIZE];
    sprintf(
        buffer,
        "+---+---+---+\n"
        "| %c | %c | %c |\n"
        "+---+---+---+\n"
        "| %c | %c | %c |\n"
        "+---+---+---+\n"
        "| %c | %c | %c |\n"
        "+---+---+---+\n",
        v(t.s[0]), v(t.s[1]), v(t.s[2]),
        v(t.s[3]), v(t.s[4]), v(t.s[5]),
        v(t.s[6]), v(t.s[7]), v(t.s[8])
    );
    return out << buffer;
}
