[BaseContainerProps()]
class RKN_AIGroupInjuredCondition : SCR_ScenarioFrameworkActivationConditionBase
{
	[Attribute(desc: "Entity to check.")]
	ref SCR_ScenarioFrameworkGet m_Getter;
	
	[Attribute(desc: "Minimum health of hit zone or group to satisfy condition. (Inclusive)", uiwidget: UIWidgets.Slider, defvalue: "0.0", params: "0 1 0.001")]
	float m_fHealthMinimum;

	[Attribute(desc: "Maximum health of hit zone or group to satisfy condition. (Inclusive)", uiwidget: UIWidgets.Slider, defvalue: "1.0", params: "0 1 0.001")]
	float m_fHealthMaximum;
	
	protected static const float S_F_HEALTH_EPSILON = 0.001;
	
	override bool Init(IEntity entity)
	{
		if (m_bDebug)
			Print("[RKN_AIGroupInjuredCondition.Init] Condition invoked with debug flag (" + __FILE__ + " L" + __LINE__ + ")", LogLevel.WARNING);
		
		if (m_Getter)
		{
			SCR_ScenarioFrameworkParam<IEntity> entityWrapper = SCR_ScenarioFrameworkParam<IEntity>.Cast(m_Getter.Get());
			entity = entityWrapper.GetValue();
		}
		else
		{
			SCR_ScenarioFrameworkLayerBase layer = SCR_ScenarioFrameworkLayerBase.Cast(entity.FindComponent(SCR_ScenarioFrameworkLayerBase));
			if (layer)
				entity = layer.GetSpawnedEntity();
		}
		if (!entity)
		{
			Print(string.Format("ScenarioFramework Condition: Entity not found for condition %1.", this), LogLevel.ERROR);
			return false;
		}

		return EvaluateInjuries(entity);
	}
	
	bool EvaluateInjuries(notnull IEntity ent)
	{
		SCR_AIGroup group = SCR_AIGroup.Cast(ent);
		if (!group)
			return true;
		
		foreach (SCR_ChimeraCharacter character : group.GetAIMembers())
		{
			SCR_CharacterDamageManagerComponent damageManager = SCR_CharacterDamageManagerComponent.Cast(character.GetDamageManager());
			if (!damageManager)
					return true;
		
			float health = damageManager.GetHealthScaled();
			if (EvaluateHealthScaled(health))
			{
				Print("Found injured member!");
				return true;
			}
		}
		return false;
	}
	
	protected bool EvaluateHealthScaled(float health)
	{
		return
			(health >= m_fHealthMinimum || float.AlmostEqual(health, m_fHealthMinimum, S_F_HEALTH_EPSILON)) &&
			(health <= m_fHealthMaximum || float.AlmostEqual(health, m_fHealthMaximum, S_F_HEALTH_EPSILON));
	}
}