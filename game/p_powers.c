#include "g_local.h"

void Geomancy(edict_t *self)
{
	vec3_t end;
	trace_t hitscan;

	AngleVectors(self->client->v_angle, end, NULL, NULL);
	VectorScale(end, 200, end);
	VectorAdd(end, self->s.origin, end);

	gi.cprintf(self, 2, "(%f, %f, %f)\n", self->s.origin[0], self->s.origin[1], self->s.origin[2]);
	gi.cprintf(self, 2, "(%f, %f, %f)\n", end[0], end[1], end[2]);
	
	hitscan = gi.trace(self->s.origin, NULL, NULL, end, self, MASK_SHOT);

	if (hitscan.ent)
	{
		gi.cprintf(self, 2, "%s\n", hitscan.ent->classname);
		gi.cprintf(self, 2, "(%f, %f, %f)\n", hitscan.endpos[0], hitscan.endpos[1], hitscan.endpos[2]);
	}
}