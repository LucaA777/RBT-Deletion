/*
   This is an implementation of a red-black tree, with searching.
   Users can also use files to add many numbers at once.

Author: Luca Ardanaz
*/

#include <iostream>
#include <string>
#include <queue>
#include <fstream>

#include "Node.h"

using namespace std;

string toLower(string str);
void addNum(Node* &node, int num, Node* &root);
void printTree(Node* tree);
void print(Node* node, int depth);
void parseNumbers(Node* &tree, string input);
void deleteTree(Node* &tree);
Node* searchTree(Node* node, int num);
void removeFromTree(Node* &node, int num, Node* &root);
void processFile(Node* &node);
void balanceTreeInsertion(Node* &node, Node* &root);
void leftRotation(Node* &node, Node* &root);
void rightRotation(Node* &node, Node* &root);
void balanceTreeDeletion(Node* &node, Node* &root, Node* originalNode);

int main() {

	Node* tree = nullptr;
	string input = "";

	do {
		//get user input
		cout << endl << endl;
		cout << "Enter numbers or a command (FILE, PRINT, SEARCH, DELETE, QUIT): " << endl;
		getline(cin, input);
		input = toLower(input);	

		//check if the input matches any commands, if not see if it's numbers

		if (input == "file") {
			processFile(tree);
			continue;
		}

		if (input == "print") {
			printTree(tree);
			continue;
		}

		if (input == "search") {
			//get input and make sure it is valid
			cout << "Enter a number to search for: " << endl;
			getline(cin, input);

			int num = 0;

			try {
				num = stoi(input);
			}
			catch(...) {
				cout << "Invalid input." << endl;
				continue;
			}

			//search the tree for this value
			bool found = searchTree(tree, num) != nullptr;

			cout << "Value" << (found ? " " : " not ") << "found in tree." << endl;

			continue;
		}

		if (input == "delete") {
			//get input and make sure it is valid
			cout << "Enter a number to remove: " << endl;
			getline(cin, input);

			int num = 0;
			try {
				num = stoi(input);
			}
			catch(...) {
				cout << "Invalid input." << endl;
				continue;
			}

			//delete the number from the tree
			removeFromTree(tree, num, tree);

			continue;
		}

		if (input == "quit") {
			continue;
		}

		//try to procces as a series of numbers
		parseNumbers(tree, input);

	} while(input != "quit");

	deleteTree(tree);

	return 0;
}

string toLower(string str) {
	for (char& c : str) {
		c = (char)tolower(c);
	}

	return str;
}

void addNum(Node* &node, int num, Node* &root) {

	//if the node is null, set as the new root and make it black
	if (node == nullptr) {
		node = new Node(num, true);

		//no rebalancing needed if its is an empty tree
		return;
	}

	//if it is not the root we add it as red

	//if lower than the node, go left, if higher, go right

	if (num < node -> getNum()) {

		//check if the left child exists
		if (node -> getLeft() != nullptr) {
			addNum(node -> getLeft(), num, root);
		}
		//otherwise add this as the new left node
		else {
			Node* newNode = new Node(num, false);
			newNode -> setParent(node);
			node -> setLeft(newNode);
			balanceTreeInsertion(newNode, root);
		}
	}

	else {

		//check if the right child exists
		if (node -> getRight() != nullptr) {
			addNum(node -> getRight(), num, root);
		}
		//otherwise add this as the new right node
		else {
			Node* newNode = new Node(num, false);
			newNode -> setParent(node);
			node -> setRight(newNode);
			balanceTreeInsertion(newNode, root);
		}
	}		

}

void printTree(Node* tree) {
	cout << "Tree: " << endl;
	print(tree, 0);
	cout << endl;
}

void print(Node* node, int depth) {
	//if node is null, output that
	if (node == nullptr) {
		cout << "The tree is empty." << endl;
		return;
	}

	//prints everything to the right first
	if (node -> getRight() != nullptr) {
		print(node -> getRight(), depth + 1);
	}

	//adds appropriate indentation
	for (int i = 0; i < depth; i++) {
		cout << "\t";
	}

	//prints value with color
	cout << (node -> isBlack() ? "B" : "R") << "[" << node -> getNum() << "]" << endl;

	//prints everything to the left
	if (node -> getLeft() != nullptr) {
		print(node -> getLeft(), depth + 1);
	}
}

void parseNumbers(Node* &tree, string input) {
	queue<int> numbers;

	string substring = "";

	for (char c : input) {

		//if there is a space, try parsing the substring to int
		if (c == ' ') {
			try {
				int num = stoi(substring);
				numbers.push(num);
				substring = "";
			}
			catch(...) {
				cout << "Invalid input." << endl;
				return;
			}
		}
		//otherwise, add the character to the substring
		else {
			substring += c;
		}
	}

	try {
		int num = stoi(substring);
		numbers.push(num);
		substring = "";
	}
	catch(...) {
		cout << "Invalid input." << endl;
		return;
	}

	//if the input was fully parsed, now just add the queue to the tree
	while (!numbers.empty()) {
		//TODO: make sure that the second tree pointer doesn't get messed up
		addNum(tree, numbers.front(), tree);
		numbers.pop();
	}
}

void deleteTree(Node* &tree) {
	//null node case
	if (tree == nullptr) {
		return;
	}

	//delete all children first
	if (tree -> getLeft() != nullptr) {
		deleteTree(tree -> getLeft());
	}

	if (tree -> getRight() != nullptr) {
		deleteTree(tree -> getRight());
	}

	delete tree;
}

Node* searchTree(Node* node, int num) {
	//if there is no node, return null
	if (node == nullptr) {
		return nullptr;
	}

	//if the node matches the value, return true
	if (node -> getNum() == num) {
		return node;
	}

	//if the number is less than the current node, go left, otherwise go right
	if (num < node -> getNum()) {

		if (node -> getLeft() != nullptr) {
			return searchTree(node -> getLeft(), num);
		}
		//if there is no further left, return false
		else {
			return nullptr;
		}
	}
	else {

		if (node -> getRight() != nullptr) {
			return searchTree(node -> getRight(), num);
		}
		//if there is no further right, return false
		else {
			return nullptr;
		}
	}
}

void removeFromTree(Node* &node, int num, Node* &root) {

	//
	//
	// --<< FINDING THE NODE TO DELETE AND OTHER VALIDATION >>--
	//
	//



	//make sure that the node is not null
	if (searchTree(node, num) == nullptr) {
		cout << "Value not found in tree." << endl;
		return;
	}

	//get the chosen node and parent node

	if (num < node -> getNum()) {
		if (node -> getLeft() != nullptr) {
			removeFromTree(node -> getLeft(), num, root);
		}
		return;
	}
	else if (num > node -> getNum()) {
		if (node -> getRight() != nullptr) {
			removeFromTree(node -> getRight(), num, root);
		}
		return;
	}

	//figure out how to handle deletion

	//
	//
	// --<< NO CHILD CASE >>--
	//
	//

	//if no children, then simply delete
	if (node -> getLeft() == nullptr && node -> getRight() == nullptr) {
		cout << "No child deletion." << endl;
		bool needsRebalance = node -> isBlack();
		//no parent case
		if (node -> getParent() == nullptr) {
			cout << "Empty tree, no recoloring." << endl;
			delete node;
			node = nullptr;

			return;
		}


		//figure out which side of the parent to delete
		else if (node -> getParent() -> getLeft() != nullptr && node -> getParent() -> getLeft() == node) {


			if (needsRebalance) {
				balanceTreeDeletion(node, root, node);

				root -> setBlack(true);
				return;
			}


			node -> getParent() -> setLeft(nullptr);
			delete node;
			node = nullptr;

			root -> setBlack(true);
			return;
		}
		else {

			if (needsRebalance) {
				balanceTreeDeletion(node, root, node);
				root -> setBlack(true);
				return;
			}

			node -> getParent() -> setRight(nullptr);
			delete node;
			node = nullptr;

			root -> setBlack(true);


			return;
		}
	}

	//
	//
	// --<< TWO CHILD CASE >>--
	//
	//

	//if has two children
	cout << "Node before double: " << node -> getNum() << endl;
	if (node -> getRight() != nullptr && node -> getLeft() != nullptr) {
		cout << "Double child deletion." << endl;
		//save the children
		Node* leftChild = node -> getLeft();
		Node* rightChild = node -> getRight();

		//find the successor
		//go one to the right and then go to the left until reaching the end
		Node* successor = node -> getRight();
		Node* previous = nullptr;

		//get to the successor
		while (successor -> getLeft() != nullptr) {
			previous = successor;
			successor = successor -> getLeft();
		}

		cout << "Sucessor: " << successor -> getNum() << endl;

		//make the successor red
		successor -> setBlack(false);

		Node* parent = node -> getParent();

		//correct the right child if it is the successor
		if (rightChild == successor) {
			rightChild = rightChild -> getRight();
		}

		cout << "Deleting and linking successor..." << endl;
		//make sure that the parent still points to the right thing
		if (parent == nullptr) {
			cout << "No parent, no parent to link." << endl;
			delete node;
			node = successor;

			//set black since root
			successor -> setBlack(true);
		}

		//node is left child
		else if (parent -> getLeft() == node) {
			cout << "Correcting left side parent linking..." << endl;
			delete node;
			node = successor;
			parent -> setLeft(successor);
			successor -> setParent(parent);
		}
		//node is right child
		else {
			cout << "Correcting right side parent linking..." << endl;
			delete node;
			node = successor;
			parent -> setRight(successor);
			successor -> setParent(parent);
		}

		cout << "Attaching successor's children..." << endl;

		cout << "Successor: " << successor -> getNum() << endl;

		//make sure that the node right before the successor points to the elements after the successor
		if (previous != nullptr) {
			cout << "Setting previous..." << endl;

			//check if the successor had anything linked to the right
			if (successor -> getRight () != nullptr) {
				successor -> getRight() -> setParent(previous);
				previous -> setLeft(successor -> getRight());
			}
			else {
				previous -> setLeft(nullptr);
			}
		}

		//cout << "Left child: " << leftChild -> getNum() << endl;
		//cout << "Right child: " << rightChild -> getNum() << endl;

		//make sure that we aren't linking a copy of the new node to itself
		if (leftChild != nullptr) {
			cout << "Linking left child..." << endl;
			leftChild -> setParent(successor);
			successor -> setLeft(leftChild);
		}

		if (rightChild != nullptr) {
			cout << "Linking right child..." << endl;
			rightChild -> setParent(successor);
			cout << "Linked to parent." << endl;
			successor -> setRight(rightChild);
			cout << "Linked to child." << endl;
		}

		cout << "Double deletion done." << endl;

		root -> setBlack(true);

		return;
	}


	//otherwise its single-child

	//
	//
	// --<< SINGLE CHILD CASE >>--
	//
	//


	Node* replacement = nullptr;

	//if only has a left child
	if (node -> getLeft() != nullptr && node -> getRight() == nullptr) {
		cout << "Left child deletion." << endl;
		replacement = node -> getLeft();
	}
	//if only has a right child
	else {
		cout << "Right child deletion." << endl;
		replacement = node -> getRight();
	}

	cout << "Replacement subtree root: " << (replacement != nullptr ? replacement -> getNum() : 0) << endl;
	cout << "Node: " << node -> getNum() << endl;

	//no parent case
	if (node -> getParent() == nullptr) {
		cout << "No parent case." << endl;
		delete node;
		node = replacement;
		replacement -> setParent(nullptr);

		//always color it black
		cout << "Coloring black." << endl;
		replacement -> setBlack(true);

		root -> setBlack(true);
		return;
	}

	//figure out which side of the parent to delete
	else if (node -> getParent() -> getLeft() != nullptr && node -> getParent() -> getLeft() == node) {
		cout << "Node parent: " << node -> getParent() -> getNum() << endl;
		cout << "Left replacement..." << endl;
		Node* parent = node -> getParent();
		delete node;
		node = nullptr;
		parent -> setLeft(replacement);
		replacement -> setParent(parent);

		//always color it black
		cout << "Coloring black." << endl;
		replacement -> setBlack(true);

		root -> setBlack(true);
		return;
	}
	else {
		cout << "Node parent: " << node -> getParent() -> getNum() << endl;
		cout << "Right replacement..." << endl;
		Node* parent = node -> getParent();
		delete node;
		node = nullptr;
		parent -> setRight(replacement);
		replacement -> setParent(parent);

		//always color it black
		cout << "Coloring black." << endl;
		replacement -> setBlack(true);

		root -> setBlack(true);
		return;
	}


}

void processFile(Node* &node) {

	//first process the entire file to see if it is all valid
	//then add it to the tree

	cout << "Enter the file name: " << endl;
	string fileName;
	getline(cin, fileName);

	try {
		fstream file(fileName);

		//store the numbers in a queue
		queue<int>* numbers = new queue<int>();

		int num;
		string numStr;		

		//push the numbers into the queue
		while (getline(file, numStr)) {
			num = stoi(numStr);
			numbers -> push(num);
		} 

		//then add all of the numbers in the queue to the tree
		while (!numbers -> empty()) {
			//TODO: same as before
			addNum(node, numbers -> front(), node);
			numbers -> pop();			
		}

		file.close();

	}
	catch(...) {
		return;
	}

}

void balanceTreeInsertion(Node* &node, Node* &root) {

	printTree(root);

	cout << "Begining rebalancing..." << endl;

	//if the node is the root, then make sure it is black and return
	if (node -> getParent() == nullptr) {
		node -> setBlack(true);
		cout << "No parent, must be the root, no changes." << endl;
		return;
	}

	//if the parent is black, then everything is fine
	if (node -> getParent() -> isBlack()) {
		cout << "The parent is black, no changes." << endl;
		return;
	}

	//if there is no grandparent, there also won't be any rebalancing
	if (node -> getParent() -> getParent() == nullptr) {
		cout << "There is no grandparent, no changes." << endl;
		return;
	}

	cout << "Finding the uncle..." << endl;

	//find the uncle
	Node* grandparent = node -> getParent() -> getParent();
	Node* uncle = nullptr;	

	if (grandparent -> getLeft() == node -> getParent() && grandparent -> getRight() != nullptr) {
		//uncle must be to the right
		uncle = grandparent -> getRight();
	}
	else if (grandparent -> getRight() == node -> getParent() && grandparent -> getLeft() != nullptr) {
		uncle = grandparent -> getLeft();
	}


	//if the uncle is red, then simply recolor
	if (uncle != nullptr && !uncle -> isBlack()) {
		cout << "Red uncle, recoloring..." << endl;
		node -> getParent() -> setBlack(true);
		uncle -> setBlack(true);
		grandparent -> setBlack(false);
		root -> setBlack(true);

		//check for balancing from the grandparent
		cout << "Red uncle accounted for." << endl;
		balanceTreeInsertion(grandparent, root);
		return;
	}

	//otherwise, the uncle must be black
	cout << "Black uncle, rotating..." << endl;

	//determine whether the node is right or left child
	if (node -> getParent() -> getParent() -> getLeft() == node -> getParent()) {
		cout << "Parent left of grandparent." << endl;

		//rotate to form a line if currently triangular
		if (node == node -> getParent() -> getRight()) {
			cout << "Left rotation on node to form line." << endl;
			leftRotation(node, root);
			printTree(root);
			node = node -> getLeft();
		}

		cout << "Rotating parent..." << endl;
		//rotate regardless
		node -> getParent() -> setBlack(true);
		node -> getParent() -> getParent() -> setBlack(false);
		rightRotation(node -> getParent(), root);
	}
	else {
		cout << "Parent right of grandparent." << endl;

		//rotate to form a line if currently triangular
		if (node == node -> getParent() -> getLeft()) {
			cout << "Right rotation on node to form line." << endl;
			rightRotation(node, root);
			printTree(root);
			node = node -> getRight();
		}

		cout << "Rotating parent..." << endl;
		//rotate regardless
		node -> getParent() -> setBlack(true);
		node -> getParent() -> getParent() -> setBlack(false);
		leftRotation(node -> getParent(), root);
	}

	//the root must always be black
	root -> setBlack(true);

	cout << "Black uncle accounted for." << endl;
	balanceTreeInsertion(node, root);
	return;

}

void leftRotation(Node* &node, Node* &root) {

	cout << "Begining left rotation..." << endl;

	//store the parent for practicality
	cout << "Storing parent..." << endl;
	Node* parent = node -> getParent();

	//if there is no parent, you can't rotate
	if (parent == nullptr) {
		cout << "No parent! Canceling rotation..." << endl;
		return;
	}

	cout << "Rotating node: " << node -> getNum() << endl;

	//move node's left subtree to become parent's right subtree
	cout << "Moving node subtree..." << endl;
	parent -> setRight(node -> getLeft());

	if (parent -> getRight() != nullptr) {
		parent -> getRight() -> setParent(parent);
	}

	//update node's parent to be parent's parent
	cout << "Updating node's parent..." << endl;
	node -> setParent(parent -> getParent());

	//link new parent to node

	cout << "Linking parent to node..." << endl;
	//if there is no grandparent, the root was found
	if (parent -> getParent() == nullptr) {
		//make the node the root
		cout << "Setting a new root." << endl;
		root = node;
	}

	//make sure that the correct side child is being relinked
	else if (parent -> getParent() -> getLeft() == parent) {
		//parent is grandparent's left child
		cout << "Is grandparent's left." << endl;
		node -> getParent() -> setLeft(node);
	}
	else {
		//parent is grandparent's right child
		cout << "Is grandparent's right." << endl;
		node -> getParent() -> setRight(node);
	}

	//make parent new left child
	cout << "Shifting parent to be new child..." << endl;
	cout << "Parent currently: " << (parent != nullptr ? parent -> getNum() : -100) << endl;
	node -> setLeft(parent);
	cout << "Node's new left (should be parent): " << (node -> getLeft() != nullptr ? node -> getLeft() -> getNum() : -100) << endl;

	//make parent's parent be node
	cout << "Set parent's new parent..." << endl;
	parent -> setParent(node);



	cout << "Rotation complete!" << endl;
	printTree(root);
}

void rightRotation(Node* &node, Node* &root) {

	cout << "Begining right rotation..." << endl;

	//store the parent for practicality
	cout << "Storing parent..." << endl;
	Node* parent = node -> getParent();

	//if there is no parent, you can't rotate
	if (parent == nullptr) {
		cout << "No parent! Canceling rotation..." << endl;
		return;
	}

	//move node's right subtree to become parent's left subtree
	cout << "Moving node subtree..." << endl;
	parent -> setLeft(node -> getRight());

	if (parent -> getLeft() != nullptr) {
		parent -> getLeft() -> setParent(parent);
	}

	//update node's parent to be parent's parent
	cout << "Updating node's parent..." << endl;
	node -> setParent(parent -> getParent());

	//link new parent to node

	cout << "Linking parent to node..." << endl;
	//if there is no grandparent, the root was found
	if (parent -> getParent() == nullptr) {
		//make the node the root
		cout << "Setting a new root." << endl;
		root = node;
	}

	//make sure that the correct side child is being relinked
	else if (parent -> getParent() -> getLeft() == parent) {
		//parent is grandparent's left child
		cout << "Is grandparent's left." << endl;
		node -> getParent() -> setLeft(node);
	}
	else {
		//parent is grandparent's right child
		cout << "Is grandparent's right." << endl;
		node -> getParent() -> setRight(node);
	}

	//make parent new right child
	cout << "Shifting parent to be new child..." << endl;
	node -> setRight(parent);

	//make parent's parent be node
	cout << "Set parent's new parent..." << endl;
	parent -> setParent(node);

	cout << "Rotation complete!" << endl;

	printTree(root);
}

void balanceTreeDeletion(Node* &node, Node* &root, Node* originalNode) {

	printTree(root);

	cout << "Begining rebalancing..." << endl;

	cout << "Original node: " << originalNode -> getNum() << endl;

	cout << "Finding nodes..." << endl;

	bool nIsLeft = true;

	//find the necessary nodes
	Node* parent = nullptr;
	Node* sibling = nullptr;
	Node* closeNephew = nullptr;
	Node* distantNephew = nullptr;

	parent = node -> getParent();

	bool parentRight;

	if (parent != nullptr) {

		cout << "Parent: " << parent -> getNum() << endl;

		nIsLeft = parent -> getLeft() == node;

		parentRight = node -> getParent() -> getRight() == node;

		//get the sibling
		if (parentRight) {
			sibling = parent -> getLeft();
		}
		else {
			sibling = parent -> getRight();
		}

		//get the nephews
		if (sibling != nullptr) {

			cout << "Sibling: " << sibling -> getNum() << endl;

			if (parentRight) {
				closeNephew = sibling -> getRight();
				distantNephew = sibling -> getLeft();
			}
			else {
				closeNephew = sibling -> getLeft();
				distantNephew = sibling -> getRight();
			}

			if (closeNephew != nullptr) {
				cout << "Close Nephew: " << closeNephew -> getNum() << endl;
			}
			if (distantNephew != nullptr) {
				cout << "Distant Nephew: " << distantNephew -> getNum() << endl;
			}
		}
	}

	cout << "Nodes found." << endl;

	//determine the case
	bool pBlack = (parent == nullptr || parent -> isBlack());
	bool cBlack = (closeNephew == nullptr || closeNephew -> isBlack());
	bool sBlack = (sibling == nullptr || sibling -> isBlack());
	bool dBlack = (distantNephew == nullptr || distantNephew -> isBlack());

	int dCase = 0;

	if (pBlack && cBlack && sBlack && dBlack) {
		dCase = 2;
	}
	else if (pBlack && cBlack && !sBlack && dBlack) {
		dCase = 3;
	}	       
	else if (!pBlack && cBlack && sBlack && dBlack) {
		dCase = 4;
	}
	else if (!cBlack && sBlack && dBlack) {
		dCase = 5;
	}
	else {
		dCase = 6;
	}

	cout << "Deletion case: " << dCase << endl;	
	
	//excecute the cases
	switch (dCase) {
		case 2:
			// --<< CASE 2 >>--
			if (sibling != nullptr) {
				sibling -> setBlack(false);
			}

			balanceTreeDeletion(parent, root, originalNode);
			break;

		case 3:
			// --<< CASE 3 >>--
			cout << "Case 3 not implemented yet..." << endl;
			if (nIsLeft) {
				leftRotation(sibling, root);
			}
			else {
				rightRotation(sibling, root);
			}

			if (parent != nullptr) {
				parent -> setBlack(false);
			}
			if (sibling != nullptr) {
				sibling -> setBlack(true);
			}

			balanceTreeDeletion(node, root, originalNode);

			break;

		case 4:
			// --<< CASE 4 >>--
			if (parent != nullptr) {
				parent -> setBlack(true);
			}

			if (closeNephew != nullptr) {
				closeNephew -> setBlack(true);
			}

			if (sibling != nullptr) {
				sibling -> setBlack(false);
			}

			if (distantNephew != nullptr) {
				distantNephew -> setBlack(true);
			}

			break;

		case 5:
			// --<< CASE 5 >>--
			if (nIsLeft) {
				rightRotation(sibling, root);

				if (sibling != nullptr) {
					sibling -> setBlack(true);
				}

				if (distantNephew != nullptr) {
					distantNephew -> setBlack(false);
				}
			}
			else {
				leftRotation(sibling, root);

				if (sibling != nullptr) {
					sibling -> setBlack(true);
				}

				if (distantNephew != nullptr) {
					distantNephew -> setBlack(false);
				}
			}

			balanceTreeDeletion(node, root, originalNode);

			break;

		case 6:
			// --<< CASE 6 >>--
			if (nIsLeft) {
				leftRotation(sibling, root);

				if (parent != nullptr) {
					parent -> setBlack(true);
				}
				
				if (distantNephew != nullptr) {
					distantNephew -> setBlack(true);
				}

				if (closeNephew != nullptr) {
					closeNephew -> setBlack(true);
				}
			}
			else {
				rightRotation(sibling, root);

				if (parent != nullptr) {
					parent -> setBlack(true);
				}

				if (distantNephew != nullptr) {
					distantNephew -> setBlack(true);
				}

				if (closeNephew != nullptr) {
					closeNephew -> setBlack(true);
				}
			}

			break;


		default:
			cout << "Error: no matching case found." << endl;

	}

	cout << "Original node after rebalancing: " << originalNode -> getNum() << endl;


	if (originalNode != nullptr) {
		cout << "Deleting node..." << endl;
		cout << "Node's new parent: " << originalNode -> getParent() -> getNum() << endl;
		if (originalNode -> getParent() -> getRight() == originalNode) {
			originalNode -> getParent() -> setRight(nullptr);
		}
		else {
			originalNode -> getParent() -> setLeft(nullptr);
		}
	
		delete originalNode;
		originalNode = nullptr;
	}


	cout << "Deletion completed." << endl;

	printTree(root);


}
