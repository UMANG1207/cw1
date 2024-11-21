#include "JumpList.h"
#include <iostream>
#include <stdexcept>
#include <string>
#include <algorithm>

Node::Node(const string& s, Node* next, Node* jump, int gap) {
	// IMPLEMENT ME
	data_ = s;
	next_ = next;
	jump_ = jump;
	gap_ = gap;
}

Node::~Node() {
	// IMPLEMENT ME

}


JumpList::JumpList() {
	// IMPLEMENT ME
	head_ = nullptr;
}

// DO NOT CHANGE
// You do not need to understand the code here to complete the assignment
JumpList::JumpList(int size, const string* arr) {

	const string s[] = {"a", "b", "blah", "c", "cat", "d", "etc", "ooo...", "x", "zzz"};
	const string* sp = (arr == nullptr) ? s : arr;

	Node** npp = new Node*[size];
	npp[size-1] = new Node(sp[size-1], nullptr, nullptr, 0);
	for(int i = size-2; i >= 0; i--)
		npp[i] = new Node(sp[i], npp[i+1], nullptr, 0);

	if (arr == nullptr) {

		if (size==1) npp[0]->gap_ = 1;
		else if (size==3) npp[0]->gap_ = 3;
 		else if (size==10) {
			npp[0]->jump_ = npp[5];
			npp[5]->jump_ = npp[8];
			npp[0]->gap_ = 5;
			npp[5]->gap_ = 3;
			npp[8]->gap_ = 2;
		}
		else throw std::invalid_argument("size must be 1, 3 or 10");

	}
	else {

		// jump node every MAX_GAP_SIZE
		int i;
		for(i=0; i < size-MAX_GAP_SIZE; i += MAX_GAP_SIZE) {
			npp[i]->jump_ = npp[i+MAX_GAP_SIZE];
			npp[i]->gap_ = MAX_GAP_SIZE;
		}
		npp[i]->gap_ = size % MAX_GAP_SIZE;
		if (npp[i]->gap_ == 0) npp[i]->gap_ = MAX_GAP_SIZE; // 0 need to become 5!

	}

	head_ = npp[0];
	delete [] npp; // note that the local array npp gets destroyed here but the nodes stay alive!

}

JumpList::~JumpList() {
	// IMPLEMENT ME

}


int JumpList::size() const {
	// IMPLEMENT ME
    int count = 0;
    Node* current = head_;
    while (current) {
        ++count;
        current = current->next_;
    }
    return count;
}

// DONE FOR YOU, DO NOT CHANGE
bool JumpList::find(const string& s) const {

	if (head_ == nullptr) return false;

	// moving along the fast lane
	Node* tmp = head_;
	while(tmp->jump_ != nullptr && tmp->jump_->data_ < s)
		tmp = tmp->jump_;

	// tmp now points to the jump node at the start of the segment where s could be

	// now slow lane
	while(tmp != nullptr) {

		if (tmp->data_ == s) return true; // match
		else if (tmp->data_ > s) return false; // went past without finding s
		else tmp = tmp->next_;
	}

	return false; // end of list
}

string JumpList::get(int i) const {
	// IMPLEMENT ME
    Node* current = head_;
    int index = 0;

    // Traverse the list until the i-th element is found
    while (current) {
        if (index == i) {
            return current->data_;  // Return the data of the i-th node
        }
        current = current->next_;  // Move to the next node
        index++;  // Increment the index
    }
  
	return ""; // dummy
}

void trimSpaces(string &line) {
    // Remove leading whitespace
    if (!line.empty() && std::isspace(line.front())) {
        line.erase(0, line.find_first_not_of(' '));
    }

    // Remove trailing whitespace
    if (!line.empty() && std::isspace(line.back())) {
        line.erase(line.find_last_not_of(' ') + 1);
    }
}

string JumpList::print() const {
    if (head_ == nullptr) {
        return "\n\n";  // Return empty lines for an empty list
    }

    // First line: list all nodes
    string allNodes;
    Node* current = head_;
    while (current) {
        allNodes += current->data_ + " ";
        current = current->next_;
    }
    allNodes.pop_back();  // Remove trailing space

    // Second line: jump nodes
    string jumps;
    current = head_;
    while (current) {
        jumps += current->data_ + " ";
        if (current->jump_ == nullptr) break;
        current = current->jump_;
    }
    jumps.pop_back();  // Remove trailing space

    // Third line: gaps for jump nodes
    string gaps;
    current = head_;
    while (current) {
        gaps += std::to_string(current->gap_) + " ";
        if (current->jump_ == nullptr) break;
        current = current->jump_;
    }
    gaps.pop_back();  // Remove trailing space

    return allNodes + "\n" + jumps + "\n" + gaps;
}




string JumpList::prettyPrint() const {
	// IMPLEMENT ME
	
    if (head_ == nullptr) {
        return "\n\n\n";  // Return three empty lines for an empty list.
    }

    string dataLine;
    string jumpLine;
    string gapLine;

    Node* current = head_;
    bool firstJumpNode = true;  // Track if we are at the first jump node.

    // First line: Collect data from all nodes and visualize with arrows
    while (current != nullptr) {
        dataLine += current->data_;
        if (current->next_ != nullptr) {
            dataLine += " --> ";  // Add arrow between nodes.
        }

        // For jump nodes, build the second and third lines.
        if (current->jump_ != nullptr) {
            if (firstJumpNode) {
                jumpLine += current->data_;
                gapLine += std::to_string(current->gap_);
                firstJumpNode = false;  // Set flag to false, so we don’t add it again.
            }
            jumpLine += " ---------------------> " + current->jump_->data_;
            gapLine += " " + std::to_string(current->gap_);
        }

        current = current->next_;  // Move to the next node.
    }

    // Remove trailing arrows, gaps, and spaces from the end
    if (!dataLine.empty()) {
        dataLine.pop_back();  // Remove the trailing space from dataLine.
        dataLine.pop_back();  // Remove the trailing arrow ('>') from dataLine.
    }

    if (!jumpLine.empty()) {
        jumpLine.pop_back();  // Remove trailing space from jumpLine.
        jumpLine.pop_back();  // Remove trailing arrow ('>') from jumpLine.
    }

    if (!gapLine.empty()) {
        gapLine.pop_back();  // Remove trailing space from gapLine.
    }

    // Handle the case of a single node, including the first node in the jump and gap lines
    if (head_->next_ == nullptr) {
        jumpLine = head_->data_;  // If only one node, set jumpLine to that node’s data.
        gapLine = "1";            // Single node, gap is 1.
    }

    // Combine the three lines with newlines
    return dataLine + "\n" + jumpLine + "\n" + gapLine;

}

void JumpList::updateJumps() {
    const int MAX_GAP_SIZE = 5;
    Node* current = head_;
    int count = 0;
    Node* lastJumpNode = head_;

    while (current) {
        if (count == MAX_GAP_SIZE) {
            lastJumpNode->jump_ = current;
            lastJumpNode->gap_ = MAX_GAP_SIZE;
            lastJumpNode = current;
            count = 0;
        }
        current = current->next_;
        ++count;
    }
    if (lastJumpNode) {
        lastJumpNode->jump_ = nullptr;
        lastJumpNode->gap_ = count;
    }
}


bool JumpList::insert(const string& s) {
	// IMPLEMENT ME
	    Node* prev = nullptr;
    Node* current = head_;

    // Find insertion point
    while (current && current->data_ < s) {
        prev = current;
        current = current->next_;
    }

    // Check for duplicates
    if (current && current->data_ == s) return false;

    // Create the new node
    Node* newNode = new Node(s, current, nullptr, 0);
    if (prev) prev->next_ = newNode;
    else head_ = newNode;

    // Update jump pointers after insertion
    updateJumps();
    if (newNode->next_ != 0) {
    std::cout <<  "--- insert data " <<  newNode->data_ << " next " << newNode->next_->data_ << " jmp " << newNode->jump_ << " gap " << newNode->gap_ << std::endl;
    } else {
        std::cout <<  "--- insert data " <<  newNode->data_ << " next " << newNode->next_ << " jmp " << newNode->jump_ << " gap " << newNode->gap_ << std::endl;
    }
    return true;

}

bool JumpList::erase(const string& s) {
	// IMPLEMENT ME
  Node* prev = nullptr;
    Node* current = head_;

    // Locate the node to delete
    while (current && current->data_ != s) {
        prev = current;
        current = current->next_;
    }

    // Node not found
    if (!current) return false;

    // Remove node
    if (prev) prev->next_ = current->next_;
    else head_ = current->next_;

    delete current;

    // Update jump pointers after deletion
    updateJumps();
    return true;

}