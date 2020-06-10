#ifndef SCRIPT_HOST_HPP
#define SCRIPT_HOST_HPP

#include <Noon/Macros.hpp>

#include <string>
using std::string;

#include <vector>
using std::vector;

class Actor;

class ScriptHost
{
public:

    DISALLOW_COPY_AND_ASSIGN(ScriptHost)

    ScriptHost();
	virtual ~ScriptHost();

    bool LoadFile(const string& filename);

    void Run();

    void SetOwner(Actor *);

private:

};


#endif // SCRIPT_HOST_HPP
