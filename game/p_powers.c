#include "g_local.h"

void RockTouch(edict_t* self, edict_t* other, cplane_t* plane, csurface_t* surf)
{
	float heightDif;
	//heightDif = other->s.origin[2]
}

void RockThink(edict_t* ent)
{
	ent->s.origin[2] += 30;
	ent->nextthink = level.time + FRAMETIME;
}

void SpawnRock(edict_t* self, vec3_t origin)
{
	edict_t* rock;

	rock = G_Spawn();

	VectorCopy(origin, rock->s.origin);
	rock->s.origin[2] -= 50;
	rock->s.modelindex = gi.modelindex("models/objects/barrels/tris.md2");
	rock->solid = SOLID_BBOX;
	rock->movetype = MOVETYPE_NONE;
	rock->think = RockThink;
	rock->nextthink = level.time + FRAMETIME;
	VectorSet(rock->mins, -15, -15, 0);
	VectorSet(rock->maxs, 15, 15, 50);
	gi.linkentity(rock);
}

void Geomancy(edict_t *self)
{
	vec3_t end1, end2;
	trace_t hitscan;

	AngleVectors(self->client->v_angle, end1, NULL, NULL);
	VectorScale(end1, 200, end1);
	VectorAdd(end1, self->s.origin, end1);
	
	hitscan = gi.trace(self->s.origin, NULL, NULL, end1, self, MASK_SOLID);

	if (VectorCompare(hitscan.endpos, end1))
	{
		end2[2] = -1.0f;
		VectorScale(end2, 100, end2);
		VectorAdd(end2, end1, end2);

		hitscan = gi.trace(end1, NULL, NULL, end2, self, MASK_SOLID);

		if (VectorCompare(hitscan.endpos, end2))
		{ 
			hitscan.ent = NULL;
		}

		
	}

	if (hitscan.ent)
	{
		gi.cprintf(self, 2, "%s\n", hitscan.ent->classname);
		SpawnRock(self, hitscan.endpos);
	}
}

