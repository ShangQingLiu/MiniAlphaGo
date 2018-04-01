#!/usr/bin/python
# -*- coding:utf-8 -*-
CELLWIDTH = 66.75
CELLHEIGHT = 67
PIECEWIDTH = 65
PIECEHEIGHT = 65
BOARDX = 33
BOARDY = 32
flag = 1
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
                           

    for x in range(8):
        for y in range(8):
            rectDst = pygame.Rect(BOARDX + x * CELLWIDTH + 0.0, BOARDY + y * CELLHEIGHT + 2.0, PIECEWIDTH, PIECEHEIGHT)
            if mainBoard[x][y] == 'black':
                windowSurface.blit(blackImage, rectDst, blackRect)
            elif mainBoard[x][y] == 'white':
                windowSurface.blit(whiteImage, rectDst, whiteRect)

    
    pygame.display.update()
    mainClock.tick(FPS)

    if(down == 1):
        Tree = MCTS(State, -1)
        nextDown = Tree.search()
        if(nextDown.action is not None):
            Change(mainBoard, State, nextDown.action[:2], -1)
    

   

