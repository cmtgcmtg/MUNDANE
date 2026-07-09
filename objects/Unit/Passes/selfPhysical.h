
#include "../../../player/appliants/pass.h"

class Unit;
class selfPhysical : public Physical{
    public:
        selfPhysical(Unit*);
        ~selfPhysical() override;
        void render() override;
        Selectcallback select(Ray ray) override;
    private:
        Unit* unit;
};