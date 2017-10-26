#include "Engine.cpp"
using namespace std;
int main()
{
    // Declare an instance of Engine
    EngineKinematic engine_kinematic;
    EngineSteering engine;
    EngineFollowPath engine_path;
    EnginePrioritySteering engine_ps;
    // engine_path.start();
    // EngineSteering engine_steering;
    //Start the engine
    // engine.start_seek();
    // engine.start_flee();
    // engine.start_arrive();
    // engine.start_pursue();
    // engine.start_wander();
    engine_ps.start();
    // Quit in the usual way when the engine is stopped
    return 0;
}
