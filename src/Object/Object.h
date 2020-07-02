#ifndef OBJECT_H
#define OBJECT_H

#include <map>

using ObjectFields = std::map<std::string, Object*>;

class Callable;

class Object {
public:
	Object();
	virtual ~Object() = default;

	bool has_field(const std::string & name) const;
	Object * find_field(const std::string & name) const;
	void set_field(const std::string & name, Object * value);

	// Helpers
	bool has_method(const std::string & name) const;
	Callable * find_method(const std::string & name) const;

	// Shorthand for method that does not receive params and just returns a value
	void set_return_method(const std::string & name, Object * val);

	Object * operator[](const std::string & name) const {
		return find_field(name);
	}

private:
	ObjectFields fields;
};

#endif