[BaseContainerProps(), SCR_ContainerActionTitle()]
class RKN_GetRadioEntity : SCR_ScenarioFrameworkGet
{
	override SCR_ScenarioFrameworkParamBase Get()
	{
		return new SCR_ScenarioFrameworkParam<IEntity>(GetGame().GetWorld().FindEntityByName("Radio"));
	}
}
