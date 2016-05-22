#include <algorithm>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>

// TODO: consider http://clang.llvm.org/docs/ClangFormat.html

// TODO: move those D statements into debuggin levels "D(1," "D(2," or verbosity levels
#if NDEBUG
#define D(x)
#else
#define D(x) cerr << __FILE__ << ':' << __LINE__ << '\t' << #x << " = " << (x) << endl;
#endif

// TODO: move datstructures and some of includes to src/*-datastructures.{h,c}pp files.
// TODO: after move, add unitests for datastructures

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
		iuid_t _parentid;
		map<string, string> _keyval;
		void set_iuid(iuid_t id) { _my_iuid = id; }
		// reset will reset also other fields than iuid if ever appear
		void reset(iuid_t id, iuid_t parentid) {
			set_iuid(id);
			set_parent_iuid(parentid);
			_keyval.clear();
		}
		void set_parent_iuid(iuid_t parent) { _parentid = parent; }
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

		item_t(iuid_t id, iuid_t parent): _my_iuid(id), _parentid(parent) {}
		iuid_t iuid() const { return _my_iuid; }
		iuid_t parent_iuid() const { return _parentid; }

		string& operator[](string key) {
			return _keyval[key];
		}
		string get_val(string key, string default_val="") {
			return has_key(key) ? _keyval[key] : default_val;
		}
		bool has_key(string key) {
			return _keyval.count(key) > 0;
		}
		string code(string default_val_if_undefined="undefined"){
			return get_val(".", default_val_if_undefined);
		}

		// Modification of _my_iuid require update of translation map in universe_t
		// that's why only unverse_t is allowed to do that.
		// Also changing _parent is tightly connected with universe_t.
		friend universe_t;
};

class universe_t : public vector<unique_ptr<item_t>> {
	private:
		// If item has defined iuid, we translate it to it's "temporary" iuid,
		// that shows position in vector.
		// To convert temporary iuid to defined (if available) query given item
		// using item_t::iuit().
		map<iuid_t, iuid_t> iuid_d2t_map;
		set<iuid_t> released_items; // iuids recycling
		map<string, set<iuid_t>> where_are; // where are items with given code code->set<iuid_t>
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
			add_new_item(0);
		}
		item_t& add_new_item(iuid_t parent){
			if (released_items.empty()) {
				auto i = std::make_unique<item_t>(-size(), parent); // TODO: std::make_uniqueptr()
				push_back(std::move(i));
				return *back();
			} else {
				iuid_t id = *released_items.begin();
				released_items.erase(id);
				item_t& it = *(at(-id));
				it.reset(id, parent);
				return it;
			}
		}
		item_t& add_new_item(iuid_t parent, const string& code){
			auto& it = add_new_item(parent);
			set_code(it, code);
			return it;
		}
		void remove_item(iuid_t iuid){
			assert(iuid!=0); // we never remove root
			iuid = iuid_def2temp(iuid);
			assert(released_items.count(iuid)==0);
			assert(-iuid < size());
			released_items.insert(iuid);
			at(-iuid)->reset(iuid, 0);
		}
		item_t& get_item(iuid_t id) {
			id = iuid_def2temp(id);
			return *at(-id);
		}
		set<iuid_t>& get_iuids_by_code(const string& code) {
			return where_are.at(code);
		}
		item_t& root() {
			assert(size()>0); // we assume that there is always root
			return *operator[](0);
		}
		const item_t& root() const {
			return *at(0);
		}
		void set_code(iuid_t id, const string& code) {
			id = iuid_def2temp(id);
			auto& it = get_item(id);
			assert(it.has_key(".") == false);
			it["."] = code;
			where_are[code].insert(id);
		}
		void set_code(item_t& it, const string& code) {
			set_code(it.iuid(), code);
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
		vector<iuid_t> path_to(iuid_t id) {
			vector<iuid_t> path;
			path.push_back(id);
			while(id!=0){
				id = get_item(id).parent_iuid();
				// do not print root if name is unspecified.
				if (id==0 && ! get_item(id).has_key("."))
					break;
				path.push_back(id);
			}
			std::reverse(path.begin(), path.end());
			return path;
		}
		string path_as_string_to(iuid_t id, string separator="->") {
			string s="";
			for(auto&& it : path_to(id)){
				if(s.size()>0)
					s+=separator;
				s+=get_item(it).code();
			}
			return s;
		}
};

bool starts_with(const string& whole_string, const string& prefix){
	return whole_string.compare(0, prefix.size(), prefix) == 0;
}

int main(){
	// TODO: add getopts http://stackoverflow.com/a/17043717/544721
	universe_t U;
	// TODO: turn below into unitests of *datastructures*.{c,h}pp
	D( U.add_new_item(0).iuid() );
	D( U.add_new_item(0).iuid() );
	D( U.root().iuid() );
	auto& iA = U.add_new_item(0);
	D( iA.iuid() );
	U.define_iuid(iA, 555);
	D( "Check:" );
	D( iA.iuid() );
	D( U.add_new_item(0).iuid() );
	D( "Check removal:" );
	U.remove_item(-2);
	U.remove_item(-1);
	D( U.add_new_item(0).iuid() );
	D( U.add_new_item(0).iuid() );
	D( U.add_new_item(0).iuid() );

	// in which containers are we in. Starting from root/universe.
	iuid_t current_container_iuid = 0;
	iuid_t last_added_item_iuid = 0;

	string inputline;
	cerr << "Waiting for $++START_SESSION:V00++$ code." << endl;
	while (getline(cin, inputline)) {
		D( inputline );
		if (starts_with(inputline, "$++START_SESSION:V0++$")
		    || starts_with(inputline, "$++START_SESSION:V00++$")) {
		//TODO: add
		// log.dumpline(inputline);
		    break;
		} else if (starts_with(inputline, "$++EXIT_SESSION++$")) {
		  cerr << "Error: Not started session. No session to exit from. Exiting even without log." << endl;
		  return -2;
		}
	}
	cerr << "Session and logging Started" << endl;
	while (getline(cin, inputline)) {
		D( inputline );
		//TODO: add
		// log.dumpline(inputline);
		if (starts_with(inputline, "$++EXIT_SESSION++$")) {
		    break;
		} else if (starts_with(inputline, "$++OPEN_CONTAINER++$")) {
			current_container_iuid = last_added_item_iuid;
		} else if (starts_with(inputline, "$++CLOSE_CONTAINER++$")) {
			current_container_iuid = U.get_item(current_container_iuid).parent_iuid();
		} else if (starts_with(inputline, "$++CLOSE_ALL++$")) {
			current_container_iuid = 0;
		} else if (starts_with(inputline, "$++WHERE++$")) {
			if (getline(cin, inputline)) {
				const auto& code = inputline;
				D( code );
				for (const auto& id : U.get_iuids_by_code(code)) {
					D("-----------");
					D( id );
					auto& it = U.get_item(id);
					D( it.code() );
					D( U.path_as_string_to(id) );
					cout << U.path_as_string_to(id) << endl;
				}
			}
		} else{
			// We have new item to be added/removed into/from current container
			const auto& code = inputline;
			auto& it = U.add_new_item(current_container_iuid, code);
			last_added_item_iuid = it.iuid();
		}
	}
	if (starts_with(inputline, "$++EXIT_SESSION++$")) {
		cerr << "Session Finished." << endl;
	} else {
		cerr << "Session Finished without $++EXIT_SESSION++$ token." << endl;
	}

	return 0;
}
