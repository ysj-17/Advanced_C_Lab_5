// Program Name: Lab 5
//
// Description:  
//  This lab uses an adjacency matrix or a list from the data in the 
//  PageRank.html file.
//  It uses data from the file to implement an algorithm to calculate Page Ranks
//  of the respective webpages contained in the file.
//
// What's on your mind about this lab?
//  This lab wasn't too difficult since it was an accumlation of lessons learned
//  throughout the quarter.
//  But it was still challenging to figure out the implementation of the file outputs
//  and the algorithm.
//
// Author: Jason Yoon
//
// Date: 12/11/2018
//
// IDE Used: Visual Microsoft
//


// THANK YOU FOR A GREAT QUARTER!!

#include <iostream>
#include <fstream>
#include <regex>
#include <vector>
#include <list>
#include <algorithm>
using namespace std;

template <class T>
class Node {
private:
	T identifier;
	vector<T> data; // data contains other strings
	double PageRank = 1.0;
public:
	Node() {};
	Node(T input) { identifier = input; }
	void set_identifier(T name) { identifier = name; }
	void set_data_push(vector<string> data) { this->data = data; }
	T get_identifer() { return identifier; }
	vector<T> get_data() { return data; }
	void set_PageRank(double rank) { PageRank = rank; }
	double get_PageRank() { return PageRank; }
};

class HTML_File_Reader { // Reads Carts
public:
	HTML_File_Reader() {};
	list<shared_ptr<Node<string>>> readHTMLFile(string FILE_NAME);
	string whiteSpaceEliminator(string withWhiteSpace);
	vector<string> otherLinksFinder(ifstream& in);
};

void PageRank_Algorithm(list<shared_ptr<Node<string>>> file_outputs);

/*------------------------------------------ HERE IS MY MAIN ------------------------------------------*/

int main() {

	list<shared_ptr<Node<string>>> file_outputs;
	vector<string> other_links;
	vector<string> page_names;
	vector<string> cleaned_links;
	HTML_File_Reader readHTML;

	file_outputs = readHTML.readHTMLFile("PageRank.html");
	PageRank_Algorithm(file_outputs);

	cout << endl << "NAMES:" << endl;
	for (auto index : file_outputs) {
		cout << index->get_identifer() << endl;
		other_links = index->get_data();
		cout << "Other Links: " << other_links.size() << endl;
		for (auto x : other_links) {
			cout << x << endl;
		}
		cout << "PAGE RANK: " << index->get_PageRank() << endl;
		cout << endl;
	}

	cin.get();
	return 0;
}


void PageRank_Algorithm(list<shared_ptr<Node<string>>> file_outputs) {
	double damping_factor = 0.85;
	double get_other_ranks = 0.0;
	double finished_rank = 0.0;
	vector<string> outbound_links;
	string page_name;
	for_each(file_outputs.begin(), file_outputs.end(), [&](shared_ptr<Node<string>> iterator) {
		page_name = iterator->get_identifer();
		for (auto page : file_outputs) {
			outbound_links = page->get_data();
			if (find(outbound_links.begin(), outbound_links.end(), page_name) != outbound_links.end()) {
				get_other_ranks += (page->get_PageRank() / page->get_data().size());
			}
		}
		finished_rank = (1.0 - damping_factor) + (damping_factor) * (get_other_ranks);
		iterator->set_PageRank(finished_rank);
	});
}

list<shared_ptr<Node<string>>> HTML_File_Reader::readHTMLFile(string FILE_NAME) {
	
	string line;
	string matched;
	smatch base_match;
	regex title_tag_match("<title>(.*)</title>");
	list<shared_ptr<Node<string>>> file_outputs;

	ifstream in("PageRank.html");
	while (getline(in, line)) {
		string tmp;
		tmp = whiteSpaceEliminator(line); // strip whitespaces from the beginning
		if (regex_search(tmp, base_match, title_tag_match)) { // does a regex_search and gets the matching string
			shared_ptr<Node<string>> website (new Node<string>);
			matched = base_match[1].str(); // gets the title of the page
			if (matched == "HISTORY")
				matched = "History";
			website->set_identifier(matched);
			website->set_data_push(otherLinksFinder(in));
			file_outputs.push_back(website);
		}
	}
	in.close();
	return file_outputs;
}

string HTML_File_Reader::whiteSpaceEliminator(string withWhiteSpace) {
	string tmp;
	for (unsigned int i = 0; i < withWhiteSpace.length(); i++)
	{
		if (withWhiteSpace[i] == '\t' || withWhiteSpace[i] == ' ' && tmp.size() == 0)
		{
		}
		else
		{
			tmp += withWhiteSpace[i];
		}
	}
	return tmp;
}

vector<string> HTML_File_Reader::otherLinksFinder(ifstream& in) {

	string tmp, line, URL_matched;
	bool done = false;
	vector<string> other_links;
	smatch URL_match;
	regex class_links_tag_match("<div class=\"other-links\">");
	regex URL_tag_match("\">(.*)</a></li>");
	regex Page_Rank_Match("(History)|(BBC History)|(.*)(Networks)|(Discovery)|(Science Channel)");

	while (!done) {
		getline(in, line);
		tmp = whiteSpaceEliminator(line);
		if (regex_match(tmp, class_links_tag_match)) { // does a regex_search and gets the matching string
			while (tmp != "</ul>") {
				getline(in, line);
				tmp = whiteSpaceEliminator(line);
				regex_search(tmp, URL_match, URL_tag_match);
				URL_matched = URL_match[1].str();
				if (regex_match(URL_matched, Page_Rank_Match)) {
					other_links.push_back(URL_matched);
					//cout << URL_matched << endl;
				}
			}
			done = true;
		}
	}
	return other_links;
}
