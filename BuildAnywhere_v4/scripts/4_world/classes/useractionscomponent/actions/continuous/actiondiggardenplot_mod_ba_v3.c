// BuildAnywhere_v3
// 11.02.2019 - 04.11.2020
// Author: BORIZZ.K (vk.com/opg_servers)
// Credits: Cleetus

modded class ActionDigGardenPlot: ActionDeployObject
{	
	//#define g_ActionDigGardenPlotMyDebug

	GardenPlot m_GardenPlot;
	
	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		//if (GetGame().IsClient())
		#ifndef SERVER
		{
			if (item)
			{
				if (!CanBePlacedHere(player, item)) return false;
			}
		}
		#endif
		return super.ActionCondition(player, target, item);
	}

	private bool CanBePlacedHere(PlayerBase player_base, ItemBase item) //custom function//
	{
		private string surface_type = "";
		private vector position = item.GetPosition();
		GetGame().SurfaceGetType(position[0], position[2], surface_type);

		#ifdef g_ActionDigGardenPlotMyDebug 
			Print("SERVER: [ActionDigGardenPlot: ActionDeployObject]: CanBePlacedHere: Player: " + player_base.GetIdentity().GetName() + " >>> item = " + item + " >>> pos: " + item.GetPosition().ToString() + " >>> surface = " + surface_type);
		#endif

		if (!GetGame().IsSurfaceFertile(surface_type)) //Changed in 1.06 exp
		{
			#ifdef g_ActionDigGardenPlotMyDebug 
				SendMessageToClient(player_base, "Incompatible surface: " + surface_type);
			#endif
			return false;
		}
		private float RayRadius 	= 0.2;
		private float RayAlt 		= 1.3;
		private vector direction 	= item.GetDirection();
		private vector orientation 	= item.GetOrientation();
		
		//Phase 1 - longitudinal
		private vector m_RayStart 	= position - (direction * 1.3);
		private vector m_RayEnd 	= position + (direction * 1.3);
		m_RayStart[1] 				= m_RayStart[1] + RayAlt;
		m_RayEnd[1] 				= m_RayEnd[1] + RayAlt;
		private array<ref RaycastRVResult> m_RayCastResults = new array<ref RaycastRVResult>;
		private RaycastRVParams m_RayCastInput 				= new RaycastRVParams(m_RayStart, m_RayEnd, item, RayRadius);
		m_RayCastInput.with 								= player_base;
		
		private bool m_RayCastResult = DayZPhysics.RaycastRVProxy(m_RayCastInput, m_RayCastResults);
		if (m_RayCastResult && m_RayCastResults.Count() > 0)
		{
			#ifdef g_ActionDigGardenPlotMyDebug 
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
		m_RayCastInput = new RaycastRVParams(m_RayStart, m_RayEnd, item, RayRadius);
		m_RayCastInput.with 	= player_base;
		
		m_RayCastResult = DayZPhysics.RaycastRVProxy(m_RayCastInput, m_RayCastResults);
		if (m_RayCastResult && m_RayCastResults.Count() > 0)
		{
			#ifdef g_ActionDigGardenPlotMyDebug 
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
		m_RayCastInput = new RaycastRVParams(m_RayStart, m_RayEnd, item, RayRadius);
		m_RayCastInput.with 	= player_base;
		
		m_RayCastResult = DayZPhysics.RaycastRVProxy(m_RayCastInput, m_RayCastResults);
		if (m_RayCastResult && m_RayCastResults.Count() > 0)
		{
			#ifdef g_ActionDigGardenPlotMyDebug 
				SendMessageToClient(player_base, "Perpendicular Raycasted object: " + m_RayCastResults[0].obj);
			#endif
			return false;
		}

		#ifdef g_ActionDigGardenPlotMyDebug
			SendMessageToClient(player_base, "Compatible surface: " + surface_type);
		#endif
		return true;
	}

	/*override void OnFinishProgressServer( ActionData action_data )
	{	
		PlaceObjectActionData poActionData;
		poActionData = PlaceObjectActionData.Cast(action_data);
		EntityAI entity_for_placing = action_data.m_MainItem;
		vector position = action_data.m_Player.GetLocalProjectionPosition();
		vector orientation = action_data.m_Player.GetLocalProjectionOrientation();
				
		if (GetGame().IsServer())
		{
			Print("SERVER: [ActionDigGardenPlot : ActionDeployObject]: entity_for_placing: " + entity_for_placing + "; holo.srv: " + action_data.m_Player.GetHologramServer().PlaceEntity( entity_for_placing ));
		}

		if ( GetGame().IsMultiplayer() )
		{		
			m_GardenPlot = GardenPlot.Cast( action_data.m_Player.GetHologramServer().PlaceEntity( entity_for_placing ));
			m_GardenPlot.SetOrientation( orientation );
			action_data.m_Player.GetHologramServer().CheckPowerSource();
			action_data.m_Player.PlacingCompleteServer();	
			
			m_GardenPlot.OnPlacementComplete( action_data.m_Player );
		}
			
		//local singleplayer
		if ( !GetGame().IsMultiplayer())
		{						
			m_GardenPlot = GardenPlot.Cast( action_data.m_Player.GetHologramLocal().PlaceEntity( entity_for_placing ));
			m_GardenPlot.SetOrientation( orientation );
			action_data.m_Player.PlacingCompleteLocal();
			
			m_GardenPlot.OnPlacementComplete( action_data.m_Player );
		}
		
		GetGame().ClearJuncture( action_data.m_Player, entity_for_placing );
		action_data.m_MainItem.SetIsBeingPlaced( false );
		action_data.m_Player.GetSoftSkillsManager().AddSpecialty( m_SpecialtyWeight );
		poActionData.m_AlreadyPlaced = true;	
		action_data.m_MainItem.SoundSynchRemoteReset();
		
		MiscGameplayFunctions.DealAbsoluteDmg(action_data.m_MainItem, 10);
		
	}*/
}
