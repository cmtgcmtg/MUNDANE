#include <functional>
#include <vector>
#include <map>
// this designates how the routine will fire
enum Routinepath
{
    delay10
};
class Routine
{
public:
    static void run(Routinepath);
    bool running;
    Routine(Routinepath);
    //~Routine();

private:
    static std::map<Routinepath, std::vector<Routine*>> all;
    virtual bool increment();
    virtual void completion();
};