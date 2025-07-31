#ifndef PARTICLE_DATA_HH
#define PARTICLE_DATA_HH

#include <string>
#include <mutex>
#include <queue>

// Structure pour stocker les données des particules
struct ParticleData {
    G4double x=0.0;
    G4double xp=0.0;
    G4double z=0.0;
    G4double zp=0.0;
    G4double s=0.0;
    G4double delta=0.0;
    G4int n=0;
  };

struct ThreadEventData {
    std::queue<ParticleData> eventQueue;
    std::mutex mutex;

};
#endif // PARTICLE_DATA_HH
