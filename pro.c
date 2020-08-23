#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node {
	int va;
	char *str;
};

struct tree {
	struct node *val;
	int t;
	struct tree **link;
	int n;
	int l;
};

struct tree * create(int degree, int marker)
{
	struct tree *root = NULL;
	root = (struct tree *) malloc(sizeof(struct tree));// allocate memory for one btree node
	root->t = degree;// initialize the degree
	root->val = (struct node *) malloc((2 * degree - 1) * sizeof(struct node));// allocate memory for number of key,value node as at most 2*t-1 keys allowed
	root->link = (struct tree **) malloc((2 * degree) * sizeof(struct tree *));// allocate memory for number of child node as at most 2*t-1 children are allowed
	root->l = marker;//initialize to check leaf node or not 1 == leaf node  and 0 == non leaf node
	root->n = 0;//initialize as oth node to keep track of overflow/underflow condition
	return root;
}

struct tree * splitchild(struct tree *Btree, int i, struct tree *child)
{
	int degree;
	int j;
	struct tree *newnode = NULL;

	order = Btree->t;//store the order of Btree
	newnode = create(child->degree, child->l);//create a copy child
	newnode->n = degree - 1;//initialize the size

	for (j = 0; j < degree - 1; j++) {
		newnode->val[j] = child->val[j + degree];// copy t-1 values to right child;
	}
	if (child->l == 0) {// if it is not the leaf node
		for (j = 0; j < degree; j++) {
			newnode->link[j] = child->link[j + degree];// copy t-1 child pointers to right child
		}
	}
	child->n = degree - 1;//decrease the size of the overflowed child;
	for (j = Btree->n; j >= i + 1; j--) {
		Btree->link[j + 1] = Btree->link[j];//shift the child pointers to create space for the newly created child
	}
	Btree->link[j + 1] = newnode;// insert the newchild at its position
	for (j = Btree->n - 1; j >= i; j--) {
		Btree->val[j + 1] = Btree->val[j];//shift the value of the parent node to insert the median value.
	}
	Btree->val[i] = child->val[degree - 1];//insert the median
	Btree->n = Btree->n + 1;//increase th size of parent

	return Btree;
}
struct tree* insert_unfull(struct tree* B_node, struct node block)
{
	int i = B_node->n - 1;// get the value number of node present in that particular node of Btree
	if (B_node->l == 1){// if leaf node
		while (i >= 0 && B_node->val[i].va > block.va) {
			B_node->val[i + 1] = B_node->val[i];// shift the content of adjacent  nodes until you find it's position to place
			i--;
		}
		B_node->val[i + 1].va = block.va;// place it in correct position and initialize it's key as given node
		B_node->val[i + 1].str = block.str;// initialize the record pointer
		B_node->n = B_node->n + 1;//increase the counter
	} else {// if not leaf node
		while (i >= 0 && B_node->val[i].va > block.va) {// decide which child node the block will insert
			i--;
		}
		if (B_node->link[i + 1]->n == 2 * B_node->t - 1) {//decides is this particular node is full
			B_node = splitchild(B_node, i + 1, B_node->link[i + 1]);//split the child into two nonfull children
			if (B_node->val[i + 1].va < block.va) {//determine which of the two children is now the correct one to descend to
				i++;
			}
		}
		B_node->link[i + 1] = insert_unfull(B_node->link[i + 1], block);//insert in that unfull child node
	}

	return B_node;
}

struct tree * insert(struct tree *root, struct node block, int order)
{
	if (root == NULL) {// if Btree has not created
		root = create(order, 1);// create node for Btree leaf node
		root->val[0] = block;//initialize the first block
		root->n = 1;//mark it as 1st node to keep track of overflow/underflow condition
	} else {
		if (root->n < 2 * order - 1) {// check overflow condition of that particular Btree node
			root = insert_unfull(root, block);// insert the node with key value pairs in Btree(root)
		} else {//if overflow create a new Btree node
			int i;
			struct tree *parent = NULL;//create a new Btree noode

			parent = create(order, 0);//create Btree node as non-leaf node
			parent->link[0] = root;// make the root as child of newly created BtreeNode
			parent = splitchild(parent, 0, root);//split the root where overflow occured
			i = 0;
			if (parent->val[0].va < block.va) {//determine which of the two children is now the correct one to descend to
				i++;
			}
			parent->link[i] = insert_unfull(parent->link[i], block);//insert in that unfull child node
			root = parent;// assign parent as root
		}
	}

	return root;
}

struct node search(struct tree * root, int k)
{
	int i = 0;
	while (i < root->n && k > root->val[i].va) {// do shifting until and unless element is less than the key
		i++;
	}
	if (root->val[i].va == k) {// if found return the record pointer.
		return root->val[i];
	}
	if (root->l == 1) {//if it is the leave node then element is not present
		return NULL;// return NULL;
	}

	return search(root->link[i], k);// go to it's child pointer to search just like BST
}

struct tree * preinsert(struct tree* root)
{
	FILE *fp;
	char line;
	int numberOfLines = 0;

	fp = fopen("yash.bin", "rb");//open the file

	if (fp != NULL) {
		int key;// primary key
		char *data = NULL;//record
		struct node block;//node for key and record pointer.
		line = getc(fp);

		for (line = getc(fp); line != EOF; line = getc(fp)) {
			if (line == '\n') {
				numberOfLines = numberOfLines + 1;//count line
			}
		}

		fclose(fp);//file close

		char *st;
		int sizeOfData;
		int loopCounter,i;

		fp = fopen("yash.bin", "rb"); // To Again Start from the top

		for (loopCounter = 0; loopCounter < numberOfLines; loopCounter++) {

			data = (char *) malloc(sizeof(char));
			fscanf(fp, "%2d", &key);//get the primary key
			fscanf(fp, "%2d", &sizeOfData);//get the length of string
			fgets(data, 1000, fp);//read the string store in data string
            st = (char*)malloc(sizeof(char)*(sizeOfData + 1));//llocate memory for string of that length

            for(i = 0;i < sizeOfData; i++){//
				st[i] = data[i];//copy the content
			}

			st[i] = '\0';// assign null at end of string
			int h = strlen(st);
			block.va = key;//initialize the p.key
			block.str = st;//initialize the record pointer
			printf(" %s ", st);
			root = insert(root, block, 3);//one by one insert the node containing key,record pair in Btree
		}
		fclose(fp);// end of loop close file
	}
//	printf("%c \n", c);
	return root;// return root node
}

void traverse(struct tree *root)
{
	int i;
	for (i = 0; i < root->n; i++) {
		if (root->l == 0) {
			traverse(root->link[i]);
		}
		printf("%d\t%s\n", root->val[i].va, root->val[i].str);
	}
	if (root->l == 0) {
		traverse(root->link[i]);
	}
}

int main()
{
	struct tree *root = NULL;
	int i;
	int n;
	int t;
	FILE *fp;
	FILE *fp1;
	int x;
	int c;

	c = 1;
	root = preinsert(root);//get the root of Btree after creating the Btree with available data
	while (c == 1) {
		printf("enter your choice\n1.insert \n 2.serach \n 3.delete\n4.Desplay\n");
		scanf("%d", &x);
		switch(x) {
			case 1: {
					int k;
					struct node block;
					char *str = NULL;

					fp = fopen("yash.bin", "ab");
					str = (char *) malloc(sizeof(char));
					printf("enter roll No\n");//ask user for roll no use it as p.key
					scanf("%d", &k);
					printf("enter name,address and branch\n");// //ask user for details use it as record pointer
					getchar();
					scanf("%[^\n]%*c", str);
					int l=strlen(str);// get the length
					fprintf(fp, "%d%d%s", k,l, str);// write it as p.key+len+user info
					fprintf(fp, "\n");//next line

					block.va = k;//p.key
					block.str = str;//record pointer
					root = insert(root, block, 3);//degree == 3
					fclose(fp);//close the file

					break;
				}
			case 2: {
					int p;
					struct node m;

					printf("enter primary key\n");
					scanf("%d", &p);

					m = search(root, p);// search with p.key
					printf("%d\t%s\n", m.va, m.str);//return record pointer

					break;
				}
			case 3: {
					int t;
					int m;
					int p;
					int k;
					int i;
					char c;
					char ch;
					char *s;
					int count;
					fp1 = fopen("shiv.bin", "wb");
					fp = fopen("yash.bin", "rb");
					rewind(fp);
					t = 1;
					printf("enter primary key for deletion\n");
					scanf("%d", &m);
					k = 0;
					i = 0;
					count = 0;
					c = getc(fp);
					for (c = getc(fp); c != EOF; c = getc(fp)) {
						if (c == '\n') {
							count = count + 1;
						}
					}
					rewind(fp);
					s = (char *) malloc(sizeof(char));
					while (! feof(fp)) {
						fscanf(fp, "%2d", &p);
						fgets(s, 1000, fp);
						i++;
						if (p == m) {
							break;
						}
					}
				//	printf("%d",i);
					if(i-1 != count) {

						rewind(fp);
						ch = fgetc(fp);
						while (ch != EOF) {
							if (ch == '\n') {
								t++;
							}

							if (t != i) {
								fputc(ch, fp1);
							}
							ch = fgetc(fp);
						}
						fclose(fp1);
						fclose(fp);
						remove("yash.txt");
						rename("shiv.bin", "yash.bin");
					//	fp = fopen("yash.bin", "rb");
					//	c = getc(fp);
					//	fclose(fp);
						root = NULL;
						root = preinsert(root);
					}
					else {
						printf("data no present\n");
                                                fclose(fp);
					}
					break;
				}
			case 4: {
					traverse(root);
					break;
				}
			default: {
					 printf("wrong input");
				 }
		}
		printf("enter 1 for continue:\n");
		scanf("%d", &c);
	}
}
