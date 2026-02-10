#ifndef NODE_H
#define NODE_H

class Node {
public:
	Node(int num, bool isBlack) {
		number = num;
		left = nullptr;
		right = nullptr;
		parent = nullptr;
		black = isBlack;
	}

	~Node() {
		left = nullptr;
		right = nullptr;
		parent = nullptr;
	}

	Node* &getLeft() {
		return left;
	}

	Node* &getRight() {
		return right;
	}

	Node* &getParent() {
		return parent;
	}

	void setLeft(Node* node) {
		left = node;
	}
	
	void setRight(Node* node) {
		right = node;
	}

	void setParent(Node* node) {
		parent = node;
	}	

	int getNum() {
		return number;
	}

	void setBlack(bool isBlack) {
		black = isBlack;
	}

	bool isBlack() {
		return black;
	}

private:
	int number;
	Node* left;
	Node* right;
	Node* parent;
	bool black;
};

#endif
