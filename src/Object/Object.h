#ifndef OBJECT_H
#define OBJECT_H

#include <map>

class Callable;

/**
 * For now forget about super classes,
 * add them in the future
 */

class Object {
public:
	Object();
	virtual ~Object() = default;

	void extend(Object * super);

	bool has_field(const std::string & name) const;
	Object * find_field(const std::string & name) const;
	void set_field(const std::string & name, Object * value);

	// Helpers
	bool has_method(const std::string & name) const;
	Callable * find_method(const std::string & name) const;

	// Shorthand for method that does not receive params and just returns a value
	void set_return_method(const std::string & name, Object * val);

private:
	Object * super;
	std::map<std::string, Object*> fields;
};

#endif