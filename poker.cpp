#include <iostream>
#include <string>
#include <cstdlib>
#include <cmath>

using namespace std;

class card {
	public:
		int strength;
		char suit;
		void setdata(string rep) {
			suit = rep[0];
			string value = rep.substr(1);
			if((value != "K") && (value != "Q") && (value != "J") && (value != "A"))
				strength = atoi(value.c_str());
			else if(value == "K")
				strength = 13;
			else if(value == "Q")
				strength = 12;
			else if(value == "J")
				strength = 11;
			else if(value == "A")
				strength = 14;
		}

		void printdata() {
			cout<<suit<<strength<<" ";
		}

		void copycard(card c) {
			suit = c.suit;
			strength = c.strength;
		}

		bool comparecard(card c) {
			bool flag = true;
			if(c.strength != strength)
				flag = false;
			if (c.suit != suit)
				flag =  false;
			return flag;
		}
};

class Hand {
	public:
		float strength;
		string representation;

	void setdata(float st, string rep) {
		strength = st;
		representation = rep;
	}

	void printdata() {
		cout<<representation<<": "<<strength;
	}
	void copyhand(Hand h) {
		strength = h.strength;
		representation = h.representation;
	}
};

//functions to check hands
bool checkflush(card A[5]) {
	char hs = A[0].suit;
	for(int i = 1; i < 5; i++){
		if(A[i].suit != hs)
			return false;
	}
	return true;
}

int checkstr(card A[5]) {
	int s = 0;
	bool flag = true, flag2 = true;
	for(int i = 0; i < 4; i++) {
		if(A[i + 1].strength != (A[i].strength + 1)) {
			flag = false;
		}
	}

	if(flag) {
		s = A[4].strength;
		return s;
	}

	// checking for first straight
	for(int j = 0; j < 3; j++) {
		if(A[j + 1].strength != (A[j].strength + 1)) {
			flag2 = false;
		}
	}

	if(flag2 && (A[3].strength == 5) && (A[4].strength == 14)) {
		s = 5;
	}
	return s;
}

int highcard(card A[5]) {
	int max = A[0].strength;
	for(int i = 1; i < 5; i++) {
		if(A[i].strength > max)
			max = A[i].strength;
	}
	return max;
}

//sorting functions
void sort(card A[]) {
	card B[5];
 	int j;
	B[0].copycard(A[0]);
	for(int i = 1; i < 5; i++) {
		j = i - 1;
		if(A[i].strength >= B[j].strength) {
			B[i].copycard(A[i]);
		} else {
			while((A[i].strength < B[j].strength) && (j >= 0)) {
				B[j + 1].copycard(B[j]);
				j--;
			}
			B[j + 1].copycard(A[i]);
		}
	}

	for(int k = 0; k < 5; k++) {
		A[k].copycard(B[k]);
	}
}

void sort_hand(Hand A[], int l) {
	Hand B[l];
 	int j;
	B[0].copyhand(A[0]);
	for(int i = 1; i < l; i++) {
		j = i - 1;
		if(A[i].strength >= B[j].strength) {
			B[i].copyhand(A[i]);
		} else {
			while((A[i].strength < B[j].strength) && (j >= 0)) {
				B[j + 1].copyhand(B[j]);
				j--;
			}
			B[j + 1].copyhand(A[i]);
		}
	}

	for(int k = 0; k < l; k++) {
		A[k].copyhand(B[k]);
	}
}

//fuction to calculate the strength of the hand
float calcst(card B[5]) {
	card A[5];
	for(int k = 0; k < 5; k++) {
		A[k].copycard(B[k]);
	}

	sort(A);

	int  xfl = 0, xstr, xstfl = 0, xp = 0, x3 = 0, x4 = 0, x2p = 0, xfh = 0;
	float st = 0;
	// checking for flush
	if(checkflush(A) == true) {
		xfl = A[4].strength;
	}

	// checking for straight
	xstr = checkstr(A);

	// checking for straight flush
	if((xfl != 0) && (xstr != 0)) {
		xstfl = xstr;
		xstr = xfl = 0;
	}

	// checking for pair, three of kind, four of kind
	int index = -1;
	for(int i = 0; i < 4; i++) {
		if(A[i].strength == A[i + 1].strength) {
			xp = A[i].strength;
			index = i + 1;
			break;
		}
	}

	if(xp && (index < 4)) {
		if(xp == A[index + 1].strength) {
			x3 = xp;
			xp = 0;
		}
	}

	if(x3 && (index < 3)) {
		if(x3 == A[index + 2].strength) {
			x4 = x3;
			x3 = 0;
		}
	}

	if(x3 && (index != 3) && (A[3].strength == A[4].strength)) {
		xfh = x3;
		x3 = 0;
	}

	else if(xp && (index == 1)) {
		if((A[2].strength == A[3].strength) && (A[3].strength == A[4].strength)) {
			x3 = A[2].strength;
			xfh = x3;
			x3 = 0;
		}
		else if((A[2].strength == A[3].strength) || (A[3].strength == A[4].strength)) {
			x2p = A[3].strength;
			x2p *= xp;
			xp = 0;
		}
	}

	else if(xp && (index == 2)) {
		if(A[3].strength == A[4].strength)
			x2p = A[3].strength;
			x2p *= xp;
			xp = 0;
	}

	st = xp + (6 * sqrt(x2p)) + (38 * x3) + (217 * xstr) + (1520 * xfl) + (10641 * xfh) + (74488 * x4) + (521416 * xstfl);
	return st;

}

//helper funtions
string stringify_rep(string B[5]) {
	string hand;
	for(int i = 0; i < 5; i++) {
		hand += B[i] + " ";
	}
	return hand;
}

string* gendeck() {
	string suit[4] = {"H", "S", "C", "D"};
	string* deck = new string[52];
	int k = 0;
	for(int i = 0; i < 4; i++) {
		for(int j = 2; j < 15; j++) {
			deck[k] = suit[i] + to_string(j);
			k++;
		}
	}
	return deck;
}

int main() {
	card hand[5];
	string hand_rep[5];
	Hand hands_arr[2550];
	int rank = 0;

	cout<<"\nEnter the value of your cards: \n";
	string rep;
	for(int i = 0; i < 2; i++) {
		cin>>rep;
		hand[i + 3].setdata(rep);
		hand_rep[i + 3] = rep;
	}

	cout<<"Enter the value of cards on table: \n";
	string c;
	for(int i = 0; i < 3; i++) {
		cin>>c;
		hand_rep[i] = c;
		hand[i].setdata(c);
	}

	float my_hand_st = calcst(hand);
	cout<<"Strength of your hand: "<<my_hand_st;

	string* deck = gendeck();

	int k = 0;
	float st;
	for(int i = 0; i < 52; i++) {
		if(deck[i] != hand_rep[0] && deck[i] != hand_rep[1] && deck[i] != hand_rep[2]) {
			hand[3].setdata(deck[i]);
			hand_rep[3] = deck[i];
		}
		else
			continue;
		for(int j = i + 1; j < 52; j++){
			if(deck[j] != hand_rep[0] && deck[j] != hand_rep[1] && deck[j] != hand_rep[2]) {
				hand[4].setdata(deck[j]);
				hand_rep[4] = deck[j];
			}
			else
				continue;
			st = calcst(hand);
			if(st != 0) {
				hands_arr[k].setdata(st, stringify_rep(hand_rep));
			}
			if(st >= my_hand_st)
				rank++;
			k++;
		}
	}

	sort_hand(hands_arr, k);

	cout<<"\nThe rank is: "<<rank<<endl;

	for(int i = 0; i < k; i++) {
		if(hands_arr[i].strength >= my_hand_st) {
			hands_arr[i].printdata();
			cout<<endl;
		}
	}

	delete[] deck;

	return 0;
}
