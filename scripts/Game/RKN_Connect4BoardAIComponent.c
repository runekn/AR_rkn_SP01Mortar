[ComponentEditorProps(category: "GameScripted/Misc", description: "")]
class RKN_Connect4BoardAIComponentClass : ScriptComponentClass
{
}

class RKN_Connect4BoardAIComponent : ScriptComponent
{
	[Attribute(defvalue: "5")]
	int m_iMinMaxDepth;
	
	int GetBestMove(int state[7][6], int player)
	{
		int x = 0;
		AlphaBetaMinMax(state, m_iMinMaxDepth, -float.INFINITY, float.INFINITY, 2, x);
		return x;
	}
	
	private float AlphaBetaMinMax(int state[7][6], int depth, float alpha, float beta, int player, out int bestColumn)
	{
		if (GetWinner(state) != 0 || depth == 0)
		{
        	float score = UtilityScore(state, player);
			if (player == 2)
				return -score;
			else
				return score;
		}
		
		int throwAway;
	    if (player == 2)
		{
	        float max_eval = -float.INFINITY;
	        for (int x; x < 7; x++)
			{
				if (!CanPlace(state, x))
					continue;
				
				int newState[7][6];
				GetResultingState(state, x, player, newState);
	            float eval = AlphaBetaMinMax(newState, depth - 1, alpha, beta, 1, throwAway);
	            if (max_eval < eval)
				{
					max_eval = eval;
					bestColumn = x;
				}
	            alpha = Math.Max(alpha, eval);
	            if (beta <= alpha)
	                break;  // Beta cut-off
			}
	        return max_eval;
		}
	    else
		{
	        float min_eval = float.INFINITY;
	        for (int x; x < 7; x++)
			{
				if (!CanPlace(state, x))
					continue;
				
				int newState[7][6];
				GetResultingState(state, x, player, newState);
	            float eval = AlphaBetaMinMax(newState, depth - 1, alpha, beta, 2, throwAway);
	            if (min_eval > eval)
				{
					min_eval = eval;
					bestColumn = x;
				}
	            beta = Math.Min(beta, eval);
	            if (beta <= alpha)
	                break;  // Alpha cut-off
			} 
	        return min_eval;
		}
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
	
	private int UtilityScore(int state[7][6], int player)
	{
		int score = 0;
		for (int x; x < 7; x++)
		{
			for (int y; y < 6; y++)
			{
				int p = state[x][y];
				if (p != player)
					continue;
				// Vertical (counting down)
				if (y > 2)
				{
					if (state[x][y-1] == player)
						score += 1;
					if (state[x][y-2] == player)
						score += 1;
					if (state[x][y-3] == player)
						score += 1;
				}
				// Horizontal (counting left)
				if (x > 2)
				{
					if (state[x-1][y] == player)
						score += 1;
					if (state[x-2][y] == player)
						score += 1;
					if (state[x-3][y] == player)
						score += 1;
				}
				// Diagonal (counting left and down)
				if (x > 2 && y > 2)
				{
					if (state[x-1][y-1] == player)
						score += 1;
					if (state[x-2][y-2] == player)
						score += 1;
					if (state[x-3][y-3] == player)
						score += 1;
				}
				// Diagonal (counting left and up)
				if (x > 2 && y < 4)
				{
					if (state[x-1][y+1] == player)
						score += 1;
					if (state[x-2][y+2] == player)
						score += 1;
					if (state[x-3][y+3] == player)
						score += 1;
				}
			}
		}
		return score;
	}
	
	private bool CanPlace(int state[7][6], int x)
	{
		return state[x][5] == 0;
	}
	
	private void GetResultingState(int state[7][6], int x, int player, out int newState[7][6])
	{
		for (int xx; xx < 7; xx++)
		{
			for (int y; y < 6; y++)
			{
				newState[xx][y] = state[xx][y];
			}
		}
		
		for (int y; y < 6; y++)
		{
			if (newState[x][y] == 0)
			{
				newState[x][y] = player;
				return;
			}
		}
	}
}
