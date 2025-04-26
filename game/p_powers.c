#include "g_local.h"

void Geomancy(edict_t *self)
{
	vec3_t end;
	trace_t hitscan;

	AngleVectors(self->client->v_angle, end, NULL, NULL);
	VectorScale(end, 200, end);
	VectorAdd(end, self->s.origin, end);
	
	hitscan = gi.trace(self->s.origin, NULL, NULL, end, self, MASK_SOLID);

	if (VectorCompare(hitscan.endpos, end))
	{
		hitscan.ent = NULL;
	}

	if (hitscan.ent)
	{
		gi.cprintf(self, 2, "%s\n", hitscan.ent->classname);
	}
}