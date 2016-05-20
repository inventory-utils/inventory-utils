#include <cstdint>
#include <iostream>
#include <map>
#include <set>
#include <string>

// TODO: move datstructures and some of includes to src/*-datastructures.{h,c}pp files.

using std::cin;
using std::cout;
using std::map;
using std::set;
using std::string;

// uids / Unique IDs of items:
// * uid<0 means it's temporary, internal uid, not allowed to be used by user.
// * uid==0 is root vertex of tree of items
// * uid>0 is Unique ID visible to user, and can be used by user.
using uid_t = int64_t;

class item_t {
	public:
		set<uid_t> children;
		// TODO: '$key=val$' to be implemented
		// map<string,
};

map<uid_t, item_t> 

int main(){
	int64_t i;
	cin >> i;
	cout << "# " << i << " #" << std::endl;
	return 0;
}
