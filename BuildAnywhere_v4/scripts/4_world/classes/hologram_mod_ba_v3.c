// BuildAnywhere_v3
// 02.02.2019 - 23.07.2021
// Author: BORIZZ.K (vk.com/opg_servers)

modded class Hologram
{
	protected static int	TENT_CAN_PLACE_ANYWHERE_MODE 	= 0; //MOD DEFAULT
	protected static float	PLACEMENT_HEIGHT_DIFF_MAX		= 1.64;
	protected static float	PLACEMENT_HEIGHT_DIFF_MIN		= -1.24;
	protected static float	PLACEMENT_PITCH_LIMIT_FACTOR	= 2.5;
	protected static float	PLACEMENT_ROLL_LIMIT_FACTOR 	= 2.5;

	void Hologram( PlayerBase player, vector pos, ItemBase item)
	{
		#ifndef SERVER
			TENT_CAN_PLACE_ANYWHERE_MODE	= player.GetTentCanBePlaceAnywhere();
			PLACEMENT_HEIGHT_DIFF_MAX		= player.GetPlacementHeightDiffMax();
			PLACEMENT_HEIGHT_DIFF_MIN		= player.GetPlacementHeightDiffMin();
			PLACEMENT_PITCH_LIMIT_FACTOR	= player.GetPlacementPitchLimitFactor();
			PLACEMENT_ROLL_LIMIT_FACTOR		= player.GetPlacementRollLimitFactor();
		#else
			TENT_CAN_PLACE_ANYWHERE_MODE	= GetBuildAnywhere_v3_Config().GetTentCanBePlaceAnywhereLevel();
			PLACEMENT_HEIGHT_DIFF_MAX		= GetBuildAnywhere_v3_Config().GetPlacementHeightDiffMaxLevel();
			PLACEMENT_HEIGHT_DIFF_MIN		= GetBuildAnywhere_v3_Config().GetPlacementHeightDiffMinLevel();
			PLACEMENT_PITCH_LIMIT_FACTOR	= GetBuildAnywhere_v3_Config().GetPlacementPitchLimitFactorLevel();
			PLACEMENT_ROLL_LIMIT_FACTOR		= GetBuildAnywhere_v3_Config().GetPlacementRollLimitFactorLevel();
		#endif
	}

	//modded
	override void EvaluateCollision(ItemBase action_item = null)
	{	
        if (m_Player.m_Trader_IsInSafezone) //13.01.2021
		{
            SetIsColliding(true);
		}
		else if (GetProjectionEntity().IsInherited(GardenPlotPlacing) /*|| GetProjectionEntity().IsInherited(Barrel_ColorBase)*/ || GetProjectionEntity().IsInherited(Fireplace)) //Garden plot, barrel, fireplace //GardenPlot.Cast(GetProjectionEntity())
		{
			super.EvaluateCollision(action_item);
		}
		else if (GetProjectionEntity().IsInherited(TentBase) || GetProjectionEntity().IsInherited(Barrel_ColorBase) || GetProjectionEntity().IsInherited(BarrelHoles_ColorBase)) //Tents, Barrels
		{
			#ifdef BuildAnywhere_v3_debug
				Print("CLIENT: [Hologram DEBUG]: EvaluateCollision: Projection: BEGIN ####################################################################################");
			#endif

			//Check restrictions level
			if (TENT_CAN_PLACE_ANYWHERE_MODE == 0) // 0 - soft restrictions
			{
				super.EvaluateCollision(action_item);
				if (IsAltutudeBad()) SetIsColliding(true);
			}
			else if (TENT_CAN_PLACE_ANYWHERE_MODE == 1) // 1 - no restrictions (little restrictions for water and altitude)
			{
				if (IsSurfaceWater(GetProjectionEntity().GetPosition()) || IsAltutudeBad())
				{
					SetIsColliding(true);
				}
				else
				{
					SetIsColliding(false);
				}
			}
			else if (TENT_CAN_PLACE_ANYWHERE_MODE == 2) // 2 - standard game restrictions
			{
				super.EvaluateCollision(action_item);
			}
			else
			{
				super.EvaluateCollision(action_item); // All others
			}
			
			#ifdef BuildAnywhere_v3_debug
				Print("CLIENT: [Hologram DEBUG]: EvaluateCollision: Projection: " + GetProjectionEntity() + ": action_item: " + action_item + ": TENT_CAN_PLACE_ANYWHERE_MODE == " + TENT_CAN_PLACE_ANYWHERE_MODE);
				Print("CLIENT: [Hologram DEBUG]: EvaluateCollision: Projection: " + GetProjectionEntity() + ": action_item: " + action_item + ": is_surface_water = " + IsSurfaceWater(m_Projection.GetPosition()) + ", m_IsHidden = " + m_IsHidden + ": IsColliding() = " + IsColliding());
				Print("CLIENT: [Hologram DEBUG]: EvaluateCollision: Projection: END ####################################################################################");
			#endif
		}
		else
		{
			#ifdef BuildAnywhere_v3_debug
				Print("CLIENT: [Hologram DEBUG]: EvaluateCollision:Projection: " + GetProjectionEntity() + ": action_item: " + action_item);
			#endif
			SetIsColliding(false); //All others
		}
	}

	//modded
	override bool IsCollidingAngle()
	{
		#ifdef BuildAnywhere_v3_debug
			Print("CLIENT: [Hologram DEBUG]: IsCollidingAngle(): Projection: " + GetProjectionEntity());
		#endif

		bool isTrue;

		if (!TentBase.Cast(GetProjectionEntity()) && !GetProjectionEntity().IsInherited(GardenPlot))
		{
			isTrue = super.IsCollidingAngle();
			#ifdef BuildAnywhere_v3_debug
				Print("CLIENT: [Hologram DEBUG]: super.IsCollidingAngle() Projection: " + GetProjectionEntity() + " >>> isTrue: " + isTrue);
			#endif
			return isTrue;
		}
		else
		{
			if (TENT_CAN_PLACE_ANYWHERE_MODE == 2)
			{
				return super.IsCollidingAngle();
			}
		}

		//TentBase

		vector projection_orientation = m_Projection.GetOrientation();
		vector tb_m_YawPitchRollLimit = m_YawPitchRollLimit; //mod
		
		tb_m_YawPitchRollLimit[1] = tb_m_YawPitchRollLimit[1] * PLACEMENT_PITCH_LIMIT_FACTOR; //increase max angle
		tb_m_YawPitchRollLimit[2] = tb_m_YawPitchRollLimit[2] * PLACEMENT_ROLL_LIMIT_FACTOR; //increase max angle
		
		isTrue = Math.AbsFloat( projection_orientation[1] ) > tb_m_YawPitchRollLimit[1] || Math.AbsFloat( projection_orientation[2] ) > tb_m_YawPitchRollLimit[2];
		#ifdef BuildAnywhere_v3_debug
			Print("DEVEL:  [Hologram DEBUG]: IsCollidingAngle(): isTrue: " + isTrue + " | (proj pitch) " + Math.AbsFloat( projection_orientation[1] ) + " > (pitch limit) " + m_YawPitchRollLimit[1] + " | (proj roll) " + Math.AbsFloat( projection_orientation[2] ) + " > (roll limit) " + m_YawPitchRollLimit[2]);
			Print("CLIENT: [Hologram DEBUG]: IsCollidingAngle(): Math.AbsFloat( projection_orientation[1] ) = " +  Math.AbsFloat( projection_orientation[1] )  + " > tb_m_YawPitchRollLimit[1] = " + tb_m_YawPitchRollLimit[1]);
			Print("CLIENT: [Hologram DEBUG]: IsCollidingAngle(): || Math.AbsFloat( projection_orientation[2] ) = " + Math.AbsFloat( projection_orientation[2] ) + " > tb_m_YawPitchRollLimit[2] = " + tb_m_YawPitchRollLimit[2]);
			Print("CLIENT: [Hologram DEBUG]: IsCollidingAngle(): Projection: " + GetProjectionEntity() + " >>> isTrue: " + isTrue);
		#endif
		return isTrue;
	}
	
	//modded
	override bool IsCollidingBBox(ItemBase action_item = null)
	{
		#ifdef BuildAnywhere_v3_debug
			Print("CLIENT: [Hologram DEBUG]: IsCollidingBBox(): Projection: " + GetProjectionEntity());
		#endif
		
		bool isTrue;

		if (!TentBase.Cast(GetProjectionEntity()))
		{
			isTrue = super.IsCollidingBBox(action_item);
			#ifdef BuildAnywhere_v3_debug
				Print("CLIENT: [Hologram DEBUG]: super.IsCollidingBBox() Projection: " + GetProjectionEntity() + " >>> isTrue: " + isTrue);
			#endif
			return isTrue;
		}
		else
		{
			if (TENT_CAN_PLACE_ANYWHERE_MODE == 2)
			{
				return super.IsCollidingBBox(action_item);
			}
		}

		vector center;
		vector relative_ofset; //we need to lift BBox, because it is calculated from the bottom of projection, and not from the middle
		vector absolute_ofset = "0 0.20 0"; //we need to lift BBox even more, because it colliddes with house floors due to various reasons (probably geometry or float imperfections) //default 0.01
		vector orientation = GetProjectionOrientation();
		vector edge_length;
		vector min_max[2];
		ref array<Object> excluded_objects = new array<Object>;
		ref array<Object> collided_objects = new array<Object>;
	
		GetProjectionCollisionBox( min_max );
		relative_ofset[1] = ( min_max[1][1] - min_max[0][1] ) / 2;
		center = m_Projection.GetPosition() + relative_ofset + absolute_ofset;
		edge_length = GetCollisionBoxSize( min_max );
		edge_length[0] = edge_length[0] * 0.8; //80% //change to 0.8 11.02.2020
		edge_length[1] = edge_length[1] * 0.8; //80% //change to 0.8 11.02.2020
		edge_length[2] = edge_length[2] * 0.8; //80% //change to 0.8 11.02.2020
		excluded_objects.Insert( m_Projection );
		excluded_objects.Insert( m_Player );
		if (action_item) excluded_objects.Insert(action_item);
			
		#ifdef BuildAnywhere_v3_debug
			Print("CLIENT: [Hologram DEBUG]: IsCollidingBBox() Projection: " + GetProjectionEntity() + " center = " + center + ", orientation = " + orientation + ", edge_length = " + edge_length);
		#endif

		//1.11 //1.12 //1.13
		isTrue = GetGame().IsBoxCollidingGeometry( center, orientation, edge_length, ObjIntersectView, ObjIntersectGeom, excluded_objects, collided_objects )/* && (!collided_objects[0].IsInherited(BaseBuildingBase) || IsFenceOrWatchtowerKit())*/;		

		#ifdef BuildAnywhere_v3_debug
			if (collided_objects.Count() > 0)
			{
				for( int i = 0; i < collided_objects.Count(); i++ )
				{
					Object obj_collided = collided_objects[i];
					Print("CLIENT: [Hologram DEBUG]: IsCollidingBBox(): Projection: " + GetProjectionEntity() + " obj_collided with: " + obj_collided + " >>> 1. isTrue: " + isTrue);
				}
			}
			else
			{
				Print("CLIENT: [Hologram DEBUG]: IsCollidingBBox(): Projection: " + GetProjectionEntity() + " no obj_collided: >>> 2. isTrue: " + isTrue);
			}
		#endif
		return isTrue;
	}

	//modded
	override bool IsBaseStatic( Object objectToCheck )
	{
		bool isTrue;

		if (!TentBase.Cast(GetProjectionEntity()))
		{
			isTrue = super.IsBaseStatic( objectToCheck );
		}
		else 
		{
			if (TENT_CAN_PLACE_ANYWHERE_MODE == 2)
			{
				isTrue = super.IsBaseStatic( objectToCheck );
			}
			else
			{
				isTrue = true;
			}
		}

		#ifdef BuildAnywhere_v3_debug
			Print("CLIENT: [Hologram DEBUG]: IsBaseStatic(): Projection: " + GetProjectionEntity() + " >>> isTrue: " + isTrue);
		#endif
		return isTrue;
	}

	//modded
	override bool IsBaseIntact( Object under_left_close, Object under_right_close, Object under_left_far, Object under_right_far )
	{
		bool isTrue;

		if (!TentBase.Cast(GetProjectionEntity()))
		{
			isTrue = super.IsBaseIntact( under_left_close,  under_right_close, under_left_far, under_right_far );
		}
		else
		{
			if (TENT_CAN_PLACE_ANYWHERE_MODE == 2)
			{
				isTrue = super.IsBaseIntact( under_left_close,  under_right_close, under_left_far, under_right_far );
			}
			else
			{
				isTrue = true;
			}
		}

		//TentBase
		#ifdef BuildAnywhere_v3_debug
			Print("CLIENT: [Hologram DEBUG]: IsBaseIntact(): Projection: " + GetProjectionEntity() + " >>> isTrue: " + isTrue);
		#endif
		return isTrue;
	}
	
	//modded
	override bool IsBaseFlat( vector contact_pos_left_close, vector contact_pos_right_close, vector contact_pos_left_far, vector contact_pos_right_far )
	{
		bool isTrue;

		if (!TentBase.Cast(GetProjectionEntity()) && !GetProjectionEntity().IsInherited(GardenPlot))
		{
			isTrue = super.IsBaseFlat( contact_pos_left_close, contact_pos_right_close, contact_pos_left_far, contact_pos_right_far );
		}
		else
		{
			if (TENT_CAN_PLACE_ANYWHERE_MODE == 2)
			{
				isTrue = super.IsBaseFlat( contact_pos_left_close, contact_pos_right_close, contact_pos_left_far, contact_pos_right_far );
			}
			else
			{
				isTrue = true;
			}
		}

		//TentBase
		#ifdef BuildAnywhere_v3_debug
			Print("CLIENT: [Hologram DEBUG]: IsBaseFlat(): Projection: " + GetProjectionEntity() + " >>> isTrue: " + isTrue);
		#endif
		return isTrue;
	}

	//modded
	override bool IsBehindObstacle()
	{		
		#ifdef BuildAnywhere_v3_debug
			Print("CLIENT: [Hologram DEBUG]: IsBehindObstacle(): Projection: " + GetProjectionEntity());
		#endif

		bool isTrue;

		if (!TentBase.Cast(GetProjectionEntity()))
		{
			isTrue = super.IsBehindObstacle();
			#ifdef BuildAnywhere_v3_debug
				Print("CLIENT: [Hologram DEBUG]: super.IsBehindObstacle() Projection: " + GetProjectionEntity() + " >>> isTrue: " + isTrue);
			#endif
			return isTrue;
		}
		else
		{
			if (TENT_CAN_PLACE_ANYWHERE_MODE == 2)
			{
				return super.IsBehindObstacle();
			}
		}
		
		vector starting_pos = GetParentEntity().GetPosition();
		vector headingDirection = MiscGameplayFunctions.GetHeadingVector(m_Player);
		array<Object> coneObjects = new array<Object>;
		vector c_MaxTargetDistance = GetProjectionPosition() - m_Player.GetPosition();
		float c_ConeAngle = c_MaxTargetDistance.Length() * 5;
	 	float c_ConeHeightMin = 0.25;
	 	float c_ConeHeightMax = 1;
		Building building;
		
		DayZPlayerUtils.GetEntitiesInCone( starting_pos, headingDirection, c_ConeAngle, c_MaxTargetDistance.Length(), c_ConeHeightMin, c_ConeHeightMax, coneObjects );
		
		for ( int i = 0; i < coneObjects.Count(); i++ )
		{
			Object object = coneObjects[i];
					
			if ( coneObjects.Count() > 1 && i > 0 )
			{
				#ifdef BuildAnywhere_v3_debug
					Print("CLIENT: [Hologram DEBUG]: IsBehindObstacle(): Projection: " + GetProjectionEntity() + " >>> object in cone: " + object);
				#endif
				if ( object == m_Player )
				{
					continue;
				}
				
				if ( object == GetParentEntity() )
				{
					continue;
				}
				
				if ( object == GetProjectionEntity() )
				{
					continue;
				}
				
				if ( object.IsTree() )
				{
					continue;
				}
				
				if ( Class.CastTo( building, object ) )
				{
					continue;
				}
				
				if ( object.IsInherited( CarTent ) )
				{
					continue;
				}

				#ifdef BuildAnywhere_v3_debug
					Print("CLIENT: [Hologram DEBUG]: IsBehindObstacle(): Projection: " + GetProjectionEntity() + ": object in cone!: " + object + ": alt = " +  object.GetPosition()[1] + ": player alt = " + m_Player.GetPosition()[1]);
					if ( object.GetType() )
					{
						Print("CLIENT: [Hologram DEBUG]: IsBehindObstacle(): Projection: " + GetProjectionEntity() + ": object in cone! type: " + object.GetType());
					}
					Print("CLIENT: [Hologram DEBUG]: IsBehindObstacle(): Projection: " + GetProjectionEntity() + ": c_ConeAngle = " + c_ConeAngle);
					Print("CLIENT: [Hologram DEBUG]: IsBehindObstacle(): Projection: " + GetProjectionEntity() + ": c_MaxTargetDistance.Length() = " + c_MaxTargetDistance.Length());
					Print("CLIENT: [Hologram DEBUG]: IsBehindObstacle(): Projection: " + GetProjectionEntity() + ": player pos = " + m_Player.GetPosition());
					Print("CLIENT: [Hologram DEBUG]: IsBehindObstacle(): Projection: " + GetProjectionEntity() + ": starting_pos = " + starting_pos);
					Print("CLIENT: [Hologram DEBUG]: IsBehindObstacle(): Projection: " + GetProjectionEntity() + ": headingDirection = " + headingDirection);
					Print("CLIENT: [Hologram DEBUG]: IsBehindObstacle(): Projection: " + GetProjectionEntity() + ": object pos = " + object.GetPosition());
				#endif
				
				string mw_Surface;
				vector position = GetProjectionEntity().GetPosition() + m_Player.GetDirection(); //test
				GetGame().SurfaceGetType(position[0], position[2], mw_Surface);

				#ifdef BuildAnywhere_v3_debug
					Print("CLIENT: [Hologram DEBUG]: IsBehindObstacle(): Projection: " + GetProjectionEntity() + ": object in cone >>> surface under: mw_Surface = " + mw_Surface);
				#endif
				
				if ((mw_Surface.Contains("asphalt") || mw_Surface == "metal_thin_ext" || mw_Surface == "concrete_ext" || mw_Surface == "cp_broadleaf") && (object.GetPosition()[1] > m_Player.GetPosition()[1] + 1.9 || object.GetPosition()[1] < m_Player.GetPosition()[1] - 1.9)) continue; //allow
				
				#ifdef BuildAnywhere_v3_debug
					Print("CLIENT: [Hologram DEBUG]: IsBehindObstacle(): Projection: " + GetProjectionEntity() + ": object in cone!!! >>> return TRUE");	
				#endif
				return true;
			}
		}
	
		#ifdef BuildAnywhere_v3_debug
			Print("CLIENT: [Hologram DEBUG]: IsBehindObstacle(): Projection: " + GetProjectionEntity() + ": no object in cone or compat surface >>> allow to place: return FALSE");
		#endif
		return false;
	}
	
	//modded
	override bool IsSurfaceWater( vector position )
	{
		if (!TentBase.Cast(GetProjectionEntity()))
		{
			return super.IsSurfaceWater(position);
		}
		else if (TENT_CAN_PLACE_ANYWHERE_MODE == 2)
		{
			return super.IsSurfaceWater(position);
		}
		
		//TentBase
		string mw_Surface;
		position = position + m_Player.GetDirection();
		GetGame().SurfaceGetType(position[0], position[2], mw_Surface);
		if (super.IsSurfaceWater(position) && (mw_Surface.Contains("asphalt") || mw_Surface == "metal_thin_ext" || mw_Surface == "concrete_ext" || mw_Surface.Contains("wood")))
		{
			return false;
		}
		return super.IsSurfaceWater(position);
	}
	
	//added
	bool IsAltutudeBad()
	{
		if ( GetProjectionEntity() )
		{
			vector position_Hologram = GetProjectionEntity().GetPosition();
			vector position_Player = m_Player.GetPosition();
			float diff = position_Player[1] - position_Hologram[1];
			if (diff > PLACEMENT_HEIGHT_DIFF_MAX || diff < PLACEMENT_HEIGHT_DIFF_MIN)
			{
				#ifdef BuildAnywhere_v3_debug
					Print("CLIENT: [Hologram DEBUG]: IsAltutudeBad(): Projection: " + GetProjectionEntity() + ": altitude not correct >>> not allow to place: return TRUE");
				#endif
				return true;
			}
		}
		return false;
	}
}
