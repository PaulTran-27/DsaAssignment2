#include "main.h"

using namespace std;
// class customer {
// 	public: 
// 		string name;

// }

class HuVLTree: {
	public: 
		HuVLNode* root;
	public:

		class HuVLNode {
			public:
				int    weight;
				char   data;
				int    balance;
				HuVLNode* left;
				HuVLNode* right;
				
				friend class HuVLTree;

			public:
				HuVLNode(int weight, char data = NULL, HuVLNode* left = nullptr, HuVLNode* right = nullptr) {
					this->weight = weight;
					this->data = data;
					this->left = left;
					this->right = right;
				};
				~HuVLNode() {
					delete this->left;
					delete this->right;
				}
		};

		HuVLTree ()
}
class JJK_Restaurant {
	public:
		int MAXSIZE;
	public:
		JJK_Restaurant() : MAXSIZE(0) {};
		~JJK_Restaurant(){};

		void set_maxsize(int size) {
			this->MAXSIZE = size;
		}
		map<char, int> counter(string customer_name) {
			map<char, int> char_to_freq;
			for (char ch: customer_name) {
				char_to_freq[ch]++;
			}
			return char_to_freq;
		}

		char convert_char(char ch, map<char, int> char_to_freq){
			char to_encrypt = ch;
			int  shift      = char_to_freq[ch];
			if ('a' <= to_encrypt && to_encrypt <= 'z') {
				to_encrypt += shift;
				if (to_encrypt > 'z') {
					to_encrypt = to_encrypt - 'z' + 'a' - 1;
				}
			} else {
				to_encrypt += shift;
				if (to_encrypt > 'Z') {
					to_encrypt = to_encrypt - 'Z' + 'A' - 1;
				}
			}
			return to_encrypt;
		}

		static bool compare_map(pair<char,int>& a, pair<char,int>& b){
			if (a.second == b.second) {
				if ('a' <= a.first <= 'z'){
					if ('a' <= b.first <= 'z') {
						return a.first < b.first;
					}
					return a.first > b.first;
				} else if ('a' <= b.first <= 'z'){
					if ('a' <= b.first <= 'z') {
						return b.first < a.first;
					}
					return b.first > a.first;
				}
				return a.first < b.first;
			} 
			return a.second < b.second;
		}

		string ceasar_encrypt(string to_encrypt){
			string encrypted = "";
			map<char, int> char_to_freq = this->counter(to_encrypt);
			// for (auto pair: char_to_freq){
			// 	cout << pair.first << " " << pair.second << setw(5);
			// }
			// cout << endl;
			for (char ch: to_encrypt){
				// cout << this->convert_char(ch, char_to_freq) << " " << setw(5);

				encrypted += this->convert_char(ch, char_to_freq);
			};


			return encrypted;
		}
		void jutsushiki_junten(string customer_name){
			cout << customer_name << endl;
			string encrypted_n = this->ceasar_encrypt(customer_name);
			cout << encrypted_n << endl;
			map<char, int> char_to_freq_after_encrypt = this->counter(encrypted_n);
			// Sort list X
			vector<pair<char,int>> sorted_list_X;
			for (auto& pair: char_to_freq_after_encrypt){
				sorted_list_X.push_back(pair);
			}
			sort(sorted_list_X.begin(), sorted_list_X.end(), this->compare_map);
			for (auto i: sorted_list_X) {
				cout << i.first << " " << i.second << setw(5);
			}
			cout << endl;
			// cout << encrypted_n << endl;
		};
};

void simulate(string filename)
{
	JJK_Restaurant* r = new JJK_Restaurant();

	cout << "Good Luck\n";
	ifstream ss(filename);
	string str, maxsize, name, energy, num;
	int count = 1;
	while(ss >> str)
	{ 
		if(str == "MAXSIZE")
		{
			ss >> maxsize;
			cout << maxsize + '\n';
			// MAXSIZE = stoi(maxsize); 
			r->set_maxsize(stoi(maxsize));
    	}
        else if(str == "LAPSE") // RED <NAME> <ENERGY>
        {
			cout << "\nJUNTEN !!\n";
            ss >> name;
			r->jutsushiki_junten(name);
    	}
    	else if(str == "HAND") // BLUE <NUM>
    	{
                // ss >> num;
    			// r->BLUE(stoi(num));
		}
    	else if(str == "CLEAVE") // PURPLE
    	{

    			// r->PURPLE();
		}
		else if(str == "KOKUSEN") // REVERSAL
		{
    			// r->REVERSAL();	
		}
    	else if(str == "LIMITLESS") // UNLIMITED_VOID
     	{   	
    			// r->UNLIMITED_VOID();
    	}
    	else if(str == "KEITEIKEN") // DOMAIN_EXPANSION
    	{
    			// r->DOMAIN_EXPANSION();
    	}
    	
    }
	
	delete r;
	return;
}