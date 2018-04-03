import random
import math
import copy
import numpy as np

class Board(object):

    def __init__(self, board, player, parent):

        self.state = board
        self.player = player
        self.available = self.getAvailable()
        self.quietSelf, self.quietOp = self.getQuiet()
        if(len(self.available) == 0):
            player *= -1
        self.parent = parent
        self.action = None
        self.early = 15
        self.step = np.sum(abs(self.state))
        self.N = 0
        self.Q = 0
        self.child = []
        self.unvisited = self.available.copy()


    def valid(self, pos):
        if(pos[0] >= 0 and pos[0] <= 7 and pos[1] >= 0 and pos[1] <= 7):
            return True
        else:
            return False

        
    def check(self, row, col):
        

        direction = [[-1,0], [1,0], [1,-1], [-1,1], [0,-1], [0,1], [1,1], [-1,-1]]
        sumInterval = 0
        for each in direction:
            pos = [row,col]
            pos[0] += each[0]
            pos[1] += each[1]
            interval = 0
            point = 0

            while(self.valid(pos)):
                if(self.state[pos[0]][pos[1]] == 0):
                    break
                if(self.state[pos[0]][pos[1]] == self.player):
                    point = 1
                    break
                pos[0] += each[0]
                pos[1] += each[1]
                interval += 1

            if(point == 1):
                sumInterval += interval
 
        return sumInterval

    def getAvailable(self):
        
        ret = []
        for i in range(8):
            for j in range(8):
                if(self.state[i][j] == 0):
                    numEat = self.check(i, j)
                    if(numEat != 0):
                        ret.append((i,j, numEat))
        return ret


    def getQuiet(self):

        retSelf = []
        retOp = []
        direction = [[-1,0], [1,0], [1,-1], [-1,1], [0,-1], [0,1], [1,1], [-1,-1]]
        for i in range(8):
            for j in range(8):
                numSelf = 0
                numOp = 0
                if(self.state[i][j] != 0):
                    continue
                for k in range(8):
                    if((i+direction[k][0])<0 or (j+direction[k][1])<0 or (i+direction[k][0])>7 or (j+direction[k][1])>7):
                        numSelf += 1
                    else:
                        if(self.state[i+direction[k][0]][j+direction[k][1]] == -self.player):
                            numSelf += 1
                    if((i+direction[k][0])<0 or (j+direction[k][1])<0 or (i+direction[k][0])>7 or (j+direction[k][1])>7):
                        numOp += 1
                    else:
                        if(self.state[i+direction[k][0]][j+direction[k][1]] == self.player):
                            numOp += 1
                       
                if(numSelf == 7):
                       retSelf.append([i,j])
                if(numOp == 7):
                       retOp.append([i,j])
                       
        return retSelf, retOp


    def needExpand(self):
        return len(self.child) < len(self.available)
    

    def valid(self, pos):
        if(pos[0] >= 0 and pos[0] <= 7 and pos[1] >= 0 and pos[1] <= 7):
            return True
        else:
            return False
    

    def change(self, state, action):
        
        direction = [[-1,0], [1,0], [1,-1], [-1,1], [0,-1], [0,1], [1,1], [-1,-1]]
        for each in direction:
            pos = list(action)
            pos[0] += each[0]
            pos[1] += each[1]
            interval = -1
            point = -1
        
            while(self.valid(pos)):
                if(state[pos[0]][pos[1]] == 0):
                    break
                if(state[pos[0]][pos[1]] == self.player):
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


    def expand(self):

        
       newPos = self.unvisited[int(random.random()*len(self.unvisited))]
       # if(abs(newPos[0] % 7) <= 1 and abs(newPos[1] % 7) <= 1):
        #    if(self.allSub    () is False):
         #       while(abs(newPos[0] % 7) <= 1 and abs(newPos[1] % 7) <= 1):
          #          newPos = self.unvisited[int(random.random()*len(self.unvisited))]
       self.unvisited.remove(newPos)
       newState = self.state.copy()
       newState[newPos[0]][newPos[1]] = self.player
       newState = self.change(newState, newPos)
       newChild = Board(newState, -self.player, self)
       newChild.eat = newPos[2]
       newChild.player *= -1
       newChild.action = newPos
       self.child.append(newChild)
       return newChild

         
    def randomGo(self):
        
        return self.child[int(random.random()*len(self.child))]

    def isWinner(self):

        score = 0
        for i in range(8):
            for j in range(8):
                score += self.state[i][j] / self.player

        if(score > 0):
            return 1
        else:
            return 0

    def  backup(self, reward, finalState):

        curState = self.parent

        while(curState is not None):
            self.N += 1
            self.Q += reward * curState.player / finalState.player
            curState = curState.parent

        

    def getScore(self, obj, C):


        if(obj.check(0,0)!=0 or obj.check(0,7)!=0 or
             obj.check(7,0)!=0 or obj.check(7,7)!=0):
            return -10
        
        if(self.N == 0):
            return 0
        else:
            k = 1
            k = pow(2, max(0,np.sum(obj.quietSelf)-np.sum(self.quietSelf)-np.sum(obj.quietOp)+np.sum(self.quietOp)))
            if(obj.action[0] == 1 or obj.action[0] == 6
                 or obj.action[1] == 1 or obj.action[1] == 6):
                k = 3 / 5
            else:
                if(obj.action[0] == 0 or obj.action[1] == 7
                 or obj.action[1] == 0 or obj.action[1] == 7):                    
                    k = 5 / 3
            if(self.step < self.early):
                k *= 1 / (1 + math.exp(obj.eat))
            return  k * (obj.Q / obj.N + C * math.sqrt(2*math.log(self.N)/obj.N))


    def bestChild(self, C):

        if(len(self.available) == 0):
            return self
        maxScore = self.getScore(self.child[0], C)
        bestChild = self.child[0]
        for each in self.child:
            score = self.getScore(each, C)
            if(score > maxScore):
                maxScore = score
                bestChild = each
        return each
             

    def notEnd(self):
        if((self.state != 0).all() is False):
            return True;

        if(len(self.available) == 0):
            self.player *= -1

            tmp = copy.copy(self)
            self.player *= -1
            if(len(tmp.available) == 0):
                return True
        return False


    def corner(self):


        for each in self.available:
     
            if(each[:2] == (0,0)):
                return (0,0)
            if(each[:2] == (0,7)):
                return (0,7)
            if(each[:2] == (7,0)):
                return (7,0)
            if(each[:2] == (7,7)):
                return (7,7)
        

        return None



    def allSubCorner(self):

        for each in self.available:
            if(abs(each[0] % 7) <= 1 and abs(each[1] % 7) <= 1):
                continue
            else:
                return False
        return True

            
        
          
    
