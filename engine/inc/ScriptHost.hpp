#ifndef SCRIPT_HOST_HPP
#define SCRIPT_HOST_HPP

#include <string>
using std::string;

#include <vector>
using std::vector;

class ScriptHost
{
public:

    ScriptHost();
	virtual ~ScriptHost();

    bool LoadFile(const string& filename);

    void Run();

private:

};


#endif // SCRIPT_HOST_HPP
