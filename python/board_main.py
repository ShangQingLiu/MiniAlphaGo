#!/usr/bin/python
# -*- coding:utf-8 -*-
CELLWIDTH = 66.75
CELLHEIGHT = 67
PIECEWIDTH = 65
PIECEHEIGHT = 65
BOARDX = 33
BOARDY = 32
flag = 1
score = [2,2]
import numpy as np
from mcts import MCTS
import copy
import time
import pygame, sys, random
from pygame.locals import *

BACKGROUNDCOLOR = (255, 255, 255)
FPS = 40


def terminate():
    pygame.quit()
    sys.exit()


pygame.init()
mainClock = pygame.time.Clock()

boardImage = pygame.image.load('board.png')
boardImage = pygame.transform.scale( boardImage,(600,600))
boardRect = boardImage.get_rect()
print(boardRect)
blackImage = pygame.image.load('Black.png')
blackImage = pygame.transform.scale(blackImage,(65,65))
blackRect = blackImage.get_rect()
mask = pygame.image.load('mask.png')
mask = pygame.transform.scale(mask,(60,60))
maskRect = mask.get_rect()
clmask = pygame.image.load("clmask.png")
clmask =pygame.transform.scale(clmask,(62,62))
clmaskRect = clmask.get_rect()
whiteImage = pygame.image.load('White.png')
whiteImage = pygame.transform.scale(whiteImage,(65,65))
whiteRect = whiteImage.get_rect()

windowSurface = pygame.display.set_mode((boardRect.width, boardRect.height),RESIZABLE)
pygame.display.set_caption('黑白棋')


def resetBoard(Board, State):
    for x in range(8):
        for y in range(8):
            Board[x][y] = 'none'

    Board[3][3] = 'black'
    Board[3][4] = 'white'
    Board[4][3] = 'white'
    Board[4][4] = 'black'

    State[3][3] = -1
    State[3][4] = 1
    State[4][3] = 1
    State[4][4] = -1
    

def getNewBoard():
    board = []
    for i in range(8):
        board.append(['none'] * 8)

    return board

def Valid(pos):
    if(pos[0] >= 0 and pos[0] <= 7 and pos[1] >= 0 and pos[1] <= 7):
        return True
    else:
        return False

def hintForUser(Board, usercolor):
    for x in range(8):
        for y in range(8):
            if Board[x][y] == usercolor:
               #whether opposite color around
                for m in range(x-1,x+2):
                    for n in range(y-1,y+2):
                        if m>7 or  m<0 or n>7 or n<0:
                            continue
                        #如果找到自己的时后
                        elif m==x and n==y:
                            continue
                        #八个方向
                        else:
                            if Board[m][n] == usercolor:
                                continue
                            elif Board[m][n] == 'none':
                                continue
                            elif Board[m][n] == 'yellow':
                                continue
                            else:
                                stepX = m-x
                                stepY = n-y
                                reState(Board,x+stepX,y+stepY,stepX,stepY,usercolor)
def reState(Board,x,y,stepX,stepY,usercolor):
       newX = x + stepX
       newY = y + stepY
       if newX>7 or newX<0 or newY>7 or newY<0:
           return
       else:
           if Board[newX][newY]!=usercolor and Board[newX][newY]!='none' and Board[newX][newY]!='yellow':
               reState(Board,newX,newY,stepX,stepY,usercolor)
           elif Board[newX][newY] == 'none':
               Board[newX][newY] = 'yellow'
               return
           else:
               return



def clearHint(board):
    for x in range(8):
        for y in range(8):
            if board[x][y] == 'yellow':
                board[x][y] = 'none'
def calScore(board):
    BScore = 0
    WScore = 0
    for x in range(8):
        for y in range(8):
           if board[x][y] == 'black' :
               BScore = BScore + 1
           elif board[x][y]== 'white':
               WScore = WScore + 1
           else:
               continue
    return [BScore,WScore]
def Change(Board, state, action, player):
    
     direction = [[-1,0], [1,0], [1,-1], [-1,1], [0,-1], [0,1], [1,1], [-1,-1]]
     if(player == 1):
         Board[action[0]][action[1]] = 'white'
     else:
         Board[action[0]][action[1]] = 'black'

     state[action[0]][action[1]] = player
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
             pos[0] -= each[0]
             pos[1] -= each[1]
             
             while(player != state[pos[0]][pos[1]]):
                 state[pos[0]][pos[1]] *= -1
                 if(player == 1):
                     Board[pos[0]][pos[1]] = 'white'
                 else:
                     Board[pos[0]][pos[1]] = 'black'
                 pos[0] -= each[0]
                 pos[1] -= each[1]

while True:
    if flag == 1:
        mainBoard = getNewBoard()
        State = np.zeros([8,8])
        resetBoard(mainBoard, State)
        windowSurface.fill(BACKGROUNDCOLOR)
        windowSurface.blit(boardImage, boardRect, boardRect)
        hintForUser(mainBoard,'white')
        flag = 0
        col = 0
        row = 0

    down = 0
    
    for event in pygame.event.get():
        if event.type == QUIT:
            terminate()
        if event.type == MOUSEBUTTONDOWN and event.button == 1:
            x,y = pygame.mouse.get_pos()
            col = int((y-BOARDX)/CELLWIDTH)
            row = int((x-BOARDY)/CELLHEIGHT)
            #chessdown(row,col,1,State,mainBoard)
            Change(mainBoard, State, [row,col], 1)
            down = 1
            score = calScore(mainBoard)
            clearHint(mainBoard)


    windowSurface.blit(boardImage, boardRect, boardRect)
    for x in range(8):
        for y in range(8):
            rectDst = pygame.Rect(BOARDX + x * CELLWIDTH + 3.0, BOARDY + y * CELLHEIGHT + 4.0, PIECEWIDTH, PIECEHEIGHT)
            windowSurface.blit(clmask, rectDst, clmaskRect)
            if mainBoard[x][y] == 'black':
                windowSurface.blit(blackImage, rectDst, blackRect)
            elif mainBoard[x][y] == 'white':
                windowSurface.blit(whiteImage, rectDst, whiteRect)
            elif mainBoard[x][y] == 'yellow':
                windowSurface.blit(mask, rectDst, maskRect)
            else:
                windowSurface.blit(clmask, rectDst, clmaskRect)

    #show score
    white = (255,255,255)
    BScore = score[0]
    WScore = score[1]
    myfont = pygame.font.SysFont("simsunnsimsun",20)
    text = myfont.render("Black:"+str(BScore)+"\nWhite:"+str(WScore),True,white)
    windowSurface.blit(text,(20,3))
    pygame.display.update()
    mainClock.tick(FPS)

    if(down == 1):
        Tree = MCTS(State, -1)
        nextDown = Tree.search()
        if(nextDown.action is not None):
            Change(mainBoard, State, nextDown.action[:2], -1)
            hintForUser(mainBoard,'white')
            score = calScore(mainBoard)



   

