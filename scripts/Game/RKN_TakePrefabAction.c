class RKN_TakePrefabAction : SCR_ScriptedUserAction
{
	[Attribute(params: "et", desc: "Prefab that is taken to hand")]
	ResourceName m_sPrefabName;
	
	protected SCR_InventoryStorageManagerComponent m_PlayerStorageManager;
	protected SCR_GadgetManagerComponent m_GadgetManager;
	protected BaseInventoryStorageComponent m_TargetStorage;
	
	protected const string REASON_CANNOT_FIT = "#AR-UserAction_LoadItems_NoSpace";
	protected const string REASON_NO_ITEMS = "#AR-UserAction_LoadItems_NoItems";
	
	//------------------------------------------------------------------------------------------------
	override void Init(IEntity pOwnerEntity, GenericComponent pManagerComponent)
	{
		m_TargetStorage = BaseInventoryStorageComponent.Cast(pOwnerEntity.FindComponent(BaseInventoryStorageComponent));
	}

	//------------------------------------------------------------------------------------------------
	override bool CanBeShownScript(IEntity user)
	{
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
		
		IEntity entity = FindFirstByPrefab();
		if (entity == null)
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

		IEntity entity = FindFirstByPrefab();
		if (entity == null)
			return;
		
		m_PlayerStorageManager.InsertItem(entity, null, m_TargetStorage, null);
		m_GadgetManager = SCR_GadgetManagerComponent.Cast(pUserEntity.FindComponent(SCR_GadgetManagerComponent));
		m_GadgetManager.SetGadgetMode(entity, EGadgetMode.IN_HAND);
	}
	
	private IEntity FindFirstByPrefab()
	{
		array<IEntity> outItems = {};
		m_TargetStorage.GetAll(outItems);
		foreach (IEntity entity : outItems)
		{
			if (entity.GetPrefabData().GetPrefabName() == m_sPrefabName)
			{
				return entity;
			}
		}
		return null;
	}
}
