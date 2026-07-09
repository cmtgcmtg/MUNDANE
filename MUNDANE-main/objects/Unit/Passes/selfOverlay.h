
#include "../../../player/appliants/pass.h"

class Unit;
class selfOverlay : public Overlay{
    public:
        selfOverlay(Unit*);
        ~selfOverlay() override;
        void render() override;
        Selectcallback select(Vector2 point) override;
    private:
        Unit* unit;
};