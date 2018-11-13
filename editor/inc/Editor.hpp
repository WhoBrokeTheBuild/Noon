#ifndef EDITOR_HPP
#define EDITOR_HPP

#include <Noon.hpp>

class Editor : public App
{
public:

    static Editor * Inst() { return _Inst; }

    Editor();

    void Start() override;

private:

    static Editor * _Inst;

};

#endif // EDITOR_HPP
