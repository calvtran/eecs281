#include "diameter.h"

int height(const BinaryTreeNode* node, int& result);

int diameter(const BinaryTreeNode* tree) {
	if (tree == nullptr) {
		return 0;
	}
	else {
		int result = -1; 
		int height_of_tree = height(tree, result);
		return result;
	}
}

int height(const BinaryTreeNode* node, int& result)
{
	if (node == nullptr) {
		return 0;
	}
	else {
		int l_height, r_height;
		l_height = height(node->left, result);
		r_height = height(node->right, result);
		if (result < l_height + r_height) {
			result = l_height + r_height;
		}

		return 1 + (l_height > r_height ? l_height : r_height);
	}
}

//int main() {
//	// Example 1 : Diameter = 8
//	BinaryTreeNode* A = new BinaryTreeNode(1);
//	BinaryTreeNode* B = new BinaryTreeNode(1);
//	BinaryTreeNode* C = new BinaryTreeNode(1);
//	BinaryTreeNode* D = new BinaryTreeNode(1);
//	BinaryTreeNode* E = new BinaryTreeNode(1);
//	BinaryTreeNode* F = new BinaryTreeNode(1);
//	BinaryTreeNode* G = new BinaryTreeNode(1);
//	BinaryTreeNode* H = new BinaryTreeNode(1);
//	BinaryTreeNode* I = new BinaryTreeNode(1);
//	BinaryTreeNode* J = new BinaryTreeNode(1);
//	BinaryTreeNode* K = new BinaryTreeNode(1);
//	BinaryTreeNode* L = new BinaryTreeNode(1);
//	BinaryTreeNode* M = new BinaryTreeNode(1);
//	A->left = B;
//	A->right = C;
//	B->left = D;
//	B->right = E;
//	D->left = H;
//	E->left = I;
//	I->right = L;
//
//	C->left = F;
//	C->right = G;
//	G->left = J;
//	G->right = K;
//	K->left = M;
//
//	std::cout << "Diameter 1 expected = 8\n";
//	std::cout << "Diameter 1 actual = " << diameter(A) << "\n\n";
//	delete A; delete B; delete C; delete D; delete E; delete F;
//	delete G; delete H; delete I; delete J; delete K; delete L; delete M;
//
//	// Example 2 : Diameter = 6
//	A = new BinaryTreeNode(1);
//	B = new BinaryTreeNode(1);
//	C = new BinaryTreeNode(1);
//	D = new BinaryTreeNode(1);
//	E = new BinaryTreeNode(1);
//	F = new BinaryTreeNode(1);
//	G = new BinaryTreeNode(1);
//	H = new BinaryTreeNode(1);
//	I = new BinaryTreeNode(1);
//	J = new BinaryTreeNode(1);
//	K = new BinaryTreeNode(1);
//	L = new BinaryTreeNode(1);
//	M = new BinaryTreeNode(1);
//	A->left = B;
//	A->right = C;
//	B->left = D;
//
//	C->left = E;
//	C->right = F;
//	E->left = G;
//	E->right = H;
//	H->left = J;
//	F->right = I;
//	I->right = K;
//
//	std::cout << "Diameter 2 expected = 6\n";
//	std::cout << "Diameter 2 actual = " << diameter(A) << "\n\n";
//	delete A; delete B; delete C; delete D; delete E; delete F;
//	delete G; delete H; delete I; delete J; delete K; delete L; delete M;
//
//	// Example 3 : Diameter = 4
//	A = new BinaryTreeNode(1);
//	C = new BinaryTreeNode(1);
//	E = new BinaryTreeNode(1);
//	G = new BinaryTreeNode(1);
//	H = new BinaryTreeNode(1);
//	J = new BinaryTreeNode(1);
//	A->right = C;
//	C->left = E;
//	E->left = G;
//	E->right = H;
//	H->left = J;
//	std::cout << "Diameter 3 expected = 4\n";
//	std::cout << "Diameter 3 actual = " << diameter(A) << "\n";
//	delete A; delete C; delete E; delete G; delete H; delete J;
//	return 0;
//}
