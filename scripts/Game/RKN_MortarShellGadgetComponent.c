modded class SCR_MortarShellGadgetComponent
{
	override void OnShellFired(notnull BaseMuzzleComponent muzzle)
	{
		super.OnShellFired(muzzle);
		RKN_MortarHitHandlerComponent hitHandler = RKN_MortarHitHandlerComponent.Cast(GetOwner().FindComponent(RKN_MortarHitHandlerComponent));
		if (hitHandler)
			hitHandler.ShellFired(muzzle);
	}
}