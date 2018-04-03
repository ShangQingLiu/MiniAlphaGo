#!/usr/bin/python
# -*- coding:utf-8 -*-
CELLWIDTH = 66.75
CELLHEIGHT = 67
PIECEWIDTH = 65
PIECEHEIGHT = 65
BOARDX = 33
BOARDY = 32
flag = 1
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
boardImage = pygame.transform.scale(boardImage,(600,600))
boardRect = boardImage.get_rect()
print boardRect
blackImage = pygame.image.load('Black.png')
blackImage = pygame.transform.scale(blackImage,(65,65))
blackRect = blackImage.get_rect()
whiteImage = pygame.image.load('White.png')
whiteImage = pygame.transform.scale(whiteImage,(65,65))
whiteRect = whiteImage.get_rect()

windowSurface = pygame.display.set_mode((boardRect.width, boardRect.height),RESIZABLE)
pygame.display.set_caption('黑白棋')


def resetBoard(board):
    for x in range(8):
        for y in range(8):
            board[x][y] = 'none'

    board[3][3] = 'black'
    board[3][4] = 'white'
    board[4][3] = 'white'
    board[4][4] = 'black'


def getNewBoard():
    board = []
    for i in range(8):
        board.append(['none'] * 8)

    return board

def chessdown(row,col,color,board):
    board[col][row] = color

while True:
    if flag == 1:
        mainBoard = getNewBoard()
        resetBoard(mainBoard)
        flag = 0
        col = 0
        row = 0
    for event in pygame.event.get():
        if event.type == QUIT:
            terminate()
        if event.type == MOUSEBUTTONDOWN and event.button == 1:
            x,y = pygame.mouse.get_pos()
            col = int((x-BOARDX)/CELLWIDTH)
            row = int((y-BOARDY)/CELLHEIGHT)
            chessdown(row,col,'white',mainBoard)
            print mainBoard
    windowSurface.fill(BACKGROUNDCOLOR)
    windowSurface.blit(boardImage, boardRect, boardRect)




    for x in range(8):
        for y in range(8):
            rectDst = pygame.Rect(BOARDX + x * CELLWIDTH + 0.0, BOARDY + y * CELLHEIGHT + 2.0, PIECEWIDTH, PIECEHEIGHT)
            if mainBoard[x][y] == 'black':
                windowSurface.blit(blackImage, rectDst, blackRect)
            elif mainBoard[x][y] == 'white':
                windowSurface.blit(whiteImage, rectDst, whiteRect)
    #x, y = pygame.mouse.get_pos()
    # 获得鼠标位置
    #print (x,y)
    pygame.display.update()
    mainClock.tick(FPS)

