#include <cstdint>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

// TODO: move datstructures and some of includes to src/*-datastructures.{h,c}pp files.

using std::cin;
using std::cout;
using std::map;
using std::set;
using std::string;
using std::vector;

// iuids / Item Unique IDs of items:
// * iuid<0 means it's temporary, internal uid, not allowed to be used by user.
// * iuid==0 is root vertex of tree of items
// * iuid>0 is Unique ID visible to user, and can be used by user.
using iuid_t = int64_t;

class item_t {
	private:
		iuid_t _my_iuid;
	public:
		set<iuid_t> children;
		// TODO: '$key=val$' to be implemented
		// map<string,

		item_t(iuid_t uid): _my_iuid(uid) {}
		iuid_t iuid() { return _my_iuid; }
};

class forest : public vector<item_t> {
	public:
		item_t& add_new_item(){
			iuid_t new_iuid = -size();
			emplace_back(new_iuid);
			return back();
		}
};

int main(){
	forest F;
	auto i1 = F.add_new_item();
	auto i2 = F.add_new_item();
	auto i3 = F.add_new_item();
	cout << "new iuid: " << i1.iuid() << std::endl;
	cout << "new iuid: " << i2.iuid() << std::endl;
	cout << "new iuid: " << i3.iuid() << std::endl;
	return 0;
}
