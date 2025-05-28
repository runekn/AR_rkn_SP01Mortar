class RKN_TakeShellAction : SCR_ScriptedUserAction
{
	protected SCR_FilteredInventoryStorageComponentClass m_Data;
	protected SCR_InventoryStorageManagerComponent m_PlayerStorageManager;
	protected SCR_GadgetManagerComponent m_GadgetManager;
	protected BaseInventoryStorageComponent m_TargetStorage;
	
	protected const string REASON_CANNOT_FIT = "#AR-UserAction_LoadItems_NoSpace";
	protected const string REASON_NO_ITEMS = "#AR-UserAction_LoadItems_NoItems";
	
	//------------------------------------------------------------------------------------------------
	override void Init(IEntity pOwnerEntity, GenericComponent pManagerComponent)
	{
		SCR_FilteredInventoryStorageComponent storageComp = SCR_FilteredInventoryStorageComponent.Cast(pOwnerEntity.FindComponent(SCR_FilteredInventoryStorageComponent));
		if (!storageComp)
			return;

		m_Data = SCR_FilteredInventoryStorageComponentClass.Cast(storageComp.GetComponentData(pOwnerEntity));
		m_TargetStorage = BaseInventoryStorageComponent.Cast(pOwnerEntity.FindComponent(BaseInventoryStorageComponent));
	}

	//------------------------------------------------------------------------------------------------
	override bool CanBeShownScript(IEntity user)
	{
		if (!m_Data || m_Data.GetNumberOfAllowedTypes() < 1)
			return false;

		if (!m_TargetStorage)
			return false;

		return super.CanBeShownScript(user);
	}
	
	override bool CanBePerformedScript(IEntity user)
	{
		ChimeraCharacter character = ChimeraCharacter.Cast(user);
		if (!user)
			return false;
		
		m_GadgetManager = SCR_GadgetManagerComponent.GetGadgetManager(character);

		CharacterControllerComponent controller = character.GetCharacterController();
		if (!controller)
			return false;

		m_PlayerStorageManager = SCR_InventoryStorageManagerComponent.Cast(controller.GetInventoryStorageManager());
		if (!m_PlayerStorageManager)
			return false;
		
		array<IEntity> outItems = {};
		m_TargetStorage.GetAll(outItems);
		if (outItems.Count() == 0)
		{
			SetCannotPerformReason(REASON_NO_ITEMS);
			return false;
		}
		
		if (m_GadgetManager.GetHeldGadget())
		{
			SetCannotPerformReason(REASON_CANNOT_FIT);
			return false;
		}
		
		return true;
	}

	//------------------------------------------------------------------------------------------------
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		if (!m_PlayerStorageManager || !m_TargetStorage)
			return;

		array<IEntity> outItems = {};
		m_TargetStorage.GetAll(outItems);
		if (outItems.Count() > 0)
		{
			m_PlayerStorageManager.InsertItem(outItems[0], null, m_TargetStorage, null);
			m_GadgetManager = SCR_GadgetManagerComponent.Cast(pUserEntity.FindComponent(SCR_GadgetManagerComponent));
			m_GadgetManager.SetGadgetMode(outItems[0], EGadgetMode.IN_HAND);
		}
	}
}
