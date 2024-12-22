/*
*   Course:       CS-300
*   Instructor:   Nathan Lebel
*   Student:      Travis Williamson
*   Assignment:   Final Project Part 2
*   Date:		  12/12/2024
*/


#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>

using namespace std;

struct Course {
	private:
		string courseNumber, courseName;
		vector<string> prerequisiteCourses;

	public:
		Course() {
			courseNumber = "";
			courseName = "";
			prerequisiteCourses.resize(0);
		}

		Course(string courseNumber, string courseName) {
			this->courseNumber = courseNumber;
			this->courseName = courseName;
		}

		void AddPrerequisite(string course) {
			this->prerequisiteCourses.push_back(course);
		}

		string GetCourseNumber() {
			return this->courseNumber;
		}

		/*
		*  Prints the course number and name of this course.
		*/
		void PrintInformation() {
			cout << this->courseNumber << ", " << this->courseName << endl;

		}

		/*
			Prints the prerequisites of this course, if any.
		*/
		void PrintPrequisites() {
			if (this->prerequisiteCourses.size() > 0) {
				cout << "Prerequisites: ";
				for (int i = 0; i < prerequisiteCourses.size(); ++i) {
					cout << prerequisiteCourses.at(i);
					if (i < prerequisiteCourses.size() - 1) {
						cout << ", ";
					}
				}
				cout << endl;
			}
		}
};

struct Node {
	Course course;
	Node* left;
	Node* right;

	Node() {
		left = nullptr;
		right = nullptr;
	}

	Node(Course course) : Node() {
		this->course = course;
	}
};

/*
*  Binary search tree definitions, 
*  
* 
*/
class BST {
	private:
		Node* root;
		void AddNode(Node* node, Course course);
		void InOrder(Node* node);

	public:
		BST();
		virtual ~BST();
		void PrintCourses(string courseNumber = "");
		void Insert(Course course);
		Course Search(string courseNumber);
};


BST::BST() {
	root = nullptr;
}


BST::~BST() {
	delete root;
}

/*
*  Prints the information of the Course objects in the tree. If the courseNumber parameter is specified,
*  then only the associated course will be printed
*
*  @courseNumber: Optional parameter, if specified then only that courses information will be printed.
*/
void BST::PrintCourses(string courseNumber) {
	if (courseNumber == "") {
		InOrder(root);
	}
	else {
		Course searchResult;
		searchResult = this->Search(courseNumber);
		searchResult.PrintInformation();
		searchResult.PrintPrequisites();
	}
}

/*
* Inserts a new course into the tree, if the root is null then the new node is created as the root.
* Otherwise, call AddNode(root, course).
* 
* @param course: The course object to be added.
*/

void BST::Insert(Course course) {
	if (root == nullptr) {
		root = new Node(course);
	}
	else {
		AddNode(root, course);
	}
}

/*
*  Search the tree for the course with the course number provided.
* 
*  @param courseNumber: The course number to search for.
*/

Course BST::Search(string courseNumber) {
	Course course;
	Node* currentNode = root;

	while (currentNode != nullptr) {

		if (currentNode->course.GetCourseNumber() == courseNumber) {
			return currentNode->course;
		}

		if (currentNode->course.GetCourseNumber() > courseNumber) {
			currentNode = currentNode->left;
		}
		else {
			currentNode = currentNode->right;
		}
	}
	return course;
}


/*
*	Adds a new node to the tree, if the course number of the provided course is less than the current nodes course number
*   then if the left node is null, create a new node, otherwise recurse down the tree along the left. If the course number
*   is greater than the currentNodes course number and the right node is null, create a new node to the right of current node.
*   Otherwise, recurse down the tree to the right.
* 
*   @param node: The current node, default starting node is root
*   @param course: The Course object to attach to the new node.
*/

void BST::AddNode(Node* node, Course course) {	
	if (node->course.GetCourseNumber() > course.GetCourseNumber()) {
		if (node->left == nullptr) {
			node->left = new Node(course);
		}
		else {
			AddNode(node->left, course);
		}
	}
	else {
		if (node->right == nullptr) {
			node->right = new Node(course);
		}
		else {
			AddNode(node->right, course);
		}
	}
}

/*
*  Prints the contents of the tree in ascending order according to course number.
*
*  @param node: Print in order starting at the provided node.
*/
void BST::InOrder(Node* node) {
	if (node != nullptr) {
		InOrder(node->left);
		node->course.PrintInformation();
		InOrder(node->right);
	}
}

/*
*   This functions takes in the data for Course objects from a user defined file, then creates the 
*	Course objects and inserts them into the tree.
* 
*	@param filePath: Path of the csv file that contains the course information
*	@param bst: Binary Search Tree data structure that the data will be loaded into
*/
void LoadCourses(string filePath, BST* bst) {
	ifstream courseData(filePath);
	string line, token;
	Course course;

	string courseNumber;
	string courseName;

	while (!courseData.eof()) {
		getline(courseData, line);

		stringstream courseInfo(line);

		getline(courseInfo, courseNumber, ',');
		getline(courseInfo, courseName, ',');

		course = Course(courseNumber, courseName);

		while (getline(courseInfo, token, ',')) {
			if (token.length() > 1) {
				course.AddPrerequisite(token);
			}
		}
		if(courseNumber != "")
			bst->Insert(course);
	}

	courseData.close();
}

/*
	Confirms that the file located at filePath can be opened and is not an empty file.

	@param filePath
*/
bool VerifyFile(string filePath) {
	ifstream courseData(filePath);

	//Unable to open file.
	if (!courseData.is_open()) {
		cerr << "Unable to open file @ " << filePath << endl;
		return false;
	}

	//Empty file 
	courseData.seekg(0, courseData.end);
	if (courseData.tellg() == 0) {
		cerr << "EOF: Empty file @ " << filePath << endl;
		return false;
	}
	return true;
	courseData.close();
}

/*
*  Displays the main menu to the user.
*  
*	1. Load file data into BST
*	2. Print sorted list
*	3. Print specific course title and information
*	9. Exit the program
* 
*  @param userOption: User provided value.
*/
void DisplayMenu(char& userOption) {
	string temp;

	cout << endl;
	cout << "\tOption 1: Load Data Structure" << endl;
	cout << "\tOption 2: Print Course List." << endl;
	cout << "\tOption 3: Print Course." << endl;
	cout << "\tOption 9: Exit" << endl << endl;

	cout << "What would you like to do? ";
	cin >> temp;

	cout << endl << endl;

	userOption = temp.at(0);
}	

int main() {
	//Take in file name
	BST* bst = new BST();
	Course searchResult;
	string filePath, searchCourse;
	char userOption;

	cout << "Welcome to the course planner!" << endl;

	do {
		cout << "Enter the file name: ";
		getline(cin, filePath);
	} while (!VerifyFile(filePath));

	//Verify file
	while (true) {
		DisplayMenu(userOption);
		switch (userOption) {
		case '1': LoadCourses(filePath, bst); 
				  break;
		case '2': cout << "Here is a sample schedule: " << endl << endl; 
				  bst->PrintCourses(); 
				  break;
		case '3': cout << "What course do you want to know about? ";
				  cin >> searchCourse;
				  bst->PrintCourses(searchCourse);
				  break;
		case '9': return 0;
		default: cout << userOption << " is not a valid option." << endl;
		}
	}

	cout << "Thank you for using the course planner!" << endl;
	return 0;
}