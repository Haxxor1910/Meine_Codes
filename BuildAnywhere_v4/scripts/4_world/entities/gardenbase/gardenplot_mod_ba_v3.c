//FIX GARDENPLOT on server side with BuildAnywhere mod
//AUTHOR: BORIZZ.K (s-platoon.ru)
//01.01.2019 - 06.05.2021

modded class GardenPlot extends GardenBase
{
	//#define classGardenPlotMyDebug
	
	override void OnPlacementComplete( Man player, vector position = "0 0 0", vector orientation = "0 0 0" )
	{	
		//Client
		if (GetGame().IsClient()) { super.OnPlacementComplete(player,position,orientation); return; }
		
		//Server
		PlayerBase player_base = PlayerBase.Cast( player );
		EntityAI item_in_hands = player_base.GetItemInHands();
		item_in_hands.AddHealth("","Health",-1);
		
		if ( GetGame().IsMultiplayer() && GetGame().IsServer() || !GetGame().IsMultiplayer() )
		{
			if (!CanBePlacedHere(player_base))
			{
				if (m_ClutterCutter) GetGame().ObjectDelete(m_ClutterCutter);
				GetGame().ObjectDelete(this);
			}
			else
			{
				super.OnPlacementComplete(player,position,orientation); //NATIVE CALL!
				if (m_ClutterCutter)
				{
					GPPlaceOnSurface(m_ClutterCutter);

				}
				GPPlaceOnSurface(this);
			}
		}
	}

	void GPPlaceOnSurface(Object obj)
	{
		obj.PlaceOnSurface();
		vector pos = obj.GetPosition();
		float ground = GetGame().SurfaceY(pos[0], pos[2]);
		pos[1] = ground + 0.01;
		obj.SetPosition(pos);
		obj.Update();
		obj.SetOrientation(obj.GetOrientation());
	}

	void GPPlaceOnSurfaceAligned(Object obj)
	{
		EntityAI ent = EntityAI.Cast(obj);
		if (!ent) return;

		vector pos = ent.GetPosition();
		float ground = GetGame().SurfaceY(pos[0], pos[2]);
		pos[1] = ground + 0.01;
		vector surface_normal = GetGame().SurfaceGetNormal(pos[0], pos[2]);

		vector mat[4];
		ent.GetTransform(mat);
		ent.PlaceOnSurfaceRotated(mat, pos, surface_normal[0] * -1, surface_normal[2] * -1, 0, true);
		ent.SetTransform(mat);
		ent.Update();
		ent.SetOrientation(ent.GetOrientation());	
	}

	bool CanBePlacedHere(PlayerBase player_base) //custom function//
	{
		private string surface_type = "";
		private vector position = this.GetPosition();
		GetGame().SurfaceGetType(position[0], position[2], surface_type);

		#ifdef classGardenPlotMyDebug
			Print("SERVER: [GardenPlot extends GardenBase]: OnPlacementComplete:CanBePlacedHere ::: Player: " + player_base.GetIdentity().GetName() + " >>> this = " + this + " >>> pos: " + this.GetPosition().ToString() + " >>> surface = " + surface_type);
		#endif

		if (!GetGame().IsSurfaceFertile(surface_type)) //Changed in 1.06 exp
		{
			#ifdef classGardenPlotMyDebug
				SendMessageToClient(player_base, "Incompatible surface: " + surface_type);
			#endif
			return false;
		}
		private float RayRadius 	= 0.2;
		private float RayAlt 		= 1.3;
		private vector direction 	= this.GetDirection();
		private vector orientation 	= this.GetOrientation();
		
		//Phase 1 - longitudinal
		private vector m_RayStart 	= position - (direction * 1.3);
		private vector m_RayEnd 	= position + (direction * 1.3);
		m_RayStart[1] 				= m_RayStart[1] + RayAlt;
		m_RayEnd[1] 				= m_RayEnd[1] + RayAlt;
		private array<ref RaycastRVResult> m_RayCastResults = new array<ref RaycastRVResult>;
		private RaycastRVParams m_RayCastInput 				= new RaycastRVParams(m_RayStart, m_RayEnd, this, RayRadius);
		m_RayCastInput.with 								= player_base;
		
		private bool m_RayCastResult = DayZPhysics.RaycastRVProxy(m_RayCastInput, m_RayCastResults);
		if (m_RayCastResult && m_RayCastResults.Count() > 0)
		{
			#ifdef classGardenPlotMyDebug
				SendMessageToClient(player_base, "Longitudinal Raycasted object: " + m_RayCastResults[0].obj);
			#endif
			return false;
		}
		
		//Phase 2 - perpendicular left
		orientation[0] 	= orientation[0] - 90;
		direction		= orientation.AnglesToVector();
		m_RayStart 		= position - (direction * 1.3);
		m_RayEnd 		= position + (direction * 1.3);
		m_RayStart[1] 	= m_RayStart[1] + RayAlt;
		m_RayEnd[1] 	= m_RayEnd[1] + RayAlt;

		m_RayCastResults.Clear();
		m_RayCastInput = new RaycastRVParams(m_RayStart, m_RayEnd, this, RayRadius);
		m_RayCastInput.with 	= player_base;
		
		m_RayCastResult = DayZPhysics.RaycastRVProxy(m_RayCastInput, m_RayCastResults);
		if (m_RayCastResult && m_RayCastResults.Count() > 0)
		{
			#ifdef classGardenPlotMyDebug
				SendMessageToClient(player_base, "Perpendicular Raycasted object: " + m_RayCastResults[0].obj);
			#endif
			return false;
		}
		
		//Phase 3 - perpendicular right
		orientation[0] 	= orientation[0] + 90;
		direction		= orientation.AnglesToVector();
		m_RayStart 		= position - (direction * 1.3);
		m_RayEnd 		= position + (direction * 1.3);
		m_RayStart[1] 	= m_RayStart[1] + RayAlt;
		m_RayEnd[1] 	= m_RayEnd[1] + RayAlt;

		m_RayCastResults.Clear();
		m_RayCastInput = new RaycastRVParams(m_RayStart, m_RayEnd, this, RayRadius);
		m_RayCastInput.with 	= player_base;
		
		m_RayCastResult = DayZPhysics.RaycastRVProxy(m_RayCastInput, m_RayCastResults);
		if (m_RayCastResult && m_RayCastResults.Count() > 0)
		{
			#ifdef classGardenPlotMyDebug
				SendMessageToClient(player_base, "Perpendicular Raycasted object: " + m_RayCastResults[0].obj);
			#endif
			return false;
		}

		#ifdef classGardenPlotMyDebug
			SendMessageToClient(player_base, "Compatible surface: " + surface_type);
		#endif
		return true;
	}

	#ifdef classGardenPlotMyDebug
	private void SendMessageToClient(PlayerBase player, string message)
	{
		if (player.GetIdentity())
		{
			Param1<string> Msgparam = new Param1<string>(message);
			GetGame().RPCSingleParam(player, ERPCs.RPC_USER_ACTION_MESSAGE, Msgparam, true, player.GetIdentity());
		}
	}
	#endif
}
