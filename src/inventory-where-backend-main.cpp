#include <cassert>
#include <cstdint>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>

#if NDEBUG
#define D(x)
#else
#define D(x) cerr << __FILE__ << ':' << __LINE__ << '\t' << #x << " = " << (x) << endl;
#endif

// TODO: move datstructures and some of includes to src/*-datastructures.{h,c}pp files.

using std::cerr;
using std::cin;
using std::cout;
using std::endl;
using std::map;
using std::set;
using std::string;
using std::unique_ptr;
using std::vector;

// iuids / Item Unique IDs of items:
// * iuid<0 means it's temporary, internal uid, not allowed to be used by user.
// * iuid==0 is root vertex of tree of items
// * iuid>0 is Unique ID visible to user, and can be used by user.
struct iuid_t {
	int64_t id;
	iuid_t(int64_t nid=0):id(nid){};
	operator int64_t() const { return id; }
	//operator int64_t&() { return id; }
	int64_t& operator=(int64_t nid) { id = nid; return id; }
	bool is_temporary_iuid() {
		return id < 0;
	}
	bool is_root() {
		return id == 0;
	}
	bool is_indexing_iuid() { // can we index universe vector with negative of this uid
		return is_temporary_iuid() || is_root();
	}
	bool is_defined_iuid() {
		return id > 0;
	}
};

class universe_t;

class item_t {
	private:
		iuid_t _my_iuid;
		void set_iuid(iuid_t id) { _my_iuid = id; }
		// reset will reset also other fields than iuid if ever appear
		void reset(iuid_t id) { set_iuid(id); }
	public:
		// forbid clone
		item_t & operator=(const item_t&) = delete;
		item_t(const item_t&) = delete;

		set<iuid_t> children;
		// TODO: '$key=val$' map to be implemented
		// map<string, string> ...

		// TODO: store timestamp of command and replay during dump.
		// using timestamp_t = ...;
		// timestamp_t timestamp;

		item_t(iuid_t id): _my_iuid(id) {}
		iuid_t iuid() const { return _my_iuid; }

		// modification of _my_iuid require update of translation map in universe_t
		// that's why only unverse_t is allowed to do that
		friend universe_t;
};

class universe_t : public vector<unique_ptr<item_t>> {
	private:
		// If item has defined iuid, we translate it to it's "temporary" iuid,
		// that shows position in vector.
		// To convert temporary iuid to defined (if available) query given item
		// using item_t::iuit().
		map<iuid_t, iuid_t> iuid_d2t_map;
		set<iuid_t> released_items;
		iuid_t iuid_def2temp(iuid_t defined_id) {
			if (defined_id.is_temporary_iuid() || defined_id.is_root()) {
				// it's already temporary iuid, so let's return it
				return defined_id;
			}
			auto i = iuid_d2t_map.find(defined_id);
			if (i == iuid_d2t_map.end()) {
				// it's undefined, so just return given value (instead of assert(!=.end()))
				return defined_id;
			}
			return i->second;
		}
	public:
		universe_t() {
			// Add root
			add_new_item();
		}
		item_t& add_new_item(){
			if (released_items.empty()) {
				auto i = std::make_unique<item_t>(-size()); // TODO: std::make_uniqueptr()
				push_back(std::move(i));
				return *back();
			} else {
				iuid_t id = *released_items.begin();
				released_items.erase(id);
				item_t& it = *(at(-id));
				it.reset(id);
				return it;
			}
		}
		void remove_item(iuid_t iuid){
			iuid = iuid_def2temp(iuid);
			assert(released_items.count(iuid)==0);
			assert(-iuid < size());
			released_items.insert(iuid);
			at(-iuid)->reset(iuid);
		}
		item_t& root() {
			return *(operator[](0));
		}
		const item_t& root() const {
			return *(at(0));
		}
		void define_iuid(iuid_t temporary_id, iuid_t defined_id){
			assert(temporary_id<0);
			assert(defined_id>0);
			assert(-temporary_id < size()); // element has to exist
			// it can not be yet defined, or if defined can not mismatch
			assert(iuid_d2t_map.count(defined_id)==0 \
					|| (iuid_d2t_map[defined_id]==temporary_id
						&& operator[](temporary_id)->iuid()==defined_id));
			iuid_d2t_map[defined_id] = temporary_id;
			at(-temporary_id)->set_iuid(defined_id);
		}
		void define_iuid(item_t& it, iuid_t defined_id) {
			assert(it.iuid().is_temporary_iuid());
			define_iuid(it.iuid(), defined_id);
		}
};

int main(){
	universe_t U;
	// TODO: turn below into unitests of *datastructures*.{c,h}pp
	D( U.add_new_item().iuid() );
	D( U.add_new_item().iuid() );
	D( U.root().iuid() );
	auto& iA = U.add_new_item();
	D( iA.iuid() );
	U.define_iuid(iA, 555);
	D( "Check:" );
	D( iA.iuid() );
	D( U.add_new_item().iuid() );
	D( "Check removal:" );
	U.remove_item(-2);
	U.remove_item(-1);
	D( U.add_new_item().iuid() );
	D( U.add_new_item().iuid() );
	D( U.add_new_item().iuid() );

	string inputline;
	while(getline(cin, inputline)){
		cout << "# " << inputline << " #" << endl;
	}
	return 0;
}
