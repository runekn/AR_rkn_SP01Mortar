[BaseContainerProps()]
class RKN_EntityExistanceCondition : SCR_ScenarioFrameworkActivationConditionBase
{
	[Attribute(desc: "True if entity exists. Otherwise true if not exists.")]
	bool m_bExists;
	
	[Attribute(desc: "Entity to evaluate")]
	ref SCR_ScenarioFrameworkGet m_Entity;
	
	override bool Init(IEntity entity)
	{
		SCR_ScenarioFrameworkParam<IEntity> entityWrapper = SCR_ScenarioFrameworkParam<IEntity>.Cast(m_Entity.Get());
		if (m_bExists) {
			return entityWrapper && entityWrapper.GetValue();
		} else {
			return !entityWrapper || !entityWrapper.GetValue();
		}
	}
}
