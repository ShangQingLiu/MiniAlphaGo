import numpy as np
from mcts import MCTS
import copy

def Valid(pos):
    if(pos[0] >= 0 and pos[0] <= 7 and pos[1] >= 0 and pos[1] <= 7):
       return True
    else:
        return False

def Change(state, action, player):
    
     direction = [[-1,0], [1,0], [1,-1], [-1,1], [0,-1], [0,1], [1,1], [-1,-1]]

     for each in direction:
         pos = list(action)
         pos[0] += each[0]
         pos[1] += each[1]
         interval = -1
         point = -1
        
         while(Valid(pos)):
             if(state[pos[0]][pos[1]] == 0):
                 break
             if(state[pos[0]][pos[1]] == player):
                 point = 1
                 break
             pos[0] += each[0]
             pos[1] += each[1]
             interval = 1
         
         if(point == 1 and interval == 1):
             val = state[pos[0]][pos[1]]
             pos[0] -= each[0]
             pos[1] -= each[1]
             
             while(val != state[pos[0]][pos[1]]):
                 state[pos[0]][pos[1]] *= -1
                 pos[0] -= each[0]
                 pos[1] -= each[1]
     return state
            

state = np.zeros([8,8])
state[3,3] = 1
state[4,4] = 1
state[4,3] = -1
state[3,4] = -1

player = 1


while(1):
    row = input("行: ")
    col = input("列：")
    player *= -1
    state[int(row),int(col)] = player
    state = Change(state, (int(row),int(col)), player)
    print(state)
    player *= -1
    Tree = MCTS(state, player)
    x = Tree.search()
    print(x.action[:2])
    state = Change(x.state, x.action, player)

