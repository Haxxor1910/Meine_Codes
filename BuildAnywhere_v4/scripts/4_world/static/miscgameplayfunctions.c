modded class MiscGameplayFunctions
{	
	static void PlaceOnSurfaceAligned(EntityAI ent)
	{
		if (!ent) return;

		vector pos = ent.GetPosition();
		pos[1] = pos[1] + 0.1;
		vector surface_normal = GetGame().SurfaceGetNormal(pos[0], pos[2]);

		vector mat[4];
		if (ent.PlaceOnSurfaceRotated(mat, pos, surface_normal[0] * -1, surface_normal[2] * -1, 0, true)) 
		{
			ent.SetOrientation(ent.GetOrientation());
		}
	}
}