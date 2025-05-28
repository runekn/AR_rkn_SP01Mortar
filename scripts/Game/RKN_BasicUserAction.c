class RKN_BasicUserAction : SCR_ScriptedUserAction
{
	[Attribute(desc: "Actions to perform", UIWidgets.Auto)]
	ref array<ref SCR_ScenarioFrameworkActionBase> m_aActions;
	
	[Attribute(defvalue: "true", UIWidgets.Auto)]
	bool m_bHideAfterActivation;
	
	//------------------------------------------------------------------------------------------------
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		foreach (SCR_ScenarioFrameworkActionBase action : m_aActions)
		{
			action.Init(pOwnerEntity);
		}
		if (m_bHideAfterActivation)
		{
			SetActionEnabled_S(false);
		}
	}
}
