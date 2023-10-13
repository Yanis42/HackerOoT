#include "ultra64.h"
#include "z64math.h"
#include "z64scene.h"


typedef enum PathMode {
    /* 0 */ PATH_MODE_REVERSE,      // A -> B -> C, when it reach C does C -> B -> A
    /* 1 */ PATH_MODE_LOOP,         // A -> B -> C, when it reach C go directly back to A
    /* 2 */ PATH_MODE_ONCE,         // A -> B -> C, do nothing when it reach C
    /* 3 */ PATH_MODE_REVERSE_ONCE, // C -> B -> A, do nothing when it reach A
    /* 4 */ PATH_MODE_TIMER,        // A -> B, waits N frames then do B -> C, see ``timerMode``
} PathMode;

typedef enum PathIndexMode {
    /* 0 */ PATH_INDEX_MODE_INCR,
    /* 1 */ PATH_INDEX_MODE_DECR,
} PathIndexMode;

typedef enum PathUpdateActorPosReturnType {
    /* 0 */ PATH_RET_TYPE_NORMAL,
    /* 1 */ PATH_RET_TYPE_REACHED_DEST,
    /* 2 */ PATH_RET_TYPE_WAIT,
} PathUpdateActorPosReturnType;

typedef struct PathInfo {
    Actor* actor;       // pointer to the actor user
    Path* path;         // pointer to the path list to use
    Vec3f currentPoint; // current path point coordinates
    u8 mode;            // main update mode
    u8 indexMode;       // index update mode
    s8 index;           // path point number
    u8 isReversed;      // decrements the index if true
    u8 updateActorPos;  // true by default
    f32 actorSpeed;     // sets and inits to ``actor.speed``
    u8 timerMode;       // update mode to use if ``mode`` is set to ``PATH_MODE_TIMER``
    u16 pauseTimer;     // stop motion for N frames
    u8 canUpdate;
} PathInfo;


Path* Path_GetByIndex(PlayState* play, s16 index, s16 max);
f32 Path_OrientAndGetDistSq(Actor* actor, Path* path, s16 waypoint, s16* yaw);
void Path_CopyLastPoint(Path* path, Vec3f* dest);
void Path_Init(PathInfo* this, Actor* actor, Path* path);
void Path_IncrementIndex(PathInfo* this);
void Path_UpdateIndex(PathInfo* this);
u8 Path_UpdateActorPos(PathInfo* this);
void Path_Update(PathInfo* this);
