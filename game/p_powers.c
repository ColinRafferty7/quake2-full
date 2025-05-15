#include "g_local.h"

void RockDecay(edict_t* self)
{
	G_FreeEdict(self);
}

void RockThink(edict_t* self, edict_t* other)
{
	self->s.origin[2] += 30;

	self->count++;

	if (self->count >= 2)
	{
		gi.linkentity(self);
		self->nextthink = level.time + (self->mass);
		self->think = RockDecay;
	}
	else
	{
		self->nextthink = level.time + FRAMETIME;
	}
}

void SpawnRock(edict_t* self, vec3_t origin)
{
	edict_t* rock;

	rock = G_Spawn();

	VectorCopy(origin, rock->s.origin);
	rock->s.origin[2] -= 50;
	rock->solid = SOLID_BBOX;
	rock->movetype = MOVETYPE_STEP;
	rock->s.modelindex = gi.modelindex("models/objects/barrels/tris.md2");
	rock->think = RockThink;
	rock->nextthink = level.time + FRAMETIME;
	rock->mass = self->client->pers.level;
	VectorSet(rock->mins, -15, -15, 0);
	VectorSet(rock->maxs, 15, 15, 50);
	gi.linkentity(rock);
}

void SpawnMonster(edict_t *self, vec_t *origin)
{
	edict_t *mon;
	mon = G_Spawn();

	VectorCopy(origin, mon->s.origin);
	
	SP_monster_singer(mon);
	mon->enemy = self;
	mon->s.origin[2] -= mon->mins[2];
	gi.linkentity(mon);
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
		SpawnRock(self, hitscan.endpos);
		//SpawnMonster(self, hitscan.endpos);
	}
}

void Lashing(edict_t *self)
{
	vec3_t dir;
	int strength;

	strength = (200 * self->client->pers.level);

	AngleVectors(self->client->v_angle, dir, NULL, NULL);
	VectorScale(dir, strength, dir);
	VectorAdd(self->velocity, dir, self->velocity);
}

void ExploThink(edict_t *self)
{
	int min, max, bombCount, damage;
	vec3_t orig;

	min = -75;
	max = 75;
	bombCount = (5 * self->owner->client->pers.level);
	damage = (20 * self->owner->client->pers.level);

	VectorCopy(self->s.origin, orig);

	self->s.origin[0] += (min + (rand() % (max - min)));
	self->s.origin[1] += (min + (rand() % (max - min)));
	self->s.origin[2] += (min + (rand() % (max - min)));

	T_RadiusDamage(self, self->owner, damage, self->owner, 120, MOD_R_SPLASH);

	
	gi.WriteByte(svc_temp_entity);
	if (self->waterlevel)
		gi.WriteByte(TE_ROCKET_EXPLOSION_WATER);
	else
		gi.WriteByte(TE_ROCKET_EXPLOSION);
	gi.WritePosition(self->s.origin);
	gi.multicast(self->s.origin, MULTICAST_PHS);

	VectorCopy(orig, self->s.origin);

	self->count++;
	
	self->nextthink = level.time + FRAMETIME;

	if (self->count >= bombCount)
	{
		G_FreeEdict(self);
	}
}

void Division(edict_t *self)
{
	edict_t* explosion;
	vec3_t origin;

	explosion = G_Spawn();

	explosion->think = ExploThink;
	explosion->nextthink = level.time + FRAMETIME;
	AngleVectors(self->client->v_angle, origin, NULL, NULL);
	VectorScale(origin, 125, origin);
	VectorAdd(origin, self->s.origin, origin);
	VectorCopy(origin, explosion->s.origin);
	explosion->owner = self;
	explosion->count = 0;
}

void LightWeaveDecay(edict_t *self)
{
	G_FreeEdict(self);
}

void LightWeaving(edict_t *self)
{
	edict_t *lightweave;
	vec3_t spawn;

	int radius, decay, health;

	lightweave = G_Spawn();

	AngleVectors(self->client->v_angle, spawn, NULL, NULL);
	VectorScale(spawn, 125, spawn);
	VectorAdd(spawn, self->s.origin, spawn);

	lightweave->decoy = true;

	SP_monster_soldier(lightweave);
	VectorCopy(self->s.origin, lightweave->s.origin);

	lightweave->s.origin[0] = spawn[0];
	lightweave->s.origin[1] = spawn[1];
	lightweave->s.angles[1] = self->client->v_angle[1];
	lightweave->solid = SOLID_BBOX;
	lightweave->monsterinfo.aiflags |= AI_GOOD_GUY;
	lightweave->kill_xp = 0;
	lightweave->think = LightWeaveDecay;
	decay = (self->client->pers.level);
	lightweave->nextthink = level.time + decay;
	lightweave->s.modelindex = gi.modelindex("models/monsters/soldier/tris.md2");
	radius = (200 * self->client->pers.level);
	health = (10 * self->client->pers.level);
	lightweave->health = health;

	T_RadiusDamage(lightweave, lightweave, 0, self, radius, MOD_R_SPLASH);
	gi.linkentity(lightweave);
}

void heal_think(edict_t *self)
{
	self->owner->health++;
	self->count++;

	if (self->count > 40 || self->owner->health >= 100)
	{
		G_FreeEdict(self);
	}

	self->nextthink = level.time + FRAMETIME;
}

void EdgeDancing(edict_t *ent)
{
	edict_t *healing;

	healing = G_Spawn();

	healing->think = heal_think;
	healing->owner = ent;
	healing->nextthink = level.time + FRAMETIME;
}

