[ComponentEditorProps(category: "GameScripted/Misc", description: "")]
class RKN_Connect4BoardComponentClass : ScriptComponentClass
{
}

class RKN_Connect4BoardComponent : ScriptComponent
{
	[Attribute()]
	ref PointInfo m_LowerLeftSlot;
	
	[Attribute()]
	ref PointInfo m_UpperRightSlot;
	
	[Attribute()]
	ResourceName m_Player1;
	
	[Attribute()]
	ResourceName m_Player2;
	
	GenericEntity m_aStones[7][6];
	int m_aState[7][6];
	
	bool win;
	
	void PlaceStone(int x, int player)
	{
		PlaceStoneImpl(x, player);
		if (!win)
			ExecuteAI();
	}
	
	void PlaceStoneImpl(int x, int player)
	{
		if (!CanPlace(x))
			return;
		Print("Player " + player + " places stone at " + x);
		for (int y; y < 6; y++)
		{
			if (m_aState[x][y] == 0)
			{
				m_aState[x][y] = player;
				SpawnStoneAt(player, x, y);
				break;
			}
		}
		int winner = GetWinner(m_aState);
		if (winner > 0)
		{
			win = true;
			Print("The winner is " + player)
		}
	}
	
	bool CanPlace(int x)
	{
		return m_aState[x][5] == 0;
	}
	
	private void ExecuteAI()
	{
		RKN_Connect4BoardAIComponent ai = RKN_Connect4BoardAIComponent.Cast(GetOwner().FindComponent(RKN_Connect4BoardAIComponent));
		if (!ai)
			return;
		
		int x = ai.GetBestMove(m_aState, 2);
		PlaceStoneImpl(x, 2);
	}
	
	
	private int GetWinner(int state[7][6])
	{
		for (int x; x < 7; x++)
		{
			for (int y; y < 6; y++)
			{
				int p = state[x][y];
				if (p == 0)
					continue;
				// Vertical (counting down)
				if (y > 2 && state[x][y-1] == p && state[x][y-2] == p && state[x][y-3] == p)
					return p;
				// Horizontal (counting left)
				if (x > 2 && state[x-1][y] == p && state[x-2][y] == p && state[x-3][y] == p)
					return p;
				// Diagonal (counting left and down)
				if (x > 2 && y > 2 && state[x-1][y-1] == p && state[x-2][y-2] == p && state[x-3][y-3] == p)
					return p;
				// Diagonal (counting left and up)
				if (x > 2 && y < 4 && state[x-1][y+1] == p && state[x-2][y+2] == p && state[x-3][y+3] == p)
					return p;
			}
		}
		return 0;
	}
	
	private void SpawnStoneAt(int player, int x, int y)
	{
		ResourceName prefab = m_Player1;
		if (player == 2)
			prefab = m_Player2;
		Resource p = Resource.Load(prefab);
		vector pos = GetSlotPosition(x, y);
		EntitySpawnParams params = new EntitySpawnParams();
		params.Transform[3] = pos;
		params.Parent = GetOwner();
		IEntity entity = GetGame().SpawnEntityPrefab(p, null, params);
		
	}
	
	private vector GetSlotPosition(int x, int y)
	{
		vector lowerLeft = GetPositionOfPoint(m_LowerLeftSlot);
		vector upperRight = GetPositionOfPoint(m_UpperRightSlot);
		float xAxisLength = upperRight[2] - lowerLeft[2];
		float xStep = xAxisLength / 6;
		float xPos = xStep * x + lowerLeft[2];
		float yAxisLength = upperRight[1] - lowerLeft[1];
		float yStep = yAxisLength / 5;
		float yPos = yStep * y + lowerLeft[1];
		return {0, yPos, xPos};
	}
	
	private vector GetPositionOfPoint(PointInfo point)
	{
		vector transform[4];
		point.GetLocalTransform(transform);
		return transform[3];
	}
}
