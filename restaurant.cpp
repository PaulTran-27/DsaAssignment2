#include "main.h"

using namespace std;
// class customer {
// 	public: 
// 		string name;

// }
class HuVLNode {
	public:
		int    weight;
		char   data;
		HuVLNode* left;
		HuVLNode* right;

		friend class HuVLTree;

	public:
		HuVLNode(int weight, char data = ' ', HuVLNode* left = nullptr, HuVLNode* right = nullptr) {
			this->weight = weight;
			this->data = data;
			this->left = left;
			this->right = right;
		};

		~HuVLNode() {
			if (this->left) delete this->left;
			
			if (this->right) delete this->right;
			this->right = nullptr;
					this->left = nullptr;
		}
		
		void print() {
			cout << "Data: " << this->data << " Frequency: " << this->weight;
			if (this->left) cout << " " << this->left->data;
			if (this->right) cout << " " << this->right->data; 
			cout << "\n";
		}
		HuVLNode* rightRotate(HuVLNode* y) {
			HuVLNode* x = y->left;
			HuVLNode* T = x->right;

			x->right = y;
			y->left = T;

			return x;
		}

		HuVLNode* leftRotate(HuVLNode* x) {
			HuVLNode* y = x->right;
			HuVLNode* T = y->left;

			y->left = x;
			x->right = T;

			return y;
		}
	
		int getHeight(HuVLNode* node) {
			if (node == nullptr) return 0;
			
			int leftHeight = getHeight(node->left);
			int rightHeight = getHeight(node->right);

			return max(leftHeight, rightHeight) + 1;
		}

		int getBalance(HuVLNode* node) {
			if (node == nullptr) return 0;

			int leftHeight = getHeight(node->left);
			int rightHeight = getHeight(node->right);

			return leftHeight - rightHeight;
		}

		HuVLNode* convertToAVL(HuVLNode* root, int& rotations) {
			if (root == nullptr || rotations == 1) return root;

			root->left = convertToAVL(root->left, rotations);
			root->right = convertToAVL(root->right, rotations);

			int balance = getBalance(root);

			// Left heavy
			if (balance > 1) {
				// cout << "---------balance: " << balance << " ROTE: " << rotations << endl;
				if (getBalance(root->left) >= 0) {
					// Left-Left Case
					// cout << "You spin me left left!!" << endl;
					root = rightRotate(root);
					rotations++;
				} else {
					// Left-Right Case
					// cout << "You spin me left right!!" << endl;

					root->left = leftRotate(root->left);
					root = rightRotate(root);
					rotations++;
				}
			}
			// Right heavy
			else if (balance < -1) {
				// cout << "---------balance: " << balance << " ROTE: " << rotations << endl;
				if (getBalance(root->right) <= 0) {
					// Right-Right Case
					// cout << "You spin me right right!!" << endl;
					root = leftRotate(root);
					rotations++;
				} else {
					// Right-Left Case
					// cout << "You spin me right left!!" << endl;
					root->right = rightRotate(root->right);
					root = leftRotate(root);
					rotations++;
				}
			}

			return root;
		}
};

class HuVLTree {


	public:
		HuVLNode* root;
		string identifier;
		map<char, string> char_to_bin;
	public:

		HuVLTree(HuVLNode* r) : root(r) {};
		HuVLTree(HuVLTree* tree) {
			this->root = this->copy_huvl(tree->root);
		}
		~HuVLTree() {
			if (this->root) delete this->root;
			char_to_bin.clear();
		};

		void set_identifier(string name){
			this->identifier = name;
		};

		HuVLNode* copy_huvl(HuVLNode* root) {
			if (root == nullptr) {
				return nullptr;
			}

			HuVLNode* newRoot = new HuVLNode(root->weight, root->data);
			newRoot->left = copy_huvl(root->left);
			newRoot->right = copy_huvl(root->right);

			return newRoot;
		}

		void inorder_print(HuVLNode* node){
			if (node == nullptr) return;
			this->inorder_print(node->left);
			
			if (!isspace(node->data)){
				cout << node->data << '\n';
			} else {
				cout << node->weight << '\n';
			}

			this->inorder_print(node->right);
		};
		void get_bin_mapping(HuVLNode* node, string bin)
		{
			if (node == nullptr) return;
			if (!node->left && !node->right) {
				this->char_to_bin[node->data] = bin;
			}
			this->get_bin_mapping(node->left,  bin + "0");
			this->get_bin_mapping(node->right, bin + "1");
		}
		void update_bin_mapping(){
			this->get_bin_mapping(this->root, "");
		}
		string get_bin_code(char data){
			if (!this->char_to_bin.size()) this->update_bin_mapping();
			return this->char_to_bin[data];
		}
		string get_huffcode(char ch){
			return this->char_to_bin[ch];
		}
};

class MuryoKusho {
	public: 
		class bst_container {
			public:
				int result;
				HuVLTree* customer;
				bst_container* left;
				bst_container* right;
				friend class bst_table;
			public:
				bst_container(HuVLTree* cus, int res, bst_container* l = nullptr, bst_container* r = nullptr) : 
							result(res), customer(cus) , left(l), right(r) {};
				~bst_container() {
					// cout << "bst_container des: " << this->result << " ";
					// if  (this->customer) cout << this->customer->identifier << endl;
					// else cout << "\n";
					delete customer;
					if (this->right) delete right;
					if (this->left)  delete left;
					this->right = nullptr;
					this->left = nullptr;
				};
				int get_result() {return this->result;}

		};
		class bst_table {
			public:
				bst_container* root;
				queue<pair<int,string>> customers_in_order;
			public:
				bst_table(bst_container* r = nullptr) : root(r) {};
				~bst_table() { delete root;};
				void clear() {
					delete root;
					this->root = nullptr;
					queue<pair<int,string>> q;
					customers_in_order.swap(q);
				}
				string generateRandomKey(int length) {
					const string charset = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
					random_device rd;
					mt19937 gen(rd());
					uniform_int_distribution<> dis(0, charset.size() - 1);
					string key;
					for (int i = 0; i < length; ++i) {
						key += charset[dis(gen)];
					}
					return key;
				}
				void insert(bst_container* cus) {
					cus->customer->identifier = this->generateRandomKey(100);
					customers_in_order.push(make_pair(cus->result,cus->customer->identifier));
					
					if (this->root == nullptr) {
						this->root = cus;
					} else {
						bst_container* root = this->root;
						while (root){
							if (cus->get_result() >= root->get_result()) {
								if (root->right) root = root->right;
								else {
									root->right = cus;
									break;
								}
							} else {
								if (root->left) root = root->left;
								else {
									root->left = cus;
									break;
								}
							}
						}
					}
				}
				int size(bst_container* node){
					if (node == nullptr) return 0;

					return 1 + size(node->left) + size(node->right);
				}
				bst_container* findNodeToDelete(bst_container* root, pair<int,string> nodeToDelete) {
					bst_container* current = root;

					while (current != nullptr) {
						if (nodeToDelete.second == current->customer->identifier) {
												//  cout<< current->customer->identifier << endl;

							return current;
						} else {
							if (nodeToDelete.first < current->result) {
								current = current->left;
							} else {
								current = current->right;
							}
						}
					}
					return current;
				}

				bst_container* delete_node(bst_container* root, pair<int,string> nodeToDelete) {
					bst_container* toDelete = findNodeToDelete(root, nodeToDelete);
					if (toDelete == nullptr) {
						return root; // Node not found
					}

					bst_container* parent = nullptr;
					bst_container* current = root;

					while (current != toDelete) {
						parent = current;
						if (toDelete->customer->identifier == current->customer->identifier) {
							break;
						} else {
							if (toDelete->result < current->result) {
								current = current->left;
							} else {
								current = current->right;
							}
						}
					}
					// cout << current->customer->identifier << endl;

					// Handling deletion based on child count
					if (toDelete->left == nullptr) {
						// cout << "-----IM HAVE ONLY RIGHT!!------" << endl;

						bst_container* temp = toDelete->right;
						if (parent == nullptr) {
							root = temp;
						} else if (parent->left == toDelete) {
							parent->left = temp;
						} else {
							parent->right = temp;
						}
						// delete toDelete->customer;
						// toDelete->customer = nullptr;
						toDelete->left = nullptr;
						toDelete->right = nullptr;
						delete toDelete;
					} else if (toDelete->right == nullptr) {
						// cout << "-----IM HAVE ONLY LEFT!!------" << endl;
						bst_container* temp = toDelete->left;
						if (parent == nullptr) {
							root = temp;
						} else if (parent->left == toDelete) {
							parent->left = temp;
						} else {
							parent->right = temp;
						}
						// delete toDelete->customer;
						// toDelete->customer = nullptr;
						toDelete->left = nullptr;
						toDelete->right = nullptr;
						delete toDelete;
					} else {
						// cout << "-----IM HAVE CHIlDRENS!!------" << endl;
						bst_container* successorParent = toDelete;
						bst_container* successor = toDelete->right;

						while (successor->left != nullptr) {
							successorParent = successor;
							successor = successor->left;
						}

						if (successorParent != toDelete) {
							successorParent->left = successor->right;
						} else {
							successorParent->right = successor->right;
						}

						delete toDelete->customer;
						toDelete->customer = successor->customer;
						toDelete->result = successor->result;

						successor->customer = nullptr;
						successor->left = nullptr;
						successor->right = nullptr;
						delete successor;
					}

					return root;
				}

				
				void pre_order_print(bst_container* root){
					if (root == nullptr) return;

					cout << root->result << " ";
					pre_order_print(root->left);
					pre_order_print(root->right);
				}

		};
	public:
		map<int, bst_table*> hash_table;
		int maxsize;
		// bst_container* most_recent_cus;
	public:
		MuryoKusho(int MAXSIZE) : maxsize(MAXSIZE) {
			for (int i = 1; i <= MAXSIZE; i++){
				this->hash_table[i] = new bst_table();
			}
		}

		~MuryoKusho() {
			for (int i = 1; i <= this->maxsize; i++){
				delete this->hash_table[i];
			}
			// no need to delete this
			// this->most_recent_cus = nullptr;

		}

		void arange_table(HuVLTree* cus, int result){
			int id = (result % this->maxsize) + 1;
			bst_container* cus_repr = new bst_container(cus, result);
			this->hash_table[id]->insert(cus_repr); 
			// this->most_recent_cus = cus_repr;
		}
		void inorder_print(bst_container* root){
			if (root == nullptr) return;
			inorder_print(root->left);
			cout << root->result << "\n"; 
			inorder_print(root->right);
		}
		void do_mugen(int id) {
			if (!this->hash_table.count(id)){
				// cout << "EMPTY" << endl;
				return;
			}
			bst_table* table_to_print = this->hash_table[id];
			if (table_to_print->root) {
				this->inorder_print(table_to_print->root);
			} 
			// else cout << "EMPTY" << endl;

		}
		class Kokusen_BST {
			public:
				class kok_node {
					public:
						int result;
						string data;
						kok_node* left;
						kok_node* right;
					public:
						kok_node(int res) : left(nullptr), right(nullptr) {
							this->data = to_string(res);
							this->result = res;
						};
						kok_node(int res, string data) : left(nullptr), right(nullptr){
							this->data = data;
							this->result = res;
						}
						~kok_node() {
							delete left;
							delete right;
						};

				};
			public:
				kok_node* root;
			public:
				Kokusen_BST(bst_table* table) {
					this->root = this->copy_bst(table->root);
					this->update_copy(this->root);
				}
				Kokusen_BST() : root(nullptr) {};

				~Kokusen_BST() {
					delete root;
				};
				kok_node* copy_bst(bst_container*  root) {
					if (root == nullptr) {
						return nullptr;
					}

					kok_node* newRoot = new kok_node(root->result);
					newRoot->left = copy_bst(root->left);
					newRoot->right = copy_bst(root->right);

					return newRoot;
				}

				void insert(int res, string data) {
					kok_node* new_node = new kok_node(res,data);
					this->insert(new_node);
				}
				void insert(kok_node* cus) {
					if (this->root == nullptr) {
						this->root = cus;
					} else {
						kok_node* root = this->root;
						while (root){
							if (cus->result >= root->result) {
								if (root->right) root = root->right;
								else {
									root->right = cus;
									break;
								}
							} else {
								if (root->left) root = root->left;
								else {
									root->left = cus;
									break;
								}
							}
						}
					}
				}

				void update_copy(kok_node* root, string str = "") {
					if (root == nullptr) {
						return;
					}
					root->data += str;
					update_copy(root->left , str+"a");
					update_copy(root->right, str+"b");
				}
				string inorder(kok_node* root) {
					string str = "";
					if (root == nullptr) {
						return str;
					}
					
					str += inorder(root->left)   + "-";

					str += to_string(root->result) + "-";

					str += inorder(root->right)  + "-";

					return str;
				}
				string postorder(kok_node* root) {
					string str = "";
					if (root == nullptr) {
						return str;
					}
					
					str += postorder(root->left)   + "-";
					str += postorder(root->right)  + "-";

					str += to_string(root->result) + "-";


					return str;
				}
				void p_postorder(kok_node* root){
					if (root == nullptr) {
						return;
					}
					p_postorder(root->left);
					p_postorder(root->right);
					cout << root->data << endl;

				}
				void v_postorder(kok_node* root, vector<int>& in_vec ){
					if (root == nullptr) {
						return;
					}
					
					v_postorder(root->left, in_vec);
					v_postorder(root->right, in_vec);

					in_vec.push_back(root->result);
				}
				void v_preorder(kok_node* root, vector<int>& in_vec ){
					if (root == nullptr) {
						return;
					}
					in_vec.push_back(root->result);

					v_preorder(root->left, in_vec);
					v_preorder(root->right, in_vec);

				}
				vector<int> levelOrderTraversal(kok_node* root) {
					vector<int> result;
					if (root == nullptr) {
						return result;
					}

					queue<kok_node*> q;
					q.push(root);

					while (!q.empty()) {
						kok_node* current = q.front();
						q.pop();
						result.push_back(current->result);

						if (current->left) {
							q.push(current->left);
						}
						if (current->right) {
							q.push(current->right);
						}
					}

					return result;
			}
		};
    
		long long  numOfWaysHelp(vector<int>& nums, vector<vector<long long>>& pascal) {
			if(nums.size() <= 2) return 1;

			vector<int> leftSubtree, rightSubtree;
			int n = nums.size();
			for(int i=1; i<n; i++){
				if( nums[i] < nums[0] ) leftSubtree.push_back(nums[i]);
				else rightSubtree.push_back(nums[i]);
			}

			long long leftWays = numOfWaysHelp(leftSubtree, pascal);
			long long rightWays = numOfWaysHelp(rightSubtree, pascal);
			int leftLen = leftSubtree.size()  ;
			int rightLen = rightSubtree.size();
			return (pascal[n-1][leftLen] * ((leftWays * rightWays)));
		}

		int numOfWays(vector<int>& nums, vector<vector<long long>>& pascal) {
			int n = nums.size();
			pascal.resize(n+1);
			for(int i=0; i<n+1; i++){
				pascal[i] = vector<long long>(i+1, 1);
				for(int j=1; j<i; j++)
					pascal[i][j] = (pascal[i-1][j-1] + pascal[i-1][j]);
			}
			
			return numOfWaysHelp(nums, pascal);
		}
		void do_kokusen(){
			// Kokusen_BST* kok = new Kokusen_BST(this->hash_table[5]);
			for (int i = 1; i <= this->maxsize; i++) {
				// cout << " black the flash out of area " << i << endl;
				if (this->hash_table[i]->root == nullptr) {
					// cout << "Empty" << endl;
					continue;
				}  
				Kokusen_BST* kok = new Kokusen_BST(this->hash_table[i]);

				// cout << kok->postorder(kok->root) << endl;;

				vector<int> perm_vec = kok->levelOrderTraversal(kok->root);
				// kok->v_preorder(kok->root, perm_vec);
				string str = "";
				// sort(perm_vec.begin(), perm_vec.end());
				// for (auto p : perm_vec) {
				// 	cout << p << "-";
				// 	str += p;
				// }
				// cout << endl;
				int Y = 0;
				// do {

				// 	Kokusen_BST* kok_cmp = new Kokusen_BST();
				// 	for (auto tmp: perm_vec) {
				// 		// cout << tmp.second << endl;
				// 		// cout << tmp << "-" ; 
				// 		Kokusen_BST::kok_node* new_node = new Kokusen_BST::kok_node(tmp);
				// 		kok_cmp->insert(new_node);
				// 	}
				// 	// cout << endl;
				// 	// cout << kok_cmp->postorder(kok_cmp->root) << endl;
				// 	// delete kok_cmp; 
				// 	// break;

				// 	if (kok_cmp->inorder(kok_cmp->root) == kok->inorder(kok->root)) {
				// 		Y++;
				// 	} else {
				// 		cout << kok_cmp->inorder(kok_cmp->root) << " " << kok->inorder(kok->root) << endl;
				// 		cout << endl;
				// 	}
				// 	delete kok_cmp;
				// } while (next_permutation(perm_vec.begin(),perm_vec.end()));
				vector<vector<long long>> pascal;
				Y = this->numOfWays(perm_vec, pascal);
				delete kok;

				
				Y = Y % this->maxsize;
				cout << "KOKUNUM " << Y << endl;
				bst_table* cur_table = this->hash_table[i];
				// cout << cur_table->size(cur_table->root) << endl;
				if (Y >= cur_table->size(cur_table->root)) {
					// cout << "WHY NOT CLEARNING" << endl;
					cur_table->clear(); 
					continue;
				}
				while (Y--){
					// cout << "Before delete: ";cur_table->pre_order_print(cur_table->root);	cout << endl;
					if (cur_table->customers_in_order.empty()) break;
					pair<int,string> cus_to_kick = cur_table->customers_in_order.front();
					// cout << cus_to_kick.second << endl;

					cur_table->root = cur_table->delete_node(cur_table->root, cus_to_kick);
					// cout << "After delete: " ;cur_table->pre_order_print(cur_table->root);	cout << endl;
					if (!cur_table->customers_in_order.empty()) cur_table->customers_in_order.pop();
					else break;
				}
				// cur_table->pre_order_print(cur_table->root);
				// cout << endl;
			}
		}

};
class FukumaMizushi {
	public:
		class sukuna_customer {
			public:
				HuVLTree* customer;
				int result;
			public:
				sukuna_customer(HuVLTree* customer, int res) : customer(customer), result(res){};
				~sukuna_customer() {delete customer;}
		};
	public:
		map<int, deque<sukuna_customer*>*> id_to_area;
		map<int, int>*					   id_to_idle;
		map<int, int>*					   id_to_nums;
		int maxsize;
	public:
		class MinHeap {
		public:
			map<int, int>*					   id_to_idle;
			map<int, int>*					   id_to_nums;
			friend class FukumaMizushi;
		public:
			std::vector<pair<int,int>> heap; // pair < id , num_customers >

			MinHeap(map<int,int>* id_to_idle, map<int,int>* id_to_nums){
				this->id_to_idle = id_to_idle; this->id_to_nums = id_to_nums;
			};
			~MinHeap(){
				// No delete here since id-idle and id-nums are shared pointers
			};
			int parent(int i) {
				return (i - 1) / 2;
			}
			int leftChild(int i) {
				return 2 * i + 1;
			}
			int rightChild(int i) {
				return 2 * i + 2;
			}
			void reheap_up(int i) {
				while (i > 0 && ((heap[parent(i)].second > heap[i].second))) {
					if (heap[parent(i)].second == heap[i].second) {
						if ((*this->id_to_idle)[heap[parent(i)].first] <
							(*this->id_to_idle)[heap[i].first]) {
								swap(heap[i], heap[parent(i)]);
								i = parent(i);
								continue;
							}
					}
					swap(heap[i], heap[parent(i)]);
					
					i = parent(i);
				}
			}

			void reheap_down(int i) {
				int minIndex = i;
				int left = leftChild(i);
				int right = rightChild(i);
				int heap_size = heap.size();
				if (left < heap_size && heap[left].second < heap[minIndex].second) {
					minIndex = left;
				}

				if (right < heap_size && heap[right].second < heap[minIndex].second) {
					minIndex = right;
				}

				if ( (right < heap_size && left < heap_size)){
					if (minIndex == i) {
						if((heap[right].second == heap[minIndex].second) || (heap[left].second == heap[minIndex].second)) {
							cout << "#####IM HERERERERERERERERER######\n";
							if ((heap[right].second == heap[minIndex].second)){
								if ((*this->id_to_idle)[heap[right].first] > (*this->id_to_idle)[heap[minIndex].first]) {
									minIndex = right;
								}
							}
							if ((heap[left].second == heap[minIndex].second)){
								if ((*this->id_to_idle)[heap[left].first] > (*this->id_to_idle)[heap[minIndex].first]) {
									minIndex = left;
								}
							}
						}
					} else if ((minIndex != i) && (heap[right].second == heap[left].second)){
						if ((*this->id_to_idle)[heap[left].first] > (*this->id_to_idle)[heap[right].first]) {
							minIndex = left;
						} else {
							minIndex = right;
						}
					}
				}
				if (!(right < heap_size && left < heap_size) && minIndex == i) {
					if (right < heap_size) {
						if ((heap[minIndex].second == heap[right].second) && 
							((*this->id_to_idle)[heap[minIndex].first] < (*this->id_to_idle)[heap[right].first])
							) {
							minIndex = right;

						}
					} else if (left < heap_size) {
						if ((heap[minIndex].second == heap[left].second) && 
							((*this->id_to_idle)[heap[minIndex].first] < (*this->id_to_idle)[heap[left].first])
							) {
							minIndex = left;
						}
					}
				}
				
				if (minIndex != i) {
					swap(heap[i], heap[minIndex]);
					reheap_down(minIndex);
				}
				
			}

		public:
			// Insert an element into the heap
			void insert(int id, int value) {
				
				heap.push_back(make_pair(id, value));
				reheap_up(heap.size() - 1);
				
			}
			void update(int id, int value) {
				bool exists = false;
				int i = 0;
				for (auto p : heap) {
					if (p.first == id) {exists = true; break;}
					i++;
				}
				if (!exists) {
					if (value != 0){
						this->insert(id, value);
					}
				} else {
					if (value != 0){
						int old = heap[i].second;
						heap[i].second = value;
						if (value != 0 && value != old) {
							if (value < old){
								reheap_up(i);	
								// reheap_down(i);
							} else {
								reheap_down(i);
								// reheap_up(i);
							}
						} 
					} else {
						heap[i].second = -1;
						reheap_up(i);
						extractMin();
					}

				}

			}
			// Extract the minimum element from the heap
			int extractMin() {
				int minVal = heap[0].second;
				heap[0] = heap.back();
				heap.pop_back();
				reheap_down(0);
				return minVal;
			}
			pair<int,int> top() {return heap[0];}
			bool empty() {
				return heap.empty();
			}
			void clear() {
				this->heap.clear();
			}
			
			void reheap_ins_check(int i){
				// this->clear();
				int size = this->id_to_nums->size();

				

				// vector<pair<int, int>> update_order = this->sortMapByValue((*this->id_to_idle));
				// for (auto &pair : update_order) {
				// 	cout << pair.first << "    ---------------    " << pair.second << endl;
				// }
				// for (int i = 1; i <= size; i++){
				// 	// if ((*this->id_to_nums)[i]){
					this->update(i, (*this->id_to_nums)[i]);
				// 	// cout << "ASDFGAFHA  " << i << " " << (*this->id_to_nums)[i] << endl;
				// 	// }
				// }
				// for (auto &pair : update_order) {
				// 	this->update(pair.first, (*this->id_to_nums)[pair.first]);
				// }
				// cout << "IM OUT !!!\n";
				// pair<int,int> top = heap[0];
				// while (top.second <= 0) {
				// 	this->extractMin();
				// 	top = heap[0];
				// }
			}
			void reheap_rem_check(){
				int size = this->id_to_nums->size();
				for (int i = 1; i <= size; i++){
					if (!(*this->id_to_nums)[i]){
						this->insert(i, (*this->id_to_nums)[i]);
						this->extractMin();
					}
				}
			}
			void preorder_traversal(vector<int>& pre_order_vec, int i = 0) {
				int heap_size = heap.size();
				if (i >= heap_size) return;

				pre_order_vec.push_back(heap[i].first);
				preorder_traversal(pre_order_vec, this->leftChild(i) );
				preorder_traversal(pre_order_vec, this->rightChild(i));

			} 
		};
	public:
		MinHeap* min_heap;

		FukumaMizushi(int maxsize) : maxsize(maxsize){
			this->id_to_idle = new map<int, int>;
			this->id_to_nums = new map<int, int>;
			this->min_heap = new MinHeap(id_to_idle, id_to_nums);
			for (int i = 1; i <= this->maxsize; i++) {
				this->id_to_area[i] = new deque<sukuna_customer*>();
				(*id_to_idle)[i] = 0;
				(*id_to_nums)[i] = 0;
			}
		}
		~FukumaMizushi() {
			for (int i = 1; i <= this->maxsize; i++) {
				deque<sukuna_customer*>* cur_area = id_to_area[i];
				while (!cur_area->empty()){
					sukuna_customer* del = cur_area->front();
					cur_area->pop_front();
					delete del;
				}
				delete cur_area;
			}
			delete min_heap;
			delete this->id_to_idle;
			delete this->id_to_nums;
		}
		void arange_area(HuVLTree* customer, int res) {
			int id = (res % this->maxsize) + 1;
			sukuna_customer* cus = new sukuna_customer(customer, res);
			this->id_to_area[id]->push_back(cus);
			(*this->id_to_nums)[id] = this->id_to_area[id]->size();
			for (int i = 1; i <= this->maxsize; i++) {
				if (i != id) (*this->id_to_idle)[i]++;
				else (*this->id_to_idle)[i] = 0;
			}
			vector<int> vec;
			for (int id = 1; id <= this->maxsize; id++) {
				if ((*this->id_to_nums)[id]) cout << "ID " << id << " IDLE " << (*this->id_to_idle)[id] << " NUMS "<< (*this->id_to_nums)[id] << endl;;
			}
			min_heap->preorder_traversal(vec);
			
			cout << "BEFORE ADD:\n";
			for (auto i : min_heap->heap) {
				cout << i.first << " ";
			}
			cout << endl;
			min_heap->reheap_ins_check(id);
			cout << "After ADD:\n";
			for (auto i : min_heap->heap) {
				cout << i.first << " ";
			}
			cout << endl;
		}
		void remove_area() {
			// If area inactive -> remove
			while (this->min_heap->top().second <= 0) {
				this->min_heap->extractMin();
			}
		}

		void do_hachi(int num){
			vector<int> preorder;
			this->min_heap->preorder_traversal(preorder);
			for (auto i : min_heap->heap){
				cout << i.first << "  "; 
			}
			cout << endl;
			// cout << "AAAAA" << min_heap->heap[0].first << "AAAAA" << endl;
			for (int id = 1; id <= this->maxsize; id++){
				cout << "ID " << id << " IDLE " << (*this->id_to_idle)[id] << " NUMS " << (*this->id_to_nums)[id] << endl;
			}
			for (auto i : preorder) {
				deque<sukuna_customer*>* cur = this->id_to_area[i];
				int cur_size = cur->size();
				if (cur_size <= num) {
					deque<sukuna_customer*>* temp = new deque<sukuna_customer*>;
					while (!cur->empty()) {
						temp->push_front(cur->back());
						cout << i << "-" << cur->back()->result << "\n";
						cur->pop_back();
					}
					while (!temp->empty()) {
						cur->push_back(temp->front());
						temp->pop_front();
					}
					delete temp;
				} else {
					int j = num;
					deque<sukuna_customer*>* temp = new deque<sukuna_customer*>;
					while (j--){
						temp->push_front(cur->back());
						cout << i << "-" << cur->back()->result << "\n";
						cur->pop_back();
					}
					while (!temp->empty()) {
						cur->push_back(temp->front());
						temp->pop_front();
					}
					delete temp;
				}
			}
		}
		map<int, pair<int, int>> merge_maps(const map<int, int> &map1, const map<int, int> &map2) {
			map<int, pair<int, int>> mergedMap;

			for (const auto &entry : map1) {
				int key = entry.first;
				auto it = map2.find(key);

				if (it != map2.end()) {
					// Found matching keys in both maps
					mergedMap[key] = make_pair(entry.second, it->second);
				}
			}

			return mergedMap;
		}
		static bool cmp(const pair<int,pair<int,int>> &a, const pair<int,pair<int,int>> &b){
				return ((a.second.first > b.second.first) && (a.second.second < b.second.second));
			}
		vector<pair<int,pair<int, int>>> get_priority_keiteiken(const map<int, int> &inputMap, const map<int, int> &idle_map) {
			map<int, pair<int, int>> merged_m = merge_maps(inputMap, idle_map);
			vector<pair<int, pair<int,int>>> sortedVec(merged_m.begin(), merged_m.end());
			
			// sort(sortedVec.begin(), sortedVec.end(), cmp);
			return sortedVec;
		}
		void do_keiteiken(int num) {
			for (int id = 1; id <= this->maxsize; id++){
				cout << "ID " << id << " IDLE " << (*this->id_to_idle)[id] << " NUMS " << (*this->id_to_nums)[id] << endl;
			}
			queue<int> to_punch;
			int num_area = num;
			queue<pair<int,int>> store;
			if (num >= this->maxsize) num = this->maxsize;
			int heap_size = this->min_heap->heap.size();
			if (heap_size < num_area){
				num_area = this->min_heap->heap.size();
			}
			vector<pair<int, pair<int,int>>> kick_prior = get_priority_keiteiken(*this->id_to_nums,*this->id_to_idle);
			for (auto i : kick_prior) {
				cout << "IM KICKING :" << i.first << " WITH NUMS: " << i.second.first << " AND IDLE TIME: " << i.second.second << endl;
			}
			while (num_area--){
				// cout << "ASDASD " << num_area << endl;
				pair<int,int> cur = this->min_heap->top();
				this->min_heap->extractMin();
				// cout << cur.first << " " << cur.second <<endl;
				store.push(cur);
				to_punch.push(cur.first);
			}
			while (!store.empty()) {
				pair<int, int> cur = store.front();
				this->min_heap->insert(cur.first, cur.second);
				store.pop();
			}

			while (!to_punch.empty()){
				int id = to_punch.front();
				to_punch.pop();
				deque<sukuna_customer*>* cur = this->id_to_area[id];
				int cur_size = cur->size();
				if (num >= cur_size) {
					while (!cur->empty()){
						sukuna_customer* to_kick = cur->front();
						cur->pop_front();
						cout << to_kick->result << "-" << id << "\n";
						delete to_kick;
					}
					cur->clear();
				} else {
					int num_kick = num;
					
					while (num_kick--){
						sukuna_customer* to_kick = cur->front();
						cout << to_kick->result << "-" << id << "\n";

						cur->pop_front();
						delete to_kick;
					}
				}	

				for (int i = 1; i <= this->maxsize; i++) {
					
					if (i == id) (*this->id_to_idle)[i] = 0;
					else (*this->id_to_idle)[i]++;
				}
				(*this->id_to_nums)[id] = cur->size(); 

				this->min_heap->reheap_ins_check(id);
			}
		}
};

class JJK_Restaurant {
	public:
		int MAXSIZE;
		FukumaMizushi* fukuma_mizushi;
		MuryoKusho*	   muryo_kusho;
		HuVLTree* most_recent_cus;

	public:
		JJK_Restaurant() : MAXSIZE(0), fukuma_mizushi(nullptr), muryo_kusho(nullptr), most_recent_cus(nullptr) {};
		~JJK_Restaurant(){
			delete fukuma_mizushi;
			delete muryo_kusho;
			delete this->most_recent_cus;
		};

		void set_maxsize(int size) {
			this->MAXSIZE = size;
			muryo_kusho = new MuryoKusho(this->MAXSIZE);
			fukuma_mizushi = new FukumaMizushi(this->MAXSIZE);
		};

		map<char, int> counter(string customer_name) {
			map<char, int> char_to_freq;
			for (char ch: customer_name) {
				char_to_freq[ch]++;
			}
			return char_to_freq;
		};

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
		};


		string ceasar_encrypt(string to_encrypt, bool& valid_name){
			string encrypted = "";
			map<char, int> char_to_freq = this->counter(to_encrypt);
			int size = char_to_freq.size();
			if (size < 3) {
				valid_name = false;
				return "";
			}
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
			// cout << customer_name << endl;
			
			bool is_name_valid = true;
			string encrypted_n = "abcde";
			// string encrypted_n = this->ceasar_encrypt(customer_name, is_name_valid);

			if (!is_name_valid) {
				return;
			}
			// cout << encrypted_n << endl;
			map<char, int> char_to_freq_after_encrypt = this->counter(encrypted_n);
			
			// Struct for comparison overloading
			struct pair_cmp_pq {
				bool operator()( HuVLNode* a, HuVLNode* b) const {
					// Comparison logic based on 'first' and 'second' elements of the pair
					if (a->weight == b->weight) {
						if (isspace(a->data)) {
							if (isspace(b->data)) return false;
							else return true;
						}
						if (isspace(b->data)){
							return false;
						}
						if (islower(a->data)){
							// if a  is lower
							if (islower(b->data)) {
								return a->data > b->data;
							}
							return false;
						} else if (islower(b->data)) {

							return true;
						}
						return a->data > b->data;
					}
					return a->weight > b->weight;
				}
			};

			priority_queue<HuVLNode*, vector<HuVLNode*>, pair_cmp_pq> pq;
			for (auto& pair: char_to_freq_after_encrypt){
				HuVLNode* node = new HuVLNode(pair.second, pair.first);
				pq.push(node);
			}

			// priority_queue<HuVLNode*, vector<HuVLNode*>, pair_cmp_pq> tmp;
			// while (!pq.empty()) {
			// 	cout << pq.top()->data << " " << pq.top()->weight << endl;
			// 	tmp.push(pq.top());
			// 	pq.pop();
			// }
			// while (!tmp.empty()) {
			// 	pq.push(tmp.top());
			// 	tmp.pop();
			// }
			bool only_one_node = false;
			if (pq.size() == 1) {
				only_one_node = true;
			}
			while (pq.size() > 1) {
				HuVLNode* left  = pq.top();
				// left->print();
				pq.pop();
				HuVLNode* right = pq.top();
				// right->print();
				pq.pop();

				int new_weight_sum = left->weight + right->weight;
				HuVLNode* node = new HuVLNode(new_weight_sum, ' ', left, right);
				// node->print();
				int time_rote = 3;
				while (time_rote--){
					int num_rotations = 0;
					node = node->convertToAVL(node, num_rotations);
				}	
				pq.push(node);
			}
			HuVLTree* customer = new HuVLTree(pq.top());
			// customer->set_identifier(encrypted_n);
			pq.pop();
			// customer->inorder_print(customer->root);
			customer->update_bin_mapping();
			// for (auto it: customer->char_to_bin) {
			// 	cout << it.first << ": " << it.second << endl;
			// }
			if (this->most_recent_cus) {
				delete this->most_recent_cus;
				this->most_recent_cus = new HuVLTree(customer);
			} else {
				this->most_recent_cus = new HuVLTree(customer);
			}
			
			string bin_res = "";
			if (only_one_node) {
				bin_res = "0";
			} else {
				for (char c: encrypted_n) {
					bin_res += customer->get_huffcode(c);
				}
				int bin_size = bin_res.size();
				if (bin_size > 10) {
					bin_res = bin_res.substr(bin_size - 10);
				}
				// for (auto it: customer->char_to_bin) {
				// 	cout << it.first << ": " << it.second << endl;
				// }
				reverse(bin_res.begin(), bin_res.end());
			}
				// delete customer;
			// int res = stoi(bin_res, nullptr, 2); // 
			int res = stoi(customer_name); 
			cout << res << endl;
			if (res % 2) {
				// if odd go gojo
				cout << "DATTE KIMI, YOWAI MO~~\n\n ";
				this->muryo_kusho->arange_table(customer, res);
			} else {
				// go sukuna
				cout << "SHIRE MONO GA~~\n\n ";
				this->fukuma_mizushi->arange_area(customer, res);
				// for now delete
				// delete customer;
			}
			// cout << bin_res << endl;
			// cout << stoi(bin_res, nullptr, 2) << endl;
			// cout << "END QUEUE" << endl;
			// cout << encrypted_n << endl;
		};

		void mugen(int id){
			this->muryo_kusho->do_mugen(id);
		}
		void za_hando(){
			if (this->most_recent_cus) this->most_recent_cus->inorder_print(this->most_recent_cus->root);
		}
		void kokusen(){
			this->muryo_kusho->do_kokusen();
		}
		void hachi(int num){
			this->fukuma_mizushi->do_hachi(num);
		}
		void keiteiken(int num){
			this->fukuma_mizushi->do_keiteiken(num);
		}
};

void simulate(string filename)
{
	JJK_Restaurant* r = new JJK_Restaurant();

	// cout << "Good Luck\n";
	ifstream ss(filename);
	string str, maxsize, name, energy, num;
	int count = 1 ;
	while(ss >> str)
	{ 
		cout << "----------------LINE NUMBER: " << count++ << endl;
		if(str == "MAXSIZE")
		{
			ss >> maxsize;
			// cout << maxsize + '\n';
			// MAXSIZE = stoi(maxsize); 
			r->set_maxsize(stoi(maxsize));
    	}
        else if(str == "LAPSE") // RED <NAME> <ENERGY>
        {
            ss >> name;
			cout << "\nJUNTEN "<< name  <<"!!\n\n";

			r->jutsushiki_junten(name);
    	}
    	else if(str == "HAND") // BLUE <NUM>
    	{
			cout << "\nZa HANDO GA KESU !! \n";
    		r->za_hando();
		}
    	else if(str == "CLEAVE") // PURPLE
    	{
			cout << "\n\nNo HEAD for you sir!!\n\n";
			ss >> num;
    		r->hachi(stoi(num));
		}
		else if(str == "KOKUSEN") // REVERSAL
		{
			cout << "\nKOKUSEN!!!\n\n";
			r->kokusen();
		}
    	else if(str == "LIMITLESS") // UNLIMITED_VOID
     	{   	
			ss >> num;
			cout << "\nMUGEN " << num << "!!\n";
    		r->mugen(stoi(num));
    	}
    	else if(str == "KEITEIKEN") // DOMAIN_EXPANSION
    	{
			ss>>num;
			cout << "\n\n KEITEIKEN " << num << "!!!\n";
			r->keiteiken(stoi(num));
    	}
    	
    }
	
	delete r;
	return;
}