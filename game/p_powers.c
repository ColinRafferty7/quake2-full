#include "g_local.h"

void SpawnRock(edict_t* self, vec3_t origin)
{
	edict_t* rock;

	rock = G_Spawn();

	VectorCopy(origin, rock->s.origin);
	rock->s.modelindex = gi.modelindex("models/objects/barrels/tris.md2");
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

