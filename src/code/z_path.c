#include "global.h"
#include "z64path.h"


Path* Path_GetByIndex(PlayState* play, s16 index, s16 max) {
    Path* path;

    if (index != max) {
        path = &play->pathList[index];
    } else {
        path = NULL;
    }

    return path;
}

f32 Path_OrientAndGetDistSq(Actor* actor, Path* path, s16 waypoint, s16* yaw) {
    f32 dx;
    f32 dz;
    Vec3s* pointPos;

    if (path == NULL) {
        return -1.0;
    }

    pointPos = SEGMENTED_TO_VIRTUAL(path->points);
    pointPos = &pointPos[waypoint];

    dx = pointPos->x - actor->world.pos.x;
    dz = pointPos->z - actor->world.pos.z;

    if (yaw != NULL) {
        *yaw = RAD_TO_BINANG(Math_FAtan2F(dx, dz));
    }

    return SQ(dx) + SQ(dz);
}

void Path_CopyLastPoint(Path* path, Vec3f* dest) {
    Vec3s* pointPos;

    if (path != NULL) {
        pointPos = &((Vec3s*)SEGMENTED_TO_VIRTUAL(path->points))[path->count - 1];

        dest->x = pointPos->x;
        dest->y = pointPos->y;
        dest->z = pointPos->z;
    }
}

void Path_Init(PathInfo* this, Actor* actor, Path* path) {
    this->actor = actor;
    this->path = path;
    this->currentPoint.x = this->currentPoint.y = this->currentPoint.z = 0.0f;
    this->mode = PATH_MODE_REVERSE;
    this->index = 0;
    this->isReversed = false;
    this->updateActorPos = true;

    if (this->actor != NULL) {
        this->actorSpeed = this->actor->speed;
    }

    this->timerMode = PATH_MODE_REVERSE;
    this->pauseTimer = 0;
    this->canUpdate = true;
}

void Path_IncrementIndex(PathInfo* this) {
    s8 maxIndex = this->path->count;

    switch (this->indexMode) {
        case PATH_INDEX_MODE_INCR:
            if (this->index < maxIndex) {
                this->index++;
            }
            break;
        case PATH_INDEX_MODE_DECR:
            if (this->index >= 0) {
                this->index--;
            }
            break;
        default:
            return;
    }
}

void Path_UpdateIndex(PathInfo* this) {
    s8 lastPointIndex = this->path->count - 1;
    u8 mode = this->mode == PATH_MODE_TIMER ? this->timerMode : this->mode;

    switch (mode) {
        case PATH_MODE_REVERSE:
            this->timerMode = this->isReversed ? PATH_INDEX_MODE_DECR : PATH_INDEX_MODE_INCR;
            Path_IncrementIndex(this);
            if (this->index > lastPointIndex) {
                this->isReversed = true;
                this->index = lastPointIndex;
            } else if (this->index < 0) {
                this->isReversed = false;
                this->index = 0;
            }
            break;
        case PATH_MODE_LOOP:
            this->timerMode = PATH_INDEX_MODE_INCR;
            this->isReversed = false;
            Path_IncrementIndex(this);
            if (this->index > lastPointIndex) {
                this->index = 0;
            }
            break;
        case PATH_MODE_ONCE:
            this->isReversed = false;
            if (this->index < lastPointIndex) {
                this->timerMode = PATH_INDEX_MODE_INCR;
                Path_IncrementIndex(this);
            }
            break;
        case PATH_MODE_REVERSE_ONCE:
            this->isReversed = true;
            if (this->index > lastPointIndex) {
                this->timerMode = PATH_INDEX_MODE_DECR;
                Path_IncrementIndex(this);
            }
            break;
        default:
            return;
    }
}

u8 Path_UpdateActorPos(PathInfo* this) {
    f32 velocityMultiplier;
    f32 distSq;
    s32 yaw;
    Vec3f dist;
    Vec3f actorPos = this->actor->world.pos;
    Vec3s actorRot = this->actor->world.rot;

    if (this->canUpdate) {
        this->actor->speed = this->actorSpeed;

        velocityMultiplier = Math_CosS(-actorRot.x) * this->actor->speed;
        this->actor->velocity.x = Math_SinS(actorRot.y) * velocityMultiplier;
        this->actor->velocity.y = Math_SinS(-actorRot.x) * this->actor->speed;
        this->actor->velocity.z = Math_CosS(actorRot.y) * velocityMultiplier;

        this->actor->world.pos = actorPos;
        this->actor->world.rot = actorRot;
        Actor_UpdatePos(this->actor);

        distSq = Path_OrientAndGetDistSq(this->actor, this->path, this->index, &yaw);
        Math_SmoothStepToS(&actorRot.y, yaw, 10, 1000, 1);

        dist.x = this->currentPoint.x - actorPos.x;
        dist.y = this->currentPoint.y - actorPos.y;
        dist.z = this->currentPoint.z - actorPos.z;
        if ((fabsf(dist.x) < 2.0f) && (fabsf(dist.y) < 2.0f) && (fabsf(dist.z)) < 2.0f) {
            Path_UpdateIndex(this);
            return PATH_RET_TYPE_REACHED_DEST;
        }
        return PATH_RET_TYPE_NORMAL;
    }

    return PATH_RET_TYPE_WAIT;
}

void Path_Update(PathInfo* this) {
    u8 result = 0;

    if (this->path == NULL) {
        osSyncPrintf("[PATH:ERROR]: Path undefined! (NULL)\n");
        return;
    }

    if (this->actor == NULL) {
        osSyncPrintf("[PATH:ERROR]: Actor undefined! (NULL)\n");
        return;
    }

    if (this->updateActorPos) {
        result = Path_UpdateActorPos(this);

        if (!this->canUpdate && (this->mode == PATH_MODE_TIMER) && (this->pauseTimer > 0)) {
            this->pauseTimer--;
        }

        if ((result != PATH_RET_TYPE_WAIT) && this->mode == PATH_MODE_TIMER) {
            this->canUpdate = false;
        } else if ((this->mode != PATH_MODE_TIMER) || (this->pauseTimer == 0)) {
            this->canUpdate = true;
        }
    }
}
