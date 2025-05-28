[BaseContainerProps(), SCR_ContainerActionTitle()]
class RKN_ActionBasedOnConditionLoop : SCR_ScenarioFrameworkActionBase
{
	[Attribute(desc: "How long to wait before executing action")]
	int	m_iDelayInSeconds;

	[Attribute(desc: "If this is set to a number larger than  Delay In Seconds, it will randomize resulted delay between these two values")]
	int m_iDelayInSecondsMax;

	[Attribute(defvalue: "1", desc: "How many times conditions can succeed before loop is terminated.")]
	int m_iSuccessMax;
	
	[Attribute(desc: "Action activation conditions")]
	protected ref array<ref SCR_ScenarioFrameworkActivationConditionBase> m_aActivationConditions;

	[Attribute(defvalue: SCR_EScenarioFrameworkLogicOperators.AND.ToString(), UIWidgets.ComboBox, "Which Boolean Logic will be used for Activation Conditions", "", enums: SCR_EScenarioFrameworkLogicOperatorHelper.GetParamInfo(), category: "Activation")]
	SCR_EScenarioFrameworkLogicOperators m_eActivationConditionLogic;

	[Attribute(desc: "Actions to be executed when condition evaluation is successful.")]
	ref array<ref SCR_ScenarioFrameworkActionBase> m_aActions;
	
	[Attribute(desc: "Actions to be executed every time condition evaluation fails.")]
	ref array<ref SCR_ScenarioFrameworkActionBase> m_aFailedActions;

	protected int m_iDelay;
	protected int m_iSuccess = 0;
	//------------------------------------------------------------------------------------------------
	//!
	//! \param[in] object
	void ExecuteActions(IEntity object)
	{
		m_iDelay = m_iDelayInSeconds;
		if (m_iDelayInSecondsMax > m_iDelayInSeconds)
			m_iDelay = Math.RandomIntInclusive(m_iDelayInSeconds, m_iDelayInSecondsMax);

		if (SCR_ScenarioFrameworkActivationConditionBase.EvaluateEmptyOrConditions(m_eActivationConditionLogic, m_aActivationConditions, object))
		{
			foreach (SCR_ScenarioFrameworkActionBase actions : m_aActions)
			{
				actions.OnActivate(object);
			}
			if (++m_iSuccess >= m_iSuccessMax)
			{
				if (m_bDebug)
					Print("RKN_ActionBasedOnConditionLoop: Max successes reached. Terminating loop.");
				SCR_ScenarioFrameworkSystem.GetCallQueuePausable().Remove(ExecuteActions);
			}
		}
		else
		{
			foreach (SCR_ScenarioFrameworkActionBase actions : m_aFailedActions)
			{
				actions.OnActivate(object);
			}
		}
	}

	//------------------------------------------------------------------------------------------------
	override void OnActivate(IEntity object)
	{
		if (!CanActivate())
			return;

		m_iDelay = m_iDelayInSeconds;
		if (m_iDelayInSecondsMax > m_iDelayInSeconds)
			m_iDelay = Math.RandomIntInclusive(m_iDelayInSeconds, m_iDelayInSecondsMax);

		//Used to delay the call as it is the feature of this action
		SCR_ScenarioFrameworkSystem.GetCallQueuePausable().CallLater(ExecuteActions, m_iDelay * 1000, true, object);
	}

	//------------------------------------------------------------------------------------------------
	override array<ref SCR_ScenarioFrameworkActionBase> GetSubActions()
	{
		array<ref SCR_ScenarioFrameworkActionBase> allActions = {};
		
		foreach (SCR_ScenarioFrameworkActionBase action : m_aActions)
		{
			allActions.Insert(action);
		}
		
		foreach (SCR_ScenarioFrameworkActionBase action : m_aFailedActions)
		{
			allActions.Insert(action);
		}
		
		return allActions;
	}
}