import time
from board import Board
import copy

class MCTS(object):

    def __init__(self, state, player, time=6):
        self.root = Board(state, player, None)
        self.root.action = None
        self.player = player
        self.time = float(5)
        self.N , self.Q, self.C = 0.001, 0, 1.95


    def search(self):

        curState = self.root
        if(len(self.root.available) == 0):
            print("No choice!")
            return self.root
        cor_avail = curState.corner()
        if(cor_avail is not None):
            return self.directBoard(curState, cor_avail)
        beg = time.time()
        while(True):
            curState = self.root
            for i in range(5):
                next_board = self.treePolicy()
                reward = self.defaultPolicy(curState)
                next_board.backup(reward, curState)
            if((time.time() - beg) > self.time):
                break
        return self.root.bestChild(self.C)



    def directBoard(self, curState, cor_avail):

        ret = copy.copy(self.root)
        ret.action = cor_avail
        ret.state[cor_avail[0]][cor_avail[1]] = ret.player
        return ret
        
            

    def treePolicy(self):

        curState = self.root
        while(True):
            if(curState.notEnd()):
                break
            if(curState.needExpand()):
                return curState.expand()
            else:
                if(len(curState.available) == 0):
                    break
                curState = curState.bestChild(self.C)
        return curState
    

    def defaultPolicy(self, curState):
        
        if(len(curState.available) == 0):
            nextState = copy.copy(curState)
            nextState.player = -nextState.player
            return defaultPolicy(nextState)
            
        while(curState.notEnd()):
            curState = curState.randomGo()
  
        return curState.isWinner()      
         

        




        
         
        
