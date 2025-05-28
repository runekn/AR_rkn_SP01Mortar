[EntityEditorProps(category: "GameScripted/ScenarioFramework/Layer", description: "")]
class RKN_LayerTaskClass : SCR_ScenarioFrameworkLayerTaskClass
{
}

class RKN_LayerTask : SCR_ScenarioFrameworkLayerTask
{
	[Attribute(category: "OnTaskCancelled")]
	ref array<ref SCR_ScenarioFrameworkActionBase> m_aTriggerActionsOnCancelled;
	
	override void OnTaskStateChanged(SCR_TaskState previousState, SCR_TaskState newState)
	{
		super.OnTaskStateChanged(previousState, newState);
		if (newState == SCR_TaskState.CANCELLED && !m_bTaskResolvedBeforeLoad)
		{
			foreach (SCR_ScenarioFrameworkActionBase triggerAction : m_aTriggerActionsOnCancelled)
			{
				triggerAction.OnActivate(GetOwner());
			}
		}
	}
}
